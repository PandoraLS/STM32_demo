/******************** (C) COPYRIGHT 2016 ****************************************
 * �ļ���  ��pwm2.c
 * ����    ��         
 * ʵ��ƽ̨��IPC9ѧϰ��  			
 * ��汾  ��ST3.5.0
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

int Encoder_Left;             //���ұ��������������
unsigned char F_Rotation[4]={0x0010,0x0020,0x0004,0x0008};//��ת���
unsigned char B_Rotation[4]={0x10,0x08,0x040,0x02};//��ת���
vu8 key=0;	//ȫ�ֱ�����vu8��ֹ���Ż�,����ֵ

int main(void)
{
//		u8 tx_buff[TX_PLOAD_WIDTH] = {4,3,2,1};
//		u8 rx_buff[RX_PLOAD_WIDTH];
//		u16 NRF_status;
//		int a=0,flog;
//		int i = 0;	
		delay_init();	//��ʼ����ʱ

		NVIC_Configuration();

		LED_Init();/*LED��ʼ��*/
		BEEP_Init();         	//��ʼ���������˿�

		/*   OLED��ʼ��    */
		OLED_Init();
//		OLED_6x8var_ch(0,0,3);
//		OLED_6x8Str(50,5,"ok");//������ ������ Ҫ��ʾ���ַ�


		/*    LCD��ʼ��    */
//		Lcd_Init();			//��ʼ��OLED 
//		LCD_Clear(BLACK); //����
//		BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 

		/*   ���ڡ��жϳ�ʼ��    */
		USART1_Config();
//		USART2_Config();
//		USART3_Config();
//		UART4_Config();
//		UART5_Config();


//  Bluetooth_usart2();//����ģ���ʼ��

//		Zigbee_usart3();//zigbee���ճ�ʼ��	
		/*   ������ʼ��    */
//		KEY_Init();		//����������ʼ��

		/*   PWM����ʼ��    *///������ֻ����һ��
//		TIM2_PWM0_Init();//�����Ļ�Ӱ�����أ����ԾͲ�����
//		TIM3_PWM0_Init();//��ӳ�񣬲�����ӳ������ؿ��г�ͻ���������ﲻ�ò�����ӳ��
//		TIM3_PWM2_Init();//��ȫ��ӳ��PC6,PC7,PC8,PC9

		/*   ��ʱ�����жϳ�ʼ��    *///tim1�Ķ�ʱ����׼��ԭ���������ԾͲ�����
//		TIMER2_INIT();
//		TIMER3_INIT();
//		TIMER4_INIT();
		TIMER5_INIT();
		TIMER6_INIT();//����adc10·����	
//		TIMER7_INIT();

		/*   �����жϳ�ʼ��    */
//		KEY_Configuration();	//�����жϳ�ʼ��



			ADCx_Init();//adc�ɼ�
			
//		Ultran_Init();//����ģ���ʼ��	

//		Encoder_Init_TIM2();            //��ʼ��������

		/*   6050��ʼ��    */
//		MPU6050_Init();//6050��ʼ��
//		delay_ms(20);
//		MPU6050_Init();

		/*   ���������ʼ��    */
//		Stepper_Motor_Init();//���������ʼ������
//		BAND_C_Init();//����������ٳ�ʼ��
//		Stepper_Init();//С���������ʼ��

//		INC_Pid_Init();//����ʽPID��ʼ��

//		NRF24L01_Init();    		//��ʼ��NRF24L01 
//		LCD_8x16var_int(0,4,34);
		delay_ms(1000);
//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART1�����ж� ��
		LED0=1;					//�ȵ������
		LED1=1;					//�ȵ������

		while(1)
		{
//			LED1 = !LED1;
			delay_ms(1000);
		}

			
}
/**************************************************************************
���������ģ��
		while(1)
		{
			LED1 = !LED1;
			Ultra_Ranging_use(&distance);
			printf("%f  \r\n",distance);
			delay_ms(1000);
		}
**************************************************************************/
/**************************************************************************
oled���Գ���
OLED_6x8var_ch(0,0,3);
OLED_6x8Str(2,2,"ok");//������ ������ Ҫ��ʾ���ַ�
**************************************************************************/
/**************************************************************************
beep���Գ���
	while(1)
	{
		BEEP = !BEEP;
		LED0 = !LED0;
		delay_ms(1000);
	}
**************************************************************************/
/**************************************************************************
���ڲ��Գ���
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
�������Գ���
	while(1)
	{
		key=KEY_Scan(0);	//�õ���ֵ
		if(key)
		{						   
			switch(key)
			{
				case KEY_1:	//ͬʱ����LED0,LED1��ת 
					LED0=!LED0;
				break;
				case KEY_2:	//���Ʒ�����
					LED1=!LED1;
				break;
				case KEY_3:	//����LED1��ת	 
					LED1=!LED1;
					LED0=!LED0;
				break;
				case KEY_4:	//ͬʱ����LED0,LED1��ת 
					LED0=!LED0;
				break;
				case KEY_5:	//���Ʒ�����
					LED1=!LED1;
				break;
				case KEY_6:	//����LED1��ת	 
					LED1=!LED1;
					LED0=!LED0;
				break;
			}
		}else delay_ms(10); 
	}
**************************************************************************/

/**********************************************************************
nrf���Գ���--�жϷ�ʽ--���Ͷ�
	while(NRF24L01_Check())
	{
		printf("check Error\r'n");
		delay_ms(400);
	}
	printf("success\r\n");
	
		while(1)
	{	
		delay_ms(200);
		NRF24L01_TX_Mode();		//����ģʽ
		if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				tmp_buf[0] = 1;
				tmp_buf[1] = 2;
				tmp_buf[2] = 3;
				tmp_buf[3] = 4;
				
				tmp_buf[32]=0;//���������	
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
//     NRF2401���Գ���   --ѭ����ʽ
//	while(NRF24L01_Check()!=SUCCESS)	//���NRF24L01�Ƿ���λ.	
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
		
/*   LCD���Գ���   */
//		LCD_ShowString(0,0,"100");
//		LCD_8x16var_int(30,0,800);
//		delay_ms(500);
//		LCD_8x16var_int(30,0,555);
//		delay_ms(500);
//		LCD_8x16var_int(30,0,-123);
//		delay_ms(500);

/*   pid���Գ���   */
//		INC_Pid_Ctrl(-20,Encoder_Left);
//		UART0_Oscilloscope(Encoder_Left,1);
//		printf("%d    ",Encoder_Left);

//stepper_low_speed_Move(1600,2000);//��������Դ�һ��

/*   С����������Գ���   */
//	for(i=0;i<4;i++)      //4��
//	 {
//		 GPIO_Write(GPIOA, F_Rotation[i]);  //�����Ӧ���� �������л��ɷ�ת���
//		 delay_ms(8);        //�ı�����������Ե������ת��
//	 }

//USART2_printf(" <G> OK");//�������Ժ���

//OLED���Գ���
// for(i=-10;i<10;i++)
//		{
//			OLED_6x8var_ch(0,0,i);
//			delay_ms(300);
//		}

//OLED_BMP(0,0,128,8,BMP1);//OLED��ʾͼƬ

// key_show();   //�����������Գ��� 

// Key_Value_Show();   //���󰴼����Գ���  

/*   MPU6050���Գ���   */
//	Angle_Calculate();
//	printf("x�Ƕ�%.2f   y�Ƕ�%.2f   z�Ƕ�%.2f   x���ٶ�%.2f    y���ٶ�%.2f      z���ٶ�%.2f  \r\n ",angle_x,angle_y,angle_z,w_x,w_y,w_z);

/*pwm�����Գ���
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
