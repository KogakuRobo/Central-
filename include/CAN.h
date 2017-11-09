#ifndef _CAN_H_
#define _CAN_H_	

//CAN�o�X�̃r�b�g���[�g�ݒ�
#define CAN_BITRATE		1000	//[kbps]

//CAN�o�X�̑O���錾
class CAN_bus;

//�n���h���̃��^�[��
typedef enum{
	NON_REQUEST,
	TX_RESENT,
	RX_RESET,
}HandleReturn;
/*
NON_REQUEST
 ���󗼕��ŁA���b�Z�[�W�o�b�t�@���~���܂��B���M�ł͓��ɈӖ��͂Ȃ��ł����A
 ��M�ł͈Ȍ�̂��̃o�b�t�@�ł̎�M�����Ȃ��Ȃ�̂ŁA�����Ď�M����ꍇ��RX_RESET��߂����ƁB

TX_RESENT
 ���M��p�B�������b�Z�[�W���ēx�]�����܂��B
 
RX_RESET
 ��M��p�B�o�b�t�@���Ďg�p���܂��B
*/

//CAN_MSG�N���X�@CAN�o�X�ɂ��̃N���X��p���ĒʐM���s���܂��B
class CAN_MSG{
public:

	// ���b�Z�\�W���̂��́BCAN�̋K�i���Q�Ƃ��邱��
	unsigned short SID;
	unsigned short EID;
	unsigned char RTR;
	unsigned char IDE;
	unsigned char DLC;
	unsigned char data[8];

	//���b�Z�[�W�������ݗp�֐��B�オpublic�����o�Ȃ̂ŕK�������K�v�ł͂Ȃ�����ǂ��ꉞ�p�ӁB
	void Set(unsigned short sid,unsigned short eid,unsigned char rtr,unsigned char ide,unsigned char dlc,unsigned char d[]){
		this->SID = sid;this->EID = eid;
		this->RTR = rtr;this->IDE = ide;
		this->DLC = dlc;
		for(int i = 0;i < dlc;i++)this->data[i] = d[i];
	}

	//�n���h���A�h���X
	/*
	�p�r�Ɠ���
	�@�n���h���̓��b�Z�[�W����M�E�������͑��M���ꂽ�Ƃ��ɌĂяo����܂��B�Ȃ��A���荞�݂ɂ��Ăяo�����̂ŁA�n���h�������𒷂����Ȃ����ƁB
	����
	�@��M���ɂ́A��M�������b�Z�[�W�������ꂽ���̃N���X�ɂȂ�܂��Bhandle��attr�͐ݒ肵�����̂Ɠ������̂�������܂��B
	�@��M�ݒ莞�ɁAattr�ɃN���X�̃A�h���X�������邱�ƂŃN���X�̃����o�֐����Ăяo�����Ƃ��\�ł��B
	*/
	HandleReturn (*handle)(CAN_MSG);
	
	//attr
	/*
	�@�⑫���n���h���Ɉ����n���⑫�������邱�Ƃ��ł��܂��B
	*/
	void *attr;
};

//CAN_BUS�C���^�[�t�F�[�X�N���X
/*
�@�f�o�C�X�ˑ������z�����邽�߂Ɋ�{�I�ȋ@�\���`�����N���X
*/
class CAN_bus{
public:
	//���M�֐��B�����̃��b�Z�[�W�𑗐M���܂��B���M��Ahandle���Ăт܂��B
	virtual int Send(CAN_MSG msg) = 0;
	//��M�����֐��B
	/*
	����
	�@msg : CAN_MSG,mask_e : bool
	�@�@SID,EID,RTR,IDE���L���B
	�@�@mask_e(MaskEnable)��1�ł���Ƃ��A(ID | MASK_ID) == (ReceiveID | MASK_ID)�̂Ƃ���M���A����ȊO�̓X���[���܂��B
	�@�@mask_e��0�̎��AID == ReceiveID�̎���M���܂��B
	�@�@��M�����Ƃ��ACAN_MSG��handle���Ăяo���܂��B
	�@num : unsigned short
	�@�@��M�Ɏg�p����o�b�t�@�𖾎��I�Ɏw�肵�܂��B
	�@�@�}�X�N�Ƃ̌��ˍ���������ꍇ�ɗp���܂��B
	*/
	virtual int ReceiveSet(CAN_MSG msg,bool mask_e = 0) = 0;
	virtual int ReceiveSet(CAN_MSG msg,unsigned short num,bool mask_e) = 0;
};

#endif