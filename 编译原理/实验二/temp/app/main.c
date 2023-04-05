/****************************************************/
/* File: main.c                                     */
/* Main program for TINY compiler                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "../inc/globals.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE TRUE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE TURE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE FALSE

#include "../inc/util.h"
#include "../inc/scan.h"

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE; 

int main( int argc, char * argv[] )
{ TreeNode * syntaxTree;
  char pgm[120]; /* source code file name */

  if (argc != 2)
  // 说明输入命令中没有tiny文件，应该输出提示以提示用户
    { fprintf(stderr,"usage: %s <filename>\n",argv[0]);
      exit(1);
    }
  // 将tiny文件的名字拷贝至pgm数组，并自动补全成完整的.tny文件
  strcpy(pgm,argv[1]) ;
  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".tny");

  // 打开该文件，并将该文件设置为只读模式
  source = fopen(pgm,"r");
  // source = fopen("C:\\Users\\86130\\Desktop\\temp\\bin\\tiny.tny", "r");
  if (source==NULL)
  // 打开失败
  { fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }

  // 正式进行TINY程序的词法分析
  listing = stdout; /* send listing to screen */
  fprintf(listing,"\nTINY COMPILATION: %s\n\n",pgm);

  while (getToken()!=ENDFILE);

  // 分析完毕，关闭程序
  fclose(source);
  // system("pause");
  return 0;
}

