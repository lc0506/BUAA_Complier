#ifndef GLOBAL_H
#define GLOBAL_H

//�궨��
#define MAX_OF_MIDCODE 3000
#define MAX_OF_SYMTABLE 1000
#define MAX_OF_FUNCTABLE 100
#define MAX_OF_STRTABLE 100
#define MAX_OF_ADDRTABLE 1000

#include <string>

using namespace std;

//Դ�ļ�������ļ�
//FileRW.cpp
extern string incode;
extern unsigned int ptrOFincode;

void getINcode(string file);


//�ʷ�����
//GetSym.cpp
enum Symbol_type{
    CONSTSYM,INTSYM,CHARSYM,VOIDSYM,MAINSYM,
    IFSYM,WHILESYM,SWITCHSYM,CASESYM,
    DEFAULTSYM,SCANFSYM,PRINTFSYM,RETURNSYM,
    PLUSYM,MINUSYM,TIMESYM,DIVSYM,
    LESSYM,LEQSYM,GTRSYM,GEQSYM,ASGSYM,EQLSYM,NEQLSYM,
    COMMA,COLON,SEMICOLON,
    LPARENT,RPARENT,LBRACKET,RBRACKET,LBRACE,RBRACE,
    INTCON,CHARCON,STRCON,IDENTIFIER
};
extern int sym_count;//���ʼ�����
extern int line_count;//�кż�����
extern int column_count;//�кż�����
extern int sym_type;//�������ʵ�����
extern int sym_num;//�������ֵ�ֵ
extern char sym_char;//�����ַ���ֵ
extern string sym_str;//�����ַ�����ֵ
extern string sym_id;//�����ı�ʶ��
extern char ctemp;//�����char

void nextchar();
void insymbol();
void outsym();

//�﷨����
//Grammar.cpp
void program();
int sentence();
void expression();
void call_sent();
string int2string(int n);

//������
//Error.cpp
extern int err_num;
enum Error_type{
    LEADINGZERO,NEEDQUOTA,NEEDOUBLEQUOTA,NEEDEQUAL,ERRORCHAR,ERRORSYMBOL,
    NEEDINTCON,NEEDIDENTIFIER,NEEDCHARCON,NEEDINTORCHAR,ERRORDECLARE,
    NEEDUNSIGNEDINT,NEEDRBRACKET,NEEDSEMICOLON,NEEDLPARENT,NEEDRPARENT,
    NEEDLBRACE,NEEDRBRACE,NOMAINFUNC,EXTERNSYMBOL,NEEDCOLON,NEEDSENTENCE,
    ERRORFACTOR,NEEDCASE,REDECLARE,TABLEOVERFLOW,UNDECLAREIDENTIFIER,
    ERRORCONST,ERRORARRAYNUM,UNMATCHEDTYPE,ERRORVOIDCALL,UNMATCHEDPARNUM,
    UNMATCHEDPARTYPE,NORETURN,ERRORRETURNTYPE,ERRORSCANF,ERRORCONDITION,
    NEEDCONST,UNMATCHEDSWITCH,OUTOFINDEX,NEEDLBRACKET
};
void error(int n);
void skip(int n);

//���ű����
//Table.cpp
enum Table_kind{
    K_CONST,K_VAR,K_ARRAY,K_FUNCTION,K_PARAMENT
};
enum Table_type{
    T_INT,T_CHAR,T_VOID
};

typedef struct{
    string name;//������
    int kind;//��������,���������������顢���������
    int type;//����ֵ�����ͣ�int��char��void
    int value;//���ŵ�ֵ(������ֵ������Ĵ�С��(�����Ĳ�������)
//    int varnum;//��������&��ʱ�����ĸ���
}Symbol_ele;
////////������֮����ܻ�����޸�////////////////////////
typedef struct{
    string name;//������
    int addr;//�ڷ��ű��е��±�
    int parcount;//��������
    int varcount;//��������
}Func_ele;

extern Symbol_ele sym_table[MAX_OF_SYMTABLE];//���ű�ָ��
extern int ptrOFsymtable;
extern Func_ele func_table[MAX_OF_FUNCTABLE];
extern int ptrOFfunctable;
extern string str_table[MAX_OF_STRTABLE];//�ַ�����
extern int ptrOFstrtable;
extern int nowfunc_index;//��ǰ�����ڷ��ű�sym_table��ַ

int search_symtable(string _name,int _begin,int _end);
int search_functable(string _name);
void enter_symtable(string _name, int _kind, int _type, int _value);
void enter_functable(string _name, int _addr, int _parcount, int _varcount);
int enter_strtable(string _str);
void popsym_table();
void out_table();

//�����м����
//Midcode.cpp
typedef struct{
    string op;
    string var1;
    string var2;
    string var3;
}Midcode_ele;

extern Midcode_ele midcode_table[MAX_OF_MIDCODE];
extern int ptrOFmidcode;

void emit(string _op,string _var1,string _var2,string _var3);
string nextempvar();
string nextlabel();
void out_midcode();
void outMidcode(string file);

//����Ŀ�����
//Tomips.cpp
enum MipsType{
    PAR,VAR,ARY
};
typedef struct{
    string name;
    int type;
    int address;
    int ifglobal;
}Addrele;

extern Addrele Addr_table[MAX_OF_ADDRTABLE];
extern int ptrOFaddrtable;
extern int ifbetter;

int str2int(string _str);
void tomips(string _file);

//�Ż��м����
//Better.cpp




#endif // GLOBAL_H
