//
//
//Manager.h
//对User类派生类Manager类的定义
//
//
#include <fstream>
#include <vector>
#include <string>
#include "User.h"
#include "Express.h"
#include "TempInquireResultsList.h"
using namespace std;

#ifndef MANAGER_H
#define MANAGER_H

class Manager : public User
{

public:
	Manager(User user) { userName = user.unOfCurrentUser(); passWord = user.pwOfCurrentUser(); permissionLevel = "1"; hasBeenDeleted = user.dsOfCurrentUser(); }
	int deleteAUser(string);														//删除某指定用户（修改删除状态标志字）
	int addAnExpress(Express);														//由业务经理向快递信息列表中添加新的待送快递，配件状态自动置为未送达，派件人置为无。成功返回0，否则返回1。
	int managerListExpress();														//供经理查看全部快递，函数内部可实现条件查询。执行正常返回0，否则返回1。
	int assignExpress(string, string);												//由业务经理将未分配的快递分配给快递员。执行正常返回0，否则返回1。
	int showUsers();																//用于经理查看用户信息，成功返回0，不成功返回1

};

int Manager::deleteAUser(string userName)
{
	
	if (userName == "manager"){
		cout << "您无权操作该账户" << endl;
		return 1;
	}

	ifstream readUser("../SupportingFiles/UserInfo.txt", ios::in);
	vector<string> userVector;

	if (!readUser){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	//检查被删除用户是否存在
	User checkedUser(userName);
	if (!checkedUser.userExists()){
		cout << "该用户尚未注册" << endl;
		return 1;
	}

	User tempUser;

	while (readUser >> tempUser){
		if (userName == tempUser.unOfCurrentUser()){								//该用户为需要被删除的用户
			userVector.push_back(tempUser.unOfCurrentUser()); userVector.push_back(" ");
			userVector.push_back(tempUser.pwOfCurrentUser()); userVector.push_back(" ");
			userVector.push_back(tempUser.plOfCurrentUser()); userVector.push_back(" ");
			userVector.push_back("true"); userVector.push_back("\n");
		}else{																		//该用户不需要被删除
			userVector.push_back(tempUser.unOfCurrentUser()); userVector.push_back(" ");
			userVector.push_back(tempUser.pwOfCurrentUser()); userVector.push_back(" ");
			userVector.push_back(tempUser.plOfCurrentUser()); userVector.push_back(" ");
			userVector.push_back(tempUser.dsOfCurrentUser()); userVector.push_back("\n");
		}
	}

	readUser.close();

	ofstream putEmpty("../SupportingFiles/UserInfo.txt", ios::out);
	
	if (!putEmpty){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	putEmpty << "";

	putEmpty.close();

	ofstream inputAgain("../SupportingFiles/UserInfo.txt", ios::app);
	
	if (!inputAgain){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	for (int i = 0; i < userVector.size(); i++){
		inputAgain << userVector[i];
	}

	inputAgain.close();

	cout << "该用户已被成功删除！" << endl;

	return 0;

}

int Manager :: addAnExpress(Express express)
{
	
	ofstream addExpress("../SupportingFiles/ExpressInfo.txt", ios::app);

	if (!addExpress){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	//检查当前快递是否已被添加至列表
	Express checkedExpress(express.numberOfCurrentExpress());
	if (checkedExpress.expressExists()){
		cout << "该快件已被添加,请勿重复添加" << endl;
		return 1;
	}

	//判断运单号是否是十位数
	int bitCount = 1;																//记录运单号的位数
	int tempNumber = atoi(express.numberOfCurrentExpress().c_str());

	while (tempNumber / 10){
		tempNumber /= 10;
		bitCount++;
	}

	if (bitCount == 10){
		addExpress << express << "\n";
	}else{
		cout << "运单号必须为10位数字" << endl;
		return 1;
	}

	addExpress.close();
	return 0;

}

int Manager::managerListExpress()													//供经理查看全部快递，函数内部可实现条件查询。执行正常返回0，否则返回1。
{

	ifstream listExpress("../SupportingFiles/ExpressInfo.txt", ios::in);

	if (!listExpress){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	Express tempExpress;

	//略去标题行
	char title[300];
	listExpress.getline(title, 300);

	string expressNumber, expressAddress, receiverName, senderName, receiverPhone, senderPhone, status, courier;
	
	//先将全部快件信息存入链表
	TempInquireResults *head = NULL;
	while(listExpress >> expressNumber >> expressAddress >> receiverName >> senderName >> receiverPhone >> senderPhone >> status >> courier){
		TempInquireResults *newNode = new TempInquireResults;
		newNode->expressNumber = expressNumber; newNode->expressAddress = expressAddress; newNode->receiverName = receiverName; newNode->senderName = senderName; newNode->receiverPhone = receiverPhone; newNode->senderPhone = senderPhone; newNode->status = status; newNode->courier = courier;
		TempInquireResults :: addFront(head, newNode);
		countExpress++;
	}

	//是否进行条件查询
	string choice;
	cout << "是否需要进行条件查询？输入Y代表是，N代表否" << endl;
	cin >> choice;
	cout << "\n";

	if ((choice == "N") || (choice == "n")){										//不进行条件查询，取出全部快递信息
		TempInquireResults :: showList(head);
		cout << "\n全部快件数量为：" << countExpress << "件" << endl;
	}else if ((choice == "Y") || (choice == "y")){									//进行条件查询
		string keyWord;																//条件查询关键字
		do{
			cout << "请输入条件查询的关键字（可以是运单号、寄件地址、收件人姓名、寄件人姓名、收件人联系电话、寄件人联系电话、快件状态、配件人）" << endl;
			cin >> keyWord;
			cout << "\n";

			TempInquireResults :: deleteNode(head, keyWord);						//逐一比较keyWord与各快件的各项信息，若均不匹配则删除该快件（节点）

			if (!countExpress){ 
				cout << "没有符合条件的快递" << endl;
				return 1;
			}

			cout << "查询结果如下：" << endl;
			TempInquireResults :: showList(head);
			cout << "符合条件的快件数量为：" << countExpress << "件" << endl;

			cout << "是否还要继续缩小范围查询？输入Y代表是，N代表否" << endl;
			cin >> choice;
			cout << "\n";
		} while ((choice == "Y") || (choice == "y"));
	}

	countExpress = 0;																//重置计数器
	listExpress.close();
	return 0;

}

int Manager :: assignExpress(string expressNumber, string courierName)
{
	
	ifstream readExpress("../SupportingFiles/ExpressInfo.txt", ios::in);
	vector<string> expressVector;

	if (!readExpress){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	Express tempExpress;
	
	//被分配的快递员不存在
	User tempUser(courierName);
	if (!tempUser.userExists()){
		cout << "该快递员尚未注册" << endl;
		return 1;
	}

	//该快递员已被删除
	if (tempUser.dsOfCurrentUser() == "true"){
		cout << "该快递员已被删除" << endl;
		return 1;
	}

	//该快递尚未被添加
	Express checkedExpress(expressNumber);
	if (!checkedExpress.expressExists()){
		cout << "该快递尚未被添加至列表" << endl;
		return 1;
	}

	while (readExpress >> tempExpress){
		if (expressNumber == tempExpress.numberOfCurrentExpress()){					//该订单为需要被分配的订单
			//重复分配给同一快递员
			if (courierName == tempExpress.courierOfCurrentExpress()){
				cout << "该快递已被分配给该快递员，请勿重复分配！" << endl;
				return 1;
			}
			//该邮件已被分配给其他快递员但还未被送达
			else if ((tempExpress.courierOfCurrentExpress() != "none") && (tempExpress.statusOfCurrentExpress() == "unsent")){
				cout << "该快递已被分配给快递员：" << tempExpress.courierOfCurrentExpress() << "，但还未被送达，是否确认要更改派送员？(Y/N)" << endl;
				string choice;
				cin >> choice;
				if ((choice == "Y") || (choice == "y")){
					expressVector.push_back(tempExpress.numberOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.addressOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.receiverOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.senderOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.receiverPhoneOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.senderPhoneOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.statusOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(courierName); expressVector.push_back("\n");
				}else if((choice == "N") || (choice == "n")){
					return 1;
				}
			}else if ((tempExpress.courierOfCurrentExpress() != "none") && (tempExpress.statusOfCurrentExpress() == "sent")){
				cout << "该快递已由其他快递员送达" << endl;
				return 1;
			}else{
				expressVector.push_back(tempExpress.numberOfCurrentExpress()); expressVector.push_back(" ");
				expressVector.push_back(tempExpress.addressOfCurrentExpress()); expressVector.push_back(" ");
				expressVector.push_back(tempExpress.receiverOfCurrentExpress()); expressVector.push_back(" ");
				expressVector.push_back(tempExpress.senderOfCurrentExpress()); expressVector.push_back(" ");
				expressVector.push_back(tempExpress.receiverPhoneOfCurrentExpress()); expressVector.push_back(" ");
				expressVector.push_back(tempExpress.senderPhoneOfCurrentExpress()); expressVector.push_back(" ");
				expressVector.push_back(tempExpress.statusOfCurrentExpress()); expressVector.push_back(" ");
				expressVector.push_back(courierName); expressVector.push_back("\n");
			}
		}else{																		//该订单不需要被分配
			expressVector.push_back(tempExpress.numberOfCurrentExpress()); expressVector.push_back(" ");
			expressVector.push_back(tempExpress.addressOfCurrentExpress()); expressVector.push_back(" ");
			expressVector.push_back(tempExpress.receiverOfCurrentExpress()); expressVector.push_back(" ");
			expressVector.push_back(tempExpress.senderOfCurrentExpress()); expressVector.push_back(" ");
			expressVector.push_back(tempExpress.receiverPhoneOfCurrentExpress()); expressVector.push_back(" ");
			expressVector.push_back(tempExpress.senderPhoneOfCurrentExpress()); expressVector.push_back(" ");
			expressVector.push_back(tempExpress.statusOfCurrentExpress()); expressVector.push_back(" ");
			expressVector.push_back(tempExpress.courierOfCurrentExpress()); expressVector.push_back("\n");
		}
	}

	readExpress.close();

	ofstream putEmpty("../SupportingFiles/ExpressInfo.txt", ios::out);

	if (!putEmpty){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	putEmpty << "";

	putEmpty.close();

	ofstream writeExpress("../SupportingFiles/ExpressInfo.txt", ios::app);

	if (!writeExpress){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	for (int i = 0; i < expressVector.size(); i++){
		writeExpress << expressVector[i];
	}

	writeExpress.close();

	cout << "该快件已被成功分配！" << endl;
	
	return 0;

}

int Manager :: showUsers()
{

	char title[300];
	string un, pw, pl;																//用户名，密码，权限级别

	ifstream inquireUser("../SupportingFiles/UserInfo.txt", ios::in);

	if (!inquireUser){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	inquireUser.getline(title, 300);

	User tempUser;

	cout << "已有用户如下：\n" << endl;

	while (inquireUser >> tempUser){
		if (tempUser.dsOfCurrentUser() == "false"){
			cout << tempUser.unOfCurrentUser() << endl;
		}
	}

	inquireUser.close();

	return 0;

}

#endif