#ifndef _CLASS_HPP_
#define _CLASS_HPP_



//****タイヤ・モータのクラス****//
class Wheel_Class {

private:
	double M_out;				//モーター出力
	double M_out_last;			//１つ前のモーター出力
	double angular_velocity;		//要求するタイヤの角速度
	double observation_angular_velocity;	//実測値のタイヤの角速度

public:
	Wheel_Class(void);			//コンストラクタ
	void W_Motor_out(int m_r_out);		//モーターの出力(書き込み)
	void W_Wheel_a_v(double w_r_a_v);	//要求するタイヤの角速度(書き込み)
	void W_Wheel_o_a_v(double w_o_r_a_v);	//実測値のタイヤの角速度(書き込み)
	double R_Wheel_a_v(void);		//要求するタイヤの角速度(読み出し)
	double R_Wheel_o_a_v(void);		//実測値のタイヤの角速度(読み出し)
	double R_Motor_out(void);		//モーターの出力(読み出し)
	double R_Motor_out_last(void);		//１つ前のモーターの出力(読み出し)
};



class Velocity_Class {//速度制御（2017NHK大会プログラム流用）
private:
	double arrival_time;			//到達時間
	double accel_time;			//加減速時間
	double ratio;				//　等速走行時間÷加速時間
	double MAX_velocity;
	static double calculated_distance;
	double K;				//出力スピードの定数
	double ideal_velocity;			//理想速度


public:
	//コンストラクタ//
	Velocity_Class(void);
	//到達時間の書き込み(値)
	void W_arrival_time(double ar_t);
	//加減速時間の算出
	//arrival_timeとratioの入力後に入力
	void C_accel_time(void);
	//加速時間の割合の書き込み(値)
	//等速走行時間÷到達時間
	void W_ratio(double r);
	//最高速度の書き込み(値)
	void C_MAX_velocity(double x);
	//計算上の距離の書き込み（値）
	void W_calculated_distance(double c_d);
	//計算上の距離修正値書き込み（値）
	void W_New_calculated_distance(double n_c_d);	
	//出力スピードの係数書き込み（値）
	void W_coefficient(double k);
	//理想速度の書き込み
	void W_ideal_velocity(double i_v);

	//到達時間の読み出し()
	double R_arrival_time(void);
	//加減速時間の読み出し()
	double R_accel_time(void);
	//到達時間の割合の読み出し()
	double R_ratio(void);
	//最高速度の読み出し()
	double R_MAX_velocity(void);
	//計算上の距離の読み出し()
	double R_calculated_distance(void);

	//出力スピードの修正計算(理論上の距離,実践上の距離)
	double revise_speed(double theory_d,double practice_d);
	//calculated_distanceのリセット
	void Reset_distance(void);
	//理想速度の読み出し
	double R_ideal_velocity(void);

};

class Target_Point_Class { //目標点関係のクラス（２０１７年プログラより流用）
private :
	double point[3];
	double target_distance;		//目標までの距離
	static double last_point_X;
	static double last_point_Y;
	

public:
	//コンストラクタ//
	Target_Point_Class(void);
	//目標点及び角度の書き込み
	void W_point(double x, double y, double r);
	//目標距離の書き込み（値）
	void W_Target_Distance(double t_d);
	//終着時点のX座標の書き込み
	void W_last_point_X(double l_x);
	//終着時点のY座標の書き込み
	void W_last_point_Y(double l_y);
	


	//目標点及び角度の読み出し()
	double R_point(int no);
	//目標距離の読み出し（）
	double R_Target_Distance(void);
	//終着時点のX座標の読み出し
	double R_last_point_X(void);
	//終着時点のY座標の読み出し
	double R_last_point_Y(void);

};

class My_Position_Class {	//自己位置
protected :
	static double position[4];		//自己位置推定｛X,Y,radian｝
	static double position_Rotary[4];	//Rotaryによる自己位置推定{X_Rotary,Y_Rotary,radian_Rotary}
	static double position_Rotary_0[4];
	
											
public:
	//コンストラクタ//
	My_Position_Class(void);

	//自己位置座標の書き込み（値,[X,Y,radian]）
	void W_Position(double posi, int no);	
	//自己位置座標の書き込み（[X,Y,radian]）
	void W_Position(double x, double y, double r);		
	//自己位置座標の読み出し（[X,Y,radian]）
	double R_Position(int no);
	//自己位置座標の読み出し（[X_Rotary,Y_Rotary,radian_Rotary]）
	double R_Position_Rotary(int no);
	//自己位置の統合
	void position_Fusion(void);
	
	void position_pri(void);
};


/****ベジェ曲線関係****/
class Bezier_Class {
private :
	double t;	//目標座標までの時間（ 0<=t<1 ）
	double p0_x, p1_x, p2_x, p3_x;	//制御点（ｘ軸）
	double p0_y, p1_y, p2_y, p3_y;	//制御点（ｙ軸）
	double vel_rate;//等速の時間割合(velクラスでいい)
	double ac_rate;//加減速の時間割合(velクラスでいい)
	static double p_x, p_y;//時間tでの座標
	
	double Px[3];	//現在の座標、１つあとの座標、差
	double Py[3];	//現在の座標、１つあとの座標、差
	double ideal_vel;//瞬間速度理想値
	
	static double t_px, t_py, t_rad;
	
public :
	Bezier_Class(void);
	void W_time(double time);//時間書き出し
	void W_p_x(double p0, double p1, double p2, double p3);//制御点（ｘ軸方向）書き出し
	void W_p_y(double p0, double p1, double p2, double p3);//制御点（ｙ軸方向）書き出し
	void W_p();//時間ごとの座標書き出し
	void W_point(double x, double y, double r);
	void W_Vel_rate(double v_rate);
	void W_Ac_rate(double a_rate);
	void W_ideal_vel(double i_v);
	
	double R_time(void);
	double R_p_x(int no);
	double R_p_y(int no);
	double R_p(int no);
	double R_Vel_rate(void);
	double R_Ac_rate(void);
	double R_ideal_vel(void);
	double R_point(int no);
};


/****ロータリーエンコーダのクラス****/
class Rotary_Class : public My_Position_Class{
private :
	double angular_velocity_self[4];	//自己位置推定用ロータリーの角速度
	double Velocity;			//機体の移動速度
public:
	//コンストラクタ//
	Rotary_Class(void);
	//機体の速度の書き込み（値）
	void W_Maschine_Velocity(double m_v);
	//自己位置座標の書き込み（値,[X_Rotary,Y_Rotary,radian_Rotary]）
	void W_Position_Rotary(double posi, int no);	
	//検知用タイヤの角速度の書き込み（値,[Right,Left]）
	void W_Angular_Velocity_Self(double a_v, int no);
	//検知用タイヤの角速度の読み出し（値[Right,Left]）
	double R_Angular_Velocity_Self(int no);
	//機体の速度の読み出し（）
	double R_Maschine_Velocity(void);
	//座標算出
	void calculation_coordinate_Rotary(void);
	//print関数
	void Rotary_pri(void);
};

class Flag_Class {//フラグ関係（2017NHK大会より）
private :
	int flag;
	bool dip_on_off[4];
public:
	//コンストラクタ//
	Flag_Class(int f);
	//flagを上げる
	void flag_up(void);
	//flagを下げる
	void flag_down(void);
	//flagのカウントアップ?
	void flag_count_up(void);
	//flagのリセット
	void flag_Reset(void);
	//フラグの書き込み
	void W_flag(int f);
	//flagの呼び出し
	int R_flag(void);
	
	void W_dip(bool s,int no);
	bool R_dip(int no);
};


//****第n走行確認クラス****//
class Run_No_Class{
	private:
	int run_no;
	public:
	//コンストラクタ//
	Run_No_Class(void);
	//読み出し
	int R_run_no(void);
	//書き込み
	void W_run_no(int n);
	
};



class TIME_Class {
private:
	double TIME;
	double Hz;
	
public:
	//コンストラクタ//
	TIME_Class(void);
	//カウント
	void Count(void);
	//周期の書き込み
	void W_HZ(double hz);
	//時間の読み出し
	double R_TIME(void);
};

//速度・座標目標値
class Ref{
	private:
	double X_ref, Y_ref, YAW_ref;
	double Vx_ref, Vy_ref, Vyaw_ref;
	
	public:
	void W_poji_ref(double xref, double yref, double yawref);
	void W_Vel_ref(double vx, double vy, double vyaw);
	double R_poji_ref(int num);
	double R_Vel_ref(int num);
};
#endif