/*
 * @Descripttion: 银行储蓄系统
 * @version: V 1.0
 * @Author: Yueyang
 * @email: 1700695611@qq.com
 * @Date: 2021-03-16 21:50:20
 * @LastEditors: Yueyang
 * @LastEditTime: 2021-03-17 12:31:40
 */
#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <sqlite3.h>
#define MAXNUM 2000
using namespace std;

class Check
{
public:
    string checkid;
    string username;
    string userid;
    string userplace;
    string phone;
    string passkey;
    double money;
    double rate;
    int state;
    friend class BANK;
public:
    void Set(string,string,string,string,string,string,double);
    string getpass();

};

class Bank
{
private:
    int realcount;
    friend class Check;
    string inputpass();
public:
    Bank();
    ~Bank();
    sqlite3 *db;
    void work();
    int menushow();
    void establish();
    void Delete();
    void save();
    void withdraw();
    void find();
     void transfer();
     void reportloss();
     void cancelloss();
     void changepass();
    bool checkkey(string key,Check *u);
};

#endif // !BANK_H


