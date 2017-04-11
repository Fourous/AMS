#include"model.h"
#ifndef Card_file_H
#define Card_file_H
	bool saveCardToFile(const Card card,const char*FilePath);
	int readCardFromFile(const char*pPath);
	Card praseCard(char *pBuf);
	int getCardCount();
	char* timeToString(time_t time);
	time_t stringTotime(char* pTime);
	int addcardToList(Card card);
	void releaseCardList();
	int initCardList();	
	Card* queryCardFromList(const char* pName);
	Card* dologin(const char* pName);
	bool dologout(const char* cardName);
	bool do_top_up(const char* cardName, float up_balance);
	bool do_refund(const char* cardName, float refund_money);
	bool do_del_card(const char* cardName);
	bool updateCardToFile(const Card* card,const char* path ,int nIndex);
	double amount(time_t t);
	void showAllcard();
#endif 