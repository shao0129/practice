#include "init.h"
/*********************************************************************************
*********************�������� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: caan.c                                                                *
* �ļ�������can��ʼ��                                                              *
* �������ڣ�2019.03.14                                                            *
* ��    ����V1.0                                                                  *
* ��    �ߣ�Clever                                                                *
* ˵    ����LED��ӦIO�ڳ�ʼ��                                                      * 
*                                  *
**********************************************************************************
*********************************************************************************/
 CanRxMsg RxMessage;
//LED��ӦIO��ʼ��
void can_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
//    NVIC_InitTypeDef NVIC_InitStructure;
    CAN_InitTypeDef  CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;


    /*����ʱ������*/
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
    
    	  //���Ÿ���ӳ������
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
        
//    CAN_DeInit(CAN1);
//    CAN_StructInit(&CAN_InitStructure);
    CAN_Init(CAN1,&CAN_InitStructure);
    // CAN cell init 
    CAN_InitStructure.CAN_TTCM=DISABLE;//��ֹʱ�䴥��ͨ��ģʽ
    CAN_InitStructure.CAN_ABOM=DISABLE;
    CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ�����sleepλ������
    CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;�����Զ��ش�
    CAN_InitStructure.CAN_RFLM=DISABLE;//�������ʱ��FIFOδ����
    CAN_InitStructure.CAN_TXFP=DISABLE;//���͵����ȼ��ɱ�ʾ���Ĵ�С����
    CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//����ģʽ��
    
        //����canͨѶ������Ϊ500kbps
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_7tq; //ʱ���1��ʱ�䵥Ԫ.  Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq; //ʱ���2��ʱ�䵥Ԫ.  Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=6;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
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
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;   //ʱ�ܹ�����
    CAN_FilterInit(&CAN_FilterInitStructure);
    
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		

}

/****************************************************************************
* ��    ��: u8 CAN1_Receive_Msg(u8 *buf)
* ��    �ܣ�can�ڽ������ݲ�ѯ
* ��ڲ�����buf:���ݻ�����;	 			     
* ���ز�����0,�����ݱ��յ�;
    		    ����,���յ����ݳ���;
* ˵    ����       
****************************************************************************/	
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;

    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}




