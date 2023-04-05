/****************************************************/
/* File: scan.c                                     */
/* The scanner implementation for the TINY compiler */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "../inc/globals.h"
#include "../inc/util.h"
#include "../inc/scan.h"

/* states in scanner DFA */
typedef enum
   { START,INASSIGN,INCOMMENT,INNUM,INID,DONE,INGREATER,INLESSER,INSTR }
   StateType;

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

/* BUFLEN = length of the input buffer for
   source code lines */
#define BUFLEN 512

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar fetches the next non-blank character
   from lineBuf, reading in a new line if lineBuf is
   exhausted */
static int getNextChar(void)
{ if (!(linepos < bufsize))
  // 若当前行已经分析完毕，则进入该if语句的结构
  { lineno++;
    if (fgets(lineBuf,BUFLEN-1,source))
    // 读入下一行
    { if (EchoSource) 
		fprintf(listing,"%d: %s",lineno,lineBuf); // 输出当前行的原本语句信息
      bufsize = strlen(lineBuf); // 记录当前行的长度
      linepos = 0; // 初始化linepos以从当前行的第一个字符开始处理
      return lineBuf[linepos++]; // 返回读入行的第一个字符
    }
    else
    // 如果读入失败，则意味着已经到达了文件的末尾，将EOF_flag标志设为1，并返回EOF
    { EOF_flag = TRUE;
      return EOF;
    }
  }
  else return lineBuf[linepos++]; // 返回读入行的下一个字符
}

/* ungetNextChar backtracks one character
   in lineBuf */
static void ungetNextChar(void)
// 实现退格功能
{ if (!EOF_flag) linepos-- ;}

/* lookup table of reserved words */
// 关键字表
static struct
    { char* str;
      TokenType tok;
    } reservedWords[MAXRESERVED]
   = {{"true", MTRUE},{"false", MFALSE},{"or",OR},{"and",AND},{"not",NOT},
      {"if",IF},{"then",THEN},{"else",ELSE},{"end",END},{"int",INT},{"bool",BOOL},
      {"repeat",REPEAT},{"until",UNTIL},{"read",READ},{"string",STRING},{"do",DO},
      {"write",WRITE},{"while",WHILE}};

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */
static TokenType reservedLookup (char * s)
{ int i;
  for (i=0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))
      return reservedWords[i].tok;
  return ID;
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the 
 * next token in source file
 */
TokenType getToken(void)
{  /* index for storing into tokenString */
   int tokenStringIndex = 0;
   /* holds current token to be returned */
   TokenType currentToken;
   /* current state - always begins at START */
   StateType state = START;
   // state表示当前状态，且总是从 START 状态开始
   int save; // save文件表示当前字符是否有必要存储
   while (state != DONE)
   { int c = getNextChar(); // 逐字符读入
     save = TRUE; // 一开始假设有必要存储
     // DFA实现流程，当state等于DONE时，说明已经完成一次单词的读入
     switch (state)
     { case START:
         if (isdigit(c))
           state = INNUM;
         else if (isalpha(c))
           state = INID;
         else if (c == ':')
           state = INASSIGN;
         else if (c == '>')
           state = INGREATER;
         else if (c == '<')
           state = INLESSER;
         else if (c == '\'')
           state = INSTR;
         else if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))
           save = FALSE;
         else if (c == '{')
         { save = FALSE;
           state = INCOMMENT;
         }
         else
         { state = DONE;
           switch (c)
           { case EOF:
               save = FALSE;
               currentToken = ENDFILE;
               break;
             case '=':
               currentToken = EQ;
               break;
             case '+':
               currentToken = PLUS;
               break;
             case '-':
               currentToken = MINUS;
               break;
             case '*':
               currentToken = TIMES;
               break;
             case '/':
               currentToken = OVER;
               break;
             case '(':
               currentToken = LPAREN;
               break;
             case ')':
               currentToken = RPAREN;
               break;
             case ';':
               currentToken = SEMI;
               break;
             case ',':
               currentToken = COMMA;
               break;
             case '\'':
               currentToken = APO;
               break;
             default:
               currentToken = ERROR;
               break;
           }
         }
         break;
       case INCOMMENT:
         save = FALSE;
         if (c == EOF)
         { state = DONE;
           currentToken = ERROR;
         }
         else if (c == '}') 
			 state = START;
         break;
       case INASSIGN:
         state = DONE;
         if (c == '=')
           currentToken = ASSIGN;
         else
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           currentToken = ERROR;
         }
         break;
       case INNUM:
         if (!isdigit(c))
         if (!(isalpha(c)))
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = NUM;
         } else {
           ungetNextChar();
           save = FALSE;
           currentToken = ERROR;
         }
         break;
       case INID:
         if (!isalpha(c) && !isdigit(c))
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = ID;
         }
         break;
       case INGREATER:
         state = DONE;
         if (c == '=')
           currentToken = GE;
         else {
          ungetNextChar();
          save = FALSE;
          currentToken = GT;
         }
         break;
       case INLESSER:
         state = DONE;
         if (c == '=')
           currentToken = LE;
         else {
          ungetNextChar();
          save = FALSE;
          currentToken = LT;
         }
         break;
       case INSTR:
        if((c == EOF) || (c == '\n') || (c == '\r'))
        {
          save = FALSE;
          state = DONE;
          currentToken = ERROR;
        }
        else if(c == '\'')
        {
          currentToken = STR;
          state = DONE;
        }
          
        break;
       case DONE:
       default: /* should never happen */
         fprintf(listing,"Scanner Bug: state= %d\n",state);
         state = DONE;
         currentToken = ERROR;
         break;
     }
     if ((save) && (tokenStringIndex <= MAXTOKENLEN))
     // 进入该if的条件是save为1（即必要存储）且当前TOKEN长度不超过预定长度，则将该字符放到tokenString的尾部
       tokenString[tokenStringIndex++] = (char) c;
     if (state == DONE)
     // 说明已经完成了一次token的读入
     { tokenString[tokenStringIndex] = '\0';
       if (currentToken == ID)
       // 若进入该if语句，则说明该token可能为KEY
         currentToken = reservedLookup(tokenString);
     }
   }
   if (TraceScan) {
     fprintf(listing,"\t%d: ",lineno);
     printToken(currentToken,tokenString);
   }
   return currentToken;
} /* end getToken */

