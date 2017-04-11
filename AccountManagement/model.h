#include<ctime>
#ifndef  Model_H
#define  Model_H
	typedef struct Card
	{
		char    Name[18];	//卡号
		char    Pwd[8];		//密码
		int     nStatus;	//卡状态（0-未上机；1-正在上机；2-已注销；3-失效）
		time_t  tStart;		//开卡时间
		time_t  tEnd;		//卡的截止时间
		float   fTotalUse;	//累计金额
		time_t  tLastTime;	//最后使用时间
		int     nUseCount;	//使用次数
		float   aBalance;	//余额
		int     nDel;		//删除标识（0-未删除；1-删除）
	} Card;
	typedef struct CardNode{
		Card data;
		struct CardNode*next;
	}CardNode,*IpCardNode;
	typedef struct Billing {
		char CardName[18];		//卡号
		time_t tstart;			//上机时间
		time_t tend;			//下机时间
		float  fAmount;			//消费金额
		int nStatus;			//消费状态，0-未结算，1-已结算
		int  nDel;				//删除标识（0-未删除；1-删除）
	}Billing;
#endif 