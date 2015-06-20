//
//
//Courier.h
//对User类派生类Courier类的定义
//
//
#include <fstream>
#include <vector>
#include <string>
#include "User.h"
#include "Express.h"
#include "TempInquireResultsList.h"
using namespace std;

#ifndef COURIER_H
#define COURIER_H

class Courier : public User
{

public:
	Courier(User user) { userName = user.unOfCurrentUser(); passWord = user.pwOfCurrentUser(); permissionLevel = "2"; hasBeenDeleted = user.dsOfCurrentUser(); }
	int addACourier();												//快递业务员注册时添加新业务员
	int haveSentExpress(string expressNumber);						//由业务员将已送达的快递进行标记。执行正常返回0，否则返回1。
	int courierListExpress(string courierName);						//供业务员查看自己的快递，函数内部可实现条件查询。执行正常返回0，否则返回1。

};

int Courier::addACourier()
{
	
	ofstream addNew("../SupportingFiles/UserInfo.txt", ios::app);

	if (!addNew){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	addNew << *this << "\n";										//向文件中添加当前快递业务员

	cout << "恭喜您，注册成功！" << endl;
	addNew.close();

	return 0;

}

int Courier :: haveSentExpress(string expressNumber)
{

	ifstream readExpress("../SupportingFiles/ExpressInfo.txt", ios::in);
	vector<string> expressVector;
	
	if (!readExpress){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	Express checkedExpress(expressNumber);
	if (!checkedExpress.expressExists()){
		cout << "该快递尚未被添加至列表" << endl;
		return 1;
	}

	Express tempExpress;

	while (readExpress >> tempExpress){
		if (tempExpress.numberOfCurrentExpress() == expressNumber){		//该运单为需要修改配送状态的订单
			if (tempExpress.courierOfCurrentExpress() == userName){
				if (tempExpress.statusOfCurrentExpress() == "unsent"){
					expressVector.push_back(tempExpress.numberOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.addressOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.receiverOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.senderOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.receiverPhoneOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.senderPhoneOfCurrentExpress()); expressVector.push_back(" ");
					expressVector.push_back("sent"); expressVector.push_back(" ");
					expressVector.push_back(tempExpress.courierOfCurrentExpress()); expressVector.push_back("\n");
				}else if (tempExpress.statusOfCurrentExpress() == "sent"){
					cout << "该快件已被标记为送达，请勿重复标记" << endl;
					return 1;
				}
			}else{
				cout << "您无权对未安排给您的快件进行操作" << endl;
				return 1;
			}
		}else{														//该运单不需要被修改
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

	cout << "该快件已被成功标记为已送达" << endl;

	return 0;

}

int Courier :: courierListExpress(string courierName)
{

	Express tempExpress;

	ifstream listExpress("../SupportingFiles/ExpressInfo.txt", ios::in);

	if (!listExpress){
		cerr << "文件无法打开" << endl;
		return 1;
	}

	//略去标题行
	char title[300];
	listExpress.getline(title, 300);

	string expressNumber, expressAddress, receiverName, senderName, receiverPhone, senderPhone, status, courier;

	int countUnSentExpress = 0;

	//将该业务员的全部快递存入链表
	TempInquireResults *head = NULL;
	while (listExpress >> expressNumber >> expressAddress >> receiverName >> senderName >> receiverPhone >> senderPhone >> status >> courier){
		if (courierName == courier){
			if (status == "unsent"){
				countUnSentExpress++;
			}
			TempInquireResults *newNode = new TempInquireResults;
			newNode->expressNumber = expressNumber; newNode->expressAddress = expressAddress; newNode->receiverName = receiverName; newNode->senderName = senderName; newNode->receiverPhone = receiverPhone; newNode->senderPhone = senderPhone; newNode->status = status; newNode->courier = courier;
			TempInquireResults::addFront(head, newNode);
			countExpress++;
		}
	}

	if (countExpress){													//有快递被分配给当前快递员
		string choice;
		cout << "是否进行条件查询？输入Y代表是，N代表否" << endl;
		cin >> choice;
		cout << "\n";

		if ((choice == "N") || (choice == "n")){
			//提取该业务员派送的所有快件信息
			cout << "业务员" << courierName << "您好，以下是分配给您的全部快件：\n" << endl;
			TempInquireResults::showList(head);
			cout << "\n全部快件数量为：" << countExpress << "件" << endl;

			//单独列出该快递员还未送达的快件
			cout << "其中您还未送达的快递有：" << endl;
			TempInquireResults::showUnSentExpress(head);
			cout << "\n您还未送达的快递数量为：" << countUnSentExpress << "件" << endl;

		}
		else if ((choice == "Y") || (choice == "y")){
			string keyWord;												//条件查询关键字

			do{
				cout << "请输入条件查询的关键字（可以是运单号、寄件地址、收件人姓名、寄件人姓名、收件人联系电话、寄件人联系电话、快件状态、配件人）" << endl;
				cin >> keyWord;

				TempInquireResults::deleteNode(head, keyWord);							//逐一比较keyWord与各快件的各项信息，若均不匹配则删除该快件（节点）

				if (!countExpress){
					cout << "没有符合条件的快件" << endl;
					return 1;
				}

				cout << "查询结果如下：" << endl;
				TempInquireResults::showList(head);
				cout << "符合条件的快件数量为：" << countExpress << "件" << endl;

				cout << "是否还要继续缩小范围查询？输入Y代表是，N代表否" << endl;
				cin >> choice;
			} while ((choice == "Y") || (choice == "y"));
		}
	}else{																		//没有快递被分配给当前快递员
		cout << "还没有快递被分配给您。" << endl;
	}

	countExpress = 0;														//重置计数器
	listExpress.close();
	return 0;

}

#endif