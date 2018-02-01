#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

//�����ݒ�
void OSC_init(void);
void MSTP_init(void);
void GPIO_init(void);
void contorol(void);
void CMT0_init(void);
void CMT1_init(void);

// MTU�ݒ�
void MTU_LIST(void);
void MTU1_init(void);	//���[�^���[�S
void MTU2_init(void);	//���[�^���[�R
void MTU4_init(void);	//���[�^�Q
void MTU6_init(void);	//���[�^�P
void MTU7_init(void);	//���[�^���[�Q
void MTU8_init(void);	//���[�^���[�P
void MTU9_init(void);	//���[�^�R
void MTU10_init(void);  //���[�^�S
void SMTP(void);

//���[�^���[
int ROTARY(int dir);

#endif