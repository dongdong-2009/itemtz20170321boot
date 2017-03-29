
#define BOOT_GLOBAL
#define 	IRQ_DISABLE()     	(__disable_irq())
#define  	IRQ_ENABLE()  		(__enable_irq())
#include "include.h"


uint8 BootFlashErase(void)
{
	uint8 i,total_page,res = TRUE;;
	
	IRQ_DISABLE();
	total_page = (FlASH_STORE_START_ADDR - 2*APP_PROGRAM_START_ADDR)/ST32_PAGE_SIZE;
	for(i=0;i<total_page;i++)///擦除文件存储区
	{
		FeedWtd();
		if(!FlashErase(APP_PROGRAM_START_ADDR+i*ST32_PAGE_SIZE))
		{
			res = FALSE;
			break;
		}
	}
	IRQ_ENABLE();
	return res;
}
uint8 BootParaInit(void)
{
	uint8 res,tmp_data[11] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	res = FlashErase(BOOT_PARA_START_ADDR);
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	boot_struct.program_update_flag = INVALID_VAL_DWORD_55;
	boot_struct.sys_para_init_flag = VALID_VAL_DWORD_AA;
	res = FlashWrite(BOOT_PARA_START_ADDR,(uint8*)&boot_struct.program_update_flag,12);
	
	FlashErase(RELAY_LOCK_PARA_START_ADDR);///清继电器锁车标志
	
	SpiFramWrite(2047,tmp_data,11);///兼容天昊
	
RETURN_LAB:
	return res;
}
uint8 BootProgramIap(void)
{
	uint8 res = FALSE,tmp_data[ST32_PAGE_SIZE];
	uint32 wr_size = 0;
	if(boot_struct.program_total_size > (FLASH_STORE_END_ADDR - FlASH_STORE_START_ADDR))
	{
		res = BootParaInit();
		goto RETURN_LAB;
	}
	
	BootFlashErase();
	
	IRQ_DISABLE();
	while(wr_size < boot_struct.program_total_size)
	{
		FeedWtd();
		FlashRead(FlASH_STORE_START_ADDR+wr_size,tmp_data,ST32_PAGE_SIZE);
		res = FlashWrite(APP_PROGRAM_START_ADDR+wr_size,tmp_data,ST32_PAGE_SIZE);
		if(!res)
		{
			break;
		}
		wr_size += ST32_PAGE_SIZE;
	}
	IRQ_ENABLE();
	if(wr_size >= boot_struct.program_total_size)
	{
		res = BootParaInit();
	}
RETURN_LAB:
	return res;
}
uint8 BootReset(void)
{
	uint8 res;

	res = BootFlashErase();
	return res;
}
void BootRxProgram(uint8 in_data[],uint16 len)
{
	uint8 res = FALSE,check_val,tx_data[1];

	tx_data[0] = 0x06;
	
	if((len != 1024+3)||(in_data[0] != boot_struct.cur_rx_packet_index))
	{
		goto RETURN_LAB;
	}
	
	if(boot_struct.cur_rx_packet_index > boot_struct.total_packet_num)
	{
		goto RETURN_LAB;
	}
	
	len -= 3;
	
	check_val = XorCheck(in_data+2,len);
	if(check_val != in_data[1026])
	{
		goto RETURN_LAB;
	}
	
	res = FlashWrite(APP_PROGRAM_START_ADDR+boot_struct.program_total_size,in_data+2,len);
	if(res)
	{
		boot_struct.program_total_size += len;
		boot_struct.cur_rx_packet_index += 1;
		LocalUartFxedLenSend(tx_data,1);
	}
	boot_struct.boot_ms_delay_counter = 0;
RETURN_LAB:
	return;
}
typedef  void (*pFunction)(void);

void BootAppProgram(void)
{ 
	uint32 JumpAddress = *(volatile uint32*)(APP_PROGRAM_START_ADDR + 4);

    pFunction Jump_To_Application = (pFunction)JumpAddress;

    Jump_To_Application();
}
void BootMain(void)
{
	uint8 rx_data[LOCAL_UART_BUF_LEN],tx_data[256],res,rx_max_packet;
	uint8 fram_wr[2] = {0x11,0x22};
	uint8 tmp_data[11];
	uint16 total_len;
	uint32 tmp_len;
	
	while(boot_struct.boot_ms_delay_counter < 5000)
///	while(1)
	{
		FeedWtd();
		DriverMain();
		
		total_len = GetLocalUartRxData(rx_data);
		
		if(total_len == 0)
		{
			continue;
		}
		
		///兼容天昊的串口调试工具开始
		if((rx_data[0] == 0x01)&&(rx_data[1] == 0x06)&&
		   (rx_data[2] == 0x05)&&(rx_data[3] == 0x01)&&(rx_data[7] == 0x0a))
		{	
			boot_struct.boot_ms_delay_counter = 0;
			boot_struct.total_packet_num = rx_data[5];
			boot_struct.program_total_size = 0;
			rx_max_packet = ((FLASH_STORE_END_ADDR - FlASH_STORE_START_ADDR)/ST32_PAGE_SIZE)*2;
			if(boot_struct.total_packet_num > rx_max_packet)
			{
				break;
			}
			
			boot_struct.cur_rx_packet_index = 0;
			
			
			BootReset();

			tx_data[0] = 0x06;tx_data[1] = 0x01;tx_data[2] = 0x03;
			tx_data[3] = 0x02;tx_data[4] = 0x06;tx_data[5] = 0x0a;
			LocalUartFxedLenSend(tx_data,6);
		//	g_local_uart_struct.rx_counter = 0;
			LongTimeDly(500);
			UsartInit(LOCAL_USART,38400,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
			
			continue;
		}
		
		BootRxProgram(rx_data,total_len);
		
		if((boot_struct.cur_rx_packet_index == boot_struct.total_packet_num)&&
		   (boot_struct.total_packet_num > 0))
		{
			LongTimeDly(500);
			res = BootParaInit();
			if(!res)
			{
				while(1);
			}
			SpiFramWrite(2047,fram_wr,2);
			break;
		}
	}	

	FeedWtd();
	FlashRead(BOOT_PARA_START_ADDR,(uint8*)&boot_struct.program_update_flag,12);
	if(boot_struct.program_update_flag == VALID_VAL_DWORD_AA)
	{
		res = BootProgramIap();
		if(!res)
		{
			while(1);
		}
	}
	
	FeedWtd();
	res = SpiFramRead(2049,tmp_data,9);
	if(res)
	{
		if((tmp_data[0] == 0xAA)&&(tmp_data[1] == 0x55)&&(tmp_data[2] == 0xA5))
		{
			tmp_len = (tmp_data[3] << 8) + tmp_data[4];
			boot_struct.program_total_size = (tmp_len - 1) << 10;
			tmp_len = (tmp_data[5] << 8) + tmp_data[6];
			boot_struct.program_total_size += tmp_len;
			
			res = BootProgramIap();
			if(!res)
			{
				while(1);
			}
		}
	}

	UsartInit(LOCAL_USART,9600,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);		
	LocalUartFxedLenSend("boot start...\r\n",StrLen("boot start...\r\n",0));
	LongTimeDly(500);

	FeedWtd();
	BootAppProgram();
	while(1);
}
