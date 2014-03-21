/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_CHAT_CHAT_PARSER_TAB_H_INCLUDED
# define YY_CHAT_CHAT_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int chat_debug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     _T_EOF = 0,
     BRACKET_OPEN = 258,
     BRACKET_CLOSE = 259,
     SQ_BRACKET_OPEN = 260,
     SQ_BRACKET_CLOSE = 261,
     PIPE = 262,
     T_WORD = 263,
     R_POSSESSION = 264,
     _T_QM = 265,
     _T_EM = 266,
     _T_FULLSTOP = 267,
     _T_COLON = 268,
     _T_SEMICOLON = 269,
     _T_COMMA = 270,
     _T_A = 271,
     _T_AFFIRMATIVE = 272,
     _T_AFTER = 273,
     _T_ALLY = 274,
     _T_AM = 275,
     _T_AN = 276,
     _T_AND = 277,
     _T_ANY = 278,
     _T_ATTACK = 279,
     _T_ATTACKING = 280,
     _T_ARMY = 281,
     _T_BEACON = 282,
     _T_BUILDING = 283,
     _T_CANT = 284,
     _T_CENTER = 285,
     _T_DEAD = 286,
     _T_DERRICK = 287,
     _T_DO = 288,
     _T_DROP = 289,
     _T_FINE = 290,
     _T_FOR = 291,
     _T_FORCE = 292,
     _T_GET = 293,
     _T_GETTING = 294,
     _T_GO = 295,
     _T_GOING = 296,
     _T_GONNA = 297,
     _T_GOT = 298,
     _T_GREAT = 299,
     _T_HAVE = 300,
     _T_HAS = 301,
     _T_HELP = 302,
     _T_HOLD = 303,
     _T_I = 304,
     _T_IM = 305,
     _T_IS = 306,
     _T_LASSAT = 307,
     _T_LETS = 308,
     _T_ME = 309,
     _T_MORE = 310,
     _T_NEED = 311,
     _T_NO = 312,
     _T_NOW = 313,
     _T_OFCOURSE = 314,
     _T_OK = 315,
     _T_ON = 316,
     _T_PLACE = 317,
     _T_POSSESSION = 318,
     _T_POWER = 319,
     _T_PUMPING = 320,
     _T_PUT = 321,
     _T_READY = 322,
     _T_REQUIRE = 323,
     _T_ROGER = 324,
     _T_RUSH = 325,
     _T_SEC = 326,
     _T_SEE = 327,
     _T_SOME = 328,
     _T_STATUS = 329,
     _T_STOP = 330,
     _T_SURE = 331,
     _T_THANK_YOU = 332,
     _T_THANKS = 333,
     _T_THE = 334,
     _T_U = 335,
     _T_UNITS = 336,
     _T_VTOLS = 337,
     _T_WAIT = 338,
     _T_WHERE = 339,
     _T_YES = 340,
     _T_YOU = 341,
     R_PLAYER = 342,
     R_INTEGER = 343
   };
#endif
/* Tokens.  */
#define _T_EOF 0
#define BRACKET_OPEN 258
#define BRACKET_CLOSE 259
#define SQ_BRACKET_OPEN 260
#define SQ_BRACKET_CLOSE 261
#define PIPE 262
#define T_WORD 263
#define R_POSSESSION 264
#define _T_QM 265
#define _T_EM 266
#define _T_FULLSTOP 267
#define _T_COLON 268
#define _T_SEMICOLON 269
#define _T_COMMA 270
#define _T_A 271
#define _T_AFFIRMATIVE 272
#define _T_AFTER 273
#define _T_ALLY 274
#define _T_AM 275
#define _T_AN 276
#define _T_AND 277
#define _T_ANY 278
#define _T_ATTACK 279
#define _T_ATTACKING 280
#define _T_ARMY 281
#define _T_BEACON 282
#define _T_BUILDING 283
#define _T_CANT 284
#define _T_CENTER 285
#define _T_DEAD 286
#define _T_DERRICK 287
#define _T_DO 288
#define _T_DROP 289
#define _T_FINE 290
#define _T_FOR 291
#define _T_FORCE 292
#define _T_GET 293
#define _T_GETTING 294
#define _T_GO 295
#define _T_GOING 296
#define _T_GONNA 297
#define _T_GOT 298
#define _T_GREAT 299
#define _T_HAVE 300
#define _T_HAS 301
#define _T_HELP 302
#define _T_HOLD 303
#define _T_I 304
#define _T_IM 305
#define _T_IS 306
#define _T_LASSAT 307
#define _T_LETS 308
#define _T_ME 309
#define _T_MORE 310
#define _T_NEED 311
#define _T_NO 312
#define _T_NOW 313
#define _T_OFCOURSE 314
#define _T_OK 315
#define _T_ON 316
#define _T_PLACE 317
#define _T_POSSESSION 318
#define _T_POWER 319
#define _T_PUMPING 320
#define _T_PUT 321
#define _T_READY 322
#define _T_REQUIRE 323
#define _T_ROGER 324
#define _T_RUSH 325
#define _T_SEC 326
#define _T_SEE 327
#define _T_SOME 328
#define _T_STATUS 329
#define _T_STOP 330
#define _T_SURE 331
#define _T_THANK_YOU 332
#define _T_THANKS 333
#define _T_THE 334
#define _T_U 335
#define _T_UNITS 336
#define _T_VTOLS 337
#define _T_WAIT 338
#define _T_WHERE 339
#define _T_YES 340
#define _T_YOU 341
#define R_PLAYER 342
#define R_INTEGER 343



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 173 "chat_parser.y"

	BOOL				bval;
	SDWORD			ival;


/* Line 2058 of yacc.c  */
#line 241 "chat_parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE chat_lval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int chat_parse (void *YYPARSE_PARAM);
#else
int chat_parse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int chat_parse (void);
#else
int chat_parse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_CHAT_CHAT_PARSER_TAB_H_INCLUDED  */
