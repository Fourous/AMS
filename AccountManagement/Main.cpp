#include<cstdio>
#include"menu.h"
#include"card_file.h"
#include"billing.h"
int main(){
	int select=-1;
	while(select!=0){
	main_menu();
	scanf("%d",&select);
	switch(select){
		case 1:add_card();break;  //��ӿ�
		case 2:search_card();break;  //��ѯ��
		case 3:login();break;   //�ϻ�
		case 4:logout();break;  //�»�
		case 5:top_up();break;   //��ֵ
		case 6:refund();break;    //�˷�
		case 7:del_card();break;   //ע����ɾ����
		case 0:releaseCardList();break;   //
		case 8:showAllcard();break;     //�г����п�
		case 9:showAllBilling();break;   //�г������շ��й�
		default:printf("�������������ѡ��:");
		} 
	}
	return 0;
}