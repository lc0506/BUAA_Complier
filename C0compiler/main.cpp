#include <iostream>
#include <string>
#include "Global.h"

using namespace std;

int main()
{
    string _incodename;
    cout<<"������Դ����·��: "<<endl;
    getline(cin,_incodename);
    getINcode(_incodename);

    string _midcode;
    cout<<"�����뱣����м����·����"<<endl;
    getline(cin,_midcode);

    string _mipscode;
    cout<<"������Ŀ��Mips�����·����"<<endl;
    getline(cin,_mipscode);

    /*cout<<"�Ƿ�����Ż�������1Ϊ�Ż���0Ϊ���Ż���"<<endl;
    cin>>ifbetter;*/

    nextchar();//Ԥ��һ��char
    insymbol();
    program();



    if(err_num==0){
        cout<<"Compile successfully!"<<endl;
        if(ifbetter){
            //�����Ż�
        }
        outMidcode(_midcode);
        tomips(_mipscode);
    }
    else{
        cout<<"Compile error, not finished! Total: "<<err_num<<" errors!"<<endl;
    }
    /*cout<<endl<<"Here's midcode:"<<endl;
    out_midcode();*/
    //cout<<endl<<endl<<"Here's symbol table!"<<endl;
    //out_table();
    //
    //cout<<nextempvar()<<endl;
    //cout<<nextempvar()<<endl;
    return 0;
}
