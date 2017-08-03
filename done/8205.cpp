/*
   weryfikator.h - pakiet funkcji do pisania weryfikatorow wejsc (inwer) i wyjsc (chk)
   Wersja: 1.1
   Autor: Piotr Niedzwiedz
   W razie problemow, bledow lub pomyslow na ulepszenie pisac na dzwiedziux@gmail.com

   Historia:
   1.1: Poprawka readInt (liczby ujemne).
*/

#ifndef __WERYFIKATOR_H
#define __WERYFIKATOR_H


#include <algorithm>
#include <assert.h>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <climits>

using namespace std;

class Reader;

enum Lang{
    EN=0,
    PL=1
};

/*************************************************************************
 *                 Scanner --- main class in the library                 *
 *************************************************************************/
class Scanner{
    protected:
        static const int realNumbersLimit=20;

        Lang lang;    
        Reader *reader;
        void(*end)(const char *msg, int line, int position);

        void readULL(unsigned long long int limit,unsigned long long int &val,bool &sign);
        void readLDB(long double limit,long double &val,bool &sign);

    public:
        /* ------------------------------ Interface ------------------------------ */
        Scanner(const char *file, Lang _lang=Lang(EN));
        Scanner(const char *file,void(*endf)(const char *msg, int line, int position), Lang _lang=Lang(EN));
        Scanner(FILE *input, Lang _lang=Lang(EN));
        Scanner(FILE *input,void(*endf)(const char *msg, int line, int position), Lang _lang=Lang(EN));
        ~Scanner();

        void                error(const char *msg);

        /* Skips all whitespaces until any occurrence of EOF or other non-white character. */
        int                 skipWhitespaces();
        /* Skips all whitespaces until any occurrence of EOF, EOLN or other non-white character. */
        int                 skipWhitespacesUntilEOLN();

        int                 readInt(int min_value=INT_MIN, int max_value=INT_MAX);  
        unsigned int        readUInt(unsigned int min_value=0, unsigned int max_value=UINT_MAX);    
        long long           readLL(long long int min_value=LLONG_MIN, long long int max_value=LLONG_MAX);   
        unsigned long long  readULL(unsigned long long int min_value=0, unsigned long long int max_value=ULLONG_MAX);   

        float               readFloat(float min_value,float max_value);
        double              readDouble(double min_value,double max_value);
        long double         readLDouble(long double min_value,long double max_value);

        char                readChar();

        /* Newline character is read, but isn't added to s */
        int                 readLine(char *s,int size);

        /* Reads a string until occurrence of EOF, EOLN or whitespace.
           Returns the number of characters read (possibly 0).
         */
        int                 readString(char *s,int size);

        void                readEof();
        void                readEofOrEoln();
        void                readEoln();
        void                readSpace();
        void                readTab();
    
        bool                isEOF();        
        /* -------------------------- End of interface --------------------------- */
};

class Reader{
    private:
        static const int bufferSize=1000;
        char Buffer[bufferSize];
        int head,tail;
        int line, position;
        void fillBuffer();
        FILE *input;
    public:
        Reader(const char *file);
        Reader(FILE *_input);
        ~Reader();
        bool isEOF();
        int getLine(){return line;}
        int getPosition(){return position;}
        char read(bool move=false);
};


const char* msgLeadingZeros[]={
    "Leading zeros",
    "Zera wiodace"};
const char* msgMinusZero[]={
    "Minus zero -0",
    "Minus zero -0"};
const char* msgNoNumber[]={
    "No number",
    "Brak liczby"};
const char* msgNoChar[]={
    "No char - EOF",
    "Brak znaku - EOF"};
const char* msgNotEof[]={
    "Not EOF",
    "Brak konca pliku"};
const char* msgNotEoln[]={
    "Not EOLN",
    "Brak konca linii"};
const char* msgNotEofOrEoln[]={
    "Not EOF or EOLN",
    "Brak konca linii i brak konca pliku"};
const char* msgNotSpace[]={
    "Not space",
    "Brak spacji"};
const char* msgNotTab[]={
    "Not tab",
    "Brak znaku tabulacji"};
const char* msgOutOfRangeInt[]={
    "Integer out of range",
    "Liczba calkowita spoza zakresu"};
const char* msgOutOfRangeReal[]={
    "Real number out of range",
    "Liczba rzeczywista spoza zakresu"};
const char* msgRealNumberLimit[]={
    "Too many digits after dot",
    "Za duzo cyfr po kropce dziesietnej"};
const char* msgBadRealNumberFormat[]={
    "Bad real number format",
    "Niepoprawny format liczby rzeczywistej"};

/* ---------------------------- Implementation ---------------------- */

typedef unsigned long long ull;
typedef unsigned int uint;
typedef long long ll;
typedef long double ldb;


inline bool isDot(char x){
    return x=='.';
}

inline bool isEOLN(char x){
    return x=='\n';
}

inline bool isMinus(char x){
    return x=='-';
}

inline bool isSpace(char x){
    return x==' ';
}

inline bool isTab(char x){
    return x=='\t';
}

inline bool isWhitespace(char x){
    return x==' ' || x=='\t' || x=='\n';
}

void endDefault(const char *msg, int line, int position){assert(0);
    printf("ERROR(line: %d, position: %d): %s\n",line, position, msg);
    exit(1);
}

/* ------------------ Reader's methods ------------------ */
Reader::Reader(const char *file){
    assert((input=fopen(file,"r"))!=NULL);
    head=tail=0;
    line=position=1;
}

Reader::Reader(FILE *_input){
    input=_input;
    head=tail=0;
    line=position=1;
}

Reader::~Reader(){
    assert(fclose(input)==0);   
}

void Reader::fillBuffer(){
    while ( (tail+1)%bufferSize != head ){
        int v=getc(input);
        if (v==EOF) break;
        Buffer[tail]=v;
        tail=(tail+1)%bufferSize;
    }
}   

bool Reader::isEOF(){
    fillBuffer();
    return head==tail;
}

char Reader::read(bool move){
    fillBuffer();
    assert((head!=tail)||(!move));
    if (head==tail) return 0;
    char v=Buffer[head];
    if(move){
        if(isEOLN(v)){
            line++;
            position=1;
        }
        else position++;
        head=(head+1)%bufferSize;
    }
    return v;
}

/* ----------------- Scanner's methods ------------------ */

Scanner::Scanner(const char *file, Lang _lang): lang(_lang){
    reader = new Reader(file);
    end = endDefault;
}

Scanner::Scanner(const char *file,void(*endf)(const char *msg, int line, int position), Lang _lang): lang(_lang){
    reader = new Reader(file);
    end = endf;
}

Scanner::Scanner(FILE *input, Lang _lang): lang(_lang){
    reader = new Reader(input);
    end = endDefault;
}

Scanner::Scanner(FILE *input,void(*endf)(const char *msg, int line, int position), Lang _lang): lang(_lang){
    reader = new Reader(input);
    end = endf;
}

Scanner::~Scanner(){
    delete reader;
}

void Scanner::error(const char *msg){
    int l = reader->getLine();
    int p = reader->getPosition();
    delete reader;
    (*end)(msg, l, p);
}

int Scanner::skipWhitespaces(){
    int result=0;
    while (isWhitespace(reader->read())){
        reader->read(1);
        result++;
    }
    return result;
}


int Scanner::skipWhitespacesUntilEOLN (){
    int result=0;
    while (isWhitespace(reader->read()) && !isEOLN(reader->read())){
        reader->read(1);
        result++;
    }
    return result;
}


/* INTEGERS */

int Scanner::readInt(int min_value,int max_value){
    return (int)readLL(min_value, max_value);
}

uint Scanner::readUInt(uint min_value,uint max_value){
    return (uint)readULL(min_value, max_value);
}

inline bool lower_equal(ull a, bool sign_a, ull b, bool sign_b){
    if(sign_a!=sign_b) return sign_a;
    if(sign_a) return a>=b;
    return a<=b;
}
inline ull spec_abs(ll x){
    if(x<0) return (-(x+1))+1;
    return x;
}

ll Scanner::readLL(ll min_value,ll max_value){
    assert(min_value<=max_value);
    bool sign;
    ull val;
    readULL(max(spec_abs(min_value),spec_abs(max_value)),val,sign);
    ll v=val; 
    if (!(lower_equal(spec_abs(min_value), min_value<0, v, sign) && 
          lower_equal(v, sign, spec_abs(max_value),max_value<0)))
        error(msgOutOfRangeInt[lang]);
    if(sign) v*=-1;
    return v;
}

ull Scanner::readULL(ull min_value,ull max_value){
    assert(min_value<=max_value);
    bool sign;
    ull val;
    readULL(max_value,val,sign);
    if (sign) error(msgOutOfRangeInt[lang]);
    if (!(min_value <= val))
        error(msgOutOfRangeInt[lang]);
    return val;
}

/* REAL NUMBERS */

float Scanner::readFloat(float min_value,float max_value){
    return (float)readLDouble(min_value, max_value);
}

double Scanner::readDouble(double min_value,double max_value){
    return (double)readLDouble(min_value, max_value);
}

long double Scanner::readLDouble(long double min_value,long double max_value){
    assert(min_value<=max_value);
    bool sign;
    ldb val;
    readLDB(max(fabsl(min_value),fabsl(max_value)),val,sign);
    if (sign) val*=-1;
    if (!(min_value <= val && val <= max_value))
        error(msgOutOfRangeReal[lang]);
    return val;
}


/* STRINGS */

int Scanner::readString(char *s,int size){
    int x=0;
    while ( x<size-1 && !isEOF() && !isWhitespace(reader->read()))
        s[x++]=reader->read(1);
    s[x]=0;
    return x;
}

int Scanner::readLine(char *s,int size){
    int x=0;
    while ( x<size-1 && !isEOLN(reader->read()) && !isEOF())
        s[x++]=reader->read(1);
    s[x]=0;
    if(isEOLN(reader->read())) reader->read(1);
    return x;
}

char Scanner::readChar(){
    if (reader->isEOF()) error(msgNoChar[lang]);
    return reader->read(1);
}

/* WHITESPACES */

void Scanner::readEof(){
    if (!reader->isEOF()) error(msgNotEof[lang]);
}

void Scanner::readEoln(){
    if (!isEOLN(reader->read())) error(msgNotEoln[lang]);
    reader->read(1);
}

    void Scanner::readEofOrEoln(){
        if (isEOLN(reader->read()))
            reader->read(1);
        else if ( !reader->isEOF())
            error(msgNotEofOrEoln[lang]);
    }


void Scanner::readSpace(){
    if (!isSpace(reader->read())) error(msgNotSpace[lang]);
    reader->read(1);
}

void Scanner::readTab(){
    if (!isTab(reader->read())) error(msgNotTab[lang]);
    reader->read(1);
}

bool Scanner::isEOF(){
    return reader->isEOF();
}


/* PROTECTED */

void Scanner::readULL(ull limit,ull &val,bool &sign){
    sign=0;
    val=0;
    sign=isMinus(reader->read());
    if (sign) reader->read(1);
    int zeros=0;
    int valDigits=0;    
    while ('0' == reader->read()){
        zeros++;
        valDigits++;
        reader->read(1);
        if (zeros > 1) error(msgLeadingZeros[lang]);
    }
    int limDigits=0;
    ull tmp=limit;
    while(tmp){ 
        limDigits++;
        tmp/=10;
    }
    if(!limDigits) limDigits=1;
    while(isdigit(reader->read())){
        valDigits++;
        if (valDigits > limDigits) error(msgOutOfRangeInt[lang]);
        char x=reader->read(1);
        if (valDigits == limDigits){
            if (limit/10 < val) error(msgOutOfRangeInt[lang]);
            if (limit/10 == val && limit%10 < (ull)(x-'0')) error(msgOutOfRangeInt[lang]);
        }
        val=val*10+x-'0';
    }
    if (val > 0 && zeros) error(msgLeadingZeros[lang]);
    if (sign && zeros) error(msgMinusZero[lang]);
    if(!valDigits) error(msgNoNumber[lang]);
}

void Scanner::readLDB(ldb limit,ldb &val,bool &sign){
    sign=0;
    val=0;
    sign=isMinus(reader->read());
    if (sign) reader->read(1);
    int zeros=0;
    int valDigits=0;
    while ('0' == reader->read()){
        zeros++;
        valDigits++;
        reader->read(1);
        if (zeros > 1) error(msgLeadingZeros[lang]);
    }
    if (zeros && isdigit(reader->read())) error(msgLeadingZeros[lang]);
    while(isdigit(reader->read())){
        valDigits++;
        char x=reader->read(1);
        val=val*10.0+x-'0';
    }
    if(!valDigits) error(msgNoNumber[lang]);
    if (isDot(reader->read())){
        reader->read(1);
        ldb dec=1;
        int dotDigits=0;
        while ( isdigit(reader->read()) ){
            dotDigits++;
            if (dotDigits > realNumbersLimit) error(msgRealNumberLimit[lang]);
            char x=reader->read(1);
            dec/=10.0;
            val+=dec*(x-'0');
        }       
        if (!dotDigits) error(msgBadRealNumberFormat[lang]);
    }
}

#endif


/*************************************************************************
 *                                                                       *
 *                    XVIII Olimpiada Informatyczna                      *
 *                                                                       *
 *   Zadanie:           Przekladanka                                     *
 *   Autor:             Miroslaw Michalski                               *
 *   Opis:              Program sprawdzajacy wyjscia                     *
 *                                                                       *
 *************************************************************************/

#include <vector>
#include <list>

#include <string.h>

using namespace std;

void my_error(const char* msg, int line, int pos)
{
    assert(0);
    exit(0);
}

bool isSorted(const list<int>  &data)
{
    int lastNumber = 0;
    for (typeof(data.begin()) it = data.begin() ; it != data.end() ; ++it) {
        if (lastNumber >= *it) {
            return false;
        }
        lastNumber = *it;
    }
    return true;
}

int main(int argc, char **argv){
    Scanner in_zad(argv[1]);
    Scanner out_zaw(argv[3], my_error, PL);
    Scanner out_nasz(argv[2]);

    list<int> Dane;
    int nDane;
    nDane = in_zad.readInt();
    in_zad.skipWhitespaces();
    for (int i = 0 ; i < nDane ; ++i) {
        Dane.push_back(in_zad.readInt());
        in_zad.skipWhitespaces();
    }

    char isSolvable = out_nasz.readChar();

    if (isSolvable == 'N') {
        const int sz = 100;
        char nie_[sz], da_[sz], sie_[sz];
        out_zaw.readString(nie_, sz);
        out_zaw.readSpace();
        out_zaw.readString(da_, sz);
        out_zaw.readSpace();
        out_zaw.readString(sie_, sz);
        out_zaw.skipWhitespaces();
        out_zaw.readEof();

        if (strcmp(nie_, "NIE") || strcmp(da_, "DA") || strcmp(sie_, "SIE")) {
            assert(0);
        } else {
            return 0;
        }
    }

    int amount = out_zaw.readInt(0, nDane * nDane);
    out_zaw.skipWhitespaces();
    
    char lastSign = 'x';
    for(int i = 0; i < amount; i++) {
        int num = out_zaw.readInt(0, nDane - 1);
        
        char sign = out_zaw.readChar();


        if (num == 0) {
            assert(0);
        }

        if (sign == 'a') {
            for (int i = 0 ; i < num ; i++) {
                int last = Dane.back();
                Dane.pop_back();
                Dane.push_front(last);
            }
        } else if (sign == 'b') {
            if (nDane <= 2) {
                assert(0);
                return 0;
            }
            num %= 3;
            for (int i = 0 ; i < num ; i++) {
                int first  = *(Dane.begin()); Dane.pop_front();
                int second = *(Dane.begin()); Dane.pop_front();
                int third  = *(Dane.begin()); Dane.pop_front();

                Dane.push_front(second);
                Dane.push_front(first);
                Dane.push_front(third);
            }
        } else {
assert(0);
        }

        if (lastSign == sign) {

assert(0);
        }

        lastSign = sign;
        out_zaw.skipWhitespaces();
    }
    out_zaw.readEof();

    if (!isSorted(Dane)) {
        assert(0);
    } 
    return 0;
}

