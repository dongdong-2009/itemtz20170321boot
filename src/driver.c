#define DRIVER_GLOBAL

#include "include.h"

void GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8 relay_lock_data[4];
/***	 
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOE);
	GPIO_DeInit(GPIOF);
***/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD  | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
	///�������
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;///GPS���õ�ؿ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;///GPS���õ�ؿ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;///�ն�״ָ̬ʾLED�����ƽ�,�ͣ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;///GSM,LED�ƿ��ƽ�,�ߣ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;///GPS,LED�ƿ��ƽ�,�ͣ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; ///CT_LOCK,�̵�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; ///CT_GPS, GPS��Դ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;///CT_5V,��س�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;///WDIι��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;///CT_GSM��GSM�ػ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; ///CT_GSMMD,4V��Դ������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	OFF_GPRS_PWR();
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;///DIR_485��485ͨ�ŷ��򣬿�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;///CT_485��485��Դ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	///��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;///PWR��Դ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;///ACC�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;///RM_SHELL��ǲ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;///GSM,RING�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;///GPS,���߼������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	

	FlashRead(RELAY_LOCK_PARA_START_ADDR,relay_lock_data,4);
	if((relay_lock_data[0] == VALID_VAL_AA)&&(relay_lock_data[1] == VALID_VAL_AA)&&
	   (relay_lock_data[2] == VALID_VAL_AA)&&(relay_lock_data[3] == VALID_VAL_AA))
	{
		RELAY_LOCK();///���̵�������
	}
	else
	{
		RELAY_UNLOCK();///�ؼ̵�������
	}

	UsartGprsDeInit();
	
	HIGH_GPRS_IGT();
	OFF_WORK_LED();
	OFF_GSM_LED();
	OFF_GPS_LED();
}
void SysTickInit(void)
{
	RCC_ClocksTypeDef RCC_ClocksStatus;
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	SysTick_Config(RCC_ClocksStatus.SYSCLK_Frequency/1000);
}
void SysTickDisable(void)
{
	SysTick->CTRL  &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}
void SysTickEnable(void)
{
	SysTick->CTRL  |= (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}

void FeedWtd(void)
{
	IWDG_ReloadCounter();
	FEED_WDT();	
}
void IwdgInit(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(2000);///1.6S
  IWDG_ReloadCounter();
  IWDG_Enable();
}
void PvdInit(void)///��ѹ�ж�
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	///ռ�����ȼ��������ȼ��ɴ�ϵ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_PVDLevelConfig(PWR_PVDLevel_2V9); 	/// �趨��ط�ֵ
	PWR_PVDCmd(ENABLE); /// ʹ��PVD 
	EXTI_StructInit(&EXTI_InitStructure); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line16; /// PVD���ӵ��ж���16��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; ///ʹ���ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;///��ѹ���ڷ�ֵʱ�����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; /// ʹ���ж���
	EXTI_Init(&EXTI_InitStructure); /// ��ʼ���жϿ�����
}
void UsartGprsDeInit(void)///Gprs�������ŷ���ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void UsartInit(USART_TypeDef* USARTx, u32 uart_bpr,uint8 data_bits,uint8 stop_bits,uint8 parity_check)///�����ʣ�����λ��,ֹͣλ����żУ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	switch (*(uint32_t*)&USARTx)
	{
		case LOCAL_USART_BASE:///LCD,PA.9,TX;PA.10,RX
		{
			g_local_uart_struct.rx_buf = local_uart_buf;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			break;
		}
		default:
		{
			goto RETURN_LAB;
		}
	}

	USART_InitStructure.USART_BaudRate = uart_bpr;
	USART_InitStructure.USART_WordLength = data_bits;
	USART_InitStructure.USART_StopBits = stop_bits;
	USART_InitStructure.USART_Parity = parity_check;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

	USART_Cmd(USARTx, ENABLE);
RETURN_LAB:
	return;
}

void LocalUartFxedLenSend(uint8 data[],uint16 len) 
{
	uint16 i;
	
	for(i=0;i<len;i++)
	{
		USART_SendData(LOCAL_USART,data[i]);
		while(USART_GetFlagStatus(LOCAL_USART,USART_FLAG_TXE) == RESET);
	}
}
void LocalUartParaInit(void)
{
	g_local_uart_struct.rx_counter = 0;
	g_local_uart_struct.rx_delay_time_counter = 0;
	g_local_uart_struct.rx_flag = INVALID_VAL_55;
}
void LocalUartIsRxDone(uint16 past_ms_val)
{
	if(g_local_uart_struct.rx_delay_time_counter > 0)
	{
		if(g_local_uart_struct.rx_delay_time_counter >= past_ms_val)
		{
			g_local_uart_struct.rx_delay_time_counter -= past_ms_val;
		}
		else
		{
			g_local_uart_struct.rx_delay_time_counter = 0;
		}
		
		if((g_local_uart_struct.rx_delay_time_counter == 0)&&(g_local_uart_struct.rx_counter > 0))
		{
			g_local_uart_struct.rx_flag = VALID_VAL_AA;
		}
	}
}
uint16 GetLocalUartRxData(uint8 rx_data[])
{
	uint16 rx_counter = 0;
	
	if(g_local_uart_struct.rx_flag == VALID_VAL_AA)
	{
		rx_counter = g_local_uart_struct.rx_counter;
		MemCpy(rx_data,g_local_uart_struct.rx_buf,rx_counter);
		LocalUartParaInit();
	}
	return rx_counter;
}
void DriverMain(void)
{
	static uint16 sys_tick_pre_val=0;
	static uint16 sys_tick_cur_val=0;
	uint16 past_ms_val;
	
	sys_tick_cur_val = boot_struct.sys_tick_ms_counter;///��ȡϵͳms������
	
	past_ms_val = (sys_tick_cur_val + (65535 - sys_tick_pre_val)) % 65535;
	LocalUartIsRxDone(past_ms_val);
	sys_tick_pre_val = sys_tick_cur_val;
}
