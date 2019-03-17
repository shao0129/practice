#ifndef __CAN_H
#define __CAN_H
#include "init.h"
////////////////////////////////////////////////////////////////////////////////////	
extern CanRxMsg RxMessage;
//函数声明
void can_Init(void);//初始化	
u8 CAN1_Receive_Msg(u8 *buf);
#endif
