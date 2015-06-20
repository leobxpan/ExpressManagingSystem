//
//
//TempInquireResultsList.h
//该文件包含了储存暂存结果的链表的相关定义
//
//
#include <iostream>
#include <iomanip>
#include <string>
#include "Express.h"
using namespace std;
#ifndef TempInquireResultsLIST_H
#define TempInquireResultsLIST_H

static int countExpress = 0;																//用于统计快件的数量

//定义暂存条件筛选结果的链表，用class实现
class TempInquireResults
{
	
public:
	string expressNumber;
	string expressAddress;
	string receiverName;
	string senderName;
	string receiverPhone;
	string senderPhone;
	string status;
	string courier;
	TempInquireResults *next;

	//定义对链表的相关操作
	TempInquireResults(){};
	TempInquireResults & operator = (Express &);											//重载赋值号使得通过一个Express类对象构建TempInquireResults对象成为可能
	static void addFront(TempInquireResults *& head, TempInquireResults *& node);			//在表头插入新节点
	static void deleteNode(TempInquireResults *& head, string keyWord);						//判断条件并删除节点
	static void showList(TempInquireResults * head);										//遍历并输出链表
	static void showUnSentExpress(TempInquireResults * head);								//打印业务员未送达的快递

};


//函数定义
TempInquireResults & TempInquireResults :: operator = (Express & ex)
{
	
	expressNumber = ex.expressNumber;
	expressAddress = ex.address;
	receiverName = ex.receiverName;
	senderName = ex.senderName;
	receiverPhone = ex.receiverPhone;
	senderPhone = ex.senderPhone;
	status = ex.status;
	courier = ex.courier;
	next = NULL;

	return *this;

}

void TempInquireResults :: addFront(TempInquireResults *& head, TempInquireResults *& node)
{

	node->next = head;
	head = node;

}

void TempInquireResults :: deleteNode(TempInquireResults *& head, string keyWord)			//逐一比较keyWord与各快件的各项信息，若均不匹配则删除该快件（节点）
{

	//被删节点是头节点
	while ((head->expressNumber != keyWord) && (head->expressAddress != keyWord) && (head->receiverName != keyWord) && (head->senderName != keyWord) && (head->receiverPhone != keyWord) && (head->senderPhone != keyWord) && (head->status != keyWord) && (head->courier != keyWord)){												//被删节点是头节点
		TempInquireResults *tempNode;
		tempNode = head;
		if (head->next){
			head = head->next;
			countExpress--;
		}else{
			countExpress--;
			break;
		}
		delete tempNode;
		tempNode = NULL;
	}

	//被删节点是后续节点，查找并删除节点
	for (TempInquireResults *tempListNode = head; tempListNode->next;){
		if ((tempListNode->next->expressNumber != keyWord) && (tempListNode->next->expressAddress != keyWord) && (tempListNode->next->receiverName != keyWord) && (tempListNode->next->senderName != keyWord) && (tempListNode->next->receiverPhone != keyWord) && (tempListNode->next->senderPhone != keyWord) && (tempListNode->next->status != keyWord) && (tempListNode->next->courier != keyWord)){
			TempInquireResults *tempNode = NULL;
			tempNode = tempListNode->next;
			if (tempNode->next){												
				tempListNode->next = tempNode->next;		
			}else{																			//被删节点已经是最后一个节点
				tempListNode->next = NULL;
			}
			delete tempNode;
			tempNode = NULL;
			countExpress--;
		}else if(tempListNode->next){
			tempListNode = tempListNode->next;
		}
	}

	//仅剩一个节点且该节点需要被删除
	if ((!head->next) && (head->expressNumber != keyWord) && (head->expressAddress != keyWord) && (head->receiverName != keyWord) && (head->senderName != keyWord) && (head->receiverPhone != keyWord) && (head->senderPhone != keyWord) && (head->status != keyWord) && (head->courier != keyWord)){															//head为最后一个节点且仍与关键词不匹配
		head = NULL;
	}

}

void TempInquireResults :: showList(TempInquireResults * head)
{

	TempInquireResults *tempHead = head;

	//打印表头
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << std::left << setw(12) << "运单号" << setw(10) << "邮寄地址" << setw(10) << "收件人姓名" << setw(10) << "寄件人姓名" << setw(10) << "收件人电话" << setw(10) << "寄件人电话" << setw(8) << "快件状态" << setw(10) << "配件人姓名" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;

	//打印表格及快件信息
	while (tempHead){
		cout << std::left << setw(12) << tempHead->expressNumber << setw(10) << tempHead->expressAddress << setw(10) << tempHead->receiverName << setw(10) << tempHead->senderName << setw(10) << tempHead->receiverPhone << setw(10) << tempHead->senderPhone << setw(8) << tempHead->status << setw(10) << tempHead->courier << endl;
		cout << "--------------------------------------------------------------------------------" << endl;
		tempHead = tempHead->next;
	}

	cout << "\n";

}

void TempInquireResults::showUnSentExpress(TempInquireResults * head)
{

	TempInquireResults *tempHead = head;

	//打印表头
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << std::left << setw(12) << "运单号" << setw(10) << "邮寄地址" << setw(10) << "收件人姓名" << setw(10) << "寄件人姓名" << setw(10) << "收件人电话" << setw(10) << "寄件人电话" << setw(8) << "快件状态" << setw(10) << "配件人姓名" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;

	//打印表格及快件信息
	while (tempHead){
		if (tempHead->status == "unsent"){
			cout << std::left << setw(12) << tempHead->expressNumber << setw(10) << tempHead->expressAddress << setw(10) << tempHead->receiverName << setw(10) << tempHead->senderName << setw(10) << tempHead->receiverPhone << setw(10) << tempHead->senderPhone << setw(8) << tempHead->status << setw(10) << tempHead->courier << endl;
			cout << "--------------------------------------------------------------------------------" << endl;
		}
		tempHead = tempHead->next;
	}

}

#endif