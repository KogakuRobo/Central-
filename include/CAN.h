#ifndef _CAN_H_
#define _CAN_H_	

//CANバスのビットレート設定
#define CAN_BITRATE		1000	//[kbps]

//CANバスの前方宣言
class CAN_bus;

//ハンドルのリターン
typedef enum{
	NON_REQUEST,
	TX_RESENT,
	RX_RESET,
}HandleReturn;
/*
NON_REQUEST
 送受両方で、メッセージバッファを停止します。送信では特に意味はないですが、
 受信では以後のそのバッファでの受信をしなくなるので、続けて受信する場合はRX_RESETを戻すこと。

TX_RESENT
 送信専用。同じメッセージを再度転送します。
 
RX_RESET
 受信専用。バッファを再使用します。
*/

//CAN_MSGクラス　CANバスにこのクラスを用いて通信を行います。
class CAN_MSG{
public:

	// メッセ―ジそのもの。CANの規格を参照すること
	unsigned short SID;
	unsigned short EID;
	unsigned char RTR;
	unsigned char IDE;
	unsigned char DLC;
	unsigned char data[8];

	//メッセージ書き込み用関数。上がpublicメンバなので必ずしも必要ではないけれども一応用意。
	void Set(unsigned short sid,unsigned short eid,unsigned char rtr,unsigned char ide,unsigned char dlc,unsigned char d[]){
		this->SID = sid;this->EID = eid;
		this->RTR = rtr;this->IDE = ide;
		this->DLC = dlc;
		for(int i = 0;i < dlc;i++)this->data[i] = d[i];
	}

	//ハンドラアドレス
	/*
	用途と動き
	　ハンドラはメッセージが受信・もしくは送信されたときに呼び出されます。なお、割り込みにより呼び出されるので、ハンドラ処理を長くしないこと。
	引数
	　受信時には、受信したメッセージが書かれたこのクラスになります。handleやattrは設定したものと同じものが入れられます。
	　受信設定時に、attrにクラスのアドレスを代入することでクラスのメンバ関数を呼び出すことも可能です。
	*/
	HandleReturn (*handle)(CAN_MSG);
	
	//attr
	/*
	　補足情報ハンドラに引き渡す補足情報を入れることができます。
	*/
	void *attr;
};

//CAN_BUSインターフェースクラス
/*
　デバイス依存性を吸収するために基本的な機能を定義したクラス
*/
class CAN_bus{
public:
	//送信関数。引数のメッセージを送信します。送信後、handleを呼びます。
	virtual int Send(CAN_MSG msg) = 0;
	//受信準備関数。
	/*
	引数
	　msg : CAN_MSG,mask_e : bool
	　　SID,EID,RTR,IDEが有効。
	　　mask_e(MaskEnable)が1であるとき、(ID | MASK_ID) == (ReceiveID | MASK_ID)のとき受信し、それ以外はスルーします。
	　　mask_eが0の時、ID == ReceiveIDの時受信します。
	　　受信したとき、CAN_MSGのhandleを呼び出します。
	　num : unsigned short
	　　受信に使用するバッファを明示的に指定します。
	　　マスクとの兼ね合いがある場合に用います。
	*/
	virtual int ReceiveSet(CAN_MSG msg,bool mask_e = 0) = 0;
	virtual int ReceiveSet(CAN_MSG msg,unsigned short num,bool mask_e) = 0;
};

#endif