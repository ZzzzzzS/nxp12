#ifndef  __DATA_H__
#define  __DATA_H__
#include "common.h"

/*============================================
电机控制相关宏定义
==========================================*/

#define MAX_SPEED			99					//定义最大速度
#define MIN_SPEED			0					//定义最小速度


#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH3
#define MOTOR2_PWM  FTM_CH4
#define MOTOR3_PWM  FTM_CH1
#define MOTOR4_PWM  FTM_CH2

#define MOTOR1_PWM_IO  FTM0_CH3
#define MOTOR2_PWM_IO  FTM0_CH4
#define MOTOR3_PWM_IO  FTM0_CH1
#define MOTOR4_PWM_IO  FTM0_CH2

#define MOTOR_HZ    20*1000				//定义电机工作频率

/*============================================
电感数据采集相关宏定义
==========================================*/

#define AMP1			ADC0_SE8						//PTB0
#define AMP2			ADC0_SE9						//PTB1
#define AMP3			ADC0_SE12					//PTB2
#define AMP4			ADC0_SE13					//PTB3
#define AMP5			ADC1_SE10					//PTB4
#define AMP_MAX	5									//定义最大ADC端口数

#define MAX_WEIGHT 10								//定义权重向前滤波最大权重值

typedef enum Inductance_Position				//枚举定义电感位置
{
	LEFT,
	RIGHT,
	MIDDLE,
	FRONT_LEFT,
	FRONT_RIGHT,
}Inductance_Position;

/*============================================
方向计算相关宏定义
==========================================*/

#define LEFT_WEIGHT				7						//定义转向权重
#define RIGHT_WEIGHT	        7						//定义转向权重

#define MAX_FUZZY_RULE		6				//模糊论域大小

#define a1				0.9976									//定义拟合曲线K值
#define a2			-1.1262									//定义拟合曲线K值
#define b1				1.0406									//定义拟合曲线b值
#define b2			-1.0226									//定义拟合曲线b值
#define c1				-0.7316
#define c2			0.7425

/*============================================
其它宏定义和typedef
==========================================*/
#define Key1   PTC13									//按键管脚定义
#define Key2   PTC11									//按键管脚定义
#define Key3   PTC9									//按键管脚定义,这个按钮有问题
#define Key4   PTC7									//按键管脚定义
#define key5	   PTC5									//按键管脚定义
//修改OLED管脚时注意修改init的管脚!
#define RESET	 PTC14_OUT						//OLED相关宏定义
#define DC		 PTC16_OUT						//OLED相关宏定义
#define D1		 PTC12_OUT						//OLED相关宏定义
#define D0		 PTC10_OUT						//OLED相关宏定义
#define Bluetooth		UART1						//宏定义Bluetooth®发送端口
#define Bluetooth_Band	9600				//宏定义Bluetooth®发送波特率
//#define CAR_STOP		PTE8						//定义停车检测管脚
//#define CAR_STOP_NUM	8					//定义停车管脚号
#define true			1									//定义逻辑真
#define false			0									//定义逻辑假
typedef char			bool;								//定义bool类型

/*============================================
调试模式宏定义
==========================================*/

#define Position(OLED_Line)				0,(OLED_Line)	//坐标定义
typedef char										data;					//定义data类型
typedef unsigned char						mode;				//定义模式选择类型
typedef unsigned char						counter;			//定义累加器类型
typedef unsigned char						error;				//定义错误类型
typedef char										flag;					//定义标志类型
typedef enum OLED_Line											//定义OLED显示位置
{
	Line1 = 0,
	Line2 = 2,
	Line3 = 4,
	Line4 = 6,

	MAX_Line
}OLED_Line;

typedef enum															//定义系统错误编号
{
	Motor_Stop,

	MAX_error
}Error_Num;

typedef enum Debug_Interface									//定义调试模式OLED界面编号
{
	Inductance_Interface,
	Speed_Interface,
	PID_Interface,

	MAX_Interface
}Debug_Interface;

/*============================================
速度相关数据结构体
==========================================*/

typedef struct speed
{
	char Out_Speed;								//最终输出到电机的速度
	char PID_Out_Speed;						//PID处理后的速度
	float P;												//pid常量
	float I;												//pid常量
	float D;												//pid常量
	int16 Aim_Speed;								//目标速度
	int16 Turn_Speed;								//转向差速度
	int16 Go_Speed;								//正常前进速度
	int16 Now_Speed;								//正交解码得出的当前速度
	int16 Error_Speed;								//目标速度与当前速度的差值
	int16 Intergate_Speed;						//定义积分速度
	float IncrementSpeed;						//速度增量
	float err_next;									//定义上一个偏差值    
	float err_last;										//定义最上前的偏差值
}speed;

/*============================================
电感相关数据结构体
==========================================*/

typedef struct
{
	int16 AD_Value;									//ADC数模转换器采集到的值,8bit
	int16 AD_Value_fixed;						//滤波后的值
	int16 Normalized_Value;					//差比和的电感值
	int16 AD_Value_Old[4];						//权重向前滤波算法储存的前几次采集到的值
	unsigned char AD_Weight[4];			//权重向前滤波算法权重值
}inductance;

/*============================================
方向相关数据结构体
==========================================*/

typedef struct direction							//差比和法方向控制
{
	int err;												//偏差误差
	double sum[3];									//差比和相关定义

	float P;												//方向P控制
	float I;												//方向I控制
	float D;												//方向D控制
	char Error_Last;									//上一次误差
	char PID_Out_Speed;						//PID输出偏差
}direction;

/*============================================
模糊控制方向相关数据结构体
==========================================*/

typedef struct position
{
	double eAngle;									//夹角信息
	double eLength;								//长度信息
}position;

typedef struct fuzzy_direction							//模糊控制法方向控制
{
	position Position;											//临时储存隶属度
	position eRule[MAX_FUZZY_RULE];				//储存模糊论域
	position eGrade[MAX_FUZZY_RULE];				//储存隶属度
	flag		isMatched;										//判断是否匹配到
}fuzzy_direction;

/*============================================
其它数据结构体
==========================================*/

typedef struct service
{
	bool isDebug;									//定义是否启动调试
	mode flag;										//调试模式相关标志位
}service;

/*============================================
"对象""实例化"
==========================================*/

extern speed Left_Speed;									//声明一个"Speed类"的"对象"，左轮数据
extern speed Right_Speed;								//声明一个"Speed类"的"对象"，右轮数据

extern inductance Road_Data[AMP_MAX];			//声明一个"Inductance类"的"对象"数组，电感信息,修改时记得修改蓝牙发送

extern direction Direction;									//声明一个"Direction类"的"对象"，方向信息

extern fuzzy_direction Fuzzy_Direction;				//声明一个"Fuzzy_Direction类"的"对象"，模糊控制方向信息

extern service Service;										//声明一个"service类"的"对象"，串口发送等服务信息
#endif  //__DATA_H__