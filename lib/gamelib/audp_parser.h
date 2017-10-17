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

#ifndef YY_AUDP_AUDP_PARSER_TAB_H_INCLUDED
# define YY_AUDP_AUDP_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int audp_debug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/* Put the tokens into the symbol table, so that GDB and other debuggers
   know about them.  */
enum yytokentype
{
	FLOAT_T = 258,
	INTEGER = 259,
	QTEXT = 260,
	ONESHOT = 261,
	LOOP = 262,
	AUDIO = 263,
	ANIM3DFRAMES = 264,
	ANIM3DTRANS = 265,
	ANIM3DFILE = 266,
	AUDIO_MODULE = 267,
	ANIM_MODULE = 268,
	ANIMOBJECT = 269
};
#endif
/* Tokens.  */
#define FLOAT_T 258
#define INTEGER 259
#define QTEXT 260
#define ONESHOT 261
#define LOOP 262
#define AUDIO 263
#define ANIM3DFRAMES 264
#define ANIM3DTRANS 265
#define ANIM3DFILE 266
#define AUDIO_MODULE 267
#define ANIM_MODULE 268
#define ANIMOBJECT 269



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
	/* Line 2058 of yacc.c  */
#line 39 "audp_parser.y"

	float		fval;
	long		ival;
	bool            bval;
	char*		sval;


	/* Line 2058 of yacc.c  */
#line 93 "audp_parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE audp_lval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int audp_parse (void *YYPARSE_PARAM);
#else
int audp_parse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int audp_parse (void);
#else
int audp_parse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_AUDP_AUDP_PARSER_TAB_H_INCLUDED  */
