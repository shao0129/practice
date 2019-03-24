#include "stm32f4xx.h"
#include "init.h"
#include "stdio.h"
void Running(void);

void NVIC_Config(void)
{   
		NVIC_InitTypeDef NVIC_InitStructure;
			/* Configure one bit for preemption priority */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
			/*÷–∂œ…Ë÷√*/ 													
	
		NVIC_InitStructure.NVIC_IRQChannel =CAN1_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	  
}

int main(void)
{
	u32 IDBuff[10]={0};
	u8 i=0;
	u8 IDExist=0;
	u8 DataBuff[9]={0};
	char DataBuffString[100]={};
	SysTick_Init();
	LCD_Init();
	LED_Init();
    can_Init();
    NVIC_Config();
	BRUSH_COLOR=RED;
	while(1)
	{
		Running();
        LCD_DisplayString(0,0,24,"ERROR");
		LCD_DisplayNum(0,24,LED0,8,24,1);
		IDExist=0;
		for(i=0;i<9;i++)
		{
			if((IDBuff[i]!=0 && IDBuff[i]==RxMessage.ExtId)||IDBuff[i]==RxMessage.StdId)
			{
				IDExist=1;
			}
			if(IDBuff[i]==0&&IDExist==0)
			{
				IDBuff[i]=RxMessage.ExtId|RxMessage.StdId;
				break;
			}			
		}
		//lcd
		DataBuff[1]=RxMessage.Data[0];
		DataBuff[2]=RxMessage.Data[1];
		DataBuff[3]=RxMessage.Data[2];
		DataBuff[4]=RxMessage.Data[3];
		DataBuff[5]=RxMessage.Data[4];
		DataBuff[6]=RxMessage.Data[5];
		DataBuff[7]=RxMessage.Data[6];
		DataBuff[8]=RxMessage.Data[7];
		//DataBuffString[0]=DataBuff[0];
		sprintf(DataBuffString,"%8x",RxMessage.ExtId|RxMessage.StdId);
		for(i=1;i<8;i++)
		{
			sprintf(DataBuffString+5+3*i,"%3x",DataBuff[i]);
		}
		for(i=0;i<100;i++)
		{
			if(DataBuffString[i]==0x00)
			{
				DataBuffString[i]=' ';
			}
		}
		
		for(i=0;i<9;i++)
		{
			if(IDBuff[i]!=0)
			{
//				LCD_DisplayCan(i+3,IDBuff[i],&RxMessage);
				LCD_DisplayString(0,108+i*36,24,DataBuffString);
			}
			else
				break;
		}
//		LCD_DisplayNum(0,48,RxMessage.Data[0],2,24,1);
//		LCD_DisplayNum(36,48,RxMessage.Data[1],2,24,1);
		
	}
}

void Running(void)
{
	if(Timer_1s)
	{
		LED0=~LED0;
		Timer_1s=0;
	}
}

