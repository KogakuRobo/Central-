#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C"{
#endif

	void printf_Init();
	
	void sci_print(char,int);
	void sci_printf(char *,...);

#ifdef __cplusplus
}
#endif

#endif