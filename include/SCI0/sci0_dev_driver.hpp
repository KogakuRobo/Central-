#ifndef _sci0_dev_driver_HPP_
#define _sci0_dev_driver_HPP_

#include "Description.h"

int _sci0_init_module(void);

//ボーレートを設定します。[bps]
//設定に失敗すると-1を返します。
//成功すると0を返します。
int _sci0_set_baudrate(unsigned long);

//パリティビットの使用するか設定します。trueで使用。falseで不使用
//常に0をかえします。
int _sci0_use_parity_bit(bool = true);

//中身なし
int _sci0_set_parity_bit(unsigned char);

//gpio設定
int _sci0_gpio_enable(bool = true);

//受信（、エラー）割り込み許可
int _sci0_rx_interrupt_enable(bool = true);

#endif