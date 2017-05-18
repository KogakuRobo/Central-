#ifndef _rx621_sci0_HPP_
#define _rx621_sci0_HPP_

#include<stdarg.h>

#define SCI0_DTC_USE

void sci0_uart_init(void);

int sci0_printf(char *format,...);

#endif