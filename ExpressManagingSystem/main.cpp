#include "Controller.h"
using namespace std;

int main()
{

	Controller controller;												//整个程序生命周期中的controller项
	string choice;

	WELCOME_PAGE:														//欢迎页面
	cout << "*********************欢迎使用由Caesar开发的快递管理系统***********************\n\n"
		<< "~~                         请输入选项前对应的序号：                          ~~\n\n"
		<< "~~                            1：老用户登录                                 ~~\n\n"
		<< "~~                            2：新快递员注册                                 ~~\n\n";
	cin >> choice;
	system("cls");

	if (choice == "1"){
		string userName;
		string passWord;
		cout << "请输入：\n用户名：";
		cin >> userName;
		cout << "密码：";
		cin >> passWord;
		cout << '\n';

		controller.login(userName, passWord);

	}else if (choice == "2"){
		REGISTER:
		string userName;
		string passWord1;
		string passWord2;

		cout << "请输入：\n用户名：";
		cin >> userName;
		cout << "密码：";
		cin >> passWord1;
		cout << "请确认密码：";
		cin >> passWord2;

		if (controller.confirmPassWord(passWord1, passWord2)){
			User registerUser(userName, passWord1);
			if (!controller.ifRegisterSucceeds(userName, passWord1, passWord2)){
				Courier registerCourier = registerUser;
				registerCourier.addACourier();
				goto WELCOME_PAGE;
				return 0;
			}else{
				cout << "该用户已存在，请更换用户名" << endl;
				goto REGISTER;
			}
		}else{
			cout << "两次密码不一致，请重新输入" << endl;
			goto REGISTER;
		}

	}else{
		cout << "无此选项，请重新输入" << endl;
		goto WELCOME_PAGE;
	}

	if (currentUserName == ""){											//退出登录后重新进入欢迎页面
		goto WELCOME_PAGE;
	}

	return 0;

}