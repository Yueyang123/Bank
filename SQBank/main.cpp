/*
 * @Descripttion: 
 * @version: 
 * @Author: Yueyang
 * @email: 1700695611@qq.com
 * @Date: 2021-03-16 21:52:15
 * @LastEditors: Yueyang
 * @LastEditTime: 2021-03-17 12:39:23
 */
#include <iostream>
#include <string>
#include "Bank.h"
using namespace std;

int main()
{
    cout<<"+-------------------------------+"<<endl;
    cout<<"         欢迎光临天地银行！"<<endl;
    cout<<"+-------------------------------+"<<endl;

   Bank *bank=new Bank();
   bank->work();
   delete bank;
   return 0;
}

