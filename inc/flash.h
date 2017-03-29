#ifndef _FLASH_H 
#define _FLASH_H

		#include "stm32f10x.h"
		
		#define ST32_PAGE_SIZE						0x800  ///STM32页大小2048字节
		
		#define APP_PROGRAM_START_ADDR            	0X2000///8k
		
		#define FlASH_STORE_START_ADDR			   	0x20000///FLASH存储起始地址128K
		
		#define BOOT_PARA_START_ADDR			   	(FlASH_STORE_START_ADDR-0x2000)///128k-8k
		
		#define RELAY_LOCK_PARA_START_ADDR			(FlASH_STORE_START_ADDR-0x1000)///128k-4k
		
		
		#define FLASH_STORE_END_ADDR				(0x40000 - (2*APP_PROGRAM_START_ADDR))///FLASH存储结束地址256k-16k
		
		uint8 FlashErase(uint32 page_addr);
		uint8 FlashWrite(uint32 start_addr,uint8 wr_data[], uint16 len);
		void FlashRead(uint32 start_addr, uint8 rd_data[], uint16 len);
#endif
