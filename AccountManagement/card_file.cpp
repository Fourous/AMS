#include<cstring>
#include<cstdio>
#include <iostream> 
#include <fstream>
#include"model.h"
#include"card_file.h"
#include"billing.h"
#include"card_service.h"
using namespace std;
char dest[20]={0};
char temp[256];
static int nCount=0;
IpCardNode CardList=NULL;
static IpCardNode head=NULL;
int initCardList(){
	head=(IpCardNode)malloc(sizeof(CardNode));
	if(head!=NULL){
		head->next=NULL;
		CardList=head;
		return 1;
	}
	return 0;
}
char* timeToString(time_t time){
	struct tm *pTimeInfo;
	pTimeInfo=localtime(&time);
	strftime(dest,20,"%Y-%m-%d %H:%M",pTimeInfo);
	return dest;
}
time_t stringTotime(char* pTime){
	struct tm tm1;
	time_t time1;
	sscanf(pTime,"%d-%d-%d %d:%d",&tm1.tm_year,&tm1.tm_mon,&tm1.tm_mday,&tm1.tm_hour,&tm1.tm_min);
	tm1.tm_sec=0;
	tm1.tm_year   -=  1900; /*年份值减去1900，得到tm结构中保存的年份序数*/
    tm1.tm_mon    -=  1;    /*月份值减去1，得到tm结构中保存的月份序数*/
	time1 = mktime(&tm1);
	return time1;

}
bool saveCardToFile(const Card card,const char*FilePath){
	FILE *fp = fopen(FilePath, "ab+");
	if(NULL==fp)
		{
			printf("卡信息文件读取错误");
			return false;//要返回错误代码
		  }
	else{
	 fwrite(&card,sizeof(Card),1,fp);
     fclose(fp);
	 fp=NULL;//需要指向空，否则会指向原打开文件地址
	 return true;
	}
}
int addcardToList(Card card){
	nCount++;//卡数量计数
	IpCardNode cur=NULL;
	if(CardList==NULL){
		initCardList();
	}
	//数据保存到节点
	cur=(IpCardNode)malloc(sizeof(CardNode));
	if(cur!=NULL){
			cur->data=card;
			cur->next=NULL;
		//遍历链表找到最后节点
		while(head->next!=NULL){
			head=head->next;
		}
		head->next=cur;
		nCount++;
		return 1;
		}
	  return 0;
}
void releaseCardList(){
	IpCardNode cur;
	if(CardList!=NULL){
		cur=CardList->next;
		while(CardList->next!=NULL){
		cur=CardList->next;
		free(CardList);
		CardList=cur;
		}
	}
	cur=NULL;
	CardList=NULL;
}
int readCardFromFile(const char*pPath){
	nCount=0;
	releaseCardList();
	FILE *fp = fopen(pPath, "rb");
	if(fp== NULL){
		return 0;
    }
	while(!feof(fp))
	{     Card tempp;
		  if(fread(&tempp,sizeof(Card),1,fp)!=NULL){
		  addcardToList(tempp);
		  }
	}
    fclose(fp);
	return 1;
}
int getCardCount(){
	return nCount;
}
Card praseCard(char* pBuf){
	char buf[256];
	strcpy(buf,pBuf);
	Card card;
	const char *de="##";
	char *str=NULL;
	char *tem=NULL;
	char flag[10][20]={0};
	int index=0;
	tem=buf;
	while((str=strtok(tem,de))!=NULL){
		strcpy(flag[index],str);
		tem=NULL;
		index++;
	   }
	strcpy(card.Name,(char*)flag[0]);
	strcpy(card.Pwd,flag[1]);
	card.nStatus=atoi(flag[2]);
	card.tStart=stringTotime(flag[3]);
	card.tEnd=stringTotime(flag[4]);
	card.fTotalUse=(float)atof(flag[5]);
	card.tLastTime=stringTotime(flag[6]);
	card.nUseCount=atoi(flag[7]);
	card.aBalance=(float)atof(flag[8]);
	card.nDel=atoi(flag[9]);
	return card;
}
Card* queryCardFromList(const char* pName){
	IpCardNode cur=NULL;
	if(CardList!=NULL){
		cur=CardList->next;
		while(cur!=NULL){
			if(strcmp(cur->data.Name,pName)==0){
				return &cur->data;
			}
			cur=cur->next;
		}
	}
	return NULL;
}
Card* dologin(const char* pName) {
	IpCardNode curr = NULL;
	int Index = 0;
	if (CardList != NULL) {
		curr = CardList->next;
		while (curr != NULL) {
			if (strcmp(curr->data.Name, pName) == 0) {
				curr->data.tLastTime = time(0);
				curr->data.nStatus = 1;
				curr->data.nUseCount++;
				Billing billing;				//修改消费记录结构体信息
				billing.nDel = 0;
				billing.fAmount = 0;
				billing.nStatus = 1;
				billing.tstart = curr->data.tLastTime;
				billing.tend=time(0);			//默认为当前时间
				strcpy(billing.CardName, curr->data.Name);
				if(saveBillingTofile(billing,".\\data\\billing.ams")==true){
					//添加一条上机记录
					if(updateCardToFile(&curr->data, ".\\data\\card.ams",Index )==true)
						return &curr->data;			//更新上机卡信息
					}
				}
			Index++;
			curr = curr->next;
		}
	}
	return NULL;
}
double amount(time_t t) {
	int sec, min, count;
	double amount;
	time_t st = time(0);
	sec = st - t; 
	min = sec / 60;
	if (min%15==0) {			//十五分钟为单位
		count = min / 15;        //恰好取整
	}
	else {
		count = min/15 + 1;    //不足十五分钟进一
	} 
	if(count==0){
		count = 1;				//登录至少计费一单位
	}
	amount = count*0.5;        //每十五分钟0.5元
	return amount;
}
bool dologout(const char* cardName) {
	IpCardNode curr = NULL;
	int Index = 0;
	if (CardList != NULL) {
		curr = CardList->next;
		while (curr != NULL) {
			if (strcmp(curr->data.Name, cardName) == 0) {
				curr->data.aBalance= curr->data.aBalance-amount(curr->data.tLastTime);	//修改余额
				curr->data.nStatus = 0;								//状态下机
					Billing billing;				//修改消费记录结构体信息
					billing.nDel = 0;
					billing.fAmount = amount(curr->data.tLastTime);
					billing.nStatus =0;
					billing.tstart = curr->data.tLastTime;
					billing.tend=time(0);			
					strcpy(billing.CardName, curr->data.Name);
					if (updateBillingTofile( billing, ".\\data\\billing.ams") == true) {     //更新消费记录
						if (updateCardToFile(&curr->data, ".\\data\\card.ams",Index) == true)	// 更新文件信息
						{
						printf("--------下机卡信息如下：--------\n");
						printf("卡号\t消费\t余额\t上机时间\t\t下机时间\n");
						printf("%s\t%.1f\t%.1f\t%s\t%s\n", curr->data.Name, amount(curr->data.tLastTime), curr->data.aBalance, timeToString(curr->data.tLastTime), timeToString(time(0)));
						return true;								//更新下机卡信息
						}
				}
			}
			Index++;
			curr = curr->next;
		}
	}
	return false;
}
bool updateCardToFile(const Card* card, const char* path ,int nIndex) {
	FILE * fp;
	int nLine = 0;
	fp = fopen(path, "rb+");
	if (fp == NULL){
		return false; 
	}
	Card tempp;
	long lposition = 0;
	while (!feof(fp) && (nLine <nIndex)) {
		if(fread(&tempp,sizeof(Card),1,fp)!=NULL){
			lposition = ftell(fp);
			nLine++;
			//获取文件标识位置
		}
	}			
	fseek(fp,lposition,0);
	fwrite(card,sizeof(Card),1,fp);
	fclose(fp);
	fp = NULL;//需要指向空，否则会指向原打开文件地址
	return true;
}
bool do_top_up(const char* cardName, float up_money) {
	int Index = 0;
	IpCardNode curr = NULL;
	if (CardList != NULL) {
		curr = CardList->next;
		while (curr != NULL) {
			if (strcmp(curr->data.Name, cardName) == 0) {
				curr->data.aBalance = curr->data.aBalance + up_money;//修改余额
				if (updateCardToFile(&curr->data, ".\\data\\card.ams",Index) == true)	//更新充值卡信息
				{
					printf("充值信息如下:\n");
					printf("卡号\t充值金额\t余额\n");
					printf("%s\t%0.1f\t\t%.1f\n", curr->data.Name, up_money, curr->data.aBalance);
					return true;
				}
			}
			Index ++;
			curr = curr->next;
		}
		return false;
	}
}
bool do_refund(const char* cardName, float refund_money) {
	IpCardNode curr = NULL;
	int Index = 0;
	if (CardList != NULL) {
		curr = CardList->next;
		while (curr != NULL) {
			if (strcmp(curr->data.Name, cardName) == 0) {
				curr->data.aBalance = curr->data.aBalance - refund_money;//修改余额
				if (curr->data.aBalance >= 0) {
					if (updateCardToFile(&curr->data, ".\\data\\card.ams",Index) == true)	//更新退费卡信息
					{
						printf("退费信息如下:\n");
						printf("卡号\t退费金额\t余额\n");
						printf("%s\t%0.1f\t\t%.1f\n", curr->data.Name, refund_money,curr->data.aBalance);
						return true;
					}
				}
				else {
					printf("退费金额超过余额");
					return false;
				}
			}
			Index ++;
			curr = curr->next;
		}
		return false;
	}
}
bool do_del_card(const char* cardName) {
	IpCardNode curr = NULL;
	int Index = 0;
	if (CardList != NULL) {
		curr = CardList->next;
		while (curr != NULL) {
			if (strcmp(curr->data.Name, cardName) == 0) {
				curr->data.nStatus = 2;
				float temp = curr->data.aBalance;
				curr->data.aBalance = 0;
					if (updateCardToFile(&curr->data, ".\\data\\card.ams",Index) == true)	//更新删除卡信息
					{
						printf("注销息如下:\n");
						printf("卡号\t退费金额\n");
						printf("%s\t%0.1f\n", curr->data.Name,temp);
						return true;
						return true;
					}
			}
			Index++;
			curr = curr->next;
		}
		return false;
	}
}
void showAllcard(){
	readfilecard();
	printf("\n卡号\t密码\t状态\t余额\t使用次数\t开卡时间\n");
	IpCardNode cur=NULL;
	if(CardList!=NULL){
		cur=CardList->next;
		while(cur!=NULL){
			printf("%s\t%s\t%d\t%.1f\t%d\t%s\n",cur->data.Name,cur->data.Pwd,cur->data.nStatus,cur->data.aBalance,cur->data.nUseCount,timeToString(cur->data.tStart));
			cur=cur->next;
		}
			
		}
}