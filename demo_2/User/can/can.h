#ifndef __CAN_H
#define __CAN_H
#include "init.h"
////////////////////////////////////////////////////////////////////////////////////	
extern CanRxMsg RxMessage;
//��������
void can_Init(void);//��ʼ��	
u8 CAN1_Receive_Msg(u8 *buf);
#endif
