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
	readfilecard();			//�����ļ��п���Ϣ
	printf("������Ҫ��ѯ�Ŀ���:");
	char Id[18];
	scanf("%s",&Id);
	if(queryCardFromList(Id)!=NULL){
		Card card;
		memcpy(&card,queryCardFromList(Id),sizeof(Card));
		printf("��ѯ�Ŀ���Ϣ��\n");
		printf("����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");
		printf("%s\t%d\t%.2f\t%.2f\t\t%d\t%s",card.Name,card.nStatus,card.aBalance,card.fTotalUse,card.nUseCount,timeToString(card.tLastTime));
		return true;
	}
	return false;

}
void Login() {
	printf("--------�ϻ�--------\n");
	readfilecard();			//�����ļ��п���Ϣ
	printf("�������ϻ����ſ���<����1~18>:");
	char Id[18];
	scanf("%s", &Id);
	printf("�������ϻ�����<����1~8>:");
	char pwd[8];
	scanf("%s", &pwd);
	if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
		if (queryCardFromList(Id) != NULL) {			//��ѯ���ǿ�
			Card card;
			memcpy(&card, queryCardFromList(Id), sizeof(Card));   
			if (strcmp(card.Pwd,pwd)==0) {				//����ƥ��
				if (card.aBalance>0) {					//��������
					if (card.nStatus == 0) {			//״̬Ϊδ�ϻ�
							memcpy(&card, dologin(Id), sizeof(Card));							//�ϻ������¿���Ϣ���ļ���Ϣ
							printf("--------�ϻ�����Ϣ���£�--------\n");
							printf("����\t���\t�ϻ�ʱ��\n");
							printf("%s\t%.2f\t%s", card.Name, card.aBalance, timeToString(card.tLastTime));
					}
					else printf("�ÿ�����ʹ�û��Ѿ�ע��");
				}
				else printf("������Ѿ�ע��");
			}
			else printf("�������");
		}
		else printf("�����ڸÿ����ϻ�ʧ��");
	}
	else printf("���Ż����볤������");
}

void Logout(){
printf("--------�»�--------\n");
readfilecard();			//�����ļ��п���Ϣ
printf("�������»����ſ���<����1~18>:");
char Id[18];
scanf("%s", &Id);
printf("�������»�����<����1~8>:");
char pwd[8];
scanf("%s", &pwd);
if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
	if (queryCardFromList(Id) != NULL) {				//��ѯ���ǿ�
		Card card;
		memcpy(&card, queryCardFromList(Id), sizeof(Card));
		if (strcmp(card.Pwd, pwd) == 0) {				//����ƥ��
			if (card.nStatus == 1) {					//��״̬Ϊ�����ϻ�
				double temp = card.aBalance - amount(card.tLastTime);
				if (temp>=0) {							//��С����
					if (dologout(Id) == true) {
						//�»������¿���Ϣ���ļ���Ϣ  
						printf("�»��ɹ�");
					}
					else {printf("�»�ʧ�ܻ����Ѽ�¼����ʧ��");}
				}
				else printf("�ÿ�����");
			}
			else printf("�ÿ�δ�ϻ����Ѿ�ע��");
		}
		else printf("�������");
	}
	else printf("�����ڸÿ����»�ʧ��");
}
else printf("���Ż����볤������");
}

void Top_up() {
	printf("--------��ֵ--------\n");
	readfilecard();			//�����ļ��п���Ϣ
	printf("�������ֵ����<����1~18>:");
	char Id[18];
	scanf("%s", &Id);
	printf("�������ֵ������<����1~8>:");
	char pwd[8];
	scanf("%s", &pwd);
	if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
		if (queryCardFromList(Id) != NULL) {				//��ѯ���ǿ�
			Card card;
			memcpy(&card, queryCardFromList(Id), sizeof(Card));
			if (strcmp(card.Pwd, pwd) == 0) {				//����ƥ��
				if (card.nStatus == 1|| card.nStatus == 0) {					//��״̬Ϊ����ʹ�û�δʹ��
						printf("�������ֵ���:\n");
						float up_banlance;
						scanf("%f", &up_banlance);
						if (do_top_up(Id,up_banlance) == true) {
							//��ֵ�ɹ�
							printf("��ֵ�ɹ�");
						}
						else printf("��ֵʧ��");
					}
				else printf("�ÿ�ʧЧ���Ѿ�ע��");
			}
			else printf("�������");
		}
		else printf("�����ڸÿ����»�ʧ��");
	}
	else printf("���Ż����볤������");
}
void Refund() {
	printf("--------�˷�--------\n");
	readfilecard();			//�����ļ��п���Ϣ
	printf("�����뿨��<����1~18>:");
	char Id[18];
	scanf("%s", &Id);
	printf("�����뿨����<����1~8>:");
	char pwd[8];
	scanf("%s", &pwd);
	if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
		if (queryCardFromList(Id) != NULL) {				//��ѯ���ǿ�
			Card card;
			memcpy(&card, queryCardFromList(Id), sizeof(Card));
			if (strcmp(card.Pwd, pwd) == 0) {				//����ƥ��
				if (card.nStatus ==0) {						//��״̬Ϊδ�ϻ�
					if (card.aBalance >= 0) {				//��С����
						printf("�������˷ѽ��:");
						float refund_money;
						scanf("%f", &refund_money);
						if (do_refund(Id,refund_money) == true) {
							printf("�˷ѳɹ�");
						}
						else printf("�˷�ʧ��");
					}
					else printf("�ÿ�����");
				}
				else printf("�ÿ�ʧЧ�������ϻ����Ѿ�ע��");
			}
			else printf("�������");
		}
		else printf("�����ڸÿ����»�ʧ��");
	}
	else printf("���Ż����볤������");
}
void readfilecard(){
	releaseCardList();
	if(readCardFromFile(".\\data\\card.ams")!=1){
	printf("��ȡʧ��-");
	}
	else printf("���ؽ���-");
}
void Del_card() {
	printf("--------ע����--------\n");
	readfilecard();			//�����ļ��п���Ϣ
	printf("�����뿨��<����1~18>:");
	char Id[18];
	scanf("%s", &Id);
	printf("����������<����1~8>:");
	char pwd[8];
	scanf("%s", &pwd);
	if (strlen(pwd) <= 8 && strlen(Id) <= 18) {
		if (queryCardFromList(Id) != NULL) {			//��ѯ���ǿ�
			Card card;
			memcpy(&card, queryCardFromList(Id), sizeof(Card));
			if (strcmp(card.Pwd, pwd) == 0) {				//����ƥ��
				if (card.nStatus == 0) {			//״̬Ϊδ�ϻ�
					if (do_del_card(Id) == true) {
						printf("�ɹ�ע���ÿ�");
					}
					else printf("ע��ʧ��");
				}
				else printf("�ÿ�����ʹ�û��Ѿ�ע����ʧЧ");
			}
			else printf("�������");
		}
		else printf("�����ڸÿ����ϻ�ʧ��");
	}
	else printf("���Ż����볤������");
}

