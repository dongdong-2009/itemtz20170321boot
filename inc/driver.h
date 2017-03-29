#ifndef __DRIVER_H
#define __DRIVER_H

	#ifdef DRIVER_GLOBAL
		#define DRIVER_EXTERN
	#else
		#define DRIVER_EXTERN extern
	#endif
	
	#include "stm32f10x.h"
	
	#define LOW_STATE		0
	#define HIGH_STATE		1
	#define PWR_STATE()		(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6))///PWR电源状态，高为有外接电源
	#define ACC_STATE()		(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9))///ACC状态，低为开
	#define GPS_ANT_STATE()	(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0))///天线检测，高正常
	#define SHELL_STATE()	(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))///外壳拆除检测，高正常
	#define RING_STATE()	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8))///GSM来电，短信通知引脚
	
	#define FEED_WDT()       	(GPIOD->ODR ^= GPIO_Pin_8)
	
	#define ON_GPS_PWR()		(GPIO_SetBits(GPIOC, GPIO_Pin_6))
	#define OFF_GPS_PWR()		(GPIO_ResetBits(GPIOC, GPIO_Pin_6))

	#define ON_GPRS_PWR()		(GPIO_SetBits(GPIOC, GPIO_Pin_4))
	#define OFF_GPRS_PWR()		(GPIO_ResetBits(GPIOC, GPIO_Pin_4))
	#define LOW_GPRS_IGT()		(GPIO_SetBits(GPIOA, GPIO_Pin_8))
	#define HIGH_GPRS_IGT()		(GPIO_ResetBits(GPIOA, GPIO_Pin_8))

	#define ON_5V_PWR()			(GPIO_SetBits(GPIOB, GPIO_Pin_1))
	#define OFF_5V_PWR()		(GPIO_ResetBits(GPIOB, GPIO_Pin_1))

	#define ON_GPS_BAT_PWR()	(GPIO_SetBits(GPIOE, GPIO_Pin_2))	
	#define OFF_GPS_BAT_PWR()	(GPIO_ResetBits(GPIOE, GPIO_Pin_2))
	
	#define ON_WORK_LED()		(GPIO_ResetBits(GPIOE, GPIO_Pin_3))///灯
	#define OFF_WORK_LED()		(GPIO_SetBits(GPIOE, GPIO_Pin_3))
	#define CPL_WORK_LED()		(GPIOE->ODR ^= GPIO_Pin_3)
	
	#define ON_GSM_LED()		(GPIO_ResetBits(GPIOB, GPIO_Pin_12))
	#define OFF_GSM_LED()		(GPIO_SetBits(GPIOB, GPIO_Pin_12))
	#define CPL_GSM_LED()		(GPIOB->ODR ^= GPIO_Pin_12)
	
	#define ON_GPS_LED()		(GPIO_ResetBits(GPIOD, GPIO_Pin_13))
	#define OFF_GPS_LED()		(GPIO_SetBits(GPIOD, GPIO_Pin_13))
	#define CPL_GPS_LED()		(GPIOD->ODR ^= GPIO_Pin_13)
	
	#define ON_CAN_PWR()		(GPIO_SetBits(GPIOB, GPIO_Pin_5))///485
	#define OFF_CAN_PWR()		(GPIO_ResetBits(GPIOB, GPIO_Pin_5))
	#define ENABLE_RX485()		(GPIO_SetBits(GPIOD, GPIO_Pin_3))
	#define ENABLE_TX485()		(GPIO_ResetBits(GPIOD, GPIO_Pin_3))

	#define RELAY_LOCK()		(GPIO_ResetBits(GPIOC, GPIO_Pin_5))///继电器
	#define RELAY_UNLOCK()		(GPIO_SetBits(GPIOC, GPIO_Pin_5))
	
	#define OFF_METER_PWR()		(OFF_CAN_PWR())
	#define ON_METER_PWR()		(ON_CAN_PWR())
	void GpioInit(void);
	
	#define LOCAL_USART_BPR		9600///本地串口
	#define GPS_USART_BPR		4800
	#define GPRS_USART_BPR		38400

	#define METER_USART			((USART_TypeDef *) UART4_BASE)
	#define METER_USART_BASE	UART4_BASE
	
	#define GPRS_USART			((USART_TypeDef *) USART3_BASE)
	#define GPRS_USART_BASE		USART3_BASE
	
	#define GPS_USART			((USART_TypeDef *) USART2_BASE)
	#define GPS_USART_BASE		USART2_BASE
	
	#define LOCAL_USART			((USART_TypeDef *) USART1_BASE)
	#define LOCAL_USART_BASE	USART1_BASE
	
	#define USART_PARITY_NO      ((uint16_t)0x0000)
	#define USART_PARITY_EVEN    ((uint16_t)0x0400)
	#define USART_PARITY_ODD     ((uint16_t)0x0600) 
	
	#define USART_STOPBITS_1      ((uint16_t)0x0000)
	#define USART_STOPBITS_2      ((uint16_t)0x2000)

	#define USART_DATA_8B         ((uint16_t)0x0000)
	#define USART_DATA_9B         ((uint16_t)0x1000)

	

	typedef struct
	{
		uint8 rx_flag;
		uint16 rx_counter;
		uint16 rx_delay_time_counter;
		uint8 *rx_buf;
	}USART_STRUCT;
	
	void LocalDebug(uint8 data[],uint16 len);
	void LocalUartFxedLenSend(uint8 data[],uint16 len);
	void LocalUartParaInit(void);
	uint16 GetLocalUartRxData(uint8 rx_data[]);
	
	void UsartGprsDeInit(void);///Gprs串口引脚反初始化
	void UsartInit(USART_TypeDef* USARTx, u32 uart_bpr,uint8 data_bits,uint8 stop_bits,uint8 parity_check);
	
	DRIVER_EXTERN USART_STRUCT g_local_uart_struct;

	
	#define UART_RX_50_MS_DELAY 	10
	
	void SysTickInit(void);
    void DriverMain(void);

	#define LOCAL_UART_BUF_LEN 		(1024+12)
	
	DRIVER_EXTERN uint8 local_uart_buf[LOCAL_UART_BUF_LEN];
	void FeedWtd(void);
	void IwdgInit(void);
	void PvdInit(void);
	
	void SysTickDisable(void);
	void SysTickEnable(void);
	void SysTickInit(void);

#endif
