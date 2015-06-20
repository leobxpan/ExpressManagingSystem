//
//
//User.h
//对用户类基类的定义
//
//
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#ifndef USER_H
#define USER_H

class User
{
public:
	User(){};
	User(string un) :userName(un),passWord(""),permissionLevel(""),hasBeenDeleted(""){};
	User(string un, string pw, string pl = "unknown", string ds = "false") : userName(un), passWord(pw), permissionLevel(pl), hasBeenDeleted(ds){};
	int userExists();															//判断当前注册用户的用户名是否已被注册，已被注册返回1，否则返回0，发生异常错误返回-1
	int login();																//判断当前用户的登录是否成功，成功返回0，否则返回1.														//
	string unOfCurrentUser(){ return userName; }
	string pwOfCurrentUser(){ return passWord; }
	string plOfCurrentUser(){ return permissionLevel; }
	string dsOfCurrentUser(){ return hasBeenDeleted; }
protected:
	string userName;															//用户名
	string passWord;															//密码
	string permissionLevel;														//用户权限级别，业务经理为1，快递业务员为2
	string hasBeenDeleted;														//记录该用户是否已被删除的标志字，值true代表已被删除，false代表未被删除

	friend ifstream & operator >> (ifstream &, User &);							//重载流提取运算符
	friend ofstream & operator << (ofstream &, User &);							//重载流插入运算符

};

int User::login()
{

	char title[300];

	ifstream inquireUser("../SupportingFiles/UserInfo.txt", ios::in);

	if (!inquireUser){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	inquireUser.getline(title, 300);

	User tempUser;

	int count = 0;																//用来统计匹配次数的计数器

	while (inquireUser>>tempUser){
		if (userName == tempUser.userName){
			count++;
			if (tempUser.permissionLevel == "1"){
				if (passWord == tempUser.passWord){
					permissionLevel = "1";										//将当前用户标记为业务经理
					return 0;
				}else{
					cout << "密码错误，请检查后重新输入" << endl;
					return 1;
				}
			}else if (tempUser.permissionLevel == "2"){
				if (passWord == tempUser.passWord){
					permissionLevel = "2";										//将当前用户标记为业务员
					return 0;
				}else{
					cout << "密码错误，请检查后重新输入" << endl;
					return 1;
				}
			}
		}
	}

	if (!count){
		cout << "无此用户，请先注册" << endl;
		return 1;
	}

	inquireUser.close();

	return 1;																	//未经过一次匹配就终止，为异常情况

}

int User :: userExists()
{

	char title[300];

	ifstream inquireUser("../SupportingFiles/UserInfo.txt", ios::in);
	if (!inquireUser){
		cerr << "文件无法打开" << endl;
		return -1;
	}

	inquireUser.getline(title, 300);											//略去标题行

	User tempUser;
	while (inquireUser >> tempUser){											//从文件读入信息
		if (userName == tempUser.userName){										//该用户名已被注册
			inquireUser.close();
			return 1;
		}
	}

	inquireUser.close();
	return 0;

}



ifstream & operator >> (ifstream & get, User & user)
{
	
	get >> user.userName >> user.passWord >> user.permissionLevel >> user.hasBeenDeleted;
	return get;

}

ofstream & operator << (ofstream & write, User & user)
{
	
	write << user.userName << " " << user.passWord << " " << user.permissionLevel<< " " << user.hasBeenDeleted;
	return write;

}

#endif