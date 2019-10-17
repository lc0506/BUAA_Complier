#include<iostream>
#include<sstream>
#include"Global.h"
/////////////////////////////////////////////////
int type_reg=0;//�Ĵ����͡�ָ����кţ����ڷ���
int inptr_reg=0;
char ctemp_reg='\0';
/////////////////////////////////////////////////
string str2mid[3]={"int","char","void"};
////////////////////////////////////////////////
int ifmain=0;//�ж��Ƿ���������
int ifreturn=0;//��¼�Ƿ���return
///////////////////////////////////////////////
int par_num=0;//��������
int var_num=0;//����/��ʱ��������
int now_functype=0;//�洢��ǰ�����ķ���ֵ����
//////////////////////////////////////////////
string last_var="";//�洢��һ�ε��ý���ı�����
int last_type=0;//�洢��һ��������������

string int2string(int n){
    stringstream newstr;
    newstr<<n;
    return newstr.str();
}
/*int read_int(){//������������ǰ���+/-��
    int flag=1;
    int flag2=0;//������з������������޷�������
    if(sym_type==PLUSYM){
        insymbol();
        flag2=1;
    }
    else if(sym_type==MINUSYM){
        flag=-1;
        insymbol();
        flag2=1;
    }
    if(sym_type==INTCON){
        sym_num=sym_num*flag;
        insymbol();
    }
    else{
        cout<<"need integer here!"<<endl;
        //skip();
        return -1;
    }
    if(flag2){
        return 1;
    }
    return 0;
    //cout<<line_count<<" is integer!"<<endl;
}*/

void con_def(){//��������
    int signflag=1;
    string name="";//��¼��ǰ������name
    int value=0;//��¼��ǰ������value
    if(sym_type==INTSYM){//���������
        do{
            insymbol();
            if(sym_type==IDENTIFIER){
                name=sym_id;//��������
                insymbol();
                if(sym_type==ASGSYM) insymbol();
                else error(NEEDEQUAL);//cout<<"here needs ="<<endl;
                signflag=1;
                if(sym_type==PLUSYM) insymbol();
                else if(sym_type==MINUSYM){
                    signflag=-1;
                    insymbol();
                }
                if(sym_type==INTCON){
                    value=sym_num*signflag;
                    insymbol();
                }
                else{
                    //cout<<"here needs int const!"<<endl;
                    error(NEEDINTCON);
                    value=0;
                }
                //��¼���ű�
                if(search_symtable(name,nowfunc_index,ptrOFsymtable)){
                    error(REDECLARE);
                }
                else{
                    enter_symtable(name,K_CONST,T_INT,value);
                }
            }
            else{
                //cout<<"here needs identifier!"<<endl;
                error(NEEDIDENTIFIER);
                skip(0);//skip to , or ;
            }
        }while(sym_type==COMMA);
    }
    else if(sym_type==CHARSYM){//������ַ�
        do{
            insymbol();
            if(sym_type==IDENTIFIER){
                name=sym_id;
                insymbol();
                if(sym_type==ASGSYM) insymbol();
                else error(NEEDEQUAL);//cout<<"here needs =!"<<endl;
                if(sym_type==CHARCON){
                    insymbol();
                    value=(int)sym_char;
                }
                else{
                    error(NEEDCHARCON);//cout<<"here needs char const!"<<endl;
                    value='a';//�ݴ���
                }
                //��¼���ű�
                if(search_symtable(name,nowfunc_index,ptrOFsymtable)){
                    error(REDECLARE);
                }
                else{
                    enter_symtable(name,K_CONST,T_CHAR,value);
                }
            }
            else{
                error(NEEDIDENTIFIER);//cout<<"here needs identifier!"<<endl;
                skip(0);//skip to , or ;
            }
        }while(sym_type==COMMA);
    }
    else{
        //cout<<"here needs int/char!"<<endl;
        error(NEEDINTORCHAR);
        skip(1);//skip to ;
    }
    //cout<<line_count<<" is const defination!"<<endl;
    if(sym_type!=SEMICOLON) error(NEEDSEMICOLON);//cout<<"need ; here!"<<endl;
    else insymbol();

}

void var_def(){
    string name="";//��¼��ǰ������name
    int value=0;//��¼��ǰ������value
    int type=0;//��¼��ǰ������ֵ����
    if(sym_type==INTSYM) type=T_INT;//�ȼ�¼����ʲôtype���Ա�֮�����
    else type=T_CHAR;
    do{
        insymbol();
        if(sym_type!=IDENTIFIER){
            //cout<<"here needs identifier!"<<endl;
            error(NEEDIDENTIFIER);
            skip(0);//skip to , or ;
            continue;
        }
        name=sym_id;
        insymbol();
        if(sym_type==LBRACKET){
            insymbol();
            /*if(sym_type==PLUSYM||sym_type==MINUSYM){
                //cout<<"need unsigned int!"<<endl;
                error(NEEDUNSIGNEDINT);
                insymbol();
            }
            if(sym_type!=INTCON){
                //cout<<"need integer here!"<<endl;
                error(NEEDUNSIGNEDINTINTCON);
                skip(0);//skip to , or ;
                continue;
            }*/
            if(sym_type!=INTCON){
                error(NEEDUNSIGNEDINT);
                skip(0);//skip to , or ;
                continue;
            }
            else if(sym_type==INTCON&&sym_num==0){
                error(NEEDUNSIGNEDINT);
                skip(0);//skip to , or ;
                continue;
            }
            value=sym_num;//value��ֵΪ�����С
            insymbol();
            if(sym_type!=RBRACKET){
                //cout<<"need ] here!"<<endl;
                error(NEEDRBRACKET);
            }
            else insymbol();
            //��¼���ű�
            if(search_symtable(name,nowfunc_index,ptrOFsymtable)){
                error(REDECLARE);
            }
            else {
                enter_symtable(name,K_ARRAY,type,value);
                //�����м����
                emit("ary",str2mid[type],int2string(value),name);
            }
        }
        else{
            if(search_symtable(name,nowfunc_index,ptrOFsymtable)){
                error(REDECLARE);
            }
            else{
                enter_symtable(name,K_VAR,type,0);
                //�����м����
                emit("var",str2mid[type]," ",name);
            }
        }
    }while(sym_type==COMMA);
    //cout<<line_count<<" is variable defination!"<<endl;
    if(sym_type!=SEMICOLON) error(NEEDSEMICOLON);//cout<<"need ; here!"<<endl;
    else insymbol();

}
void factor(){
    int loc=0;//��¼��ʶ���ڷ��ű��λ��
    int signflag=1;
    string name=" ";
    string var1=" ",var2=" ",var3=" ";
    if(sym_type==IDENTIFIER){
        name=sym_id;//��������ʶ��������
        loc=search_symtable(sym_id,1,ptrOFsymtable);
        /*insymbol();
        if(sym_type==LPARENT){
            call_sent();
        }*/
        if(loc==0){//�������ӱ�ʶ��δ����ı���
            error(UNDECLAREIDENTIFIER);
            last_type=T_INT;
            last_var="1";
            insymbol();
        }
        else if(sym_table[loc].kind==K_FUNCTION){
            call_sent();
            if(sym_table[loc].type==T_VOID){
                error(ERRORVOIDCALL);
                last_type=T_INT;//�ݴ���
                last_var="1";//�ݴ���
            }
            else{
                last_type=sym_table[loc].type;
                var3=nextempvar();
                emit("gtr"," "," ",var3);
                last_var=var3;
            }
        }
        else if(sym_table[loc].kind==K_ARRAY){
            insymbol();
            if(sym_type!=LBRACKET){
                error(NEEDLBRACKET);
            }
            else if(sym_type==LBRACKET){
                insymbol();
                expression();
                if(last_type!=T_INT){
                    error(ERRORARRAYNUM);//����������ͱ����ݴ���
                }
                else if(last_var[0]>='0'&&last_var[0]<='9'){
                    if(str2int(last_var)>=sym_table[loc].value){
                        error(OUTOFINDEX);
                    }
                }
                var3=nextempvar();
                var2=last_var;
                var1=name;
                emit("gta",var1,var2,var3);
                last_var=var3;
                last_type=sym_table[loc].type;
                if(sym_type!=RBRACKET){
                    //cout<<"here needs ]"<<endl;
                    error(NEEDRBRACKET);
                }
                else insymbol();
            }
        }
        else{//֮�󲹳䳣��������������
            if(sym_table[loc].kind==K_CONST){
                last_var=int2string(sym_table[loc].value);
                last_type=sym_table[loc].type;
            }
            else{
                last_var=sym_table[loc].name;
                last_type=sym_table[loc].type;
                var3=nextempvar();
                emit("asg",last_var," ",var3);
                last_var=var3;
            }
            insymbol();
        }
    }
    else if(sym_type==LPARENT){
        insymbol();
        expression();
        if(sym_type!=RPARENT){
            //cout<<"here needs )"<<endl;
            error(NEEDRPARENT);
        }
        else insymbol();
        //������������ʦ˵��������������Ժ��൱��ǿ��ת��Ϊint��
        last_type=T_INT;
    }
    else if(sym_type==PLUSYM||sym_type==MINUSYM){
        if(sym_type==MINUSYM) signflag=-1;
        insymbol();
        sym_num=sym_num*signflag;
        last_var=int2string(sym_num);
        last_type=T_INT;
        insymbol();
    }
    else if(sym_type==INTCON){
        last_var=int2string(sym_num);
        last_type=T_INT;
        insymbol();
    }
    else if(sym_type==CHARCON){
        last_var=int2string((int)sym_char);
        last_type=T_CHAR;
        insymbol();
    }
    else{
        //cout<<"wrong factor!"<<endl;
        error(ERRORFACTOR);
        //֮���ݴ���
        last_var="1";
        last_type=T_INT;
    }
    //cout<<line_count<<" is factor!"<<endl;
}
void term(){
    string var1=" ",var2=" ",var3=" ";
    int optype=0;
    int type=0;//�洢����ֵ����
    factor();
    type=last_type;
    var1=last_var;
    while(sym_type==TIMESYM||sym_type==DIVSYM){
        type=T_INT;//������������㣬��һ����int��
        if(sym_type==TIMESYM) optype=1;
        else optype=-1;
        insymbol();
        factor();
        var3=nextempvar();
        var2=last_var;
        if(optype==1) emit("mul",var1,var2,var3);
        else emit("div",var1,var2,var3);
        last_var=var3;
        var1=last_var;
    }
    last_type=type;
    //cout<<line_count<<" is term!"<<endl;
}
void expression(){
    string var1=" ",var2=" ",var3=" ";
    int optype=0;//�洢��������
    int type=0;//�洢���ʽ�������
    if(sym_type==PLUSYM||sym_type==MINUSYM){
        if(sym_type==MINUSYM) optype=-1;
        else optype=1;
        insymbol();
    }
    term();
    type=last_type;
    if(optype) type=T_INT;//ֻҪ���������ţ����ʽ��ֵǿ�Ʊ�Ϊint
    if(optype==-1){
        var1=last_var;
        var3=nextempvar();
        emit("mul",var1,"-1",var3);
        last_var=var3;
    }
    var1=last_var;
    while(sym_type==PLUSYM||sym_type==MINUSYM){
        type=T_INT;//ֻҪ�μ������㣬���ʽ��ֵǿ��ת��Ϊint
        if(sym_type==PLUSYM) optype=1;
        else optype=-1;
        insymbol();
        term();
        var3=nextempvar();
        var2=last_var;
        if(optype==1) emit("add",var1,var2,var3);
        else emit("mis",var1,var2,var3);
        last_var=var3;
        var1=last_var;
    }
    last_type=type;
    //cout<<line_count<<" is expression!"<<endl;
}
void condition(string _label){//���Ƿ��ŵģ���>������<=
    string var1=" ",var2=" ";
    int op=0;
    expression();
    if(last_type!=T_INT){
        error(ERRORCONDITION);
    }
    var1=last_var;
    if(sym_type>=LESSYM&&sym_type<=NEQLSYM){
        op=sym_type;//���������
        insymbol();
        expression();
        if(last_type!=T_INT){
            error(ERRORCONDITION);
        }
        var2=last_var;
        if(op==LESSYM) emit("bge",var1,var2,_label);
        else if(op==LEQSYM) emit("bgt",var1,var2,_label);
        else if(op==GTRSYM) emit("ble",var1,var2,_label);
        else if(op==GEQSYM) emit("blt",var1,var2,_label);
        else if(op==EQLSYM) emit("bne",var1,var2,_label);
        else emit("beq",var1,var2,_label);
    }
    else{
        emit("beq",var1,"0",_label);
    }
    //cout<<line_count<<" is condition!"<<endl;
}
void if_sent(){
    emit("#IF_BEGIN"," "," "," ");
    string endlabel="IF"+nextlabel()+"END";
    insymbol();
    if(sym_type!=LPARENT){
        //cout<<"here needs ("<<endl;
        error(NEEDLPARENT);
    }
    else insymbol();
    condition(endlabel);
    if(sym_type!=RPARENT){
        //cout<<"here needs )"<<endl;
        error(NEEDRPARENT);
    }
    else insymbol();
    emit("#IF_STATE"," "," "," ");
    if(sentence()==0){
        //cout<<"wrong sentence"<<endl;
        //skip();
        error(NEEDSENTENCE);
        return;
    }
    emit("set"," "," ",endlabel);
    //cout<<line_count<<" is if sentence!"<<endl;
}
void while_sent(){
    emit("#WHILE_BEGIN"," "," "," ");
    string label=nextlabel();
    string beginlabel="WHILE"+label+"BEGIN";
    string endlabel="WHILE"+label+"END";
    emit("set"," "," ",beginlabel);
    insymbol();
    if(sym_type!=LPARENT){
        //cout<<"here needs ("<<endl;
        error(NEEDLPARENT);
    }
    else insymbol();
    condition(endlabel);
    if(sym_type!=RPARENT){
        //cout<<"here needs )"<<endl;
        error(NEEDRPARENT);
    }
    else insymbol();
    if(sentence()==0){
        //cout<<"wrong sentence"<<endl;
        //skip();
        error(NEEDSENTENCE);
        return;
    }
    emit("jmp"," "," ",beginlabel);
    emit("set"," "," ",endlabel);
    //cout<<line_count<<" is while sentence!"<<endl;
}
void switch_sent(){
    emit("#SWITCH_BEGIN"," "," "," ");
    int type1=0;//��¼switch������
    //int type2=0;//��¼case������
    int signflag=1;//���ڼ�������������
    string var1=" ",var2=" ";
    string label=" ";//����case����ת�ı�ǩ
    string endlabel="SWITCH"+nextlabel()+"END";
    int case_count=0;
    insymbol();
    if(sym_type!=LPARENT){
        //cout<<"here need ("<<endl;
        error(NEEDLPARENT);
    }
    else insymbol();
    expression();
    type1=last_type;
    var1=last_var;
    if(sym_type!=RPARENT){
        //cout<<"here need }"<<endl;
        error(NEEDRPARENT);
    }
    else insymbol();
    if(sym_type!=LBRACE){
        //cout<<"here need {"<<endl;
        error(NEEDLBRACE);
    }
    else insymbol();
    while(sym_type==CASESYM){
        insymbol();
        /*if(sym_type!=INTCON&&sym_type!=CHARCON){
            cout<<"here needs const!"<<endl;//��ʱ����ݷ��ű�����ж�
        }*/
        //////�ж��Ƿ��ǳ����������¼���ɱȽ����/////////////
        if(sym_type==PLUSYM||sym_type==MINUSYM){
            if(sym_type==MINUSYM) signflag=-1;
            insymbol();
            if(sym_type!=INTCON){
                error(NEEDINTCON);
                sym_num=1;
            }
            sym_num=sym_num*signflag;
            var2=int2string(sym_num);
            if(type1!=T_INT) error(UNMATCHEDSWITCH);
            label="CASE"+nextlabel()+"END";
            emit("bne",var1,var2,label);
            insymbol();
        }
        else if(sym_type==INTCON){
            var2=int2string(sym_num);
            if(type1!=T_INT) error(UNMATCHEDSWITCH);
            label="CASE"+nextlabel()+"END";
            emit("bne",var1,var2,label);
            insymbol();
        }
        else if(sym_type==CHARCON){
            var2=int2string((int)sym_char);
            if(type1!=T_CHAR) error(UNMATCHEDSWITCH);
            label="CASE"+nextlabel()+"END";
            emit("bne",var1,var2,label);
            insymbol();
        }
        else {
            error(NEEDCONST);
            skip(5);//skip to :
        }
        //////////��ʼ�ж�ð��///////////////
        if(sym_type!=COLON){
            //cout<<"here needs :"<<endl;
            error(NEEDCOLON);
        }
        else insymbol();
        if(sentence()==0){
            error(NEEDSENTENCE);
        }
        emit("jmp"," "," ",endlabel);
        emit("set"," "," ",label);
        case_count++;
    }
    if(case_count==0){
        error(NEEDCASE);
    }
    if(sym_type==DEFAULTSYM){
        insymbol();
        if(sym_type!=COLON){
            //cout<<"here needs :"<<endl;
            error(NEEDCOLON);
        }
        else insymbol();
        if(sentence()==0){
            error(NEEDSENTENCE);
        }
    }
    if(sym_type!=RBRACE){
        //cout<<"here needs }"<<endl;
        error(NEEDRBRACE);
    }
    else insymbol();
    emit("set"," "," ",endlabel);
    //cout<<line_count<<" is switch-case sentence!"<<endl;
}
void scanf_sent(){
    int loc=0;//��¼��ʶ���ڷ��ű��е�λ��
    insymbol();
    if(sym_type!=LPARENT){
        //cout<<"here needs ("<<endl;
        error(NEEDLPARENT);
    }
    else insymbol();
    if(sym_type!=IDENTIFIER){
        //cout<<"here needs identifier!"<<endl;
        error(NEEDIDENTIFIER);
        skip(3);//skip to , or )
    }
    else{
        loc=search_symtable(sym_id,1,ptrOFsymtable);
        if(loc==0) error(UNDECLAREIDENTIFIER);
        else if(sym_table[loc].kind!=K_VAR&&sym_table[loc].kind!=K_PARAMENT){
            error(ERRORSCANF);
        }
        else{
            if(sym_table[loc].type==T_INT){
                emit("scf","int"," ",sym_table[loc].name);
            }
            else{
                emit("scf","char"," ",sym_table[loc].name);
            }
        }
        insymbol();
    }
    while(sym_type==COMMA){
        insymbol();
        if(sym_type!=IDENTIFIER){
            //cout<<"here needs identifier!"<<endl;
            error(NEEDIDENTIFIER);
            skip(3);// skip to , or )
            continue;
        }
        loc=search_symtable(sym_id,1,ptrOFsymtable);
        if(loc==0) error(UNDECLAREIDENTIFIER);
        else if(sym_table[loc].kind!=K_VAR&&sym_table[loc].kind!=K_PARAMENT){
            error(ERRORSCANF);
        }
        else{
            if(sym_table[loc].type==T_INT){
                emit("scf","int"," ",sym_table[loc].name);
            }
            else{
                emit("scf","char"," ",sym_table[loc].name);
            }
        }
        insymbol();
    }
    if(sym_type!=RPARENT){
        //cout<<"here needs )"<<endl;
        error(NEEDRPARENT);
    }
    else insymbol();
    //cout<<line_count<<" is scanf sentence!"<<endl;
}
void printf_sent(){
    string _str=" ";
    insymbol();
    if(sym_type!=LPARENT){
        //cout<<"here needs ("<<endl;
        error(NEEDLPARENT);
    }
    else insymbol();
    if(sym_type==STRCON){
        //emit("prt","str"," ",int2string(enter_strtable(sym_str)));
        _str=int2string(enter_strtable(sym_str));//�ȼ�¼��Ҫ������ַ������
        insymbol();
        if(sym_type==COMMA){
            insymbol();
            expression();
            if(last_type==T_CHAR){
                emit("prt","char",_str,last_var);
            }
            else{
                emit("prt","int",_str,last_var);
            }
        }
        else emit("prt"," ",_str," ");
    }
    else{
        expression();
        if(last_type==T_CHAR){
            emit("prt","char"," ",last_var);
        }
        else{
            emit("prt","int"," ",last_var);
        }
    }
    emit("prt","char"," ","10");//ÿ��printf�����һ������
    if(sym_type!=RPARENT){
        //cout<<"here needs )"<<endl;
        error(NEEDRPARENT);
    }
    else insymbol();

    //cout<<line_count<<" is printf sentence!"<<endl;
}
void return_sent(){
    ifreturn=1;
    insymbol();
    if(sym_type==LPARENT){
        insymbol();
        expression();
        //�������ֵ���ͺ͵�ǰ�������Ͳ�ƥ�䣬����
        if(last_type!=now_functype) error(ERRORRETURNTYPE);
        if(sym_type!=RPARENT){
            //cout<<"here needs )"<<endl;
            error(NEEDRPARENT);
        }
        else insymbol();
        emit("ret"," "," ",last_var);
    }
    else{
        if(now_functype!=T_VOID) error(ERRORRETURNTYPE);
        emit("ret"," "," "," ");
    }
    //cout<<line_count<<" is return sentence!"<<endl;
}
void call_sent(){
    int loc=search_symtable(sym_id,1,ptrOFsymtable);
    int par=0;//��¼��ǰ�����Ĳ�������
    insymbol();
    if(sym_type!=LPARENT){
        //cout<<"here needs ("<<endl;
        error(NEEDLPARENT);
    }
    else insymbol();
    if(sym_type==RPARENT){
        if(par!=sym_table[loc].value){
            error(UNMATCHEDPARNUM);
        }
        insymbol();
        emit("cal"," "," ",sym_table[loc].name);
        //cout<<line_count<<" is call function!"<<endl;
        return;
    }
    expression();
    emit("psh"," "," ",last_var);
    par++;
    if(last_type!=sym_table[loc+par].type) error(UNMATCHEDPARTYPE);
    while(sym_type==COMMA){
        insymbol();
        expression();
        emit("psh"," "," ",last_var);
        par++;
        if(last_type!=sym_table[loc+par].type) error(UNMATCHEDPARTYPE);
    }
    if(par!=sym_table[loc].value){
        error(UNMATCHEDPARNUM);
    }
    if(sym_type!=RPARENT){
        //cout<<"here needs )"<<endl;
        error(NEEDRPARENT);
    }
    else insymbol();
    emit("cal"," "," ",sym_table[loc].name);
    //cout<<line_count<<" is call function!"<<endl;
}
void asg_sent(){
    int loc=0;//��¼��ʶ����λ��
    string var1=" ",var2=" ",var3=" ";
    var3=sym_id;//����������Ϊ��ʶ��/������
    loc=search_symtable(sym_id,1,ptrOFsymtable);
    int kind=sym_table[loc].kind;//��¼��ʶ������
    int type=sym_table[loc].type;//��¼��ʶ��ֵ������
    insymbol();
    if(kind==K_ARRAY){
        if(sym_type!=LBRACKET){
            error(NEEDLBRACKET);
        }
        else insymbol();
        expression();
        //�жϱ��ʽ��ֵ�ǲ�������
        if(last_type!=T_INT){
            error(ERRORARRAYNUM);//����������,֮����ݴ����
        }
        else if(last_var[0]>='0'&&last_var[0]<='9'){
            if(str2int(last_var)>=sym_table[loc].value){
                error(OUTOFINDEX);
            }
        }
        var2=last_var;
        if(sym_type!=RBRACKET){
            error(NEEDRBRACKET);
        }
        else insymbol();

    }
    /*if(sym_type==LBRACKET){
        insymbol();
        expression();
        if(sym_type!=RBRACKET){
            //cout<<"here needs ]"<<endl;
            error(NEEDRBRACKET);
        }
        else insymbol();
    }*/
    if(sym_type!=ASGSYM){
        //cout<<"here needs = !"<<endl;
        error(NEEDEQUAL);
    }
    else insymbol();
    expression();
    //�жϸ�ֵ��������ֵ�������Ƿ�һ�£���һ���򱨴��ݴ���
    if(last_type!=type){
        error(UNMATCHEDTYPE);
    }
    var1=last_var;
    if(kind==K_ARRAY){
        emit("sta",var1,var2,var3);
    }
    else{
        emit("asg",var1,var2,var3);
    }
    //cout<<line_count<<" is assign sentence!"<<endl;
}
void sent_list(){//�����
    while(sentence());
    //cout<<line_count<<" is sentence list!"<<endl;
}

int sentence(){
    if(sym_type==IFSYM){
        if_sent();
    }
    else if(sym_type==WHILESYM){
        while_sent();
    }
    else if(sym_type==SWITCHSYM){
        switch_sent();
    }
    else if(sym_type==LBRACE){
        insymbol();
        sent_list();
        if(sym_type!=RBRACE) error(NEEDRBRACE);//cout<<"here needs }"<<endl;
        else insymbol();
    }
    else if(sym_type==SCANFSYM){
        scanf_sent();
        if(sym_type!=SEMICOLON){
            //cout<<"here needs ;"<<endl;
            error(NEEDSEMICOLON);
        }
        else insymbol();
    }
    else if(sym_type==PRINTFSYM){
        printf_sent();
        if(sym_type!=SEMICOLON){
            //cout<<"here needs ;"<<endl;
            error(NEEDSEMICOLON);
        }
        else insymbol();
    }
    else if(sym_type==RETURNSYM){
        return_sent();
        if(sym_type!=SEMICOLON){
            //cout<<"here needs ;"<<endl;
            error(NEEDSEMICOLON);
        }
        else insymbol();
    }
    else if(sym_type==SEMICOLON){
        insymbol();
    }
    else if(sym_type==IDENTIFIER){
        int loc=0;//��¼�ñ�ʶ���ڷ��ű�ĵ�ַ
            //����ж��Ǳ������Ǻ�������������û�з��ű����ټ�������
            //if(var){asg_sent};
            //else if(func){call_sent);
        ////��ʼ����ж��Ǳ��������顢���Ǻ���///////////////////
        loc=search_symtable(sym_id,1,ptrOFsymtable);
        if(loc==0){//���δ�鵽��ʶ�����򱨴���ת��}��;
            error(UNDECLAREIDENTIFIER);
            skip(4);//skip to ; or }
            insymbol();
            return 1;
        }
        /*insymbol();
        if(sym_type==LPARENT){
            call_sent();
            if(sym_type!=SEMICOLON){
                //cout<<"here needs ;"<<endl;
                error(NEEDSEMICOLON);
            }
            else insymbol();
        }
        else{
            asg_sent();
            if(sym_type!=SEMICOLON){
                //cout<<"here needs ;"<<endl;
                error(NEEDSEMICOLON);
            }
            else insymbol();
        }*/
        if(sym_table[loc].kind==K_FUNCTION){
            call_sent();
        }
        else if(sym_table[loc].kind==K_CONST){
            error(ERRORCONST);
            skip(4);//skip to ; or }
            insymbol();
            return 1;
        }
        else{
            asg_sent();
        }
        if(sym_type!=SEMICOLON){
            error(NEEDSEMICOLON);
        }
        else insymbol();
    }
    else{
        return 0;//������������ж϶�����
    }
    return 1;
}

void statement(){//�������
    while(sym_type==CONSTSYM){//����˵��
        insymbol();
        con_def();
    }
    while(sym_type==INTSYM||sym_type==CHARSYM){
        var_def();
    }
    sent_list();
    //cout<<line_count<<" is statement!"<<endl;
}
void par_list(){//�����б�
    string name=" ";
    int type=0;
    if(sym_type==RPARENT){
        //cout<<line_count<<" is parament list!"<<endl;
        return;
    }
    if(sym_type!=INTSYM&&sym_type!=CHARSYM){
        //cout<<sym_type<<endl;
        //cout<<"here need int/char"<<endl;
        error(NEEDINTORCHAR);
        skip(3);//skip to , or )
    }
    else{
        type=sym_type-1;
        insymbol();
        if(sym_type!=IDENTIFIER){
            //cout<<"here need identifier!"<<endl;
            error(NEEDIDENTIFIER);
            skip(3);//skip to , or )
        }
        else{
            name=sym_id;
            insymbol();
        }
        /////////////�����¼///////////////////
        if(search_symtable(name,nowfunc_index,ptrOFsymtable)){
            error(REDECLARE);
            skip(3);//skip to , or )
        }
        else{
            par_num++;
            enter_symtable(name,K_PARAMENT,type,0);
            emit("par",str2mid[type]," ",name);
        }
    }
    while(sym_type==COMMA){
        insymbol();
        if(sym_type!=INTSYM&&sym_type!=CHARSYM){
            //cout<<"here need int/char"<<endl;
            error(NEEDINTORCHAR);
            skip(3);//to , or )
            continue;
        }
        type=sym_type-1;
        insymbol();
        if(sym_type!=IDENTIFIER){
            //cout<<"here need identifier!"<<endl;
            error(NEEDIDENTIFIER);
            skip(3);
            continue;
        }
        name=sym_id;
        insymbol();
        /////////////�����¼///////////////////
        if(search_symtable(name,nowfunc_index,ptrOFsymtable)){
            error(REDECLARE);
            skip(3);//skip to , or )
            continue;
        }
        par_num++;
        enter_symtable(name,K_PARAMENT,type,0);
        emit("par",str2mid[type]," ",name);
    }
    //cout<<line_count<<" is parament list!"<<endl;
}
void main_func(){
    insymbol();
    //////////////��¼���ű�,�����м����////////////////////////////////
    enter_symtable("main",K_FUNCTION,T_VOID,0);
    emit("fun",str2mid[now_functype]," ","main");
    //////////////////////////////////////////////////////////////////////
    if(sym_type!=LPARENT) error(NEEDLPARENT);//cout<<"here needs ( !"<<endl;
    else insymbol();
    if(sym_type!=RPARENT) error(NEEDRPARENT);//cout<<"here needs ) !"<<endl;
    else insymbol();
    if(sym_type!=LBRACE) error(NEEDLBRACE);//cout<<"here needs { !"<<endl;
    else insymbol();
    statement();
    if(sym_type!=RBRACE) error(NEEDRBRACE);//cout<<"here needs } !"<<endl;
    ////////������ͷŷ���ջ�������м����///////////////
    enter_functable("main",nowfunc_index,0,var_num);//���֮���ٿ���
    popsym_table();
    emit("end"," "," ","main");

    //ע�⣬���ﲻ��insymbol!����nextchar����
    while(ctemp!=-1){
        if(ctemp==' '||ctemp=='\t'||ctemp=='\n'){
            nextchar();
            continue;
        }
        cout<<"Wrong symbol after main function!"<<endl;
        break;
    }
    //cout<<line_count<<" is main function!"<<endl;
}
void func_def(){//��������
    ////////////////////////////
    //int type=0;//�洢��ǰ���庯���ķ���ֵ����
    string name="";
    while(sym_type==INTSYM||sym_type==CHARSYM||sym_type==VOIDSYM){
        ifreturn=0;//��շ��ؼ�����
        //���ȶ�ָ�룺
        nowfunc_index=ptrOFsymtable;
        ////////////////////////////////////////////////
        now_functype=sym_type-1;//��symbol��ö�����ò�1����ȥ����
        insymbol();
        if(now_functype==T_VOID&&sym_type==MAINSYM){
            main_func();
            ifmain=1;
            return;
        }
        if(sym_type!=IDENTIFIER){
            //cout<<"needs identifier here!"<<endl;
            error(NEEDIDENTIFIER);
            skip(2);//skip to int/char/void
            continue;
        }
        name=sym_id;
        //�����¼�������м����
        if(search_symtable(name,1,ptrOFsymtable)){
            error(REDECLARE);
            skip(2);//skip to int/char/void
            continue;
        }
        enter_symtable(name,K_FUNCTION,now_functype,0);//���
        emit("fun",str2mid[now_functype]," ",name);
        //////////////����///////////////
        insymbol();
        if(sym_type!=LPARENT){
            //cout<<"needs ( here!"<<endl;
            error(NEEDLPARENT);
        }
        else insymbol();
        par_num=0;//��������������
        par_list();
        sym_table[nowfunc_index].value=par_num;//������ű��к����Ĳ���
        if(sym_type!=RPARENT){
            //cout<<"needs ) here!"<<endl;
            error(NEEDRPARENT);
        }
        insymbol();
        if(sym_type!=LBRACE){
            //cout<<"needs { here!"<<endl;
            error(NEEDLBRACE);
        }
        insymbol();
        statement();
        //////////���û�з������ͱ���//////////////
        /*if(now_functype!=T_VOID&&ifreturn==0){
            error(NORETURN);
        }*/
        if(ifreturn==0){
            if(now_functype!=T_VOID) error(NORETURN);
            //else emit("ret"," "," "," ");
        }
        if(sym_type!=RBRACE){
            //cout<<"needs } here!"<<endl;
            error(NEEDRBRACE);
        }
        /////////����������ű��ͷŷ��ű�ջ/////////////////
        enter_functable(name,nowfunc_index,par_num,var_num);//֮���ٿ����Ƿ���
        //sym_table[nowfunc_index].value=par_num;//������ű��к����Ĳ���=========>��仰�����ƶ���ǰ�棬�����������ҵ��þͲ�������
        popsym_table();
        emit("end"," "," ",name);
        //cout<<line_count<<" is function defination!"<<endl;
        insymbol();
    }
    //cout<<"need int/char/void"<<endl;
    error(ERRORDECLARE);
}

void program(){//������֮ǰ����Ԥ��һ��insymbol
    while(sym_type==CONSTSYM){//����˵��
        insymbol();
        con_def();
    }

    while(sym_type==INTSYM||sym_type==CHARSYM){//����˵��
        type_reg=sym_type;//��¼�������Լ�Դ�ļ�ָ�룬����֮��Ļ���
        inptr_reg=ptrOFincode;
        ctemp_reg=ctemp;
        insymbol();

        /*if(sym_type!=IDENTIFIER){
            cout<<"here needs identifier!"<<endl;
            //skip();
            continue;
        }*///���ɾ������Ϊ�ڶ�Ӧ���ֻ����ж�
        insymbol();
        if(sym_type==COMMA||sym_type==LBRACKET||sym_type==SEMICOLON){
            sym_type=type_reg;
            ptrOFincode=inptr_reg;
            ctemp=ctemp_reg;
            var_def();
            continue;
        }
        else if(sym_type==LPARENT){
            sym_type=type_reg;
            ptrOFincode=inptr_reg;
            ctemp=ctemp_reg;
            break;
        }
        else{
            //cout<<"Wrong char!"<<endl;
            error(ERRORDECLARE);
            skip(2);//skip to int or char or void
            continue;
        }
    }

    func_def();//���뺯������

    if(ifmain==0){
        error(NOMAINFUNC);
    }

    //cout<<line_count<<" is program!"<<endl;
}
