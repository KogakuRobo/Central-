#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include "HAL_Robot.h"

HAL_Robot::HAL_Robot(Localization *_l,
	MotorSystem* _motora,
	MotorSystem* _motorb,
	MotorSystem* _motorc,
	MotorSystem* _motord,
	my_position* _my_posi,
	target_point* _t_point,
	speed_control* _s_con) : Robot(_l,
		_motora,
		_motorb,
		_motorc,
		_motord
	), t_point(_t_point),
	my_posi(_my_posi),
	s_con(_s_con) {
		operating_time=0;
		flag=1;
	}

void HAL_Robot::Begin(void) {
	this->Robot::Begin();
}

void HAL_Robot::Safe(void){
		 static float i=0.0;
		 static float t=0;
		 static float count=0;
		 static float sji_section=0;
		 static double Angular_Velocity=0;
	/*t_point->t_posi_st_get(0,0,0,0.01);
		for(i=0;i<1;i+=0.1){
			//printf("i %f",i);
		if(i<1){
			//t_point->Sji_get(0.0,0.0,1.0,1.0,i);
		}
		else{i=1;}
			//printf("TEST1%f\n",i);
			t_point->Sji_get(0,0,1.0,1.0,i);
			//printf("TEST2%f\n",i);
			
			pri=s_con->output_F_give(1);
			//printf("TEST3%f\n",i);
			
			leg_motora->SetVelocity(pri);
			//printf("TEST4%f\n",i);
			
			//s_con->output_B_give();
			//printf("TEST5%f\n",i);
			//leg_motora->SetVelocity(s_con->output_B_get(1));
			/*leg_motorb->SetVelocity(s_con->output_B_get(2));
			leg_motorc->SetVelocity(s_con->output_B_get(3));
			leg_motord->SetVelocity(s_con->output_B_get(4));*/
			/*t+=1;
			
			//printf("%fs\n",t*0.1);
			//printf("%f\n",pri);
			
			msleep(10);
			
		}*/
			switch(flag){
				case 1:
					
				operating_time=1;  //稼働時間
					//t_point->t_posi_st_get(0,0,0,6);	//目標位置設定(初期x,y,目標x,y)
					//t_point->t_posi_get(0,0,0,10,10,10);
			i+=0.01/operating_time;			
			if(i>1.0){
				flag=2;
				//sji_section=1;
				break;
			}
			//printf("%f\n",i);
					//t_point->Sji_get(0,0,1.0,1.0,i);	//S字time作成
			if(i<0.333){	
				sji_section=3*i;
				//printf("%f\n",sji_section);
			t_point->Sji_get(0,0,1,1,sji_section);	//S字time作成
			}
			else if(i>0.666){
				sji_section=3*(i-0.66666);
						t_point->Sji_get(0,0,1,1,sji_section);	//S字time作成
			}
			s_con->angular_velocity_F_input(ST,0,0,0,6,operating_time,1);		//フィードフォワード出力値準備
			s_con->angular_velocity_B_input();	//フィードバック出力値準備
			//printf("%f\n",s_con->Angular_Velocity_F(1));
			//leg_motora->SetVelocity(s_con->Angular_Velocity_F(1)+s_con->Angular_Velocity_B(1));	//タイヤ別の出力値を与える
			leg_motord->SetVelocity(60);
			leg_motord->GetVelocityRequest();
			printf("%f\n",leg_motord->GetVelocity()); 
			/*leg_motord->SetVelocity(s_con->Angular_Velocity_B(2));*/
			t+=1;
			/*	}
			else if(sji_section==1){
			s_con->angular_velocity_F_input(ST);		//フィードフォワード出力値準備
			s_con->angular_velocity_B_input();	//フィードバック出力値準備
			//printf("%f\n",s_con->Angular_Velocity_F(1));
			leg_motora->SetVelocity(s_con->Angular_Velocity_F(1)+s_con->Angular_Velocity_B(1));	//タイヤ別の出力値を与える
			leg_motord->SetVelocity(s_con->Angular_Velocity_F(2)+s_con->Angular_Velocity_B(2));
			}
			//printf("%f,",i);
			//printf("%f,",t*0.01);
			//printf("%f\n",s_con->output_F_get(4,0));
			//printf("%f\n",s_con->Angular_Velocity_F(1));
		*/
			break;
				case 2:
				t_point->Sji_get(0,0,1.0,1.0,1);
				s_con->angular_velocity_F_input(ST,0,6,0,6,operating_time,2);
				s_con->angular_velocity_B_input();
				//printf("%f\n",s_con->Angular_Velocity_F(1));
				//leg_motora->SetVelocity(s_con->Angular_Velocity_F(1)+s_con->Angular_Velocity_B(1));
				leg_motord->SetVelocity(60);
				leg_motord->GetVelocityRequest();
				printf("%f\n",leg_motord->GetVelocity()); 
				/*leg_motora->SetVelocity(s_con->Angular_Velocity_B(1)); 
				leg_motord->SetVelocity(s_con->Angular_Velocity_B(2));*/
				count+=0.01;
				
				if(count>1){
					flag=2;
					i=0;
					count=0;
				}

				break;
				case 3:
				operating_time=6;  //稼働時間
					//t_point->t_posi_st_get(0,6,0,0);	//目標位置設定(初期x,y,目標x,y)
					//t_point->t_posi_get(0,0,0,10,10,10);
			i+=0.01/operating_time;		
			if(i>1.0){
				flag=4;
				break;}
			//t_point->Sji_get(0,0,1.0,1.0,i);	//S字time作成
			
			if(i<0.333){	
				sji_section=3*i;
				//printf("%f\n",sji_section);
			t_point->Sji_get(0,0,1,1,sji_section);	//S字time作成
			}
			else if(i>0.666){
				sji_section=3*(i-0.66666);
						t_point->Sji_get(0,0,1,1,sji_section);	//S字time作成
			}
			
			s_con->angular_velocity_F_input(ST,0,6,0,0,operating_time,3);		//フィードフォワード出力値準備
			s_con->angular_velocity_B_input();	//フィードバック出力値準備
			//printf("%f\n",s_con->Angular_Velocity_F(1));
			leg_motora->SetVelocity(s_con->Angular_Velocity_F(1)+s_con->Angular_Velocity_B(1));	//タイヤ別の出力値を与える
			leg_motord->SetVelocity(s_con->Angular_Velocity_F(2)+s_con->Angular_Velocity_B(2));
			/*leg_motora->SetVelocity(s_con->Angular_Velocity_B(1)); 
			leg_motord->SetVelocity(s_con->Angular_Velocity_B(2));*/
			t+=1;
			//printf("%f,",i);
			//printf("%f,",t*0.01);
			//printf("%f\n",s_con->output_F_get(4,0));
			//printf("%f\n",s_con->Angular_Velocity_F(1));
		
				break;
				case 4:
				t_point->Sji_get(0,0,1.0,1.0,1);
				s_con->angular_velocity_F_input(ST,0,0,0,0,operating_time,1);
				s_con->angular_velocity_B_input();
				//printf("%f\n",s_con->Angular_Velocity_F(1));
				leg_motora->SetVelocity(s_con->Angular_Velocity_F(1)+s_con->Angular_Velocity_B(1));
				leg_motord->SetVelocity(s_con->Angular_Velocity_F(2)+s_con->Angular_Velocity_B(2));
				/*leg_motora->SetVelocity(s_con->Angular_Velocity_B(1)); 
				leg_motord->SetVelocity(s_con->Angular_Velocity_B(2));*/
				//count+=0.1;
				
				break;
				default:
				break;
			}
}