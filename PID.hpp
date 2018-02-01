
#ifndef _PID_HPP_
#define _PID_HPP_

template<typename type,typename inner_val_type = float>
class PID{
	inner_val_type befor;	//前回偏差
	inner_val_type sum;	//偏差積和
	
	inner_val_type K;
	inner_val_type Ti;
	inner_val_type Td;
	
	inner_val_type delta_time;
	
	inner_val_type Proportion;
	inner_val_type Integration;
	inner_val_type Differentiation;
public:
	PID(inner_val_type k,inner_val_type ti,inner_val_type td,inner_val_type d_time):K(k),Ti(ti),Td(td),delta_time(d_time)
	{
		Reset();
	}
	
	void Reset(void)
	{
		sum = 0;
		befor = 0;
	}
	
	void SetK(inner_val_type _k)
	{
		K = _k;
	}
	
	void SetTi(inner_val_type _ti)
	{
		Ti = _ti;
	}
	
	void SetTd(inner_val_type _td)
	{
		Td = _td;
	}
	
	inner_val_type Run(type data,type ref)//ロータリ、値
	{
		inner_val_type error;
		inner_val_type ret = 0;
		error = ref - data;
		sum += (error + befor) / 2 * delta_time;
		
		Proportion = 0;
		Integration = 0;
		Differentiation = 0;
		
		Proportion = K * error;
		if(Ti != 0.0){
			Integration = K / Ti * sum;
		}
		Differentiation = K * Td * (error - befor) / delta_time;
		
		befor = error;
		ret = Proportion + Integration + Differentiation;
		return ret;
	}
};

#endif