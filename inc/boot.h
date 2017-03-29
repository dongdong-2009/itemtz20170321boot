#ifndef _BOOT_H 
#define _BOOT_H

		#include "stm32f10x.h"
		
		#ifdef BOOT_GLOBAL
			#define BOOT_EXTERN
		#else
			#define BOOT_EXTERN extern
		#endif

		#define LOCAL_COMM_CMD_RESET 				0X01		
		#define LOCAL_COMM_CMD_UPDATE_PROGRAM 		0X02
		#define LOCAL_COMM_CMD_QUERY_SYS_STATE		0X06		
		#define LOCAL_COMM_CMD_ACK					0x80

		#define	LOCAL_COMM_LEN_INDEX				0X01
		#define	LOCAL_COMM_CMD_INDEX				0X03
		#define	LOCAL_COMM_PASSWORD_INDEX			0X04
		#define	LOCAL_COMM_DATA_INDEX				0X08

		#define FAILURE_ACK 						0X01
		#define SUCCESS_ACK 						0X00
		
		#define VALID_VAL_DWORD_AA		0xAAAAAAAA
		#define INVALID_VAL_DWORD_55	0x55555555
		#define VALID_VAL_AA			0xAA
		#define INVALID_VAL_55			0x55	
		#define VALID_VAL_2A			0x2A
		#define INVALID_VAL_FF			0xFF

		typedef struct
		{
			uint32 program_update_flag;///������±�־
			uint32 program_total_size;///�ֽ�����
			uint32 sys_para_init_flag;///�������³�ʼ����־
			
			uint8 total_packet_num;///�ܰ���
			uint8 cur_rx_packet_index;///��ǰ���հ����
			uint32 boot_ms_delay_counter;///boot ������ʱ
			uint32 sys_tick_ms_counter;	 ///�������
		}SYS_BOOT_PARA_STRUCT;///ϵͳ��������

		BOOT_EXTERN SYS_BOOT_PARA_STRUCT boot_struct;
		
		void BootAck(uint8 ack_data,uint8 cmd,uint32 pwd);
		uint8 BootFlashErase(void);
		uint8 BootParaInit(void);
		uint8 BootProgramIap(void);
		uint8 BootReset(void);
		void BootRxProgram(uint8 in_data[],uint16 len);
		void BootAppProgram(void);
		void BootMain(void);
		
		
#endif
