#include "stm32f4xx.h"
#include "init.h"
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
		LCD_DisplayNum(0,48,RxMessage.Data[0],2,24,1);
		LCD_DisplayNum(36,48,RxMessage.Data[1],2,24,1);
		
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

