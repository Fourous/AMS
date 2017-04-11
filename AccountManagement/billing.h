#include"model.h"
#ifndef  Billing_H
#define	 Billing_H
bool saveBillingTofile(const Billing billing,const char* path);
bool updateBillingTofile(const Billing billing, const char* path);
void showAllBilling();
#endif 