#include "stm32f4xx.h"
#include "init.h"
void Running(void);

int main(void)
{
	
	SysTick_Init();
	LCD_Init();
	LED_Init();
	BRUSH_COLOR=RED;
	while(1)
	{
		Running();
		//LCD_DisplayChar(0,1,0,24);
		LCD_DisplayNum(0,0,LED0,8,24,1);
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

