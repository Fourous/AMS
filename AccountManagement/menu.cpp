#include<cstdio>
#include<cstring>
#include"model.h"
#include"card_service.h"
#include"card_file.h"
#include"billing.h"
		//主菜单
void main_menu(){
	printf("\n*****欢迎进入计费管理系统*****\n\n");
	printf("--------菜单--------\n1:添加卡\n2:查询卡\n3:上机\n4:下机\n5:充值\n6:退费\n7:注销卡\n8:显示所有卡信息\n9:显示所有消费记录\n0:退出\n");
	printf("请选择菜单编号（0~8):\n---------------------\n");
}
		//添加卡
void add_card(){
	printf("请输入要添加的卡号<长度1~18>:");
	char Id[18];
	scanf("%s",&Id);
	if(strlen(Id)<=18){
			printf("请输入密码<长度1~8>:");
			char pwd[8];
			scanf("%s",&pwd);
			if(strlen(pwd)<=8){
				printf("请确认密码<长度1~8>:");
				char affirm_pwd[8];
				scanf("%s",&affirm_pwd);
				if(strcmp(affirm_pwd,pwd)==0){
					printf("请输入开卡金额:");
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
					readfilecard();			//加载文件中卡信息
					if(queryCardFromList(Id)==NULL){
						if(addCard(card)==true){
							printf("添加成功\n--------添加的信息如下--------\n");
							printf("卡号\t密码\t状态\t开卡金额\n");
							printf("%s\t%s\t%d\t%.2f",card.Name,card.Pwd,card.nStatus,card.aBalance);
						}else
							printf("添加失败");
					}else printf("该卡已存在");
					
				}
				else {
					printf("两次密码不一致，请重新添加\n");
					add_card();
				}
			}
			else{
				printf("输入的密码过长，请重新添加\n");
		        add_card();
			}																																																														2
	;}else{
		printf("输入的卡号过长，请重新添加\n");
		add_card();
	}
}

		//查询卡
void  search_card(){
	if(search_Card()==false){
	printf("未查询到该卡");
	};
}

		//上机
void login(){
	Login();
}

		//下机
void logout(){
	Logout();
}

		//充值
void top_up(){
	Top_up();
}

		//退费
void refund(){
	Refund();
}

		//注销卡
void del_card(){
	Del_card();
}
