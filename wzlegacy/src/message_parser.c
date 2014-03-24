
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         message_parse
#define yylex           message_lex
#define yyerror         message_error
#define yylval          message_lval
#define yychar          message_char
#define yydebug         message_debug
#define yynerrs         message_nerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 20 "message_parser.y"

/** @file
 *
 *  Parser for message data
 */

#include "lib/framework/frame.h"
#include "lib/framework/strres.h"
#include "lib/framework/frameresource.h"
#include "message.h"
#include "messagedef.h"
#include "messagely.h"
#include "text.h"

extern void yyerror(const char *msg);
void yyerror(const char *msg)
{
    debug(LOG_ERROR, "SMSG file parse error:\n%s at line %d\nText: '%s'", msg, message_get_lineno(), message_get_text());
}

typedef struct TEXT_MESSAGE
{
    char *str;
    struct TEXT_MESSAGE *psNext;
} TEXT_MESSAGE;

static void freeTextMessageList(TEXT_MESSAGE *list)
{
    while (list)
    {
        TEXT_MESSAGE *const toDelete = list;
        list = list->psNext;
        free(toDelete->str);
        free(toDelete);
    }
}

typedef struct VIEWDATAMESSAGE
{
    VIEWDATA view;
    struct VIEWDATAMESSAGE *psNext;
} VIEWDATAMESSAGE;

static void freeViewDataMessageList(VIEWDATAMESSAGE *list)
{
    while (list)
    {
        VIEWDATAMESSAGE *const toDelete = list;
        list = list->psNext;
        free(toDelete->view.pName);
        free(toDelete->view.ppTextMsg);
        switch (toDelete->view.type)
        {
            case VIEW_RES:
                {
                    VIEW_RESEARCH *const psViewRes = toDelete->view.pData;
                    if (psViewRes->pAudio)
                    {
                        free(psViewRes->pAudio);
                    }
                    free(psViewRes);
                    break;
                }
            default:
                ASSERT(!"Unhandled view data type", "Unhandled view data type %u", toDelete->view.type);
        }
        free(toDelete);
    }
}

#define YYPARSE_PARAM ppsViewData



/* Line 189 of yacc.c  */
#line 154 "message_parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/* Put the tokens into the symbol table, so that GDB and other debuggers
   know about them.  */
enum yytokentype
{
    TEXT_T = 258,
    QTEXT_T = 259,
    NULL_T = 260,
    VIEW_T_RES = 261,
    VIEW_T_RPL = 262,
    VIEW_T_PROX = 263,
    VIEW_T_RPLX = 264,
    VIEW_T_BEACON = 265,
    IMD_NAME_T = 266,
    IMD_NAME2_T = 267,
    SEQUENCE_NAME_T = 268,
    AUDIO_NAME_T = 269
};
#endif
/* Tokens.  */
#define TEXT_T 258
#define QTEXT_T 259
#define NULL_T 260
#define VIEW_T_RES 261
#define VIEW_T_RPL 262
#define VIEW_T_PROX 263
#define VIEW_T_RPLX 264
#define VIEW_T_BEACON 265
#define IMD_NAME_T 266
#define IMD_NAME2_T 267
#define SEQUENCE_NAME_T 268
#define AUDIO_NAME_T 269




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

    /* Line 214 of yacc.c  */
#line 94 "message_parser.y"

    char                   *sval;
    struct VIEWDATAMESSAGE *viewdatamsg;
    struct TEXT_MESSAGE    *txtmsg;
    VIEW_TYPE               viewtype;
    VIEW_RESEARCH          *researchdata;
    struct
    {
        const char    **stringArray;
        unsigned int    count;
    }                       msg_list;



    /* Line 214 of yacc.c  */
#line 233 "message_parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 245 "message_parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
int yyi;
#endif
{
    return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
/* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
/* The OS might guarantee only one guard page at the bottom of the stack,
   and a page size can be as small as 4096 bytes.  So we cannot safely
   invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
   to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
    yytype_int16 yyss_alloc;
    YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_int8_tS(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   45

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  23
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  23
/* YYNRULES -- Number of states.  */
#define YYNSTATES  51

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   269

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
    0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    21,    22,     2,     2,    16,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,    18,
    2,    19,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,    20,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,    15,     2,    17,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
    5,     6,     7,     8,     9,    10,    11,    12,    13,    14
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
    0,     0,     3,     5,     7,    10,    18,    27,    31,    33,
    37,    39,    43,    45,    49,    51,    53,    55,    59,    61,
    64,    68,    70,    72
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
    24,     0,    -1,    25,    -1,    26,    -1,    26,    25,    -1,
    3,    15,    33,    16,    27,    17,    18,    -1,    28,    16,
    29,    16,    30,    16,    31,    16,    -1,    11,    19,     4,
    -1,     4,    -1,    12,    19,    32,    -1,    32,    -1,    13,
    19,     4,    -1,     4,    -1,    14,    19,    32,    -1,    32,
    -1,     4,    -1,     5,    -1,    15,    34,    17,    -1,    35,
    -1,    35,    16,    -1,    35,    16,    34,    -1,     3,    -1,
    4,    -1,    20,    21,     4,    22,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
    0,   153,   153,   195,   196,   203,   227,   274,   275,   278,
    279,   282,   283,   286,   287,   290,   291,   294,   347,   349,
    350,   357,   379,   393
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
    "$end", "error", "$undefined", "TEXT_T", "QTEXT_T", "NULL_T",
    "VIEW_T_RES", "VIEW_T_RPL", "VIEW_T_PROX", "VIEW_T_RPLX",
    "VIEW_T_BEACON", "IMD_NAME_T", "IMD_NAME2_T", "SEQUENCE_NAME_T",
    "AUDIO_NAME_T", "'{'", "','", "'}'", "';'", "'='", "'_'", "'('", "')'",
    "$accept", "file", "all_messages", "message", "research_message",
    "imd_name", "imd_name2", "sequence_name", "audio_name",
    "optional_string", "message_list", "text_messages", "text_message", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
    0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
    265,   266,   267,   268,   269,   123,    44,   125,    59,    61,
    95,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
    0,    23,    24,    25,    25,    26,    27,    28,    28,    29,
    29,    30,    30,    31,    31,    32,    32,    33,    34,    34,
    34,    35,    35,    35
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
    0,     2,     1,     1,     2,     7,     8,     3,     1,     3,
    1,     3,     1,     3,     1,     1,     1,     3,     1,     2,
    3,     1,     1,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
    0,     0,     0,     2,     3,     0,     1,     4,     0,     0,
    21,    22,     0,     0,    18,     0,     0,    17,    19,     8,
    0,     0,     0,     0,    20,     0,     0,     0,    23,     7,
    5,    15,    16,     0,     0,    10,     0,     0,     9,    12,
    0,     0,     0,     0,    11,     0,     0,    14,     0,     6,
    13
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
    -1,     2,     3,     4,    21,    22,    34,    41,    46,    35,
    9,    13,    14
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -35
static const yytype_int8 yypact[] =
{
    13,     4,    20,   -35,    13,     7,   -35,   -35,    -3,     5,
    -35,   -35,     8,     9,    11,     1,    19,   -35,    -3,   -35,
    12,    15,    14,    16,   -35,    21,    10,     6,   -35,   -35,
    -35,   -35,   -35,    17,    18,   -35,     3,     2,   -35,   -35,
    22,    23,    29,    -1,   -35,    24,    26,   -35,     3,   -35,
    -35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -35,   -35,    31,   -35,   -35,   -35,   -35,   -35,   -35,   -34,
    -35,    27,   -35
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
    10,    11,    38,    31,    32,    19,    39,    31,    32,    47,
    31,    32,    20,    45,    50,    40,     1,    12,    33,     5,
    6,    15,     8,    23,     0,    29,    17,    18,    30,    16,
    27,    25,    26,    44,    37,     7,    36,     0,    28,    43,
    0,    42,    49,    48,     0,    24
};

static const yytype_int8 yycheck[] =
{
    3,     4,    36,     4,     5,     4,     4,     4,     5,    43,
    4,     5,    11,    14,    48,    13,     3,    20,    12,    15,
    0,    16,    15,     4,    -1,     4,    17,    16,    18,    21,
    16,    19,    17,     4,    16,     4,    19,    -1,    22,    16,
    -1,    19,    16,    19,    -1,    18
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
    0,     3,    24,    25,    26,    15,     0,    25,    15,    33,
    3,     4,    20,    34,    35,    16,    21,    17,    16,     4,
    11,    27,    28,     4,    34,    19,    17,    16,    22,     4,
    18,     4,     5,    12,    29,    32,    19,    16,    32,     4,
    13,    30,    19,    16,     4,    14,    31,    32,    19,    16,
    32
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const *const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
FILE *yyoutput;
int yytype;
YYSTYPE const *const yyvaluep;
#endif
{
    if (!yyvaluep)
    {
        return;
    }
# ifdef YYPRINT
    if (yytype < YYNTOKENS)
    {
        YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
    }
# else
    YYUSE (yyoutput);
# endif
    switch (yytype)
    {
        default:
            break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const *const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
FILE *yyoutput;
int yytype;
YYSTYPE const *const yyvaluep;
#endif
{
    if (yytype < YYNTOKENS)
    {
        YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
    }
    else
    {
        YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);
    }

    yy_symbol_value_print (yyoutput, yytype, yyvaluep);
    YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
yytype_int16 *yybottom;
yytype_int16 *yytop;
#endif
{
    YYFPRINTF (stderr, "Stack now");
    for (; yybottom <= yytop; yybottom++)
    {
        int yybot = *yybottom;
        YYFPRINTF (stderr, " %d", yybot);
    }
    YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
YYSTYPE *yyvsp;
int yyrule;
#endif
{
    int yynrhs = yyr2[yyrule];
    int yyi;
    unsigned long int yylno = yyrline[yyrule];
    YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
               yyrule - 1, yylno);
    /* The symbols being reduced.  */
    for (yyi = 0; yyi < yynrhs; yyi++)
    {
        YYFPRINTF (stderr, "   $%d = ", yyi + 1);
        yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
                         &(yyvsp[(yyi + 1) - (yynrhs)])
                        );
        YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_int8_tS (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
const char *yystr;
#endif
{
    YYSIZE_T yylen;
    for (yylen = 0; yystr[yylen]; yylen++)
    {
        continue;
    }
    return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
char *yydest;
const char *yysrc;
#endif
{
    char *yyd = yydest;
    const char *yys = yysrc;

    while ((*yyd++ = *yys++) != '\0')
    {
        continue;
    }

    return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
    if (*yystr == '"')
    {
        YYSIZE_T yyn = 0;
        char const *yyp = yystr;

        for (;;)
            switch (*++yyp)
            {
                case '\'':
                case ',':
                    goto do_not_strip_quotes;

                case '\\':
                    if (*++yyp != '\\')
                    {
                        goto do_not_strip_quotes;
                    }
                    /* Fall through.  */
                default:
                    if (yyres)
                    {
                        yyres[yyn] = *yyp;
                    }
                    yyn++;
                    break;

                case '"':
                    if (yyres)
                    {
                        yyres[yyn] = '\0';
                    }
                    return yyn;
            }
do_not_strip_quotes:
        ;
    }

    if (! yyres)
    {
        return yystrlen (yystr);
    }

    return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
    int yyn = yypact[yystate];

    if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    {
        return 0;
    }
    else
    {
        int yytype = YYTRANSLATE (yychar);
        YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
        YYSIZE_T yysize = yysize0;
        YYSIZE_T yysize1;
        int yysize_overflow = 0;
        enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
        char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
        int yyx;

# if 0
        /* This is so xgettext sees the translatable formats that are
        constructed on the fly.  */
        YY_("syntax error, unexpected %s");
        YY_("syntax error, unexpected %s, expecting %s");
        YY_("syntax error, unexpected %s, expecting %s or %s");
        YY_("syntax error, unexpected %s, expecting %s or %s or %s");
        YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
        char *yyfmt;
        char const *yyf;
        static char const yyunexpected[] = "syntax error, unexpected %s";
        static char const yyexpecting[] = ", expecting %s";
        static char const yyor[] = " or %s";
        char yyformat[sizeof yyunexpected
                      + sizeof yyexpecting - 1
                      + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
                         * (sizeof yyor - 1))];
        char const *yyprefix = yyexpecting;

        /* Start YYX at -YYN if negative to avoid negative indexes in
        YYCHECK.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;

        /* Stay within bounds of both yycheck and yytname.  */
        int yychecklim = YYLAST - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        int yycount = 1;

        yyarg[0] = yytname[yytype];
        yyfmt = yystpcpy (yyformat, yyunexpected);

        for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
            {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                {
                    yycount = 1;
                    yysize = yysize0;
                    yyformat[sizeof yyunexpected - 1] = '\0';
                    break;
                }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                yysize_overflow |= (yysize1 < yysize);
                yysize = yysize1;
                yyfmt = yystpcpy (yyfmt, yyprefix);
                yyprefix = yyor;
            }

        yyf = YY_(yyformat);
        yysize1 = yysize + yystrlen (yyf);
        yysize_overflow |= (yysize1 < yysize);
        yysize = yysize1;

        if (yysize_overflow)
        {
            return YYSIZE_MAXIMUM;
        }

        if (yyresult)
        {
            /* Avoid sprintf, as that infringes on the user's name space.
               Don't have undefined behavior even if the translation
               produced a string with the wrong number of "%s"s.  */
            char *yyp = yyresult;
            int yyi = 0;
            while ((*yyp = *yyf) != '\0')
            {
                if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
                {
                    yyp += yytnamerr (yyp, yyarg[yyi++]);
                    yyf += 2;
                }
                else
                {
                    yyp++;
                    yyf++;
                }
            }
        }
        return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
const char *yymsg;
int yytype;
YYSTYPE *yyvaluep;
#endif
{
    YYUSE (yyvaluep);

    if (!yymsg)
    {
        yymsg = "Deleting";
    }
    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
    {
        case 3: /* "TEXT_T" */

            /* Line 1000 of yacc.c  */
#line 120 "message_parser.y"
            {
#ifndef WZ_OS_WIN
                // Force type checking by the compiler
                char *const s = (yyvaluep->sval);

                if (s)
                {
                    free(s);
                }
#endif
            };

            /* Line 1000 of yacc.c  */
#line 1182 "message_parser.c"
            break;
        case 4: /* "QTEXT_T" */

            /* Line 1000 of yacc.c  */
#line 120 "message_parser.y"
            {
#ifndef WZ_OS_WIN
                // Force type checking by the compiler
                char *const s = (yyvaluep->sval);

                if (s)
                {
                    free(s);
                }
#endif
            };

            /* Line 1000 of yacc.c  */
#line 1199 "message_parser.c"
            break;
        case 25: /* "all_messages" */

            /* Line 1000 of yacc.c  */
#line 147 "message_parser.y"
            {
                freeViewDataMessageList((yyvaluep->viewdatamsg));
            };

            /* Line 1000 of yacc.c  */
#line 1210 "message_parser.c"
            break;
        case 26: /* "message" */

            /* Line 1000 of yacc.c  */
#line 147 "message_parser.y"
            {
                freeViewDataMessageList((yyvaluep->viewdatamsg));
            };

            /* Line 1000 of yacc.c  */
#line 1221 "message_parser.c"
            break;
        case 27: /* "research_message" */

            /* Line 1000 of yacc.c  */
#line 134 "message_parser.y"
            {
                // Force type checking by the compiler
                VIEW_RESEARCH *const r = (yyvaluep->researchdata);

                if (r)
                {
                    if (r->pAudio)
                    {
                        free(r->pAudio);
                    }

                    free(r);
                }
            };

            /* Line 1000 of yacc.c  */
#line 1241 "message_parser.c"
            break;
        case 28: /* "imd_name" */

            /* Line 1000 of yacc.c  */
#line 120 "message_parser.y"
            {
#ifndef WZ_OS_WIN
                // Force type checking by the compiler
                char *const s = (yyvaluep->sval);

                if (s)
                {
                    free(s);
                }
#endif
            };

            /* Line 1000 of yacc.c  */
#line 1258 "message_parser.c"
            break;
        case 29: /* "imd_name2" */

            /* Line 1000 of yacc.c  */
#line 120 "message_parser.y"
            {
#ifndef WZ_OS_WIN
                // Force type checking by the compiler
                char *const s = (yyvaluep->sval);

                if (s)
                {
                    free(s);
                }
#endif
            };

            /* Line 1000 of yacc.c  */
#line 1275 "message_parser.c"
            break;
        case 30: /* "sequence_name" */

            /* Line 1000 of yacc.c  */
#line 120 "message_parser.y"
            {
#ifndef WZ_OS_WIN
                // Force type checking by the compiler
                char *const s = (yyvaluep->sval);

                if (s)
                {
                    free(s);
                }
#endif
            };

            /* Line 1000 of yacc.c  */
#line 1292 "message_parser.c"
            break;
        case 31: /* "audio_name" */

            /* Line 1000 of yacc.c  */
#line 120 "message_parser.y"
            {
#ifndef WZ_OS_WIN
                // Force type checking by the compiler
                char *const s = (yyvaluep->sval);

                if (s)
                {
                    free(s);
                }
#endif
            };

            /* Line 1000 of yacc.c  */
#line 1309 "message_parser.c"
            break;
        case 32: /* "optional_string" */

            /* Line 1000 of yacc.c  */
#line 120 "message_parser.y"
            {
#ifndef WZ_OS_WIN
                // Force type checking by the compiler
                char *const s = (yyvaluep->sval);

                if (s)
                {
                    free(s);
                }
#endif
            };

            /* Line 1000 of yacc.c  */
#line 1326 "message_parser.c"
            break;
        case 34: /* "text_messages" */

            /* Line 1000 of yacc.c  */
#line 130 "message_parser.y"
            {
                freeTextMessageList((yyvaluep->txtmsg));
            };

            /* Line 1000 of yacc.c  */
#line 1337 "message_parser.c"
            break;
        case 35: /* "text_message" */

            /* Line 1000 of yacc.c  */
#line 130 "message_parser.y"
            {
                freeTextMessageList((yyvaluep->txtmsg));
            };

            /* Line 1000 of yacc.c  */
#line 1348 "message_parser.c"
            break;

        default:
            break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

    int yyn;
    int yyresult;
    /* Lookahead token as an internal (translated) token number.  */
    int yytoken;
    /* The variables used to return semantic value and location from the
       action routines.  */
    YYSTYPE yyval;

#if YYERROR_VERBOSE
    /* Buffer for error messages, and its allocated size.  */
    char yymsgbuf[128];
    char *yymsg = yymsgbuf;
    YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

    /* The number of symbols on the RHS of the reduced rule.
       Keep to zero when no symbol should be popped.  */
    int yylen = 0;

    yytoken = 0;
    yyss = yyssa;
    yyvs = yyvsa;
    yystacksize = YYINITDEPTH;

    YYDPRINTF ((stderr, "Starting parse\n"));

    yystate = 0;
    yyerrstatus = 0;
    yynerrs = 0;
    yychar = YYEMPTY; /* Cause a token to be read.  */

    /* Initialize stack pointers.
       Waste one element of value and location stack
       so that they stay on the same level as the state stack.
       The wasted elements are never initialized.  */
    yyssp = yyss;
    yyvsp = yyvs;

    goto yysetstate;

    /*------------------------------------------------------------.
    | yynewstate -- Push a new state, which is found in yystate.  |
    `------------------------------------------------------------*/
yynewstate:
    /* In all cases, when you get here, the value and location stacks
       have just been pushed.  So pushing a state here evens the stacks.  */
    yyssp++;

yysetstate:
    *yyssp = yystate;

    if (yyss + yystacksize - 1 <= yyssp)
    {
        /* Get the current used size of the three stacks, in elements.  */
        YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
        {
            /* Give user a chance to reallocate the stack.  Use copies of
               these so that the &'s don't force the real ones into
               memory.  */
            YYSTYPE *yyvs1 = yyvs;
            yytype_int16 *yyss1 = yyss;

            /* Each stack pointer address is followed by the size of the
               data in use in that stack, in bytes.  This used to be a
               conditional around just the two extra args, but that might
               be undefined if yyoverflow is a macro.  */
            yyoverflow (YY_("memory exhausted"),
                        &yyss1, yysize * sizeof (*yyssp),
                        &yyvs1, yysize * sizeof (*yyvsp),
                        &yystacksize);

            yyss = yyss1;
            yyvs = yyvs1;
        }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
        goto yyexhaustedlab;
# else
        /* Extend the stack our own way.  */
        if (YYMAXDEPTH <= yystacksize)
        {
            goto yyexhaustedlab;
        }
        yystacksize *= 2;
        if (YYMAXDEPTH < yystacksize)
        {
            yystacksize = YYMAXDEPTH;
        }

        {
            yytype_int16 *yyss1 = yyss;
            union yyalloc *yyptr =
                        (union yyalloc *) YYSTACK_ALLOC (YYSTACK_int8_tS (yystacksize));
            if (! yyptr)
        {
                goto yyexhaustedlab;
            }
            YYSTACK_RELOCATE (yyss_alloc, yyss);
            YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
            if (yyss1 != yyssa)
            {
                YYSTACK_FREE (yyss1);
            }
        }
# endif
#endif /* no yyoverflow */

        yyssp = yyss + yysize - 1;
        yyvsp = yyvs + yysize - 1;

        YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                    (unsigned long int) yystacksize));

        if (yyss + yystacksize - 1 <= yyssp)
        {
            YYABORT;
        }
    }

    YYDPRINTF ((stderr, "Entering state %d\n", yystate));

    if (yystate == YYFINAL)
    {
        YYACCEPT;
    }

    goto yybackup;

    /*-----------.
    | yybackup.  |
    `-----------*/
yybackup:

    /* Do appropriate processing given the current state.  Read a
       lookahead token if we need one and don't already have one.  */

    /* First try to decide what to do without reference to lookahead token.  */
    yyn = yypact[yystate];
    if (yyn == YYPACT_NINF)
    {
        goto yydefault;
    }

    /* Not known => get a lookahead token if don't already have one.  */

    /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
    if (yychar == YYEMPTY)
    {
        YYDPRINTF ((stderr, "Reading a token: "));
        yychar = YYLEX;
    }

    if (yychar <= YYEOF)
    {
        yychar = yytoken = YYEOF;
        YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
    else
    {
        yytoken = YYTRANSLATE (yychar);
        YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
        goto yydefault;
    }
    yyn = yytable[yyn];
    if (yyn <= 0)
    {
        if (yyn == 0 || yyn == YYTABLE_NINF)
        {
            goto yyerrlab;
        }
        yyn = -yyn;
        goto yyreduce;
    }

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus)
    {
        yyerrstatus--;
    }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the shifted token.  */
    yychar = YYEMPTY;

    yystate = yyn;
    *++yyvsp = yylval;

    goto yynewstate;


    /*-----------------------------------------------------------.
    | yydefault -- do the default action for the current state.  |
    `-----------------------------------------------------------*/
yydefault:
    yyn = yydefact[yystate];
    if (yyn == 0)
    {
        goto yyerrlab;
    }
    goto yyreduce;


    /*-----------------------------.
    | yyreduce -- Do a reduction.  |
    `-----------------------------*/
yyreduce:
    /* yyn is the number of a rule to reduce with.  */
    yylen = yyr2[yyn];

    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  Assigning to YYVAL
       unconditionally makes the parser a bit smaller, and it avoids a
       GCC warning that YYVAL may be used uninitialized.  */
    yyval = yyvsp[1-yylen];


    YY_REDUCE_PRINT (yyn);
    switch (yyn)
    {
        case 2:

            /* Line 1455 of yacc.c  */
#line 154 "message_parser.y"
            {
                unsigned int numData = 0, i;
                VIEWDATAMESSAGE *curMsg;
                VIEWDATA *psViewData;

                for (curMsg = (yyvsp[(1) - (1)].viewdatamsg); curMsg != NULL; curMsg = curMsg->psNext)
                {
                    ++numData;
                }

                ASSERT(numData <= uint8_t_MAX, "loadViewData: Didn't expect %d (or more) viewData messages (got %u)!", uint8_t_MAX, numData);
                if (numData > uint8_t_MAX)
                {
                    freeViewDataMessageList((yyvsp[(1) - (1)].viewdatamsg));
                    YYABORT;
                }

                psViewData = malloc(numData * sizeof(*psViewData));
                if (psViewData == NULL)
                {
                    debug(LOG_ERROR, "Out of memory");
                    abort();
                    freeViewDataMessageList((yyvsp[(1) - (1)].viewdatamsg));
                    YYABORT;
                }

                curMsg = (yyvsp[(1) - (1)].viewdatamsg);
                for (i = 0; i < numData; ++i)
                {
                    VIEWDATAMESSAGE *const toMove = curMsg;
                    assert(toMove != NULL);
                    curMsg = curMsg->psNext;
                    memcpy(&psViewData[i], &toMove->view, sizeof(psViewData[i]));
                    free(toMove);
                }

                addToViewDataList(psViewData, numData);
                *(VIEWDATA **)ppsViewData = psViewData;
            }
            break;

        case 4:

            /* Line 1455 of yacc.c  */
#line 197 "message_parser.y"
            {
                (yyvsp[(1) - (2)].viewdatamsg)->psNext = (yyvsp[(2) - (2)].viewdatamsg);
                (yyval.viewdatamsg) = (yyvsp[(1) - (2)].viewdatamsg);
            }
            break;

        case 5:

            /* Line 1455 of yacc.c  */
#line 204 "message_parser.y"
            {
                (yyval.viewdatamsg) = malloc(sizeof(*(yyval.viewdatamsg)));
                if ((yyval.viewdatamsg) == NULL)
                {
                    debug(LOG_ERROR, "Out of memory");
                    abort();
                    free((yyvsp[(1) - (7)].sval));
                    free((yyvsp[(3) - (7)].msg_list).stringArray);
                    if ((yyvsp[(5) - (7)].researchdata))
                    {
                        free((yyvsp[(5) - (7)].researchdata)->pAudio);
                    }
                    free((yyvsp[(5) - (7)].researchdata));
                    YYABORT;
                }

                (yyval.viewdatamsg)->view.pName = (yyvsp[(1) - (7)].sval);
                (yyval.viewdatamsg)->view.numText = (yyvsp[(3) - (7)].msg_list).count;
                (yyval.viewdatamsg)->view.ppTextMsg = (yyvsp[(3) - (7)].msg_list).stringArray;
                (yyval.viewdatamsg)->view.type = VIEW_RES;
                (yyval.viewdatamsg)->view.pData = (yyvsp[(5) - (7)].researchdata);
                (yyval.viewdatamsg)->psNext = NULL;
            }
            break;

        case 6:

            /* Line 1455 of yacc.c  */
#line 228 "message_parser.y"
            {
                (yyval.researchdata) = malloc(sizeof(*(yyval.researchdata)));
                if ((yyval.researchdata) == NULL)
                {
                    debug(LOG_ERROR, "Out of memory");
                    abort();
                    free((yyvsp[(1) - (8)].sval));
                    free((yyvsp[(3) - (8)].sval));
                    free((yyvsp[(5) - (8)].sval));
                    free((yyvsp[(7) - (8)].sval));
                    YYABORT;
                }

                (yyval.researchdata)->pAudio = (yyvsp[(7) - (8)].sval);
                sstrcpy((yyval.researchdata)->sequenceName, (yyvsp[(5) - (8)].sval));
                // Get rid of our tokens ASAP (so that the free() lists on errors become shorter)
                free((yyvsp[(5) - (8)].sval));

                (yyval.researchdata)->pIMD = (iIMDShape *) resGetData("IMD", (yyvsp[(1) - (8)].sval));
                if ((yyval.researchdata)->pIMD == NULL)
                {
                    ASSERT(LOG_ERROR, "Cannot find PIE \"%s\"", (yyvsp[(1) - (8)].sval));
                    free((yyvsp[(1) - (8)].sval));
                    free((yyvsp[(3) - (8)].sval));
                    YYABORT;
                }
                free((yyvsp[(1) - (8)].sval));

                if ((yyvsp[(3) - (8)].sval))
                {
                    (yyval.researchdata)->pIMD2 = (iIMDShape *) resGetData("IMD", (yyvsp[(3) - (8)].sval));
                    if ((yyval.researchdata)->pIMD2 == NULL)
                    {
                        ASSERT(false, "Cannot find 2nd PIE \"%s\"", (yyvsp[(3) - (8)].sval));
                        free((yyvsp[(3) - (8)].sval));
                        YYABORT;
                    }
                    free((yyvsp[(3) - (8)].sval));
                }
                else
                {
                    (yyval.researchdata)->pIMD2 = NULL;
                }
            }
            break;

        case 7:

            /* Line 1455 of yacc.c  */
#line 274 "message_parser.y"
            { (yyval.sval) = (yyvsp[(3) - (3)].sval); }
            break;

        case 9:

            /* Line 1455 of yacc.c  */
#line 278 "message_parser.y"
            { (yyval.sval) = (yyvsp[(3) - (3)].sval); }
            break;

        case 11:

            /* Line 1455 of yacc.c  */
#line 282 "message_parser.y"
            { (yyval.sval) = (yyvsp[(3) - (3)].sval); }
            break;

        case 13:

            /* Line 1455 of yacc.c  */
#line 286 "message_parser.y"
            { (yyval.sval) = (yyvsp[(3) - (3)].sval); }
            break;

        case 17:

            /* Line 1455 of yacc.c  */
#line 295 "message_parser.y"
            {
                size_t bytes = 0;
                unsigned int i;
                TEXT_MESSAGE *psCur;
                char *stringStart;

                (yyval.msg_list).count = 0;

                // Compute the required space for all strings and an array of pointers to hold it
                for (psCur = (yyvsp[(2) - (3)].txtmsg); psCur != NULL; psCur = psCur->psNext)
                {
                    ++(yyval.msg_list).count;
                    bytes += sizeof(char *) + strlen(psCur->str) + 1;
                }

                ASSERT((yyval.msg_list).count <= MAX_DATA, "Too many text strings (%u) provided, with %u as maximum", (yyval.msg_list).count, (unsigned int)MAX_DATA);
                if ((yyval.msg_list).count > MAX_DATA)
                {
                    YYABORT;
                }

                if ((yyval.msg_list).count)
                {
                    (yyval.msg_list).stringArray = malloc(bytes);
                    if ((yyval.msg_list).stringArray == NULL)
                    {
                        debug(LOG_ERROR, "Out of memory");
                        abort();
                        freeTextMessageList((yyvsp[(2) - (3)].txtmsg));
                        YYABORT;
                    }

                    stringStart = (char *)&(yyval.msg_list).stringArray[(yyval.msg_list).count];
                    for (psCur = (yyvsp[(2) - (3)].txtmsg), i = 0;
                            stringStart && psCur != NULL && i < (yyval.msg_list).count;
                            psCur = psCur->psNext, ++i)
                    {
                        assert(&stringStart[strlen(psCur->str)] - (char *)(yyval.msg_list).stringArray < bytes);
                        (yyval.msg_list).stringArray[i] = strcpy(stringStart, psCur->str);
                        stringStart = &stringStart[strlen(psCur->str) + 1];
                    }
                }
                else
                {
                    (yyval.msg_list).stringArray = NULL;
                }

                // Clean up our tokens
                freeTextMessageList((yyvsp[(2) - (3)].txtmsg));
            }
            break;

        case 20:

            /* Line 1455 of yacc.c  */
#line 351 "message_parser.y"
            {
                (yyvsp[(1) - (3)].txtmsg)->psNext = (yyvsp[(3) - (3)].txtmsg);
                (yyval.txtmsg) = (yyvsp[(1) - (3)].txtmsg);
            }
            break;

        case 21:

            /* Line 1455 of yacc.c  */
#line 358 "message_parser.y"
            {
                const char *const msg = strresGetString(psStringRes, (yyvsp[(1) - (1)].sval));
                if (!msg)
                {
                    ASSERT(!"Cannot find string resource", "Cannot find the view data string with id \"%s\"", (yyvsp[(1) - (1)].sval));
                    free((yyvsp[(1) - (1)].sval));
                    YYABORT;
                }

                (yyval.txtmsg) = malloc(sizeof(*(yyval.txtmsg)));
                if ((yyval.txtmsg) == NULL)
                {
                    debug(LOG_ERROR, "Out of memory");
                    abort();
                    free((yyvsp[(1) - (1)].sval));
                    YYABORT;
                }

                (yyval.txtmsg)->str = (yyvsp[(1) - (1)].sval);
                (yyval.txtmsg)->psNext = NULL;
            }
            break;

        case 22:

            /* Line 1455 of yacc.c  */
#line 380 "message_parser.y"
            {
                (yyval.txtmsg) = malloc(sizeof(*(yyval.txtmsg)));
                if ((yyval.txtmsg) == NULL)
                {
                    debug(LOG_ERROR, "Out of memory");
                    abort();
                    free((yyvsp[(1) - (1)].sval));
                    YYABORT;
                }

                (yyval.txtmsg)->str = (yyvsp[(1) - (1)].sval);
                (yyval.txtmsg)->psNext = NULL;
            }
            break;

        case 23:

            /* Line 1455 of yacc.c  */
#line 394 "message_parser.y"
            {
                (yyval.txtmsg) = malloc(sizeof(*(yyval.txtmsg)));
                if ((yyval.txtmsg) == NULL)
                {
                    debug(LOG_ERROR, "Out of memory");
                    abort();
                    free((yyvsp[(3) - (4)].sval));
                    YYABORT;
                }

                (yyval.txtmsg)->str = (yyvsp[(3) - (4)].sval);
                (yyval.txtmsg)->psNext = NULL;
            }
            break;



            /* Line 1455 of yacc.c  */
#line 1944 "message_parser.c"
        default:
            break;
    }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

    YYPOPSTACK (yylen);
    yylen = 0;
    YY_STACK_PRINT (yyss, yyssp);

    *++yyvsp = yyval;

    /* Now `shift' the result of the reduction.  Determine what state
       that goes to, based on the state we popped back to and the rule
       number reduced by.  */

    yyn = yyr1[yyn];

    yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
    if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    {
        yystate = yytable[yystate];
    }
    else
    {
        yystate = yydefgoto[yyn - YYNTOKENS];
    }

    goto yynewstate;


    /*------------------------------------.
    | yyerrlab -- here on detecting error |
    `------------------------------------*/
yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus)
    {
        ++yynerrs;
#if ! YYERROR_VERBOSE
        yyerror (YY_("syntax error"));
#else
        {
            YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
            if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
            {
                YYSIZE_T yyalloc = 2 * yysize;
                if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
                {
                    yyalloc = YYSTACK_ALLOC_MAXIMUM;
                }
                if (yymsg != yymsgbuf)
                {
                    YYSTACK_FREE (yymsg);
                }
                yymsg = (char *) YYSTACK_ALLOC (yyalloc);
                if (yymsg)
                {
                    yymsg_alloc = yyalloc;
                }
                else
                {
                    yymsg = yymsgbuf;
                    yymsg_alloc = sizeof yymsgbuf;
                }
            }

            if (0 < yysize && yysize <= yymsg_alloc)
            {
                (void) yysyntax_error (yymsg, yystate, yychar);
                yyerror (yymsg);
            }
            else
            {
                yyerror (YY_("syntax error"));
                if (yysize != 0)
                {
                    goto yyexhaustedlab;
                }
            }
        }
#endif
    }



    if (yyerrstatus == 3)
    {
        /* If just tried and failed to reuse lookahead token after an
        error, discard it.  */

        if (yychar <= YYEOF)
        {
            /* Return failure if at end of input.  */
            if (yychar == YYEOF)
            {
                YYABORT;
            }
        }
        else
        {
            yydestruct ("Error: discarding",
                        yytoken, &yylval);
            yychar = YYEMPTY;
        }
    }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


    /*---------------------------------------------------.
    | yyerrorlab -- error raised explicitly by YYERROR.  |
    `---------------------------------------------------*/
yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (/*CONSTCOND*/ 0)
    {
        goto yyerrorlab;
    }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    YYPOPSTACK (yylen);
    yylen = 0;
    YY_STACK_PRINT (yyss, yyssp);
    yystate = *yyssp;
    goto yyerrlab1;


    /*-------------------------------------------------------------.
    | yyerrlab1 -- common code for both syntax error and YYERROR.  |
    `-------------------------------------------------------------*/
yyerrlab1:
    yyerrstatus = 3;	/* Each real token shifted decrements this.  */

    for (;;)
    {
        yyn = yypact[yystate];
        if (yyn != YYPACT_NINF)
        {
            yyn += YYTERROR;
            if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
                yyn = yytable[yyn];
                if (0 < yyn)
                {
                    break;
                }
            }
        }

        /* Pop the current state because it cannot handle the error token.  */
        if (yyssp == yyss)
        {
            YYABORT;
        }


        yydestruct ("Error: popping",
                    yystos[yystate], yyvsp);
        YYPOPSTACK (1);
        yystate = *yyssp;
        YY_STACK_PRINT (yyss, yyssp);
    }

    *++yyvsp = yylval;


    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

    yystate = yyn;
    goto yynewstate;


    /*-------------------------------------.
    | yyacceptlab -- YYACCEPT comes here.  |
    `-------------------------------------*/
yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /*-----------------------------------.
    | yyabortlab -- YYABORT comes here.  |
    `-----------------------------------*/
yyabortlab:
    yyresult = 1;
    goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
    /*-------------------------------------------------.
    | yyexhaustedlab -- memory exhaustion comes here.  |
    `-------------------------------------------------*/
yyexhaustedlab:
    yyerror (YY_("memory exhausted"));
    yyresult = 2;
    /* Fall through.  */
#endif

yyreturn:
    if (yychar != YYEMPTY)
        yydestruct ("Cleanup: discarding lookahead",
                    yytoken, &yylval);
    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    YYPOPSTACK (yylen);
    YY_STACK_PRINT (yyss, yyssp);
    while (yyssp != yyss)
    {
        yydestruct ("Cleanup: popping",
                    yystos[*yyssp], yyvsp);
        YYPOPSTACK (1);
    }
#ifndef yyoverflow
    if (yyss != yyssa)
    {
        YYSTACK_FREE (yyss);
    }
#endif
#if YYERROR_VERBOSE
    if (yymsg != yymsgbuf)
    {
        YYSTACK_FREE (yymsg);
    }
#endif
    /* Make sure YYID is used.  */
    return YYID (yyresult);
}



