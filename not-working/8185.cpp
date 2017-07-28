/*************************************************************************
 *                                                                       *
 *                     XVII Olimpiada Informatyczna                      *
 *                                                                       *
 *   Zadanie:  Kolej (KOL)                                               *
 *   Plik:     kolchk.cpp                                                *
 *   Autor:    Michal Pilipczuk, na podstawie szablonu Tomka Czajki      *
 *   Opis:     Program sprawdzajacy wyjscie zawodnika                    *
 *   Format:   ./kolchk.e in out_contestant out_jury                     *
 *                                                                       *
 *************************************************************************/

#include <assert.h>

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <stack>

#define TOKEN_STD       0
#define TOKEN_EOF      -1
#define TOKEN_EOLN     -2
#define TOKEN_DOS_EOLN -3
#define TOKEN_SPACE    -4
#define TOKEN_TAB      -5
#define MAX_TOKEN_LEN 512

class TextFile {
public:
  TextFile();
  ~TextFile();
  int init(const char *inFilename, const char *outFilename, const char *hintFilename);
  int getToken(char *token, int max_len);
  int readInt(int min,int max);
  int readEof();
  int error(char *message);
protected:
  FILE *in;
  FILE *hint;
private:
  FILE *out;
  int line;
  int tno;
  char *token2str(int token, char *t_str);
  int err(char *expectedtoken, char *readtoken);
};


#define WHITE(ch) (ch==' ' || ch=='\t' || ch==13 || ch==EOF)
#define WHITE_OR_EOLN(ch) (ch==' ' || ch=='\t' || ch==13 || ch=='\n' || ch==EOF)

// ----------------------------------------------------------------------

class InputChecker : public TextFile {
public:
  int check();
};


int main(int argc, char *argv[]) {
    InputChecker *ic;


    ic=new InputChecker;

    if (argc==4) {
      if (ic->init(argv[1],argv[3],argv[2])) {assert(0);
        fprintf(stderr,"can't open: %s\n",argv[1]);
        return 1;
      }
    } else {assert(0);
      fprintf(stderr,"no file names supplied\n");
      return 1;
    }

    ic->check();
    return 0;
}

//----
/**
 * Klasa wspomagaj�ca obs�ug� plik�w tekstowych
 */

/**
 * Konstruktor, inicjalizacja wewn�trznych struktur
 */
TextFile::TextFile() {
  in=NULL;
  out=NULL;
  hint=NULL;
  line=1;
  tno=0;
}

/**
 * Destruktor, zamkni�cie pliku
 */
TextFile::~TextFile() {
  if (in!=NULL) fclose(in);
  if (out!=NULL) fclose(out);
  if (hint!=NULL) fclose(hint);
}

/**
 * Otworzenie pliku
 * @param filename nazwa pliku
 */
int TextFile::init(const char *inFilename, const char *outFilename, const char *hintFilename) {
  if( strlen(inFilename)==0 || 0==strlen(outFilename) || 0==strlen(hintFilename) ) {
    return -1;
  }
  in = fopen(inFilename,"r");
  out = fopen(outFilename,"r");
  hint = fopen(hintFilename,"r");

  if( NULL==in || NULL==out || NULL == hint )
    return -1;

  return 0;
}

/**
 * Pobranie pojedynczego tokena z pliku
 * @param token wska�nik do miejsca na wynik
 * @param max_len maksymalna d�ugo�� wyniku
 */
int TextFile::getToken(char *token,int max_len) {
  int i;
  int ch;

  token[0]=0;

  // omijanie bialych znakow
  ch=EOF;
  if (!feof(out)) ch=fgetc(out);
  while(!feof(out) && WHITE_OR_EOLN(ch)) ch = fgetc(out);

  if (ch==EOF)   return TOKEN_EOF;

  i=0;
  while (!WHITE_OR_EOLN(ch)) {
    token[i]=ch;
    i++;
    if (feof(out)) break;
    if (i==max_len-1) break;
    ch=fgetc(out);
  }
  token[i]=0;

  tno++;
  return TOKEN_STD;
}

int TextFile::readInt(int min,int max) {
  char buf[10];
  int token=getToken(buf,sizeof(buf));
  if (token!=TOKEN_STD) {
    err("Int",token2str(token,buf));
  }
  // teraz trzeba policzyc wartosc
  long long value=0;
  int minus=0;
  int i,n;
  i=0;n=strlen(buf);
  if (buf[0]=='-') { i++; minus=1; }
  while (i<n) {
    if (buf[i]<'0' || buf[i]>'9')        err("Int",buf);
    if (buf[i]=='0' && value==0 && n!=1) err("Int",buf);
    value=value*10+(buf[i]-'0');
    if (value>LONG_MAX)  err("Int",buf);
    i++;
  }
  if (minus) value=-value;
  char int_desc[100];
  snprintf(int_desc,sizeof(int_desc),"Int[%d,%d]",min,max);
  if (value>max || value<min) err(int_desc,buf);
  return value;
}

int TextFile::readEof() {
  char buf[100];
  int token=getToken(buf,sizeof(buf));
  if (token!=TOKEN_EOF) err("EOF",token2str(token,buf));
  return 0;
}

char *TextFile::token2str(int token, char *t_str) {
  static char buf[1024];
  bzero(&buf,sizeof(buf));
  switch(token) {
  case TOKEN_STD:   strncpy(buf,t_str,sizeof(buf)-1); break;
  case TOKEN_EOF:   sprintf(buf,"EOF");   break;
  default:
    sprintf(buf,"UNKNOWN");
  }
  return (char *)&buf;
}

int TextFile::err(char *expectedtoken, char *readtoken) {  assert(0);
  fprintf(stdout,"Blad[linia:%d,token:%d] oczekiwano: %s, wczytano: %s\n",
    line,tno,expectedtoken,readtoken);
  exit(1);
}

int TextFile::error(char *message)
{
  assert(0);
     //fprintf(stdout,"BLAD[linia:%d] %s\n", line, message);
     exit(1);
}


// <MP>

#include <utility>

using namespace std;

int readIntUnchecked(FILE* f){
    int ret; 
    fscanf(f,"%d",&ret); 
    return ret;
}

int n, c=1;
vector<int> input;

int k_cont;
bool answer;

char contestant_answer[100];
char error_message[1000];

stack<int> st[2];

void flush(){
    int flag;
    for(;;){
        flag = 1;
        for(int t=0; t<2; t++) if (st[t].top()==c){
	    c++;
	    st[t].pop();
	    flag = 0;
	}
	if (flag) break;
    }
}
	
int InputChecker::check() {

  /* Input */
  n = readIntUnchecked(in);
  for (int i=0; i<n; i++) {
    int p = readIntUnchecked(in);
    input.push_back(p);
  }
  
  /* Jury's answer */
  answer = ('T'==fgetc(hint));
  
  /* Contestant's answer */
  getToken(contestant_answer,50);
  if (strcmp(contestant_answer,(answer?"TAK":"NIE"))!=0){
    sprintf(error_message,"Zla odpowiedz: oczekiwano odpowiedzi %s, wczytano %s.",
						    (answer?"TAK":"NIE"),
						    contestant_answer);
    error(error_message);  
  }
  if (answer){
    for (int i=0; i<2; i++) st[i].push(n+2);
  
    for (int i=0; i<n; i++){
      k_cont = readInt(1,2);
      st[k_cont-1].push(input[i]);
      flush();
    }
 
    /* Checking the answer */  
    if (c < n+1){
      sprintf(error_message,"Zla odpowiedz: podany ciag nie ustawia pociagow w odpowiedniej kolejnosci.");
      error(error_message);  
    }
  }
  
  readEof();
  return 0;
}
// </MP>
