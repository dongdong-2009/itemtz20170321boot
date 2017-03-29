
#define LOCAL_COMM_CMD_RESET 				0X01		
#define LOCAL_COMM_CMD_UPDATE_PROGRAM 		0X02	
#define LOCAL_COMM_CMD_QUERY_SYS_PARA 		0X03	
#define LOCAL_COMM_CMD_SET_SYS_PARA 		0X04
#define LOCAL_COMM_CMD_GET_WORK_INFO 		0X05
#define LOCAL_COMM_CMD_ACK					0x80

#define	LOCAL_COMM_LEN_INDEX				0X01
#define	LOCAL_COMM_CMD_INDEX				0X03
#define	LOCAL_COMM_PASSWORD_INDEX			0X04
#define	LOCAL_COMM_DATA_INDEX				0X08

void LocalCommTxPacket(uint8 data[],uint16 len,uint8 cmd,uint32 pwd)
{
	uint8 *p;
	
	p = data;
	*p++ = '*';
	*p++ = (len + 8) >> 8;
	*p++ = len + 8;
	*p++ = cmd;
	
	*p++ = pwd >> 24;
	*p++ = pwd >> 16;
	*p++ = pwd >> 8;
	*p++ = pwd;
	
	data[len+8] = U8SumCheck(data+1,len+7);
	data[len+9] = '#';
	LocalUartFxedLenSend(data,len+10);
}	
void LocalCommDealRxMsg(void)
{
	uint8 tx_data[600],*p,cmd;
	uint8 rx_data[UART_RX_MAX_LEN];
	uint16 tx_len,rx_len,total_len;
	uint32 tmp_val_32;
		
	///*,len_h,len_l,cmd,terminal_pass_word(4字节，高字节在前，默认00 01 B2 07),data,add_check,#
	total_len = GetLocalUartRxData(rx_data);
	
	if(total_len > 0)
	{
		if(('*' == rx_data[0]) && ('#' == rx_data[rx_len-1])) 
		{
			rx_len = rx_data[LOCAL_COMM_LEN_INDEX];
			rx_len	= (rx_len << 8) + rx_data[LOCAL_COMM_LEN_INDEX+1];

			if(rx_len != total_len - 2)
			{
				goto RETURN_LAB;
			}
			
			check_val = U8SumCheck(rx_data+1,rx_len-1);
			
			if(check_val != rx_data[rx_len])
			{
				goto RETURN_LAB;
			}
			
			tmp_val_32 = U8ToUint32(rx_data+LOCAL_COMM_PASSWORD_INDEX);
			if((tmp_val_32 != 0x1B207)&&(tmp_val_32 != GetSysPrivatePwdPara()))
			{
				goto RETURN_LAB;
			}
			
			rx_len = rx_len - 3;		///内容长度
			switch(rx_data[LOCAL_COMM_CMD_INDEX])
			{
				case LOCAL_COMM_CMD_RESET:
				{
					TermReset();
					break;
				}
				case LOCAL_COMM_CMD_QUERY_SYS_PARA:
				{
					if((rx_len % 2)!= 0X00)
					{
						goto RETURN_LAB;
					}

					p = tx_data;
					tx_len = QuerySysPrivatePara(tx_data+LOCAL_COMM_DATA_INDEX,data+LOCAL_COMM_DATA_INDEX,rx_len);
					cmd = LOCAL_COMM_CMD_ACK + LOCAL_COMM_CMD_QUERY_SYS_PARA;
					break;
				}
				case LOCAL_COMM_CMD_SET_SYS_PARA:
				{
					res = SetSysPrivatePara(data+LOCAL_COMM_DATA_INDEX,rx_len);
					tx_len = 1;
					tx_data[LOCAL_COMM_DATA_INDEX] = res;
					cmd = LOCAL_COMM_CMD_ACK + LOCAL_COMM_CMD_SET_SYS_PARA;
					break;
				}
				default: 
				{
					goto RETURN_LAB;
				}
			}
		}
	}
	
	LocalCommTxPacket(tx_data,tx_len,cmd,tmp_val_32);
	
RETURN_LAB:
	return;
}
void LocalCommMain(void)
{
	LocalCommDealRxMsg();
}
