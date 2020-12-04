/******************** (C) COPYRIGHT 2015 ****************************************
 * �ļ���  : oled.c
 * ����    ����ʾ�������Ͻ�Ϊ(0,0),x��ʾ������ʾ��Χ(0~120),y��ʾ������ʾ��Χ(0~7)
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ������
---------------------  
|  PA.05--------SCL   |
|  PA.07--------SDA   |
|  PA.02--------RST   | 
|  PA.03--------DC    |
--------------------- 
 * ��汾  ��ST3.0.0
 *
 * ����    �����  QQ: 116****038
**********************************************************************************/

/************************************************************************************
* Description:128*64�����OLED��ʾ�������ļ����������ڻ����Զ���(heltec.taobao.com)��SD1306����SPIͨ�ŷ�ʽ��ʾ��
* Function List:
*
* 2. void OLED_WrDat(unsigned char dat) -- ��OLEDд����
* 3. void OLED_WrCmd(unsigned char cmd) -- ��OLEDд����
* 4. void OLED_SetPos(unsigned char x, unsigned char y) -- ������ʼ������
* 5. void OLED_Fill(unsigned char bmp_dat) -- ȫ�����(0x00��������������0xff��������ȫ������)
* 6. void OLED_CLS(void) -- ����
* 7. void OLED_Init(void) -- OLED��ʾ����ʼ��
* 8. void OLED_6x8Str(unsigned char x, y,unsigned char ch[]) -- ��ʾ6x8��ASCII�ַ�
* 9. void OLED_8x16Str(unsigned char x, y,unsigned char ch[]) -- ��ʾ8x16��ASCII�ַ�
* 10.void OLED_16x16CN(unsigned char x, y, N) -- ��ʾ16x16���ĺ���,����Ҫ����ȡģ�����ȡģ
* 11.void OLED_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- ȫ����ʾ128*64��BMPͼƬ
* 12.void OLED_6x8var_ch(unsigned char x, unsigned char y, int N)--N����ʾ��ΧΪ-32768~32767(����)
*************************************************************************************/

#include "OLED.h"
#include "delay.h"
#include "codetab.h"	//�ֿ�
#define STR_WIDTH		6		/* �ַ���� */

/*
 * ��������OLED_WrDat(unsigned char dat)
 * ����  ����OLEDд����
 * ����  ��dat
 * ���  ����
 * ����  ���ڲ�����
 */
void OLED_WrDat(unsigned char dat)
{
	unsigned char i;
	OLED_DC_Set();
	for(i=0;i<8;i++)
	{
		if((dat << i) & 0x80)
		{
			OLED_SDA_Set();
		}
		else
		OLED_SDA_Clr();
		OLED_SCL_Clr();
		OLED_SCL_Set();
	}
}

/*
 * ��������OLED_WrCmd(unsigned char cmd)
 * ����  ����OLEDд����
 * ����  ��dat
 * ���  ����
 * ����  ���ڲ�����
 */
void OLED_WrCmd(unsigned char cmd)
{
	unsigned char i;
	OLED_DC_Clr();
	for(i=0;i<8;i++) //����һ����λ����
	{
		if((cmd << i) & 0x80)
		{
			OLED_SDA_Set();
		}
		else
		{
			OLED_SDA_Clr();
		}
		OLED_SCL_Clr();
		OLED_SCL_Set();
	}
}

/*
 * ��������void OLED_SetPos(unsigned char x, unsigned char y)
 * ����  ����OLED��������ʼ������
 * ����  ��x��ʾoled�����꣺0-120
           y��ʾoled�����꣺0-7
 * ���  ����
 * ����  ���⡢�ڲ�����
 */
void OLED_SetPos(unsigned char x, unsigned char y)
{
	OLED_WrCmd(0xb0 + y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
}

/*
 * ��������void OLED_Fill(unsigned char bmp_dat)
 * ����  ����OLED��ȫ�����
 * ����  ��bmp_dat(0x00��������������0xff��������ȫ������)
 * ���  ����
 * ����  ���ڲ�����
 */
void OLED_Fill(unsigned char bmp_dat)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		{
			OLED_WrDat(bmp_dat);
		}
	}
}

/*
 * ��������void OLED_CLS(void)
 * ����  ����OLED����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void OLED_CLS(void)
{
	OLED_Fill(0x00);
}

/*
 * ��������void OLED_Init(void)
 * ����  ����oled��ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX, ENABLE);	 //ʹ�ܶ˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CLK|GPIO_Pin_MOSI|GPIO_Pin_CS|GPIO_Pin_DC;	 //IO�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOX, &GPIO_InitStructure);	  //��ʼ��
 	GPIO_SetBits(GPIOX,GPIO_Pin_CLK|GPIO_Pin_MOSI|GPIO_Pin_CS|GPIO_Pin_DC);	//�����
  
	OLED_CS_Clr();
	delay_ms(100);	//��ʱ��ʼ��
	OLED_CS_Set();
	
	OLED_WrCmd(0xae);
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0,0xa1
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0,0xc8
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);
	OLED_SetPos(0,0);
}

/*
 * ��������void OLED_6x8Str(unsigned char x, unsigned char y, unsigned char ch[])
 * ����  ����OLED����ʾ�ַ���(�ַ���СΪ6*8)
 * ����  ��x��ʾoled�����꣺0-120
           y��ʾoled�����꣺0-7
           ch��ʾΪ�ַ������顣��ch[]={"nifeng"};
 * ���  ����
 * ����  ���ⲿ����
 */
void OLED_6x8Str(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c = ch[j]-32;
		if(x>126)
		{
			x=0;y++;
		}
		OLED_SetPos(x,y);
		for(i=0;i<6;i++)
		{
			OLED_WrDat(F6x8[c][i]);
		}
		x+=6;
		j++;
	}
}
/*
 * ��������void OLED_8x16Str(unsigned char x, unsigned char y, unsigned char ch[])
 * ����  ����OLED����ʾ�ַ���(�ַ���СΪ8*16)
 * ����  ��x��ʾoled�����꣺0-120
           y��ʾoled�����꣺0-6
           ch��ʾΪ�ַ������顣��ch[]={"nifeng"};
 * ���  ����
 * ����  ���ⲿ����
 */
void OLED_8x16Str(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120)
		{
			x=0;y++;
		}
		OLED_SetPos(x,y);
		for(i=0;i<8;i++)
		{
			OLED_WrDat(F8X16[c*16+i]);
		}
		OLED_SetPos(x,y+1);
		for(i=0;i<8;i++)
		{
			OLED_WrDat(F8X16[c*16+i+8]);
		}
		x+=8;
		j++;
	}
}

/*
 * ��������void OLED_P14x16Str(u8 x,u8 y,u8 ch[])
 * ����  ����OLED����ʾ�ַ���(�ַ���СΪ14x16)
 * ����  ��x��ʾoled�����꣺0-120
           y��ʾoled�����꣺0-6
           ch��ʾΪ�ַ������顣��ch[]={"nifeng"};
 * ���  ����
 * ����  ���ⲿ����
 */
void OLED_P14x16Str(u8 x,u8 y,u8 ch[])
{
	u8 wm=0,ii = 0;
	u16 adder=1; 
	
	while(ch[ii] != '\0')
	{
  	wm = 0;
  	adder = 1;
  	while(F14x16_Idx[wm] > 127)
  	{
  		if(F14x16_Idx[wm] == ch[ii])
  		{
  			if(F14x16_Idx[wm + 1] == ch[ii + 1])
  			{
  				adder = wm * 14;
  				break;
  			}
  		}
  		wm += 2;			
  	}
  	if(x>118){x=0;y++;}
  	OLED_SetPos(x , y); 
  	if(adder != 1)// ????					
  	{
  		OLED_SetPos(x , y);
  		for(wm = 0;wm < 14;wm++)               
  		{
  			OLED_WrDat(F14x16[adder]);	
  			adder += 1;
  		}      
  		OLED_SetPos(x,y + 1); 
  		for(wm = 0;wm < 14;wm++)          
  		{
  			OLED_WrDat(F14x16[adder]);
  			adder += 1;
  		}   		
  	}
  	else			  //??????			
  	{
  		ii += 1;
      OLED_SetPos(x,y);
  		for(wm = 0;wm < 16;wm++)
  		{
  				OLED_WrDat(0);
  		}
  		OLED_SetPos(x,y + 1);
  		for(wm = 0;wm < 16;wm++)
  		{   		
  				OLED_WrDat(0);	
  		}
  	}
  	x += 14;
  	ii += 2;
	}
} 

/*
 * ��������void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N)
 * ����  ����OLED����ʾ16x16���ĺ���,����Ҫ����ȡģ�����ȡģ
 * ����  ��x��ʾoled�����꣺0-110
           y��ʾoled�����꣺0-6
           N��ʾΪ�����������Ҫ��ʾ�����ڵڼ�λ��
 * ���  ����
 * ����  ���ⲿ����
 */
void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
}

/*
 * ��������void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
 * ����  ����OLED��ȫ����ʾ128*64��BMPͼƬ
 * ����  ��x��ʾoled�����꣺0-127
           y��ʾoled�����꣺0-7
           BMP[]ΪͼƬ������
 * �÷�  ��OLED_BMP(0, 0, 127, 7,BMP1);
 * ���  ����
 * ����  ���ⲿ����
 */

void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, const u8 * BMP)
{
	unsigned int j=0;
	unsigned char x,y;
	
  if(y1%8==0)
	{
		y=y1/8;
	}
  else
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			OLED_WrDat(BMP[j++]);
		}
	}
}



/*
 * ��������void OLED_var(u8 x,u8 y,unsigned int p)
 * ����  ����OLED��ȫ����ʾ0~9���ַ�(16*16)
 * ����  ��x��ʾoled�����꣺0-120
           y��ʾoled�����꣺0-6
               P��ʾ�ķ�Χ��0~9
 * ���  ����
 * ����  ���ڲ�����
 */
void OLED_var(u8 x,u8 y,unsigned int p)
{
	switch(p)
	{
		case 0:OLED_8x16Str(x,y,"0");break;
		case 1:OLED_8x16Str(x,y,"1");break;
		case 2:OLED_8x16Str(x,y,"2");break;
		case 3:OLED_8x16Str(x,y,"3");break;
		case 4:OLED_8x16Str(x,y,"4");break;
		case 5:OLED_8x16Str(x,y,"5");break;
		case 6:OLED_8x16Str(x,y,"6");break;
		case 7:OLED_8x16Str(x,y,"7");break;
		case 8:OLED_8x16Str(x,y,"8");break;
		case 9:OLED_8x16Str(x,y,"9");break;
	}
}	

/*
 * ��������void OLED_8x16var(unsigned char x, unsigned char y, u32 N)
 * ����  ����OLED��ȫ����ʾ������
 * ����  ��x��ʾoled�����꣺0-80
           y��ʾoled�����꣺0-6
               N��ʾ�ķ�Χ��0~99999
 * ���  ����
 * ����  ���ⲿ����
 */
void OLED_8x16var(unsigned char x, unsigned char y, u32 N)
{
	unsigned int ge = 0;
	unsigned int shi = 0;
	unsigned int bai = 0;
	unsigned int qian = 0;
	unsigned int wan = 0;
	ge = N%10;
	shi = N/10%10;
	bai = (N/100)%10;
	qian = (N/1000)%10;
	wan = (N/10000)%10;
	OLED_var(x+32,y,ge);
	OLED_var(x+24,y,shi);
	OLED_var(x+16,y,bai);
	OLED_var(x+8,y,qian);
	OLED_var(x,y,wan);
}

/*
 * ��������void OLED_var1(u8 x,u8 y,unsigned int p)
 * ����  ����OLED��ȫ����ʾ0~9���ַ�(8*8)
 * ����  ��x��ʾoled�����꣺0-120
           y��ʾoled�����꣺0-7
               P��ʾ�ķ�Χ��0~9
 * ���  ����
 * ����  ���ڲ�����
 */
void OLED_var1(u8 x,u8 y,unsigned int p)
{
	switch(p)
	{
		case 0:OLED_6x8Str(x,y,"0");break;
		case 1:OLED_6x8Str(x,y,"1");break;
		case 2:OLED_6x8Str(x,y,"2");break;
		case 3:OLED_6x8Str(x,y,"3");break;
		case 4:OLED_6x8Str(x,y,"4");break;
		case 5:OLED_6x8Str(x,y,"5");break;
		case 6:OLED_6x8Str(x,y,"6");break;
		case 7:OLED_6x8Str(x,y,"7");break;
		case 8:OLED_6x8Str(x,y,"8");break;
		case 9:OLED_6x8Str(x,y,"9");break;
	}
}	

/*
 * ��������void OLED_6x8var(unsigned char x, unsigned char y, u32 N)
 * ����  ����OLED��ȫ����ʾ������(8*8)
 * ����  ��x��ʾoled�����꣺0-100
           y��ʾoled�����꣺0-7
               N��ʾ�ķ�Χ��0~9999
 * ���  ����
 * ����  ���ⲿ����
 */
void OLED_6x8var(unsigned char x, unsigned char y, u32 N)
{
	unsigned int ge = 0;
	unsigned int shi = 0;
	unsigned int bai = 0;
	unsigned int qian = 0;
	ge = N%10;
	shi = N/10%10;
	bai = (N/100)%10;
	qian = (N/1000)%10;
	OLED_var1(x+18,y,ge);
	OLED_var1(x+12,y,shi);
	OLED_var1(x+6,y,bai);
	OLED_var1(x,y,qian);
}

/*
 * ��������void OLED_6x8var_ch(unsigned char x, unsigned char y, int N)
 * ����  ����OLED��ȫ������Ϊ�������������(8*8),�������λ����ʾ����λ
 * ����  ��x��ʾoled�����꣺0-120-N��λ��*10
           y��ʾoled�����꣺0-7
           N��ʾ�ķ�Χ��-32768~32767
 * ���  ����
 * ����  ���ⲿ����
 */
void OLED_6x8var_ch(unsigned char x, unsigned char y, int N)
{
	u32 length = 0, temp = 0;
	unsigned char i;
	if(N==0)
	{
		temp = 0;
		length =1;
		OLED_SetPos(x,y);//�����������
		for(i=STR_WIDTH*(length);i<STR_WIDTH*10;i++)//���ˢ����������
		{
			OLED_WrDat(0x00);
		}
		OLED_var1(x, y, (temp%10));
	}
	else if(N<0)
	{
		temp = 0-N;
		while( temp )// �õ�num�ĳ���
		{
			temp /= 10;
			length ++;
		}
		OLED_SetPos(x,y);
		for(i=STR_WIDTH*(length+1);i<STR_WIDTH*11;i++)
		{
			OLED_WrDat(0x00);
		}
		temp = 0-N;
		OLED_6x8Str((x),y,"-");
		while( temp )
		{
			/* �Ӹ�λ��ʼ��ʾ */
			OLED_var1((x+STR_WIDTH*(length--)), y, (temp%10));
			temp/= 10;
		}
	}
	else
	{
		temp = N;
    if( temp == 0 )
		{
			OLED_var1(x+6,y,0);
			return;
		}
		while( temp )// �õ�num�ĳ���
		{
			temp /= 10;
			length ++;
		}
		OLED_SetPos(x,y);
		for(i=STR_WIDTH*(length);i<STR_WIDTH*10;i++)
		{
			OLED_WrDat(0x00);
		}
		while( N )
		{
			/* �Ӹ�λ��ʼ��ʾ */
			OLED_var1((x+STR_WIDTH*(length--)-STR_WIDTH), y, (N%10));
			N /= 10;
		}		
		
  }
}
/******************* (C) COPYRIGHT 2015 **********************************END OF FILE*****/


