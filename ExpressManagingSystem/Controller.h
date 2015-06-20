//
//
//Controller.h
//对处理用户动作的后台操作的Controller类定义
//
//
#include <iostream>
#include <string>
#include "User.h"
#include "Manager.h"
#include "Courier.h"
#include "Express.h"
using namespace std;

#ifndef CONTROLLER_H
#define CONTROLLER_H

//定义全局变量currentUserName
static string currentUserName;

class Controller
{
public:
	bool confirmPassWord(string passWord1, string passWord2);										//用于在注册新用户时确认密码，两次密码一致则返回1，不一致则返回0。
	bool ifRegisterSucceeds(string userName, string passWord1, string passWord2);					//用于判断新用户注册是否成功，成功返回0，不成功返回1。
	int login(string userName, string passWord);													//用户登录时判断登录是否成功，若成功则返回1，不成功（用户不存在或密码错误）则返回0，发生异常错误返回-1
	void logOut();																					//用于用户登出
};

bool Controller::confirmPassWord(string passWord1, string passWord2)
{

	if (passWord1 == passWord2){
		return 1;
	}else{
		return 0;
	}

}

bool Controller::ifRegisterSucceeds(string userName, string passWord1, string passWord2)
{
	
	bool unJudgingResult, pwJudingResult;															//用于储存用户名和密码是否符合要求的布尔类型变量

	pwJudingResult = confirmPassWord(passWord1, passWord2);
	
	if (!pwJudingResult){
		cout << "两次密码不一致" << endl;
		return 1;
	}

	User newUser(userName, passWord1, "2");

	unJudgingResult = newUser.userExists();

	if (unJudgingResult){
		return 1;
	}

	return 0;

}

int Controller :: login(string userName, string passWord)
{

	User currentUser(userName, passWord);
	
	if (!currentUser.login()){
		system("cls");																				//清屏
		if (currentUser.plOfCurrentUser() == "1"){
			cout << "经理，欢迎您回来！您可选择：\n1、添加新快件\n2、查看现有快件\n3、分配现有快件\n4、查看用户信息\n5、删除用户\n6、退出登录\n请输入选项前对应数字" << endl;
			Manager currentManager = currentUser;
			currentUserName = currentManager.unOfCurrentUser();

			string choice;

			MANAGER_CHOOSE:
			cin >> choice;
			cout << "\n";

			if (choice == "1"){
				cout << "请依次输入要添加快件的运单号（单号为10位数字）、寄件地址、收件人姓名、寄件人姓名、收件人联系电话、寄件人联系电话，中间以回车分隔" << endl;
				string expressNumber, address, receiverName, senderName, receiverPhone, senderPhone;
				cin >> expressNumber >> address >> receiverName >> senderName >> receiverPhone >> senderPhone;

				Express newExpress(expressNumber, address, receiverName, senderName, receiverPhone, senderPhone);		//创建新快递
				currentManager.addAnExpress(newExpress);

				cout << "\n";
				cout << "请继续选择：\n1、添加新快件\n2、查看现有快件\n3、分配现有快件\n4、查看用户信息\n5、删除用户\n6、退出登录\n请输入选项前对应数字" << endl;
				goto MANAGER_CHOOSE;
			}else if (choice == "2"){
				currentManager.managerListExpress();

				cout << "\n";
				cout << "请继续选择：\n1、添加新快件\n2、查看现有快件\n3、分配现有快件\n4、查看用户信息\n5、删除用户\n6、退出登录\n请输入选项前对应数字" << endl;
				goto MANAGER_CHOOSE;
			}else if (choice == "3"){
				cout << "请输入要分配快件的运单号和分配给的业务员姓名：" << endl;
				string expressNumber, courierName;
				cin >> expressNumber >> courierName;
				currentManager.assignExpress(expressNumber, courierName);

				cout << "\n";
				cout << "请继续选择：\n1、添加新快件\n2、查看现有快件\n3、分配现有快件\n4、查看用户信息\n5、删除用户\n6、退出登录\n请输入选项前对应数字" << endl;
				goto MANAGER_CHOOSE;
			}else if (choice == "4"){
				currentManager.showUsers();

				cout << "\n";
				cout << "请继续选择：\n1、添加新快件\n2、查看现有快件\n3、分配现有快件\n4、查看用户信息\n5、删除用户\n6、退出登录\n请输入选项前对应数字" << endl;
				goto MANAGER_CHOOSE;
			}else if (choice == "5"){
				cout << "请输入要删除的用户的用户名：" << endl;
				string userName;
				cin >> userName;
				currentManager.deleteAUser(userName);

				cout << "\n";
				cout << "请继续选择：\n1、添加新快件\n2、查看现有快件\n3、分配现有快件\n4、查看用户信息\n5、删除用户\n6、退出登录\n请输入选项前对应数字" << endl;
				goto MANAGER_CHOOSE;
			}else if (choice == "6"){
				cout << "\n";
				logOut();
				system("cls");
			}else{
				cout << "\n";
				cout << "无此选项，请重新输入" << endl;
				goto MANAGER_CHOOSE;
			}
		}else{
			Courier currentCourier = currentUser;
			cout << "业务员" << currentCourier.unOfCurrentUser() << ",您好！您可选择：\n1、查看自己需派送的快件\n2、标记已送达快件\n3、退出登录\n请输入选项前对应的数字" << endl;

			string choice;

			COURIER_CHOOSE:
			cin >> choice;
			cout << "\n" << endl;

			if (choice == "1"){
				currentCourier.courierListExpress(currentCourier.unOfCurrentUser());
				
				cout << "\n";
				cout << "请继续选择：\n1、查看自己需派送的快件\n2、标记已送达快件\n3、退出登录\n请输入选项前对应的数字" << endl;
				goto COURIER_CHOOSE;
			}else if (choice == "2"){
				cout << "请输入您需要标记为已送达的快件运单号" << endl;
				string expressNumber;
				cin >> expressNumber;
				currentCourier.haveSentExpress(expressNumber);
				
				cout << "\n";
				cout << "请继续选择：\n1、查看自己需派送的快件\n2、标记已送达快件\n3、退出登录\n请输入选项前对应的数字" << endl;
				goto COURIER_CHOOSE;
			}else if (choice == "3"){
				cout << "\n";
				logOut();
				system("cls");
			}else{
				cout << "\n";
				cout << "无此选项，请重新输入" << endl;
				goto COURIER_CHOOSE;
			}
		}
		return 0;
	}

	return 1;

}

void Controller :: logOut()
{

	currentUserName = "";

}

#endif