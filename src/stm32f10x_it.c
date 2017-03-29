#include "include.h"

void SysTick_Handler(void)
{
	boot_struct.sys_tick_ms_counter++;
	boot_struct.boot_ms_delay_counter++;
}
void USART1_IRQHandler(void)///±¾µØ´®¿Ú
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if(g_local_uart_struct.rx_counter >= LOCAL_UART_BUF_LEN)
		{
			g_local_uart_struct.rx_counter -= 1;
		}
		g_local_uart_struct.rx_buf[g_local_uart_struct.rx_counter++] = USART_ReceiveData(USART1);
		g_local_uart_struct.rx_delay_time_counter = UART_RX_50_MS_DELAY;
	}
}
void PVD_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line16);
	if (PWR_GetFlagStatus(PWR_FLAG_PVDO))
	{
		while(1);
	}
}

void NMI_Handler(void)
{
	while (1)
	{
	}
}

void HardFault_Handler(void)
{

	while (1)
	{
	}
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}
void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
