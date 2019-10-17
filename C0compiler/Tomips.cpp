#include<iostream>
#include<string>
#include<fstream>
#include"Global.h"
using namespace std;
////////////////////
ofstream outmips;//���mips��
////////////////////
int ifbetter=0;//�Ƿ�����Ż�
int nowmain=0;//�����Ƿ���������
string nowname=" ";//���ں���������
////////////////////
Addrele Addr_table[MAX_OF_ADDRTABLE];//������ַ��
int ptrOFaddrtable=1;//������ַ���ָ��
int nowmidcode=0;//��¼��ǰ���뵽�ڼ����м������
int totalvar=0;//��¼��ǰ���ֵı�������
int totalpar=0;//��¼��ǰ���ֵĲ�������
int pushnum=0;//��¼��ǰ�Ѿ�push�Ĳ�������
int str2int(string _str){
    unsigned i;
    int result=0;
    for(i=0;i<_str.length();i++){
        result=result*10;
        result+=_str[i]-'0';
    }
    return result;
}
int search_addr(string _name){
    int i;
    for(i=ptrOFaddrtable-1;i>0;i--){
        if(Addr_table[i].name==_name) return i;
    }
    return 0;
}
void enter_addr(string _name,int _type,int _address,int _ifglobal){
    int i=ptrOFaddrtable;
    Addr_table[i].name=_name;
    Addr_table[i].type=_type;
    Addr_table[i].address=_address;
    Addr_table[i].ifglobal=_ifglobal;
    ptrOFaddrtable++;
}
string getloc(string _name,int offset,string reg){
    int loc=search_addr(_name);
    if(Addr_table[loc].ifglobal){
        if(Addr_table[loc].type==ARY){
            if(reg==" ") return int2string((Addr_table[loc].address+offset)*4)+"($gp)";
            else{
                outmips<<"sll "+reg+","+reg+",2"<<endl;
                outmips<<"add "+reg+","+reg+",$gp"<<endl;
                return int2string(Addr_table[loc].address*4)+"("+reg+")";
            }
        }
        return int2string(Addr_table[loc].address*4)+"($gp)";
    }
    else if(Addr_table[loc].type==PAR) return int2string(Addr_table[loc].address*4)+"($fp)";
    else {
        if(Addr_table[loc].type==ARY){
            if(reg==" ") return int2string((Addr_table[loc].address+offset)*4)+"($sp)";
            else{
                outmips<<"sll "+reg+","+reg+",2"<<endl;
                outmips<<"add "+reg+","+reg+",$sp"<<endl;
                return int2string(Addr_table[loc].address*4)+"("+reg+")";
            }
        }
        return int2string(Addr_table[loc].address*4)+"($sp)";
    }
}
void retasm(){
    string _var3=midcode_table[nowmidcode].var3;
    if(_var3!=" "){
        if((_var3[0]>='0'&&_var3[0]<='9')||_var3[0]=='-'){
            outmips<<"ori $v0,$0,"+_var3<<endl;
        }
        else{
            _var3=getloc(_var3,0," ");
            outmips<<"lw $v0,"+_var3<<endl;
        }
    }
    outmips<<"j "+nowname+"_end"<<endl;
}
void calasm(){
    string _var3=midcode_table[nowmidcode].var3;
    //���Ƚ�push���ջָ������//
    outmips<<"addi $sp,$sp,"+int2string(pushnum*(-4))<<endl;
    //pushnum=0;//ǧ��ǵ�pushnumҪ���㣡������������������
    outmips<<"jal "+midcode_table[nowmidcode].var3<<endl;
    //������Ҫ�ָ�ջָ��
    outmips<<"addi $sp,$sp,"+int2string(pushnum*4)<<endl;
    //������Ҫ��pushnum����������ȥcall�����Ĳ�������
    pushnum=pushnum-func_table[search_functable(_var3)].parcount;
}
void pshasm(){
    string _var3=midcode_table[nowmidcode].var3;
    pushnum++;
    if((_var3[0]>='0'&&_var3[0]<='9')||_var3[0]=='-'){
        outmips<<"ori $t0,$0,"+_var3<<endl;
        outmips<<"sw $t0,"+int2string(pushnum*(-4))+"($sp)"<<endl;
    }
    else{
        outmips<<"lw $t0,"+getloc(_var3,0," ")<<endl;
        outmips<<"sw $t0,"+int2string(pushnum*(-4))+"($sp)"<<endl;
    }
}
void gtrasm(){
    string _var3=midcode_table[nowmidcode].var3;
    outmips<<"sw $v0,"+getloc(_var3,0," ")<<endl;
}
void gtaasm(){
    string _var1=midcode_table[nowmidcode].var1;
    string _var2=midcode_table[nowmidcode].var2;
    string _var3=midcode_table[nowmidcode].var3;
    if(_var2[0]>='0'&&_var2[0]<='9'){
        outmips<<"lw $t0,"+getloc(_var1,str2int(_var2)," ")<<endl;
        outmips<<"sw $t0,"+getloc(_var3,0," ")<<endl;
    }
    else{
        outmips<<"lw $t1,"+getloc(_var2,0," ")<<endl;
        outmips<<"lw $t0,"+getloc(_var1,0,"$t1")<<endl;
        outmips<<"sw $t0,"+getloc(_var3,0," ")<<endl;
    }
}
void staasm(){
    string _var1=midcode_table[nowmidcode].var1;
    string _var2=midcode_table[nowmidcode].var2;
    string _var3=midcode_table[nowmidcode].var3;
    if(_var2[0]>='0'&&_var2[0]<='9'){
        if((_var1[0]>='0'&&_var1[0]<='9')||_var1[0]=='-') outmips<<"ori $t0,$0,"+_var1<<endl;
        else outmips<<"lw $t0,"+getloc(_var1,0," ")<<endl;
        outmips<<"sw $t0,"+getloc(_var3,str2int(_var2)," ")<<endl;
    }
    else{
        if((_var1[0]>='0'&&_var1[0]<='9')||_var1[0]=='-') outmips<<"ori $t0,$0,"+_var1<<endl;
        else outmips<<"lw $t0,"+getloc(_var1,0," ")<<endl;
        outmips<<"lw $t1,"+getloc(_var2,0," ")<<endl;
        outmips<<"sw $t0,"+getloc(_var3,0,"$t1")<<endl;
    }
}
void asgasm(){
    string _var1=midcode_table[nowmidcode].var1;
    string _var3=midcode_table[nowmidcode].var3;
    if((_var1[0]>='0'&&_var1[0]<='9')||_var1[0]=='-') outmips<<"ori $t0,$0,"+_var1<<endl;
    else outmips<<"lw $t0,"+getloc(_var1,0," ")<<endl;
    outmips<<"sw $t0,"+getloc(_var3,0," ")<<endl;
}
void scfasm(){
    string _var1=midcode_table[nowmidcode].var1;
    string _var3=midcode_table[nowmidcode].var3;
    if(_var1=="int") outmips<<"ori $v0,$0,5"<<endl;
    else if(_var1=="char") outmips<<"ori $v0,$0,12"<<endl;
    outmips<<"syscall"<<endl;
    outmips<<"sw $v0,"+getloc(_var3,0," ")<<endl;
}
void prtasm(){
    string _var1=midcode_table[nowmidcode].var1;
    string _var2=midcode_table[nowmidcode].var2;
    string _var3=midcode_table[nowmidcode].var3;
    if(_var2!=" "){
        outmips<<"la $a0,str_"+_var2<<endl;
        outmips<<"ori $v0,$0,4"<<endl;
        outmips<<"syscall"<<endl;
    }
    if(_var1=="int"){
        if((_var3[0]>='0'&&_var3[0]<='9')||_var3[0]=='-') outmips<<"ori $a0,$0,"+_var3<<endl;
        else outmips<<"lw $a0,"+getloc(_var3,0," ")<<endl;
        outmips<<"ori $v0,$0,1"<<endl;
        outmips<<"syscall"<<endl;
    }
    else if(_var1=="char"){
        if(_var3[0]>='0'&&_var3[0]<='9') outmips<<"ori $a0,$0,"+_var3<<endl;
        else outmips<<"lw $a0,"+getloc(_var3,0," ")<<endl;
        outmips<<"ori $v0,$0,11"<<endl;
        outmips<<"syscall"<<endl;
    }
    /*else if(_var1=="str"){
        outmips<<"la $a0,str_"+_var3<<endl;
        outmips<<"ori $v0,$0,4"<<endl;
    }
    outmips<<"syscall"<<endl;*/
}
void addasm(){
    string _var1=midcode_table[nowmidcode].var1;
    string _var2=midcode_table[nowmidcode].var2;
    string _var3=midcode_table[nowmidcode].var3;
    if((_var1[0]>='0'&&_var1[0]<='9')||_var1[0]=='-') outmips<<"ori $t0,$0,"+_var1<<endl;
    else outmips<<"lw $t0,"+getloc(_var1,0," ")<<endl;
    if((_var2[0]>='0'&&_var2[0]<='9')||_var2[0]=='-') outmips<<"ori $t1,$0,"+_var2<<endl;
    else outmips<<"lw $t1,"+getloc(_var2,0," ")<<endl;
    outmips<<"add $t0,$t0,$t1"<<endl;
    outmips<<"sw $t0,"+getloc(_var3,0," ")<<endl;
}
void misasm(){
    string _var1=midcode_table[nowmidcode].var1;
    string _var2=midcode_table[nowmidcode].var2;
    string _var3=midcode_table[nowmidcode].var3;
    if((_var1[0]>='0'&&_var1[0]<='9')||_var1[0]=='-') outmips<<"ori $t0,$0,"+_var1<<endl;
    else outmips<<"lw $t0,"+getloc(_var1,0," ")<<endl;
    if((_var2[0]>='0'&&_var2[0]<='9')||_var2[0]=='-'){
        outmips<<"ori $t1,$0,"+_var2<<endl;
        outmips<<"mul $t1,$t1,-1"<<endl;
    }
    else{
        outmips<<"lw $t1,"+getloc(_var2,0," ")<<endl;
        outmips<<"mul $t1,$t1,-1"<<endl;
    }
    outmips<<"add $t0,$t0,$t1"<<endl;
    outmips<<"sw $t0,"+getloc(_var3,0," ")<<endl;
}
void mulasm(){
    string _var1=midcode_table[nowmidcode].var1;
    string _var2=midcode_table[nowmidcode].var2;
    string _var3=midcode_table[nowmidcode].var3;
    if((_var1[0]>='0'&&_var1[0]<='9')||_var1[0]=='-') outmips<<"ori $t0,$0,"+_var1<<endl;
    else outmips<<"lw $t0,"+getloc(_var1,0," ")<<endl;
    if((_var2[0]>='0'&&_var2[0]<='9')||_var2[0]=='-') outmips<<"ori $t1,$0,"+_var2<<endl;
    else outmips<<"lw $t1,"+getloc(_var2,0," ")<<endl;
    outmips<<"mul $t0,$t0,$t1"<<endl;
    outmips<<"sw $t0,"+getloc(_var3,0," ")<<endl;
}

void divasm(){
    string _var1=midcode_table[nowmidcode].var1;
    string _var2=midcode_table[nowmidcode].var2;
    string _var3=midcode_table[nowmidcode].var3;
    if((_var1[0]>='0'&&_var1[0]<='9')||_var1[0]=='-') outmips<<"ori $t0,$0,"+_var1<<endl;
    else outmips<<"lw $t0,"+getloc(_var1,0," ")<<endl;
    if((_var2[0]>='0'&&_var2[0]<='9')||_var2[0]=='-') outmips<<"ori $t1,$0,"+_var2<<endl;
    else outmips<<"lw $t1,"+getloc(_var2,0," ")<<endl;
    outmips<<"div $t0,$t0,$t1"<<endl;
    outmips<<"sw $t0,"+getloc(_var3,0," ")<<endl;
}
void beqasm(){
    string _op=midcode_table[nowmidcode].op;
    string _var1=midcode_table[nowmidcode].var1;
    string _var2=midcode_table[nowmidcode].var2;
    string _var3=midcode_table[nowmidcode].var3;
    if((_var1[0]>='0'&&_var1[0]<='9')||_var1[0]=='-') outmips<<"ori $t0,$0,"+_var1<<endl;
    else outmips<<"lw $t0,"+getloc(_var1,0," ")<<endl;
    if((_var2[0]>='0'&&_var2[0]<='9')||_var2[0]=='-') outmips<<"ori $t1,$0,"+_var2<<endl;
    else outmips<<"lw $t1,"+getloc(_var2,0," ")<<endl;
    outmips<<_op+" $t0,$t1,"+_var3<<endl;
}
void funasm(){
    /////////////�����������������Ҫ���и��ֱ��棬��ʡָ������,���򱣻��ֳ�//////////////
    if(nowmain){
        outmips<<"ori $fp,$sp,0"<<endl;//����ָ֡�룬��仰֮��Ҳ����ʡ�ԣ�ĿǰΪ���������ȼ���
        outmips<<"addi $sp,$sp,"+int2string(totalvar*(-4))<<endl;//Ϊ��ʱ��������ռ�
    }
    else{
        outmips<<"addi $sp,$sp,-4"<<endl;//ջָ������
        outmips<<"sw $fp,0($sp)"<<endl;//����ָ֡��
        outmips<<"addi $fp,$sp,4"<<endl;//��������ָ֡��
        outmips<<"addi $sp,$sp,-4"<<endl;//ջָ������
        outmips<<"sw $fp,0($sp)"<<endl;//����ջָ��
        outmips<<"addi $sp,$sp,-4"<<endl;//ջָ������
        outmips<<"sw $ra,0($sp)"<<endl;//���淵�ص�ַ
        outmips<<"addi $sp,$sp,"+int2string(totalvar*(-4))<<endl;//��������Ĵ洢�ռ�
    }
    ///////////////////////��ʼ�����м����ķ���//////////////////////////////
    while(midcode_table[nowmidcode].op!="end"){
        if(midcode_table[nowmidcode].op=="ret") retasm();
        else if(midcode_table[nowmidcode].op=="set") outmips<<midcode_table[nowmidcode].var3+":"<<endl;
        else if(midcode_table[nowmidcode].op=="jmp") outmips<<"j "+midcode_table[nowmidcode].var3<<endl;
        else if(midcode_table[nowmidcode].op=="cal") calasm();
        else if(midcode_table[nowmidcode].op=="psh") pshasm();
        else if(midcode_table[nowmidcode].op=="gtr") gtrasm();
        else if(midcode_table[nowmidcode].op=="gta") gtaasm();
        else if(midcode_table[nowmidcode].op=="sta") staasm();
        else if(midcode_table[nowmidcode].op=="asg") asgasm();
        else if(midcode_table[nowmidcode].op=="scf") scfasm();
        else if(midcode_table[nowmidcode].op=="prt") prtasm();
        else if(midcode_table[nowmidcode].op=="add") addasm();
        else if(midcode_table[nowmidcode].op=="mis") misasm();
        else if(midcode_table[nowmidcode].op=="mul") mulasm();
        else if(midcode_table[nowmidcode].op=="div") divasm();
        else if(midcode_table[nowmidcode].op=="beq"||
                midcode_table[nowmidcode].op=="bne"||
                midcode_table[nowmidcode].op=="bge"||
                midcode_table[nowmidcode].op=="bgt"||
                midcode_table[nowmidcode].op=="ble"||
                midcode_table[nowmidcode].op=="blt") beqasm();//������һ����תָ��Ĵ���
        nowmidcode++;
    }
    /////////////////////��ʼ�ָ�///////////////////////////////////////////////
    if(nowmain){
        outmips<<"main_end:"<<endl;
    }
    else{
        outmips<<nowname+"_end:"<<endl;
        outmips<<"lw $ra,-12($fp)"<<endl;//�ָ����ص�ַ
        outmips<<"lw $sp,-8($fp)"<<endl;//�ָ�ջָ��
        outmips<<"lw $fp,-4($fp)"<<endl;//�ָ�ָ֡��
        //outmips<<"addi $sp,$sp,"+int2string(4*totalpar)<<endl;//��push��ջ���Ĳ������
        outmips<<"jr $ra"<<endl;//��ת�ص��ú���
    }
}
void tomips(string _file){

    int i;
    outmips.open(_file.data());
    /////����д���ַ�����/////
    outmips<<".data"<<endl;
    for(i=1;i<ptrOFstrtable;i++){
        outmips<<"str_"+int2string(i)+": .asciiz \""+str_table[i]+"\""<<endl;
    }
    outmips<<".text"<<endl;
    /////֮��д��ȫ�ֱ���/////////////////////////////
    while(midcode_table[nowmidcode].op=="var"||midcode_table[nowmidcode].op=="ary"){
        if(midcode_table[nowmidcode].op=="var"){
            enter_addr(midcode_table[nowmidcode].var3,VAR,totalvar,1);
            totalvar++;
        }
        else{
            enter_addr(midcode_table[nowmidcode].var3,ARY,totalvar,1);
            totalvar+=str2int(midcode_table[nowmidcode].var2);
        }
        nowmidcode++;
    }
    //outmips<<"ori $gp,$sp,0"<<endl;//����ȫ�ֱ�������ջ��ջָ��
    outmips<<"addi $sp,$sp,"+int2string(totalvar*(-4))<<endl;//Ϊȫ�ֱ������ٿռ�
    outmips<<"ori $gp,$sp,0"<<endl;//����ȫ�ֱ�������ջ��ջָ��
    outmips<<"j main"<<endl;//��ת��main����
    ///////////////��ʼ������////////////////////////////////////////////////
    while(nowmidcode!=ptrOFmidcode&&midcode_table[nowmidcode].op=="fun"){
        nowname=midcode_table[nowmidcode].var3;
        if(nowname=="main") nowmain=1;
        outmips<<midcode_table[nowmidcode].var3+":"<<endl;
        //cout<<midcode_table[nowmidcode].var3+":"<<endl;
        totalvar=0;//����������������
        int temp_ptrOFaddr=ptrOFaddrtable;//��¼�µ�ǰ���ű��ָ�룬�Ա�֮�����
        totalpar=func_table[search_functable(midcode_table[nowmidcode].var3)].parcount;//��ȡ��ǰ�����Ĳ�������
        int temp_totalpar=totalpar;//�洢��ǰ���������������������
        nowmidcode++;

        while(midcode_table[nowmidcode].op=="par"){//���������
            temp_totalpar--;
            enter_addr(midcode_table[nowmidcode].var3,PAR,temp_totalpar,0);
            nowmidcode++;
        }

        while(midcode_table[nowmidcode].op=="var"||midcode_table[nowmidcode].op=="ary"){//�������ı������
            if(midcode_table[nowmidcode].op=="var"){
                enter_addr(midcode_table[nowmidcode].var3,VAR,totalvar,0);
                totalvar++;
            }
            else{
                enter_addr(midcode_table[nowmidcode].var3,ARY,totalvar,0);
                totalvar+=str2int(midcode_table[nowmidcode].var2);
            }
            nowmidcode++;
        }

        int temp_nowmidcode=nowmidcode;
        while(midcode_table[nowmidcode].op!="end"){//����ʱ�������
            if(midcode_table[nowmidcode].var1[0]=='#'){
                if(search_addr(midcode_table[nowmidcode].var1)==0){
                    enter_addr(midcode_table[nowmidcode].var1,VAR,totalvar,0);
                    totalvar++;
                }
            }
            if(midcode_table[nowmidcode].var2[0]=='#'){
                if(search_addr(midcode_table[nowmidcode].var2)==0){
                    enter_addr(midcode_table[nowmidcode].var2,VAR,totalvar,0);
                    totalvar++;
                }
            }
            if(midcode_table[nowmidcode].var3[0]=='#'){
                if(search_addr(midcode_table[nowmidcode].var3)==0){
                    enter_addr(midcode_table[nowmidcode].var3,VAR,totalvar,0);
                    totalvar++;
                }
            }
            nowmidcode++;
        }
        nowmidcode=temp_nowmidcode;//�ָ��м����ָ��
        funasm();//��ʼ�ݹ麯����Ĵ�������
        ////////////�ָ����ּ���ָ��///////////////////
        ptrOFaddrtable=temp_ptrOFaddr;
        nowmidcode++;//����end���
    }
    outmips.close();
}

