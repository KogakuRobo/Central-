#ifndef _SPI_H_
#define _SPI_H_

#include"SPI.hpp"
#include"../DTC/_rx621_dtc.h"

#define TX_RX_BUFFER_SIZE 256

#define RSPI_DTC_USE

class RSPI1_SSLB0 : public SPI_Channel{
};

#pragma unpack
class RSPI1_Bus : public SPI_Bus{
	static unsigned long tx_data[TX_RX_BUFFER_SIZE];	//転送中の送信バッファ
	static unsigned long rx_data[TX_RX_BUFFER_SIZE];	//転送中の受信バッファ
	static unsigned char len;
	static volatile DTC_TABLE *tx_table;
	static volatile DTC_TABLE *rx_table;
	
	static bool singleton;
public:
	RSPI1_Bus(void);
	int Begin(unsigned int);
	int SetBitrate(unsigned char);
	
	unsigned int DataRW(unsigned int,unsigned char*,unsigned char *,unsigned short);
	
public:
	static void RSPI1_SPTI1(void);
	static void RSPI1_SPRI1(void);
};

#pragma packoption
#endif