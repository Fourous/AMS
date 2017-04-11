#include <malloc.h>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>
#include"model.h"
#include"card_file.h"
#include"card_service.h"
#include"billing.h"
bool addCard(Card card){
	if(saveCardToFile(card,".\\data\\card.ams")==true)
		return true;
	return false;
}
bool search_Card(){
	readfilecard();			//加载文件中卡信息
	printf("请输入要查询的卡号:");
	char Id[18];
	scanf("%s",&Id);
	if(queryCardFromList(Id)!=NULL){
		Card card;
		memcpy(&card,queryCardFromList(Id),sizeof(Card));
		printf("查询的卡信息：\n");
		printf("卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间\n");
		printf("%s\t%d\t%.2f\t%.2f\t\t%d\t%s",card.Name,card.nStatus,card.aBalance,card.fTotalUse,card.nUseCount,timeToString(card.tLastTime));
		return true;
	}
	return false;

}
void Login() {
	printf("--------上机--------\n");
	readfilecard();			//加载文件中卡信息
	printf("请输入上机卡号卡号<长度1~18>:");
	char Id[18];
	scanf("%s", &Id);
	printf("请输入上机密码<长度1~8>:");
	char pwd[8];
	scanf("%s", &pwd);
	if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
		if (queryCardFromList(Id) != NULL) {			//查询卡非空
			Card card;
			memcpy(&card, queryCardFromList(Id), sizeof(Card));   
			if (strcmp(card.Pwd,pwd)==0) {				//密码匹配
				if (card.aBalance>0) {					//余额大于零
					if (card.nStatus == 0) {			//状态为未上机
							memcpy(&card, dologin(Id), sizeof(Card));							//上机并更新卡信息，文件信息
							printf("--------上机卡信息如下：--------\n");
							printf("卡号\t余额\t上机时间\n");
							printf("%s\t%.2f\t%s", card.Name, card.aBalance, timeToString(card.tLastTime));
					}
					else printf("该卡正在使用或已经注销");
				}
				else printf("余额不足或已经注销");
			}
			else printf("密码错误");
		}
		else printf("不存在该卡，上机失败");
	}
	else printf("卡号或密码长度有误");
}

void Logout(){
printf("--------下机--------\n");
readfilecard();			//加载文件中卡信息
printf("请输入下机卡号卡号<长度1~18>:");
char Id[18];
scanf("%s", &Id);
printf("请输入下机密码<长度1~8>:");
char pwd[8];
scanf("%s", &pwd);
if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
	if (queryCardFromList(Id) != NULL) {				//查询卡非空
		Card card;
		memcpy(&card, queryCardFromList(Id), sizeof(Card));
		if (strcmp(card.Pwd, pwd) == 0) {				//密码匹配
			if (card.nStatus == 1) {					//卡状态为正在上机
				double temp = card.aBalance - amount(card.tLastTime);
				if (temp>=0) {							//余额不小于零
					if (dologout(Id) == true) {
						//下机并更新卡信息，文件信息  
						printf("下机成功");
					}
					else {printf("下机失败或消费记录更新失败");}
				}
				else printf("该卡余额不足");
			}
			else printf("该卡未上机或已经注销");
		}
		else printf("密码错误");
	}
	else printf("不存在该卡，下机失败");
}
else printf("卡号或密码长度有误");
}

void Top_up() {
	printf("--------充值--------\n");
	readfilecard();			//加载文件中卡信息
	printf("请输入充值卡号<长度1~18>:");
	char Id[18];
	scanf("%s", &Id);
	printf("请输入充值卡密码<长度1~8>:");
	char pwd[8];
	scanf("%s", &pwd);
	if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
		if (queryCardFromList(Id) != NULL) {				//查询卡非空
			Card card;
			memcpy(&card, queryCardFromList(Id), sizeof(Card));
			if (strcmp(card.Pwd, pwd) == 0) {				//密码匹配
				if (card.nStatus == 1|| card.nStatus == 0) {					//卡状态为正在使用或未使用
						printf("请输入充值金额:\n");
						float up_banlance;
						scanf("%f", &up_banlance);
						if (do_top_up(Id,up_banlance) == true) {
							//充值成功
							printf("充值成功");
						}
						else printf("充值失败");
					}
				else printf("该卡失效或已经注销");
			}
			else printf("密码错误");
		}
		else printf("不存在该卡，下机失败");
	}
	else printf("卡号或密码长度有误");
}
void Refund() {
	printf("--------退费--------\n");
	readfilecard();			//加载文件中卡信息
	printf("请输入卡号<长度1~18>:");
	char Id[18];
	scanf("%s", &Id);
	printf("请输入卡密码<长度1~8>:");
	char pwd[8];
	scanf("%s", &pwd);
	if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
		if (queryCardFromList(Id) != NULL) {				//查询卡非空
			Card card;
			memcpy(&card, queryCardFromList(Id), sizeof(Card));
			if (strcmp(card.Pwd, pwd) == 0) {				//密码匹配
				if (card.nStatus ==0) {						//卡状态为未上机
					if (card.aBalance >= 0) {				//余额不小于零
						printf("请输入退费金额:");
						float refund_money;
						scanf("%f", &refund_money);
						if (do_refund(Id,refund_money) == true) {
							printf("退费成功");
						}
						else printf("退费失败");
					}
					else printf("该卡余额不足");
				}
				else printf("该卡失效或正在上机或已经注销");
			}
			else printf("密码错误");
		}
		else printf("不存在该卡，下机失败");
	}
	else printf("卡号或密码长度有误");
}
void readfilecard(){
	releaseCardList();
	if(readCardFromFile(".\\data\\card.ams")!=1){
	printf("读取失败-");
	}
	else printf("加载结束-");
}
void Del_card() {
	printf("--------注销卡--------\n");
	readfilecard();			//加载文件中卡信息
	printf("请输入卡号<长度1~18>:");
	char Id[18];
	scanf("%s", &Id);
	printf("请输入密码<长度1~8>:");
	char pwd[8];
	scanf("%s", &pwd);
	if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
		if (queryCardFromList(Id) != NULL) {			//查询卡非空
			Card card;
			memcpy(&card, queryCardFromList(Id), sizeof(Card));
			if (strcmp(card.Pwd, pwd) == 0) {				//密码匹配
				if (card.nStatus == 0) {			//状态为未上机
					if (do_del_card(Id) == true) {
						printf("成功注销该卡");
					}
					else printf("注销失败");
				}
				else printf("该卡正在使用或已经注销或失效");
			}
			else printf("密码错误");
		}
		else printf("不存在该卡，上机失败");
	}
	else printf("卡号或密码长度有误");
}

