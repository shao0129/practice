#include "init.h"
/*********************************************************************************
*********************启明欣欣 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: caan.c                                                                *
* 文件简述：can初始化                                                              *
* 创建日期：2019.03.14                                                            *
* 版    本：V1.0                                                                  *
* 作    者：Clever                                                                *
* 说    明：LED对应IO口初始化                                                      * 
*                                  *
**********************************************************************************
*********************************************************************************/
 CanRxMsg RxMessage;
//LED对应IO初始化
void can_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
//    NVIC_InitTypeDef NVIC_InitStructure;
    CAN_InitTypeDef  CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;


    /*外设时钟设置*/
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    	  //引脚复用映射配置
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1
        
//    CAN_DeInit(CAN1);
//    CAN_StructInit(&CAN_InitStructure);
    CAN_Init(CAN1,&CAN_InitStructure);
    // CAN cell init 
    CAN_InitStructure.CAN_TTCM=DISABLE;//禁止时间触发通信模式
    CAN_InitStructure.CAN_ABOM=DISABLE;
    CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过清除sleep位来唤醒
    CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;报文自动重传
    CAN_InitStructure.CAN_RFLM=DISABLE;//接收溢出时，FIFO未锁定
    CAN_InitStructure.CAN_TXFP=DISABLE;//发送的优先级由标示符的大小决定
    CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//正常模式下
    
        //设置can通讯波特率为500kbps
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_7tq; //时间段1的时间单元.  Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq; //时间段2的时间单元.  Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=6;  //分频系数(Fdiv)为brp+1	
//    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
//    CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;
//    CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
//    CAN_InitStructure.CAN_Prescaler=9;

    CAN_Init(CAN1,&CAN_InitStructure);

    // CAN filter init 
    CAN_FilterInitStructure.CAN_FilterNumber=0;
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//CAN_FilterScale_16bit; //32bit

    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;   //时能过滤器
    CAN_FilterInit(&CAN_FilterInitStructure);
    
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		

}

/****************************************************************************
* 名    称: u8 CAN1_Receive_Msg(u8 *buf)
* 功    能：can口接收数据查询
* 入口参数：buf:数据缓存区;	 			     
* 返回参数：0,无数据被收到;
    		    其他,接收的数据长度;
* 说    明：       
****************************************************************************/	
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;

    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}




