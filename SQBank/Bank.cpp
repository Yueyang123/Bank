/*
 * @Descripttion: 
 * @version: 
 * @Author: Yueyang
 * @email: 1700695611@qq.com
 * @Date: 2021-03-16 21:50:10
 * @LastEditors: Yueyang
 * @LastEditTime: 2021-03-17 12:49:01
 */
#include "Bank.h"
#define   RATE 0.043//银行当前利率
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

int callbacktimes=0;//回调次数
Check *check[MAXNUM];//所有账户
/**
 * 
    create table userInfomation(
    ID      TEXT(20) PRIMARY KEY NOT NULL,
    NAME    TEXT(10),
    STACARD TEXT(30),
    ADDRESS TEXT(20),
    PHONE   TEXT(15),
    CODE    TEXT(20),
    MONEY   DOUBLE
    );

INSERT INTO userInfomation (ID,NAME,STACARD,ADDRESS,PHONE,CODE,MONEY)
VALUES('0','杨越','321023200004210416','中国','18852566109','197696',1000);

INSERT INTO userInfomation (ID,NAME,STACARD,ADDRESS,PHONE,CODE,MONEY)
VALUES('1','杨越','321023200004210416','中国','18852566109','197696',1000);
*/
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
        check[callbacktimes]=new Check;
        check[callbacktimes]->Set(argv[0],argv[1],argv[2],argv[3],argv[4],argv[5],atof(argv[6]));
        i++;
   }
   callbacktimes++;
   return 0;
}

Bank::Bank()
{
    char *zErrMsg = 0;
    int rc;
    char *sql;
    const char* data = "";
    int i=0;
    double Money,Rate,Key;
    string id,usename,usid,place,phone;
    rc = sqlite3_open("bank.db", &db);
    callbacktimes=0;
    /* Create SQL statement */
   sql = (char*)"SELECT * from userInfomation";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    realcount=callbacktimes;
}

//功能：用析构函数，在退出系统时，将数据保存到文件中
/**
 * UPDATE userInfomation Set MONEY=10 WHERE ID=1
 * 
INSERT INTO userInfomation (ID,NAME,STACARD,ADDRESS,PHONE,CODE,MONEY)
VALUES('1','杨越','321023200004210416','中国','18852566109','197696',1000);

*/
Bank::~Bank()
{
    int i;
    char *pErrMsg = 0;
    char sql[3000];
    if (realcount<=callbacktimes){//没有新增用户
        cout<<1;
        for(i=0; i<realcount; i++)
        {
            if(check[i]->state!=0){
            cout<<2;
            sprintf(sql, (char*)"UPDATE userInfomation Set MONEY=%f WHERE ID='%d';",check[i]->money, i);
            sqlite3_exec(db, sql, NULL, 0, &pErrMsg);
            }
        }
    }else if(realcount>callbacktimes){
        for(i=callbacktimes;i<realcount;i++)
        {
            sprintf(sql,(char*)"INSERT INTO userInfomation (ID,NAME,STACARD,ADDRESS,PHONE,CODE,MONEY) VALUES('%s','%s','%s','%s','%s','%s',%f)"
                        ,check[i]->checkid.c_str(),check[i]->username.c_str(),check[i]->userid.c_str(),check[i]->userplace.c_str(),check[i]->phone.c_str(),check[i]->passkey.c_str(),check[i]->money);
            sqlite3_exec(db, sql, NULL, 0, &pErrMsg);  
        }
    }
}


int Bank::menushow()
{
    int i;
       cout<<endl;
    cout<<"+--------------------------------------+"<<endl;
    cout<<"+ 1 开户         2 销户         3 存款 +"<<endl;
    cout<<"+ 4 取款         5 查询         6 转账 +"<<endl;
    cout<<"+ 7 挂失         8 解挂         9 改密 +"<<endl;
    cout<<"+                               0 退出 +"<<endl;
    cout<<"+--------------------------------------+"<<endl;
    cout<<"请输入操作指令:";
 
    while(1)
    {
        cin>>i;
        if(i>=0&&i<=9)
            break;
        else
            cout<<"输入错误，请重新选择：";
    }
    return i;
}

//功能：办理业务
void Bank::work()
{
    int choose;
    do
    {
        choose=menushow();
        switch(choose)
        {
        case 0:
            break;//退出
        case 1:
            establish();//开户
            break;
        case 2:
            Delete();//销户
            break;
        case 3:
            save();//存款
            break;
        case 4:
            withdraw();//取款
            break;
        case 5:
            find();//查询
            break;
        case 6:
            transfer();//转账
            break;
        case 7:
            reportloss();//挂失
            break;
        case 8:
           cancelloss();//解挂
            break;
        case 9:
            changepass();//改密
            break;
            break;
        }
    }
    while(choose);
}

void Bank::establish()
{
    if (realcount==MAXNUM)
    {
        cout<<"非常抱歉，银行储户已达上限"<<endl;
        return;
    }
    double Money,Rate;
    string id,usename,usid,place,phone;
    string Key0,Key1;
    cout<<"请输入姓名：";
    cin>>usename;
    cout<<endl;
    cout<<"请输入身份证号码：";
    cin>>usid;
    cout<<endl;
    cout<<"请输入居住地址：";
    cin>>place;
    cout<<endl;
    cout<<"请输入电话号码：";
    cin>>phone;
    cout<<endl;
ERR:
    cout<<"请输入密码：";
    Key0=inputpass();
    cout<<endl;
    cout<<"请确认密码：" ;    
    Key1=inputpass();
    cout<<endl;
    if(Key0!=Key1)
    {
     goto ERR;
    }

    check[realcount]=new Check;//动态数组开辟空间
    check[realcount]->Set(std::to_string(realcount),usename,usid,place,phone, Key0,0.0f);
    cout<<"开户成功！"<<endl;
    cout<<"+------------------------------------------------------------+"<<endl;
    cout<<"+                           开户信息                         +"<<endl;
    cout<<"          姓名: " <<check[realcount]->username                 <<endl;
    cout<<"          单号: " <<check[realcount]->checkid                 <<endl;
    cout<<"          密码: " <<"******"                                 <<endl;
    cout<<"          地址: " <<check[realcount]->userplace                 <<endl;    
    cout<<"          电话: " <<check[realcount]->phone                 <<endl;
    cout<<"+-------------------------------------------------------------+"<<endl;
    realcount++;
}

void Bank::Delete()
{
    int id;
    string Key;
    char *pErrMsg = 0;
    char sql[3000];
    cout<<"请输入销户账号：";
    cin>>id;
    cout<<endl;
    if(id<0||id>=realcount)
    {
        cout<<"该账户不存在!"<<endl;
        return;
    }
    if(check[id]->state==0)
    {
        cout<<"该账户已销户!"<<endl;
        return;
    }
    cout<<"请输入密码：";
    Key=inputpass();
    cout<<endl;
    if(checkkey(Key,check[id]))
    {
        check[id]->state=0;
        cout<<"销户成功！请取款"<<check[id]->money<<"元."<<endl;
        check[id]->money=0;
        sprintf(sql,"delete from userInfomation where ID='%d';",id);
        sqlite3_exec(db, sql, NULL, 0, &pErrMsg);  
    }
    else
    {
        cout<<"操作取消！"<<endl;
    }
}

void Bank::find()
{
   int num;
   string key;
    string sta[3]= {"注销","正常","挂失"};
    cout<<"请输入账号：";
    cin>>num;
    if(num<0||num>=realcount)
    {
        cout<<"该账户不存在！"<<endl;
        return;
    }
    cout<<"姓名："<<check[num]->username<<endl;
    cout<<"请输入密码：";
    key=inputpass();
    if(checkkey(key,check[num]))
    {
        cout<<"身份证号："<<check[num]->userid<<endl;
        cout<<"家庭住址："<<check[num]->userplace<<endl;
        cout<<"余额："<<check[num]->money<<endl;
        cout<<"状态："<<sta[check[num]->state]<<endl;
    }
    else
        cout<<"操作失败！"<<endl;

}

//功能：存款
//说明：增加账户金额，需要保证账户存在，且处于正常状态
void Bank::save()
{
    int num;
    double m;
    cout<<"请输入账号:";
    cin>>num;
    if(num<0||num>=realcount)
    {
        cout<<"该账户不存在！"<<endl;
        return;
    }
    if(check[num]->state!=1)
    {
        cout<<"该账户已经处于"<<(check[num]->state==0?"销户":"挂失")<<"状态，无法操作！"<<endl;
        return ;
    }
    cout<<"姓名："<<check[num]->username<<endl;
    cout<<"请输入存款金额：";
    cin>>m;
    check[num]->money+=m;
    cout<<"存款成功！"<<endl;
    cout<<"余额："<<check[num]->money<<endl;
}
 
//功能：取款
//说明：减少账户金额。需要保证账户存在，且处于正常状态，另外，余额要足够取
void Bank::withdraw()
{
    int num;
    string key;
    double m;
    cout<<"请输入账号：";
    cin>>num;
    if(num<0||num>=realcount)
    {
        cout<<"该账户不存在！"<<endl;
        return;
    }
    if(check[num]->state!=1)
    {
        cout<<"该账户已经处于"<<(check[num]->state==0?"销户":"挂失")<<"状态，无法操作！"<<endl;
        return ;
    }
    cout<<"姓名："<<check[num]->username<<endl;
    cout<<"请输入密码：";
    key=inputpass();
    if(checkkey(key,check[num]))
    {
        cout<<"余额："<<check[num]->money<<endl;
        cout<<"请输入取款金额：";
        cin>>m;
        if(m<=check[num]->money)
        {
            check[num]->money-=m;
            cout<<"取款成功！"<<endl;
            cout<<"余额："<<check[num]->money<<endl;
        }
        else
            cout<<"余额不足！"<<endl;
    }
    else
        cout<<"操作取消！"<<endl;
}
 


//功能：转账
//说明：需要保证两个账户都存在，且处于正常状态，另外，转出账户的余额要足够
void Bank::transfer()
{
    int in,out;
    string key;
    double m;
    cout<<"输入转出账号：";
    cin>>out;
    if(out<0||out>=realcount)
    {
        cout<<"该账户不存在！"<<endl;
        return;
    }
    if(check[out]->state!=1)
    {
        cout<<"该账户已经处于"<<(check[out]->state==0?"销户":"挂失")<<"状态，无法操作！"<<endl;
        return ;
    }
    cout<<"姓名："<<check[out]->username<<endl;
    cout<<"请输入密码：";
    key=inputpass();
    if(checkkey(key,check[out]))
    {
        cout<<"余额："<<check[out]->money<<endl;
        cout<<"请输入转账金额：";
        cin>>m;
        if(m<=check[out]->money)
        {
            cout<<"输入转入帐号：";
            cin>>in;
            if(in<0||in>=realcount)
            {
                cout<<"该账户不存在！"<<endl;
                return;
            }
            if(check[in]->state!=1)
            {
                cout<<"该账户已经处于"<<(check[in]->state==0?"销户":"挂失")<<"状态，无法操作！"<<endl;
                return ;
            }
            check[out]->money-=m;
            check[in]->money+=m;
            cout<<"转账成功！"<<endl;
            cout<<"余额："<<check[out]->money<<endl;
        }
        else
            cout<<"余额不足！"<<endl;
    }
    else
        cout<<"操作取消！"<<endl;
}
 
//功能：挂失
//说明：将用户账户状态改为挂失
void Bank::reportloss()
{
    int num;
    string key;
    cout<<"请输入账号：";
    cin>>num;
    if(num<0||num>=realcount)
    {
        cout<<"该账户不存在！"<<endl;
        return;
    }
    if(check[num]->state!=1)
    {
        cout<<"该账户已经处于"<<(check[num]->state==0?"销户":"挂失")<<"状态，无法操作！"<<endl;
        return ;
    }
    cout<<"姓名："<<check[num]->username<<endl;
    cout<<"请输入密码：";
    key=inputpass();
    if(checkkey(key,check[num]))
    {
        check[num]->state=2;
        cout<<"挂失成功！"<<endl;
    }
    else
        cout<<"操作取消！"<<endl;
}
 
//功能：解挂
//说明：将用户账户状态改为正常
void Bank::cancelloss()
{
    int num;
    string key;
    cout<<"请输入账号：";
    cin>>num;
    if(num<0||num>=realcount)
    {
        cout<<"该账户不存在！"<<endl;
        return;
    }
    if(check[num]->state!=2)
    {
        cout<<"该账户已经处于"<<(check[num]->state==0?"销户状态，无法操作！":"正常状态，无需解挂。")<<endl;
        return ;
    }
    cout<<"姓名："<<check[num]->username<<endl;
    cout<<"请输入密码：";
    key=inputpass();
    if(checkkey(key,check[num]))
    {
        check[num]->state=1;
        cout<<"解挂成功！"<<endl;
    }
    else
        cout<<"操作取消！"<<endl;
}
 
//功能：改密
//说明：更改账户密码，需在账户正常的状态下进行
void Bank::changepass()
{
    int num;
    string  key,key1,key2;
    cout<<"请输入账号：";
    cin>>num;
    if(num<0||num>=realcount)
    {
        cout<<"该账户不存在！"<<endl;
        return;
    }
    if(check[num]->state!=1)
    {
        cout<<"该账户已经处于"<<(check[num]->state==0?"销户":"挂失")<<"状态，无法操作！"<<endl;
        return ;
    }
    cout<<"姓名："<<check[num]->username<<endl;
    cout<<"请输入密码：";
    key=inputpass();
    if(checkkey(key,check[num]))
    {
        cout<<"输入新密码：";
        key1=inputpass();
        cout<<"确认密码：";
        key2=inputpass();
        if(key1==key2)
        {
            check[num]->passkey=key1;
            cout<<"修改密码成功!"<<endl;
        }
        else
            cout<<"2次密码不一致，修改失败！"<<endl;
    }
    else
        cout<<"操作取消！"<<endl;
}

void itoa(int i, char *string)
{
	int power = 0, j = 0;
 
	j = i;
	for (power = 1; j>10; j /= 10)
		power *= 10;
 
	for (; power>0; power /= 10)
	{
		*string++ = '0' + i / power;
		i %= power;
	}
	*string = '\0';
	printf("%s\n", string);
}

 int atoi(char *str)
{
	if (!str)
		return -1;
	bool bMinus = false;
	int result = 0;
 
	if (('0'>*str || *str>'9') && (*str == '+' || *str == '-'))
	{
		if (*str == '-')
			bMinus = true;
		*str++;
	}
	while (*str != '\0')
	{
		if ('0'> *str || *str>'9')
			break;
		else
			result = result * 10 + (*str++ - '0');
	}
 
	if (*str != '\0')//no-normal end  
		return -2;
 
	return bMinus ? -result : result;
}



string Bank::inputpass()
{
    cout<<"密码格式为六位数字！！！"<<endl;
    int pass=0,i;
    char ch;
    while(ch!='p'&&ch!='P')
    {
        cout<<"点击p确认"<<endl;
        cin>>ch;
    }
ERR:
    printf("请输入密码------\b\b\b\b\b\b");
    for(i=0;i<6;i++)
    {
        ch=getchar();
        if(ch>='0'&&ch<='9')
        {
            pass=pass*10+(ch-'0');
        }
        else
        {
            pass=0;
            break;
        }
    }
    if(pass==0)
    {
        printf("请输入密码------\b\b\b\b\b\b");
        goto ERR;
    }
    cout<<endl;
    return to_string(pass);
}
//功能：验证密码输入是否正确
bool Bank::checkkey(string key,Check *u)
{
    string temp=u->getpass();
    if(key==temp)
        return true;
    else
        cout<<"密码错误！"<<endl;
    return false;
}

string Check::getpass()
{
    return passkey;
}


void Check::Set(string id,string usename,string usid,string place,string pon,string key,double m)
{
    checkid=id;
    username=usename;
    userid=usid;
    userplace=place;
    phone=pon;
    passkey=key;
    state=1;
    money=m;
    rate=RATE;
}





