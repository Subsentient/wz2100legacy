/*
Warzone 2100 Legacy Project Lobby Server,
Copyright (c) 2014 The Warzone 2100 Legacy Project

This file is part of Warzone 2100 Legacy.
 
Warzone 2100 Legacy is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Warzone 2100 Legacy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Warzone 2100 Legacy.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>

#define LOBBYVER "0.2"
#define LOBBYPORT 9990
#define MAX_SIMULTANIOUS 1024

#define PACKED_GS_SIZE 814 /*Computed by this:
sizeof InStruct.StructVer +
sizeof InStruct.GameName +
sizeof(int32_t) * 8 +
sizeof InStruct.NetSpecs.HostIP + 
sizeof InStruct.SecondaryHosts + 
sizeof InStruct.Extra + 
sizeof InStruct.Map + 
sizeof InStruct.HostNick + 
sizeof InStruct.VersionString + 
sizeof InStruct.ModList + 
sizeof(uint32_t) * 9
*/

typedef signed char Bool;
enum { false, true };

typedef struct
{
	uint32_t StructVer;
	char GameName[64];
	
	struct
	{
		int32_t Size;
		int32_t Flags;
		char HostIP[40];
		int32_t MaxPlayers, CurPlayers;
		int32_t UserFlags[4];
	} NetSpecs;
	
	char SecondaryHosts[2][40];
	char Extra[159];
	char Map[40];
	char HostNick[40];
	char VersionString[64];
	char ModList[255];
	uint32_t MajorVer, MinorVer;
	uint32_t PrivateGame;
	uint32_t PureGame;
	uint32_t Mods;
	
	uint32_t GameID;
	
	uint32_t Unused1, Unused2, Unused3;
} GameStruct;

static struct _GameTree
{
	int Sock;
	time_t TimeHosted;
	GameStruct Game;
	Bool Completed;
	
	struct _GameTree *Next;
	struct _GameTree *Prev;
} *GameTree;

static int SocketDescriptor;
static int SocketFamily;
static time_t LastHostTime;

static Bool NetRead(int SockDescriptor, void *OutStream_, unsigned long MaxLength, Bool IsText);
static Bool NetWrite(int SockDescriptor, void *InMsg, unsigned long WriteSize);
static Bool NetInit(unsigned short PortNum);
static void NetShutdown(void);
static Bool GameCompleteCreate(GameStruct *Game);
static struct _GameTree *GameCreate(uint32_t GameID, int SockDesc);
static uint32_t GameGetGameID(void);
static int GameCountGames(void);
/*
static struct _GameTree *GameLookup(uint32_t GameID);
*/
static Bool GameRemove(uint32_t GameID);
static void GameRemoveAll(void);
static void LobbyLoop(void);
static void ProtocolEncodeGS(GameStruct *InStruct, unsigned char *OutStream);
static void ProtocolDecodeGS(unsigned char *InStream, GameStruct *OutStream);

/*Functions*/
static Bool NetRead(int SockDescriptor, void *OutStream_, unsigned long MaxLength, Bool IsText)
{
	int Status = 0;
	unsigned char Byte = 0;
	unsigned char *OutStream = OutStream_;
	unsigned long Inc = 0;
	
	do
	{
		Status = recv(SockDescriptor, &Byte, 1, 0);
		
		*OutStream++ = Byte;
		
		if ((Byte == '\n' || Byte == '\0') && IsText) break;
		
	} while (++Inc, Status > 0 && Inc < MaxLength);
	
	if (IsText)
	{
		--OutStream;
		
		while (*OutStream == '\n' || *OutStream == '\r') *OutStream-- = '\0';
	}
	
	return Status != -1;
}

static Bool NetWrite(int SockDescriptor, void *InMsg, unsigned long WriteSize)
{
	unsigned long Transferred = 0, TotalTransferred = 0;

	do
	{
		Transferred = send(SockDescriptor, InMsg, (WriteSize - TotalTransferred), MSG_NOSIGNAL);
		
		if (Transferred == -1) /*This is ugly I know, but it's converted implicitly, so shut up.*/
		{
			return false;
		}
		
		TotalTransferred += Transferred;
	} while (WriteSize > TotalTransferred);
	
	return true;
}

static Bool NetInit(unsigned short PortNum)
{ /*this is mostly just from Beej's network guide,
	because I care too little to actually try and learn this stuff.*/
	struct addrinfo BStruct, *Res = NULL;
	char AsciiPort[16] =  { '\0' };
	int True = true;
	int GAIExit;
	
	snprintf(AsciiPort, sizeof AsciiPort, "%hu", PortNum);
	
	memset(&BStruct, 0, sizeof(struct addrinfo));
	
	BStruct.ai_family = AF_UNSPEC;
	BStruct.ai_socktype = SOCK_STREAM;
	BStruct.ai_flags = AI_PASSIVE;

	if ((GAIExit = getaddrinfo(NULL, AsciiPort, &BStruct, &Res)) != 0)
	{
		fprintf(stderr, "Failed to getaddrinfo(): %s\n", gai_strerror(GAIExit));
		return false;
	}
	
	SocketFamily = Res->ai_family;
	
	if (!(SocketDescriptor = socket(Res->ai_family, Res->ai_socktype, Res->ai_protocol)))
	{
		fprintf(stderr, "Failed to open a socket on port %hu.\n", PortNum);
		return false;
	}
	
	setsockopt(SocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &True, sizeof(int));
	
	if (bind(SocketDescriptor, Res->ai_addr, Res->ai_addrlen) == -1)
	{
		perror("bind()");
		return false;
	}
	
	listen(SocketDescriptor, MAX_SIMULTANIOUS);

	freeaddrinfo(Res);
	
	return true;
}
	
static void NetShutdown(void)
{
	struct _GameTree *Worker = GameTree;
	
	for (; Worker; Worker = Worker->Next)
	{
		close(Worker->Sock);
		Worker->Sock = 0;
	}
	close(SocketDescriptor);
	SocketFamily = 0;
	SocketDescriptor = 0;
}

static uint32_t GameGetGameID(void)
{
	struct _GameTree *Worker = GameTree;
	
	uint32_t CurHighest = 0;
	
	for (; Worker; Worker = Worker->Next)
	{
		if (Worker->Game.GameID > CurHighest) CurHighest = Worker->Game.GameID;
	}
	
	return CurHighest + 1;
}

static struct _GameTree *GameCreate(uint32_t GameID, int SockDesc)
{
	struct _GameTree *Worker = GameTree;
	if (!GameTree)
	{
		Worker = GameTree = malloc(sizeof(struct _GameTree));
		memset(GameTree, 0, sizeof(struct _GameTree));
	}
	else
	{
		while (Worker->Next) Worker = Worker->Next;
		
		Worker->Next = malloc(sizeof(struct _GameTree));
		memset(Worker->Next, 0, sizeof(struct _GameTree));
		Worker->Next->Prev = Worker;
		Worker = Worker->Next;
	}
	
	Worker->Game.GameID = GameID;
	Worker->Sock = SockDesc;
	
	return Worker;
}

static Bool GameCompleteCreate(GameStruct *Game)
{
	struct _GameTree *Worker = GameTree;
	
	if (!GameTree) return false;
	
	for (; Worker; Worker = Worker->Next)
	{
		if (Game->GameID == Worker->Game.GameID)
		{
			char Addr[sizeof Game->NetSpecs.HostIP];
			
			memcpy(Addr, Worker->Game.NetSpecs.HostIP, sizeof Addr);

			Worker->Game = *Game;
			
			memcpy(Worker->Game.NetSpecs.HostIP, Addr, sizeof Addr);

			Worker->Completed = true;
			
			LastHostTime = Worker->TimeHosted = time(NULL);
			
			return true;
		}
	}
	
	return false;
}

static Bool GameRemove(uint32_t GameID)
{
	struct _GameTree *Worker = GameTree;
	Bool Found = false;

	if (!GameTree) return false;
	
	for (; Worker; Worker = Worker->Next)
	{
		if (Worker->Game.GameID == GameID)
		{
			if (Worker == GameTree)
			{
				if (Worker->Next)
				{
					Worker->Next->Prev = NULL;
					GameTree = Worker->Next;
					free(Worker);
				}
				else
				{
					free(Worker);
					GameTree = NULL;
				}
			}
			else
			{
				if (Worker->Next) Worker->Next->Prev = Worker->Prev;
				Worker->Prev->Next = Worker->Next;
				free(Worker);
			}
			
			Found = true;
		}
	}
	
	return Found;
}
/*
static struct _GameTree *GameLookup(uint32_t GameID)
{
	struct _GameTree *Worker = GameTree;
	
	for (; Worker; Worker = Worker->Next)
	{
		if (Worker->Game.GameID == GameID)
		{
			return Worker;
		}
	}
	
	return NULL;
}*/
	
static void GameRemoveAll(void)
{
	struct _GameTree *Worker = GameTree, *Temp;
	
	for (; Worker; Worker = Temp)
	{
		Temp = Worker->Next;
		free(Worker);
	}
}

static void LobbyLoop(void)
{
	int ClientDescriptor = 0;
	struct sockaddr ClientInfo;
	socklen_t SockaddrSize = sizeof(struct sockaddr);
	char InBuf[PACKED_GS_SIZE + 2048], OutBuf[PACKED_GS_SIZE + 2048];
	struct _GameTree *Worker = NULL;
	struct sockaddr_in Addr;
	socklen_t AddrSize = sizeof Addr;
	char AddrBuf[128];
	
	memset(&ClientInfo, 0, sizeof ClientInfo);
	
	puts("--[Lobby loop started. Waiting for clients.]--");
	
	while (1)
	{
		fflush(NULL); /*For logging etc*/
		
		if ((ClientDescriptor = accept(SocketDescriptor, &ClientInfo, &SockaddrSize)) == -1)
		{
			/*Just try and restart the server if an error occurs.*/
			fprintf(stderr, "Socket accept() error. Cycling sockets.");
			NetShutdown();
			GameRemoveAll();
			NetInit(LOBBYPORT);
			continue;
		}
		
		/*Get the IP of the client.*/
		memset(&Addr, 0, AddrSize); /*Probably not necessary*/
		getpeername(ClientDescriptor, (void*)&Addr, &AddrSize);
		inet_ntop(SocketFamily, (void*)&Addr.sin_addr, AddrBuf, sizeof AddrBuf);
		
	L2Start:
		for (Worker = GameTree; Worker; Worker = Worker->Next)
		{ /*Handle existing sockets, remove dead games, etc*/
			if (!Worker->Sock)
			{
				GameRemove(Worker->Game.GameID);
				goto L2Start;
			}
			else
			{
				char RecvChar;
				int Status = recv(Worker->Sock, &RecvChar, 1, MSG_DONTWAIT);
				
				if (Status == 0 || (Status == -1 && errno != EAGAIN && errno != EWOULDBLOCK) ||
					(Worker->TimeHosted && time(NULL) - Worker->TimeHosted > 60 * 120))
					/*You can only have a game open for two hours before you must re-register.*/
				{
					printf("--[Removing defunct game %s::%u::\"%s\" before handling client %s.]--\n",
							Worker->Game.NetSpecs.HostIP, Worker->Game.GameID, Worker->Game.GameName, AddrBuf);
					close(Worker->Sock);
					GameRemove(Worker->Game.GameID);
					goto L2Start;
				}
				else continue;
			}
		}
		
		/*Get command.*/
		if (!NetRead(ClientDescriptor, InBuf, sizeof InBuf, true))
		{
			close(ClientDescriptor);
			continue;
		}
		
		if (!strcmp("list", InBuf))
		{
			int Count = htonl(GameCountGames());
			struct _GameTree *Worker = GameTree;
			uint32_t TempTime = LastHostTime ? htonl((time(NULL) - LastHostTime) / 60) : 0;
			
			printf("--[Game list requested by %s, sending total of %d games.]--\n", AddrBuf, (int)ntohl(Count));
			
			NetWrite(ClientDescriptor, &Count, sizeof(int));
			
			if (Count == 0)
			{
				close(ClientDescriptor);
				continue;
			}
			
			for (; Worker; Worker = Worker->Next)
			{
				if (!Worker->Completed) continue;
				ProtocolEncodeGS(&Worker->Game, (void*)OutBuf);
				NetWrite(ClientDescriptor, OutBuf, PACKED_GS_SIZE);
			}
			
			/*Write the time in minutes since the last game was hosted.*/
			NetWrite(ClientDescriptor, &TempTime, sizeof(uint32_t));
			
			close(ClientDescriptor);
			continue;
		}
		else if (!strcmp("txlist", InBuf))
		{
			struct _GameTree *Worker = GameTree;
			int Count = GameCountGames();
			uint32_t TempTime = LastHostTime ? (time(NULL) - LastHostTime) / 60: 0;
			
			printf("--[Text based game list requested by %s, sending total of %d games.]--\n", AddrBuf, Count);
			
			snprintf(OutBuf, sizeof OutBuf, "%d Games\n",Count);
			
			NetWrite(ClientDescriptor, OutBuf, strlen(OutBuf));
			
			for (; Worker; Worker = Worker->Next)
			{
				snprintf(OutBuf, sizeof OutBuf, "Name: %s | Map: %s | Host: %s | Players: %d/%d | Mods: %s | Private: %s\n", Worker->Game.GameName,
						Worker->Game.Map, Worker->Game.HostNick, Worker->Game.NetSpecs.CurPlayers, Worker->Game.NetSpecs.MaxPlayers,
						*Worker->Game.ModList ? Worker->Game.ModList : "None", Worker->Game.PrivateGame ? "Yes" : "No");
						
				NetWrite(ClientDescriptor, OutBuf, strlen(OutBuf));
			}
			
			
			snprintf(OutBuf, sizeof OutBuf, "Last game was hosted %lu minutes ago.\n", (unsigned long)TempTime);
			NetWrite(ClientDescriptor, OutBuf, strlen(OutBuf) + 1); /*Write a null terminator, so +1.*/
			
			close(ClientDescriptor);
			continue;
		}
		else if (!strcmp("gaId", InBuf))
		{
			uint32_t GameID = GameGetGameID();
			struct _GameTree *Game;

			printf("--[Game ID Request from %s. Assigning Game ID \"%u\".]--\n", AddrBuf, (unsigned int)GameID);
			
			Game = GameCreate(GameID, ClientDescriptor); /*We store the client's socket descriptor.*/
			
			/*Copy in IP.*/
			strncpy(Game->Game.NetSpecs.HostIP, AddrBuf, sizeof Game->Game.NetSpecs.HostIP - 1);
			Game->Game.NetSpecs.HostIP[sizeof Game->Game.NetSpecs.HostIP - 1] = '\0';
			GameID = htonl(GameID);
			NetWrite(ClientDescriptor, &GameID, sizeof(uint32_t));
			
			while (!NetRead(ClientDescriptor, InBuf, sizeof InBuf, true)) usleep(1000);

			if (!strcmp("addg", InBuf))
			{
				GameStruct RecvGame = { 0 };
				uint32_t StatusCode = 0, MOTDLength = 0;
				const char *Msg = NULL;
				char MsgFromFile[128] = { '\0' };
				Bool ES = false;
				
				while (!NetRead(ClientDescriptor, (void*)InBuf, PACKED_GS_SIZE, false)) usleep(1000);
	
				ProtocolDecodeGS((void*)InBuf, &RecvGame);
				RecvGame.GameID = ntohl(GameID);
				
				if (!(ES = GameCompleteCreate(&RecvGame)))
				{
					Msg = "Bad game ID provided.";
					printf("--[Bad game ID %u provided by game creation request from %s.]--\n", RecvGame.GameID, AddrBuf);
					StatusCode = htonl(8); /*Bad game ID.*/
					MOTDLength = htonl(strlen(Msg));
				}
				else
				{
					FILE *FileDescriptor = fopen("motd.txt", "r");
					const char *FallbackMsg = "Welcome to the Warzone 2100 Legacy Lobby Server!";
					
					if (!FileDescriptor)
					{
						Msg = FallbackMsg;
					}
					else
					{
						int Inc = 0, TChar;
						
						for (; (TChar = getc(FileDescriptor)) != EOF && Inc < sizeof MsgFromFile - 1; ++Inc)
						{
							((unsigned char*)MsgFromFile)[Inc] = TChar; /*Cast is for in case someone is trying to send binary
							* for whatever reason, because that can cause an overflow or trap etc if it's number is higher than
							* a signed char.*/
							
						}
						MsgFromFile[Inc] = '\0';
						
						fclose(FileDescriptor);
						
						Msg = MsgFromFile;
					}

					printf("--[Creating game for %s. Game ID %u, Name \"%s\", Map \"%s\", Hoster \"%s\".]--\n",
							AddrBuf, RecvGame.GameID, RecvGame.GameName, RecvGame.Map, RecvGame.HostNick);
					StatusCode = htonl(200);
					MOTDLength = htonl(strlen(Msg));
				}
				
				memcpy(OutBuf, &StatusCode, sizeof(uint32_t));
				memcpy(OutBuf + sizeof(uint32_t), &MOTDLength, sizeof(uint32_t));
				memcpy(OutBuf + sizeof(uint32_t) * 2, Msg, ntohl(MOTDLength));
				
				NetWrite(ClientDescriptor, OutBuf, sizeof(uint32_t) * 2 + ntohl(MOTDLength));
				
				if (!ES)
				{
					printf("--[Closing socket to %s]--\n", AddrBuf);
					close(ClientDescriptor);
				}
				else
				{
					printf("--[Storing socket descriptor for client %s, who created Game %u::\"%s\".]--\n", AddrBuf, RecvGame.GameID, RecvGame.GameName);
				}
				continue;
			}
			continue;
		}
		else
		{
			NetWrite(ClientDescriptor, "Not known.\n", sizeof "Not known.\n");
			close(ClientDescriptor);
			continue;
		}
	}
	
	NetShutdown();
}

static int GameCountGames(void)
{
	struct _GameTree *Worker = GameTree;
	int Inc = 0;
	
	for (; Worker; Worker = Worker->Next) ++Inc;
	
	return Inc;
}

static void ProtocolEncodeGS(GameStruct *InStruct, unsigned char *OutStream)
{
	int Inc = 0;
	GameStruct CStruct = { 0 };
	unsigned char *Worker = OutStream;
	
	CStruct = *InStruct;
	
	CStruct.StructVer = htonl(CStruct.StructVer);
	memcpy(Worker, &CStruct.StructVer, sizeof CStruct.StructVer);
	Worker += sizeof CStruct.StructVer;
	
	memcpy(Worker, CStruct.GameName, sizeof CStruct.GameName);
	Worker += sizeof CStruct.GameName;
	
	CStruct.NetSpecs.Size = htonl(CStruct.NetSpecs.Size);
	memcpy(Worker, &CStruct.NetSpecs.Size, sizeof CStruct.NetSpecs.Size);
	Worker += sizeof CStruct.NetSpecs.Size;
	
	CStruct.NetSpecs.Flags = htonl(CStruct.NetSpecs.Flags);
	memcpy(Worker, &CStruct.NetSpecs.Flags, sizeof CStruct.NetSpecs.Flags);
	Worker += sizeof CStruct.NetSpecs.Flags;
	
	memcpy(Worker, CStruct.NetSpecs.HostIP, sizeof CStruct.NetSpecs.HostIP);
	Worker += sizeof CStruct.NetSpecs.HostIP;
	
	CStruct.NetSpecs.MaxPlayers = htonl(CStruct.NetSpecs.MaxPlayers);
	memcpy(Worker, &CStruct.NetSpecs.MaxPlayers, sizeof(int32_t));
	Worker += sizeof(int32_t);
	
	CStruct.NetSpecs.CurPlayers = htonl(CStruct.NetSpecs.CurPlayers);
	memcpy(Worker, &CStruct.NetSpecs.CurPlayers, sizeof(int32_t));
	Worker += sizeof(int32_t);
	
	for (Inc = 0; Inc < 4; ++Inc)
	{
		CStruct.NetSpecs.UserFlags[Inc] = htonl(CStruct.NetSpecs.UserFlags[Inc]);
		memcpy(Worker, &CStruct.NetSpecs.UserFlags[Inc], sizeof(int32_t));
		Worker += sizeof(int32_t);
	}
	
	memcpy(Worker, CStruct.SecondaryHosts, sizeof CStruct.SecondaryHosts);
	Worker += sizeof CStruct.SecondaryHosts;
	
	memcpy(Worker, CStruct.Extra, sizeof CStruct.Extra);
	Worker += sizeof CStruct.Extra;
	
	memcpy(Worker, CStruct.Map, sizeof CStruct.Map);
	Worker += sizeof CStruct.Map;
	
	memcpy(Worker, CStruct.HostNick, sizeof CStruct.HostNick);
	Worker += sizeof CStruct.HostNick;
	
	memcpy(Worker, CStruct.VersionString, sizeof CStruct.VersionString);
	Worker += sizeof CStruct.VersionString;
	
	memcpy(Worker, CStruct.ModList, sizeof CStruct.ModList);
	Worker += sizeof CStruct.ModList;

	CStruct.MajorVer = htonl(CStruct.MajorVer);
	memcpy(Worker, &CStruct.MajorVer, sizeof(uint32_t));
	Worker += sizeof(uint32_t);
	
	CStruct.MinorVer = htonl(CStruct.MinorVer);
	memcpy(Worker, &CStruct.MinorVer, sizeof(uint32_t));
	Worker += sizeof(uint32_t);
	
	CStruct.PrivateGame = htonl(CStruct.PrivateGame);
	memcpy(Worker, &CStruct.PrivateGame, sizeof(uint32_t));
	Worker += sizeof(uint32_t);
	
	CStruct.PureGame = htonl(CStruct.PureGame);
	memcpy(Worker, &CStruct.PureGame, sizeof(uint32_t));
	Worker += sizeof(uint32_t);
	
	CStruct.Mods = htonl(CStruct.Mods);
	memcpy(Worker, &CStruct.Mods, sizeof(uint32_t));
	Worker += sizeof(uint32_t);
	
	CStruct.GameID = htonl(CStruct.GameID);
	memcpy(Worker, &CStruct.GameID, sizeof(uint32_t));
	Worker += sizeof(uint32_t);
	
	CStruct.Unused1 = htonl(CStruct.Unused1);
	memcpy(Worker, &CStruct.Unused1, sizeof(uint32_t));
	Worker += sizeof(uint32_t);
	
	CStruct.Unused2 = htonl(CStruct.Unused2);
	memcpy(Worker, &CStruct.Unused2, sizeof(uint32_t));
	Worker += sizeof(uint32_t);
	
	CStruct.Unused3 = htonl(CStruct.Unused3);
	memcpy(Worker, &CStruct.Unused3, sizeof(uint32_t));
	Worker += sizeof(uint32_t);
}

static void ProtocolDecodeGS(unsigned char *InStream, GameStruct *OutStream)
{
	int Inc = 0;
	unsigned char *Worker = InStream;
	
	memcpy(&OutStream->StructVer, Worker, sizeof OutStream->StructVer);
	OutStream->StructVer = ntohl(OutStream->StructVer);
	Worker += sizeof OutStream->StructVer;
	
	memcpy(OutStream->GameName, Worker, sizeof OutStream->GameName);
	Worker += sizeof OutStream->GameName;
	
	memcpy(&OutStream->NetSpecs.Size, Worker, sizeof OutStream->NetSpecs.Size);
	OutStream->NetSpecs.Size = ntohl(OutStream->NetSpecs.Size);
	Worker += sizeof OutStream->NetSpecs.Size;
	
	memcpy(&OutStream->NetSpecs.Flags, Worker, sizeof OutStream->NetSpecs.Flags);
	OutStream->NetSpecs.Flags = ntohl(OutStream->NetSpecs.Flags);
	Worker += sizeof OutStream->NetSpecs.Flags;
	
	memcpy(OutStream->NetSpecs.HostIP, Worker, sizeof OutStream->NetSpecs.HostIP);
	Worker += sizeof OutStream->NetSpecs.HostIP;
	
	memcpy(&OutStream->NetSpecs.MaxPlayers, Worker, sizeof(int32_t));
	OutStream->NetSpecs.MaxPlayers = ntohl(OutStream->NetSpecs.MaxPlayers);
	Worker += sizeof(int32_t);
	
	memcpy(&OutStream->NetSpecs.CurPlayers, Worker, sizeof(int32_t));
	OutStream->NetSpecs.CurPlayers = ntohl(OutStream->NetSpecs.CurPlayers);
	Worker += sizeof(int32_t);
	
	for (; Inc < 4; ++Inc)
	{
		memcpy(&OutStream->NetSpecs.UserFlags[Inc], Worker, sizeof(int32_t));
		OutStream->NetSpecs.UserFlags[Inc] = ntohl(OutStream->NetSpecs.UserFlags[Inc]);
	}
	Worker += sizeof(int32_t) * 4;
	
	memcpy(OutStream->SecondaryHosts, Worker, sizeof OutStream->SecondaryHosts);
	Worker += sizeof OutStream->SecondaryHosts;
	
	memcpy(OutStream->Extra, Worker, sizeof OutStream->Extra);
	Worker += sizeof OutStream->Extra;
	
	memcpy(OutStream->Map, Worker, sizeof OutStream->Map);
	Worker += sizeof OutStream->Map;
	
	memcpy(OutStream->HostNick, Worker, sizeof OutStream->HostNick);
	Worker += sizeof OutStream->HostNick;
	
	memcpy(OutStream->VersionString, Worker, sizeof OutStream->VersionString);
	Worker += sizeof OutStream->VersionString;
	
	memcpy(OutStream->ModList, Worker, sizeof OutStream->ModList);
	Worker += sizeof OutStream->ModList;
	
	memcpy(&OutStream->MajorVer, Worker, sizeof(uint32_t));
	OutStream->MajorVer = ntohl(OutStream->MajorVer);
	Worker += sizeof(uint32_t);
	
	memcpy(&OutStream->MinorVer, Worker, sizeof(uint32_t));
	OutStream->MinorVer = ntohl(OutStream->MinorVer);
	Worker += sizeof(uint32_t);
	
	memcpy(&OutStream->PrivateGame, Worker, sizeof(uint32_t));
	OutStream->PrivateGame = ntohl(OutStream->PrivateGame);
	Worker += sizeof(uint32_t);
		
	memcpy(&OutStream->PureGame, Worker, sizeof(uint32_t));
	OutStream->PureGame = ntohl(OutStream->PureGame);
	Worker += sizeof(uint32_t);
	
	memcpy(&OutStream->Mods, Worker, sizeof(uint32_t));
	OutStream->Mods = ntohl(OutStream->Mods);
	Worker += sizeof(uint32_t);
	
	memcpy(&OutStream->GameID, Worker, sizeof(uint32_t));
	OutStream->GameID = ntohl(OutStream->GameID);
	Worker += sizeof(uint32_t);
	
	memcpy(&OutStream->Unused1, Worker, sizeof(uint32_t));
	OutStream->Unused1 = ntohl(OutStream->Unused1);
	Worker += sizeof(uint32_t);
	
	memcpy(&OutStream->Unused2, Worker, sizeof(uint32_t));
	OutStream->Unused2 = ntohl(OutStream->Unused2);
	Worker += sizeof(uint32_t);
	
	memcpy(&OutStream->Unused3, Worker, sizeof(uint32_t));
	OutStream->Unused3 = ntohl(OutStream->Unused3);
	Worker += sizeof(uint32_t);
}

int main(int argc, char **argv)
{
	puts("Warzone 2100 Legacy Lobby Server v" LOBBYVER "\n\n"
		"Copyright 2014 The Warzone 2100 Legacy Project\n"
		"This software is released under the GPLv2.\n"
		"See the included file COPYING to read the license.\n---\n");

	if (argc == 2 && !strcmp(argv[1], "--background"))
	{
		pid_t PID = fork();
		
		if (PID == -1)
		{
			fprintf(stderr, "Failed to fork()!");
			exit(1);
		}
		
		if (PID > 0)
		{
			puts("Backgrounding.");
			signal(SIGCHLD, SIG_IGN);
			exit(0);
		}
		
		if (PID == 0)
		{
			setsid();
			freopen("lobby.log", "a", stdout);
			freopen("lobby.log", "a", stderr);
		}
	}
	
	printf("Opening socket on port %d... ", LOBBYPORT);
	
	if (!NetInit(LOBBYPORT)) exit(1);
	puts("Ok");
	
	LobbyLoop();
	
	return exit(0), 0;
}
