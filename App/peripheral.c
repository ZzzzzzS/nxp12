#include "include.h"
#include "peripheral.h"

/*============================================
函数名：Stop_Car_Init()
作用：停车检测初始化
==========================================*/

/*void Stop_Car_Init()
{//不清楚上升沿还是下降沿
	port_init(CAR_STOP, ALT1 | IRQ_FALLING | PULLUP);			//初始化停车干簧管，下降沿中断
	set_vector_handler(PORTE_VECTORn, Stop_Car);				//设置停车中断向量
	enable_irq(PORTE_IRQn);										//使能向量
}*/

/*============================================
函数名：Stop_Car()
作用：停车检测
==========================================*/

/*void Stop_Car()
{
	if (PORTA_ISFR & (1 << CAR_STOP_NUM))						//确定中断管脚号
	{
		PORTA_ISFR = (1 << CAR_STOP_NUM);						//清除中断标志位

		disable_irq(LPTMR_IRQn);								//禁止低功耗定时计数器中断
		Right_Speed.Out_Speed = 0;								//调整速度为0
		Left_Speed.Out_Speed = 0;								//调整速度为0
		ftm_pwm_duty(FTM0, FTM_CH1, Right_Speed.Out_Speed);		//控制左轮转动
		ftm_pwm_duty(FTM0, FTM_CH2, Left_Speed.Out_Speed);		//控制右轮转动
	}
}*/

/*============================================
函数名：Send_Data()
作用：蓝牙串口发送
==========================================*/

void Send_Data()
{
	char var[2];
	for (char i = 0; i < 2; i++)
	{
		var[i] = Road_Data[i].Normalized_Value;					//向上位机发送电感归一化后的值
	}
	vcan_sendware(var, sizeof(var));							//发送到上位机，注意发送协议，发送端口
}

/*============================================
函数名：Save_Inductance()
作用：flash储存电感数值
==========================================*/
/*============================================
flash储存位置: L=最大值 S=最小值
偏移量:0  1  2  3  4  5  6  7
	   L0 L1 L2 L3 S0 S1 S2 S3
==========================================*/

void Save_Inductance()
{
	char i;
	//flash_erase_sector(SECTOR_NUM);								//擦除flash扇区，准备写入

	for (i = 0; i < 4; i++)										//储存电感归一化的分母
	{
		//do
		//{
			//flash_write(SECTOR_NUM, i * 4, Road_Data[i].normalization);
		//} while //(flash_read(SECTOR_NUM, i * 4, int16) != Road_Data[i].normalization);	//储存后验证是否储存正确
	}

	for (i = 4; i < 8; i++)										//储存电感最小值
	{
		//do
		//{
			//flash_write(SECTOR_NUM, i * 4, Road_Data[i-4].Min_AD_Value);
		//} while //(flash_read(SECTOR_NUM, i * 4, int16) != Road_Data[i - 4].Min_AD_Value);//储存后验证是否储存正确
	}
}

/*============================================
函数名：Save_Inductance()
作用：flash储存电感数值
==========================================*/

void load_Inductance()
{
	/*char i;
	for (i = 0; i < 4; i++)
	{
		Road_Data[i].Max_AD_Value = flash_read(SECTOR_NUM, i * 4, int16);
	}
	for (i = 4; i < 8; i++)
	{
		Road_Data[i - 4].Min_AD_Value = flash_read(SECTOR_NUM, i * 4, int16);
	}*/
        char i;

	for (i = 0; i < 2; i++)
	{
		Road_Data[i].Max_AD_Value = 0;
		Road_Data[i].Min_AD_Value = 100;
		Road_Data[i].normalization = Road_Data[i].Max_AD_Value - Road_Data[i].Min_AD_Value;
	}

}

/*============================================
函数名：Key_Init()
作用：初始化按键
==========================================*/

void Init_Key()
{
	key_init(KEY_U);
	key_init(KEY_D);
	key_init(KEY_R);
	key_init(KEY_L);
	key_init(KEY_A);
	key_init(KEY_B);
}

/*============================================
函数名：OLED_Interface()
作用：OLED显示发车前界面
==========================================*/

void OLED_Interface()
{
	OLED_Print(15, 0, "哈尔滨工业大学");
	OLED_Print(35, 2, "威海校区");
	OLED_Print(15, 4, "718创新实验室");
	OLED_Print(27, 6, "牛逼车神组");


	Service.Debug = true;



	/*while (key_check(KEY_A) == KEY_UP);								//等待继续操作
	OLED_CLS();														//清屏OLED
	OLED_Print(0, 0, "选择模式");
	OLED_Print(0, 2, "A调试模式");
	OLED_Print(0, 4, "B比赛模式");
	while (key_check(KEY_B) == KEY_UP&&key_check(KEY_D) == KEY_UP);//等待继续操作
	if (key_check(KEY_B) == KEY_DOWN)								//定义KEY_B按下为调试模式
	{
		Service.Debug = true;
	}
	else if (key_check(KEY_D) == KEY_DOWN)
	{
		Service.Debug = false;
	}*/
}

/*============================================
函数名：OLED_Normalization_Interface()
作用：OLED显示电感归一化中的界面
==========================================*/

void OLED_Normalization_Interface()
{
	char OLED_Temp[30];												//OLED显示使用的临时数组
	OLED_CLS();														//输出前先清理屏幕，防止乱码出现
	OLED_Print(0, 0, "正在获取最大最小值");
	sprintf(OLED_Temp, "最大%d %d", Road_Data[0].Max_AD_Value, Road_Data[1].Max_AD_Value);
	OLED_Print(0, 2, OLED_Temp);									//输出采集的最大电感值
	sprintf(OLED_Temp, "最小%d %d", Road_Data[0].Min_AD_Value, Road_Data[1].Min_AD_Value);
	OLED_Print(0, 4, OLED_Temp);									//输出采集的最小电感值
	sprintf(OLED_Temp, "当前%d %d", Road_Data[0].AD_Value, Road_Data[1].AD_Value);
	OLED_Print(0, 6, OLED_Temp);									//输出当前电感值
}