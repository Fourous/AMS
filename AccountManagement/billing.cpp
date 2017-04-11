#include"billing.h"
#include"card_file.h"
#include <fstream>
bool saveBillingTofile(const Billing billing, const char* path){
	FILE *fp = fopen(path, "ab+");
	if (NULL == fp)
	{
		printf("文件读取错误");
		return false;//要返回错误代码
	}
	else {
		fwrite(&billing,sizeof(Billing),1,fp);//添加上机记录
		fclose(fp);
		fp = NULL;//需要指向空，否则会指向原打开文件地址
		return true;
	}
}
bool updateBillingTofile(const Billing billing, const char* path) {
	FILE *fp = fopen(path, "rb+");
	long lposition = 0;
	if (NULL == fp)
	{
		printf("文件读取错误");
		return false;//要返回错误代码
	}
	else {
		Billing  temp;
		while ((!feof(fp))) {
			if (fread(&temp, sizeof(Billing), 1, fp)!= NULL) {
				//遍历所有Billing
				if (strcmp(billing.CardName,temp.CardName)==0&&temp.nStatus == 1) {
					//通过卡号与状态查到该上机卡，
					fseek(fp, lposition, 0);
					fwrite(&billing, sizeof(Billing), 1, fp);//更新消费记录
					fclose(fp);
					fp = NULL;//需要指向空，否则会指向原打开文件地址
					return true;
				};
				lposition = ftell(fp);
			}
		}
		fclose(fp);
		fp = NULL;//需要指向空，否则会指向原打开文件地址
		return false;
	}
}
void showAllBilling(){
	FILE *fp = fopen(".\\data\\billing.ams", "rb+");
	if (NULL == fp){
		printf("文件读取错误");
	}
	else {
		Billing  temp;
		printf("\n卡号\t状态\t消费金额\t删除标识\t上机时间\t下机时间\n");
		while ((!feof(fp))) {
			if (fread(&temp, sizeof(Billing), 1, fp)!= NULL) {
				char stt[20];
				strcpy(stt,timeToString(temp.tend));
				printf("%s\t%d\t%.1f\t\t%d\t\t%s\t%s\t\n",temp.CardName,temp.nStatus,temp.fAmount,temp.nDel,timeToString(temp.tstart),&stt);
			}
		}
		fclose(fp);
		fp = NULL;//需要指向空，否则会指向原打开文件地址
}
}