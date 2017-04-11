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
		case 1:add_card();break;  //添加卡
		case 2:search_card();break;  //查询卡
		case 3:login();break;   //上机
		case 4:logout();break;  //下机
		case 5:top_up();break;   //充值
		case 6:refund();break;    //退费
		case 7:del_card();break;   //注销卡删除卡
		case 0:releaseCardList();break;   //
		case 8:showAllcard();break;     //列出所有卡
		case 9:showAllBilling();break;   //列出所有收费有关
		default:printf("输入错误，请重新选择:");
		} 
	}
	return 0;
}