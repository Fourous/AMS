#include<ctime>
#ifndef  Model_H
#define  Model_H
	typedef struct Card
	{
		char    Name[18];	//����
		char    Pwd[8];		//����
		int     nStatus;	//��״̬��0-δ�ϻ���1-�����ϻ���2-��ע����3-ʧЧ��
		time_t  tStart;		//����ʱ��
		time_t  tEnd;		//���Ľ�ֹʱ��
		float   fTotalUse;	//�ۼƽ��
		time_t  tLastTime;	//���ʹ��ʱ��
		int     nUseCount;	//ʹ�ô���
		float   aBalance;	//���
		int     nDel;		//ɾ����ʶ��0-δɾ����1-ɾ����
	} Card;
	typedef struct CardNode{
		Card data;
		struct CardNode*next;
	}CardNode,*IpCardNode;
	typedef struct Billing {
		char CardName[18];		//����
		time_t tstart;			//�ϻ�ʱ��
		time_t tend;			//�»�ʱ��
		float  fAmount;			//���ѽ��
		int nStatus;			//����״̬��0-δ���㣬1-�ѽ���
		int  nDel;				//ɾ����ʶ��0-δɾ����1-ɾ����
	}Billing;
#endif 