#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

//初期設定
void OSC_init(void);
void MSTP_init(void);
void GPIO_init(void);
void contorol(void);
void CMT0_init(void);
void CMT1_init(void);

// MTU設定
void MTU_LIST(void);
void MTU1_init(void);	//ロータリー４
void MTU2_init(void);	//ロータリー３
void MTU4_init(void);	//モータ２
void MTU6_init(void);	//モータ１
void MTU7_init(void);	//ロータリー２
void MTU8_init(void);	//ロータリー１
void MTU9_init(void);	//モータ３
void MTU10_init(void);  //モータ４
void SMTP(void);

//ロータリー
int ROTARY(int dir);

#endif