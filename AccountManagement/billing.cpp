#include"billing.h"
#include"card_file.h"
#include <fstream>
bool saveBillingTofile(const Billing billing, const char* path){
	FILE *fp = fopen(path, "ab+");
	if (NULL == fp)
	{
		printf("�ļ���ȡ����");
		return false;//Ҫ���ش������
	}
	else {
		fwrite(&billing,sizeof(Billing),1,fp);//����ϻ���¼
		fclose(fp);
		fp = NULL;//��Ҫָ��գ������ָ��ԭ���ļ���ַ
		return true;
	}
}
bool updateBillingTofile(const Billing billing, const char* path) {
	FILE *fp = fopen(path, "rb+");
	long lposition = 0;
	if (NULL == fp)
	{
		printf("�ļ���ȡ����");
		return false;//Ҫ���ش������
	}
	else {
		Billing  temp;
		while ((!feof(fp))) {
			if (fread(&temp, sizeof(Billing), 1, fp)!= NULL) {
				//��������Billing
				if (strcmp(billing.CardName,temp.CardName)==0&&temp.nStatus == 1) {
					//ͨ��������״̬�鵽���ϻ�����
					fseek(fp, lposition, 0);
					fwrite(&billing, sizeof(Billing), 1, fp);//�������Ѽ�¼
					fclose(fp);
					fp = NULL;//��Ҫָ��գ������ָ��ԭ���ļ���ַ
					return true;
				};
				lposition = ftell(fp);
			}
		}
		fclose(fp);
		fp = NULL;//��Ҫָ��գ������ָ��ԭ���ļ���ַ
		return false;
	}
}
void showAllBilling(){
	FILE *fp = fopen(".\\data\\billing.ams", "rb+");
	if (NULL == fp){
		printf("�ļ���ȡ����");
	}
	else {
		Billing  temp;
		printf("\n����\t״̬\t���ѽ��\tɾ����ʶ\t�ϻ�ʱ��\t�»�ʱ��\n");
		while ((!feof(fp))) {
			if (fread(&temp, sizeof(Billing), 1, fp)!= NULL) {
				char stt[20];
				strcpy(stt,timeToString(temp.tend));
				printf("%s\t%d\t%.1f\t\t%d\t\t%s\t%s\t\n",temp.CardName,temp.nStatus,temp.fAmount,temp.nDel,timeToString(temp.tstart),&stt);
			}
		}
		fclose(fp);
		fp = NULL;//��Ҫָ��գ������ָ��ԭ���ļ���ַ
}
}