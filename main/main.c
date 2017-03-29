#include "include.h"

#define ENABLE_WATCHDOG

int main(void)
{ 
	GpioInit();
	ON_GPS_PWR();///GPS电源，LOCAL 232电源
	UsartInit(LOCAL_USART,9600,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
	SysTickInit();
	PvdInit();
	SpiFramInit();
	#ifdef ENABLE_WATCHDOG
		IwdgInit();
		if(FLASH_GetReadOutProtectionStatus() != SET)
		{
			FLASH_Unlock();
			FLASH_ReadOutProtection(ENABLE); 
		}
	#endif
	FeedWtd();
	LongTimeDly(20000);
	FeedWtd();

	BootMain();	///BOOT任务
}
