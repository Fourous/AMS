#include<cstdio>
#include<cstring>
#include"model.h"
#include"card_service.h"
#include"card_file.h"
#include"billing.h"
		//���˵�
void main_menu(){
	printf("\n*****��ӭ����Ʒѹ���ϵͳ*****\n\n");
	printf("--------�˵�--------\n1:��ӿ�\n2:��ѯ��\n3:�ϻ�\n4:�»�\n5:��ֵ\n6:�˷�\n7:ע����\n8:��ʾ���п���Ϣ\n9:��ʾ�������Ѽ�¼\n0:�˳�\n");
	printf("��ѡ��˵���ţ�0~8):\n---------------------\n");
}
		//��ӿ�
void add_card(){
	printf("������Ҫ��ӵĿ���<����1~18>:");
	char Id[18];
	scanf("%s",&Id);
	if(strlen(Id)<=18){
			printf("����������<����1~8>:");
			char pwd[8];
			scanf("%s",&pwd);
			if(strlen(pwd)<=8){
				printf("��ȷ������<����1~8>:");
				char affirm_pwd[8];
				scanf("%s",&affirm_pwd);
				if(strcmp(affirm_pwd,pwd)==0){
					printf("�����뿪�����:");
					float balan;
					scanf("%f",&balan);
					time_t st = time(0);
					Card card;
					strcpy(card.Name,Id);
					strcpy(card.Pwd,pwd);
					card.nStatus=0;
					card.tStart=st;
					card.tEnd=st;
					card.fTotalUse=0;
					card.tLastTime=st;
					card.nUseCount=0;
					card.aBalance=balan;
					card.nDel=0;
					readfilecard();			//�����ļ��п���Ϣ
					if(queryCardFromList(Id)==NULL){
						if(addCard(card)==true){
							printf("��ӳɹ�\n--------��ӵ���Ϣ����--------\n");
							printf("����\t����\t״̬\t�������\n");
							printf("%s\t%s\t%d\t%.2f",card.Name,card.Pwd,card.nStatus,card.aBalance);
						}else
							printf("���ʧ��");
					}else printf("�ÿ��Ѵ���");
					
				}
				else {
					printf("�������벻һ�£����������\n");
					add_card();
				}
			}
			else{
				printf("�����������������������\n");
		        add_card();
			}																																																														2
	;}else{
		printf("����Ŀ��Ź��������������\n");
		add_card();
	}
}

		//��ѯ��
void  search_card(){
	if(search_Card()==false){
	printf("δ��ѯ���ÿ�");
	};
}

		//�ϻ�
void login(){
	Login();
}

		//�»�
void logout(){
	Logout();
}

		//��ֵ
void top_up(){
	Top_up();
}

		//�˷�
void refund(){
	Refund();
}

		//ע����
void del_card(){
	Del_card();
}
