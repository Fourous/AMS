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
	tm1.tm_year   -=  1900; /*���ֵ��ȥ1900���õ�tm�ṹ�б�����������*/
    tm1.tm_mon    -=  1;    /*�·�ֵ��ȥ1���õ�tm�ṹ�б�����·�����*/
	time1 = mktime(&tm1);
	return time1;

}
bool saveCardToFile(const Card card,const char*FilePath){
	FILE *fp = fopen(FilePath, "ab+");
	if(NULL==fp)
		{
			printf("����Ϣ�ļ���ȡ����");
			return false;//Ҫ���ش������
		  }
	else{
	 fwrite(&card,sizeof(Card),1,fp);
     fclose(fp);
	 fp=NULL;//��Ҫָ��գ������ָ��ԭ���ļ���ַ
	 return true;
	}
}
int addcardToList(Card card){
	nCount++;//����������
	IpCardNode cur=NULL;
	if(CardList==NULL){
		initCardList();
	}
	//���ݱ��浽�ڵ�
	cur=(IpCardNode)malloc(sizeof(CardNode));
	if(cur!=NULL){
			cur->data=card;
			cur->next=NULL;
		//���������ҵ����ڵ�
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
				Billing billing;				//�޸����Ѽ�¼�ṹ����Ϣ
				billing.nDel = 0;
				billing.fAmount = 0;
				billing.nStatus = 1;
				billing.tstart = curr->data.tLastTime;
				billing.tend=time(0);			//Ĭ��Ϊ��ǰʱ��
				strcpy(billing.CardName, curr->data.Name);
				if(saveBillingTofile(billing,".\\data\\billing.ams")==true){
					//���һ���ϻ���¼
					if(updateCardToFile(&curr->data, ".\\data\\card.ams",Index )==true)
						return &curr->data;			//�����ϻ�����Ϣ
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
	if (min%15==0) {			//ʮ�����Ϊ��λ
		count = min / 15;        //ǡ��ȡ��
	}
	else {
		count = min/15 + 1;    //����ʮ����ӽ�һ
	} 
	if(count==0){
		count = 1;				//��¼���ټƷ�һ��λ
	}
	amount = count*0.5;        //ÿʮ�����0.5Ԫ
	return amount;
}
bool dologout(const char* cardName) {
	IpCardNode curr = NULL;
	int Index = 0;
	if (CardList != NULL) {
		curr = CardList->next;
		while (curr != NULL) {
			if (strcmp(curr->data.Name, cardName) == 0) {
				curr->data.aBalance= curr->data.aBalance-amount(curr->data.tLastTime);	//�޸����
				curr->data.nStatus = 0;								//״̬�»�
					Billing billing;				//�޸����Ѽ�¼�ṹ����Ϣ
					billing.nDel = 0;
					billing.fAmount = amount(curr->data.tLastTime);
					billing.nStatus =0;
					billing.tstart = curr->data.tLastTime;
					billing.tend=time(0);			
					strcpy(billing.CardName, curr->data.Name);
					if (updateBillingTofile( billing, ".\\data\\billing.ams") == true) {     //�������Ѽ�¼
						if (updateCardToFile(&curr->data, ".\\data\\card.ams",Index) == true)	// �����ļ���Ϣ
						{
						printf("--------�»�����Ϣ���£�--------\n");
						printf("����\t����\t���\t�ϻ�ʱ��\t\t�»�ʱ��\n");
						printf("%s\t%.1f\t%.1f\t%s\t%s\n", curr->data.Name, amount(curr->data.tLastTime), curr->data.aBalance, timeToString(curr->data.tLastTime), timeToString(time(0)));
						return true;								//�����»�����Ϣ
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
			//��ȡ�ļ���ʶλ��
		}
	}			
	fseek(fp,lposition,0);
	fwrite(card,sizeof(Card),1,fp);
	fclose(fp);
	fp = NULL;//��Ҫָ��գ������ָ��ԭ���ļ���ַ
	return true;
}
bool do_top_up(const char* cardName, float up_money) {
	int Index = 0;
	IpCardNode curr = NULL;
	if (CardList != NULL) {
		curr = CardList->next;
		while (curr != NULL) {
			if (strcmp(curr->data.Name, cardName) == 0) {
				curr->data.aBalance = curr->data.aBalance + up_money;//�޸����
				if (updateCardToFile(&curr->data, ".\\data\\card.ams",Index) == true)	//���³�ֵ����Ϣ
				{
					printf("��ֵ��Ϣ����:\n");
					printf("����\t��ֵ���\t���\n");
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
				curr->data.aBalance = curr->data.aBalance - refund_money;//�޸����
				if (curr->data.aBalance >= 0) {
					if (updateCardToFile(&curr->data, ".\\data\\card.ams",Index) == true)	//�����˷ѿ���Ϣ
					{
						printf("�˷���Ϣ����:\n");
						printf("����\t�˷ѽ��\t���\n");
						printf("%s\t%0.1f\t\t%.1f\n", curr->data.Name, refund_money,curr->data.aBalance);
						return true;
					}
				}
				else {
					printf("�˷ѽ������");
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
					if (updateCardToFile(&curr->data, ".\\data\\card.ams",Index) == true)	//����ɾ������Ϣ
					{
						printf("ע��Ϣ����:\n");
						printf("����\t�˷ѽ��\n");
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
	printf("\n����\t����\t״̬\t���\tʹ�ô���\t����ʱ��\n");
	IpCardNode cur=NULL;
	if(CardList!=NULL){
		cur=CardList->next;
		while(cur!=NULL){
			printf("%s\t%s\t%d\t%.1f\t%d\t%s\n",cur->data.Name,cur->data.Pwd,cur->data.nStatus,cur->data.aBalance,cur->data.nUseCount,timeToString(cur->data.tStart));
			cur=cur->next;
		}
			
		}
}