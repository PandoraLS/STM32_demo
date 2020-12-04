/******************** (C) COPYRIGHT 2016 ****************************************
 * 文件名  ：pwm2.c
 * 描述    ：         
 * 实验平台：IPC9学习板  			
 * 库版本  ：ST3.5.0
**********************************************************************************/
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "OLED.h"
#include "key.h"
#include "init_key.h"
#include "pwm0.h"
#include "pwm1.h"
#include "pwm2.h"
#include "time.h"
#include "exti.h"
#include "encoder.h"
#include "iic_analog.h"
#include "mpu6050.h"
#include "steppermotor.h"
#include "incrementalpid.h"
#include "led.h"
#include "beep.h"
#include "24l01.h"
#include "spi.h"
#include "Bluetooth.h" 
#include "zigbee.h"
#include "ultrasonic.h"
#include "adc.h"

int Encoder_Left;             //左右编码器的脉冲计数
unsigned char F_Rotation[4]={0x0010,0x0020,0x0004,0x0008};//正转表格
unsigned char B_Rotation[4]={0x10,0x08,0x040,0x02};//反转表格
vu8 key=0;	//全局变量，vu8防止被优化,按键值

int main(void)
{
//		u8 tx_buff[TX_PLOAD_WIDTH] = {4,3,2,1};
//		u8 rx_buff[RX_PLOAD_WIDTH];
//		u16 NRF_status;
//		int a=0,flog;
//		int i = 0;	
		delay_init();	//初始化延时

		NVIC_Configuration();

		LED_Init();/*LED初始化*/
		BEEP_Init();         	//初始化蜂鸣器端口

		/*   OLED初始化    */
		OLED_Init();
//		OLED_6x8var_ch(0,0,3);
//		OLED_6x8Str(50,5,"ok");//横坐标 纵坐标 要显示的字符


		/*    LCD初始化    */
//		Lcd_Init();			//初始化OLED 
//		LCD_Clear(BLACK); //清屏
//		BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 

		/*   串口、中断初始化    */
		USART1_Config();
//		USART2_Config();
//		USART3_Config();
//		UART4_Config();
//		UART5_Config();


//  Bluetooth_usart2();//蓝牙模块初始化

//		Zigbee_usart3();//zigbee接收初始化	
		/*   按键初始化    */
//		KEY_Init();		//独立按键初始化

		/*   PWM波初始化    *///这两个只能用一个
//		TIM2_PWM0_Init();//其他的会影响下载，所以就不搞了
//		TIM3_PWM0_Init();//无映像，部分重映像和下载口有冲突，索性这里不用部分重映像
//		TIM3_PWM2_Init();//完全重映像PC6,PC7,PC8,PC9

		/*   定时器、中断初始化    *///tim1的定时器不准，原因不明，所以就不用了
//		TIMER2_INIT();
//		TIMER3_INIT();
//		TIMER4_INIT();
		TIMER5_INIT();
		TIMER6_INIT();//用于adc10路采样	
//		TIMER7_INIT();

		/*   按键中断初始化    */
//		KEY_Configuration();	//按键中断初始化



			ADCx_Init();//adc采集
			
//		Ultran_Init();//超声模块初始化	

//		Encoder_Init_TIM2();            //初始化编码器

		/*   6050初始化    */
//		MPU6050_Init();//6050初始化
//		delay_ms(20);
//		MPU6050_Init();

		/*   步进电机初始化    */
//		Stepper_Motor_Init();//步进电机初始化程序
//		BAND_C_Init();//步进电机高速初始化
//		Stepper_Init();//小步进电机初始化

//		INC_Pid_Init();//增量式PID初始化

//		NRF24L01_Init();    		//初始化NRF24L01 
//		LCD_8x16var_int(0,4,34);
		delay_ms(1000);
//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  // 使能指定的USART1发送中断 ；
		LED0=1;					//先点亮红灯
		LED1=1;					//先点亮红灯

		while(1)
		{
//			LED1 = !LED1;
			delay_ms(1000);
		}

			
}
/**************************************************************************
超声波测距模块
		while(1)
		{
			LED1 = !LED1;
			Ultra_Ranging_use(&distance);
			printf("%f  \r\n",distance);
			delay_ms(1000);
		}
**************************************************************************/
/**************************************************************************
oled测试程序
OLED_6x8var_ch(0,0,3);
OLED_6x8Str(2,2,"ok");//横坐标 纵坐标 要显示的字符
**************************************************************************/
/**************************************************************************
beep测试程序
	while(1)
	{
		BEEP = !BEEP;
		LED0 = !LED0;
		delay_ms(1000);
	}
**************************************************************************/
/**************************************************************************
串口测试程序
	while(1)
	{
		USART1_printf(" ...USART1... \r\n");
		USART2_printf(" ...USART2... \r\n");
		USART3_printf(" ...USART3... \r\n");
		 UART4_printf(" ...UART4.... \r\n");
		 UART5_printf(" ...UART5.... \r\n");
		delay_ms(1000);
	}
**************************************************************************/
/**************************************************************************
按键测试程序
	while(1)
	{
		key=KEY_Scan(0);	//得到键值
		if(key)
		{						   
			switch(key)
			{
				case KEY_1:	//同时控制LED0,LED1翻转 
					LED0=!LED0;
				break;
				case KEY_2:	//控制蜂鸣器
					LED1=!LED1;
				break;
				case KEY_3:	//控制LED1翻转	 
					LED1=!LED1;
					LED0=!LED0;
				break;
				case KEY_4:	//同时控制LED0,LED1翻转 
					LED0=!LED0;
				break;
				case KEY_5:	//控制蜂鸣器
					LED1=!LED1;
				break;
				case KEY_6:	//控制LED1翻转	 
					LED1=!LED1;
					LED0=!LED0;
				break;
			}
		}else delay_ms(10); 
	}
**************************************************************************/

/**********************************************************************
nrf测试程序--中断方式--发送端
	while(NRF24L01_Check())
	{
		printf("check Error\r'n");
		delay_ms(400);
	}
	printf("success\r\n");
	
		while(1)
	{	
		delay_ms(200);
		NRF24L01_TX_Mode();		//发送模式
		if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				tmp_buf[0] = 1;
				tmp_buf[1] = 2;
				tmp_buf[2] = 3;
				tmp_buf[3] = 4;
				
				tmp_buf[32]=0;//加入结束符	
				printf("Send Data is %d  %d   %d   %d \r\n",
									tmp_buf[0],tmp_buf[1],tmp_buf[2],tmp_buf[3]);
			}
				else 
				{
					LED0 = !LED0;
				}
				delay_ms(200);
		}
**********************************************************************/

/**************************************************************************
//     NRF2401测试程序   --循环方式
//	while(NRF24L01_Check()!=SUCCESS)	//检查NRF24L01是否在位.	
//	{
//		printf("NRF24L01 Error");
//		delay_ms(200);
//	}
//	printf("   ok");
////	while(1)
////		{  	
////		NRF24L01_RxMode();
////		i = NRF24L01_ReceiveData(rx_buff);  
////		printf("%d\t\n",rx_buff[0]);
////		}
//			while(1)
//		{  	
//		NRF24L01_TxMode();
//		i = NRF24L01_SendData(tx_buff);  
//		}
**************************************************************************/
		
/*   LCD测试程序   */
//		LCD_ShowString(0,0,"100");
//		LCD_8x16var_int(30,0,800);
//		delay_ms(500);
//		LCD_8x16var_int(30,0,555);
//		delay_ms(500);
//		LCD_8x16var_int(30,0,-123);
//		delay_ms(500);

/*   pid测试程序   */
//		INC_Pid_Ctrl(-20,Encoder_Left);
//		UART0_Oscilloscope(Encoder_Left,1);
//		printf("%d    ",Encoder_Left);

//stepper_low_speed_Move(1600,2000);//步进电机自传一周

/*   小步进电机测试程序   */
//	for(i=0;i<4;i++)      //4相
//	 {
//		 GPIO_Write(GPIOA, F_Rotation[i]);  //输出对应的相 可以自行换成反转表格
//		 delay_ms(8);        //改变这个参数可以调整电机转速
//	 }

//USART2_printf(" <G> OK");//语音测试函数

//OLED测试程序
// for(i=-10;i<10;i++)
//		{
//			OLED_6x8var_ch(0,0,i);
//			delay_ms(300);
//		}

//OLED_BMP(0,0,128,8,BMP1);//OLED显示图片

// key_show();   //独立按键测试程序 

// Key_Value_Show();   //矩阵按键测试程序  

/*   MPU6050测试程序   */
//	Angle_Calculate();
//	printf("x角度%.2f   y角度%.2f   z角度%.2f   x角速度%.2f    y角速度%.2f      z角速度%.2f  \r\n ",angle_x,angle_y,angle_z,w_x,w_y,w_z);

/*pwm波测试程序
	if(flog ==1)
		{
			a=a+1;
		}
		else if(flog ==0)
		{
			a=a-1;
		}
		if(a>99)	flog =0;
		if(a<=0) flog = 1;
		TIM3->CCR1 = a;	
		TIM3->CCR2 = a;
		TIM3->CCR3 = a;
		TIM3->CCR4 = a;
		delay_ms(20);		
*/

/******************* (C) COPYRIGHT 2016 **********************************END OF FILE*****/
