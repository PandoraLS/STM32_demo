/**
  ******************************************************************************
  * @file    nrf24l01.c
  * @author  xllj
  * @version V1.0
  * @date    2016-1-15
  * @brief   ��3.5.0�汾�⽨�Ĺ���ģ��
  ******************************************************************************
  * ʵ��ƽ̨:IPC9������ 
  ******************************************************************************
  */

#include "stm32f10x.h"                  // Device header
#include "nrf24l01.h"

u8 RX_BUF[RX_PLOAD_WIDTH];		//�������ݻ���
u8 TX_BUF[TX_PLOAD_WIDTH];		//�������ݻ���
u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // ����һ����̬���͵�ַ
u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};

static void NRF24L01_Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 


void NRF24L01_GPIOConfigure(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/*������ӦIO�˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB,ENABLE);

	/*���� SPI_NRF_SPI�� SCK,MISO,MOSI���ţ�GPIOA^5,GPIOA^6,GPIOA^7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���ù���
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	/*����SPI_NRF_SPI��CE����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*SPI_NRF_SPI�� CSN ����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*����SPI_NRF_SPI��IRQ����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

void NRF24L01_SPIConfigure(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	/*ʹ��SPI1ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 	//˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 					//��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 				//���ݴ�С8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 					//ʱ�Ӽ��ԣ�����ʱΪ��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//��1��������Ч��������Ϊ����ʱ��
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   					//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //8��Ƶ��9MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  				//��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);
}

/**
  * @brief   ������NRF��/дһ�ֽ�����
  * @param   д�������
  *		@arg dat 
  * @retval  ��ȡ�õ�����
  */
u8 NRF24L01_WriteReadByte(u8 dat)
{  	
   /* �� SPI���ͻ������ǿ�ʱ�ȴ� */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  
   /* ͨ�� SPI2����һ�ֽ����� */
  SPI_I2S_SendData(SPI1, dat);		
 
   /* ��SPI���ջ�����Ϊ��ʱ�ȴ� */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}

/************************NRF24L01*************************************
*********************************************************************/
/**
  * @brief   ������NRF�ض��ļĴ���д������
  * @param   
  *		@arg reg:NRF������+�Ĵ�����ַ
  *		@arg dat:��Ҫ��Ĵ���д�������
  * @retval  NRF��status�Ĵ�����״̬
  */
u8 NRF24L01_WriteReg(u8 reg,u8 dat)
{
	u8 status;
	NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
	NRF_CSN_LOW();
			
	/*��������Ĵ����� */
	status = NRF24L01_WriteReadByte(reg);
	 
	/*��Ĵ���д������*/
	NRF24L01_WriteReadByte(dat); 
		  
	/*CSN���ߣ����*/	   
	NRF_CSN_HIGH();	

	/*����״̬�Ĵ�����ֵ*/
	return(status);
}

/**
  * @brief   ���ڴ�NRF�ض��ļĴ�����������
  * @param   
  *		@arg reg:NRF������+�Ĵ�����ַ
  * @retval  �Ĵ����е�����
  */
u8 NRF24L01_ReadReg(u8 reg)
{
 	u8 reg_val;

	NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
 	NRF_CSN_LOW();
				
  	 /*���ͼĴ�����*/
	NRF24L01_WriteReadByte(reg); 

	 /*��ȡ�Ĵ�����ֵ */
	reg_val = NRF24L01_WriteReadByte(NOP);
	            
   	/*CSN���ߣ����*/
	NRF_CSN_HIGH();		
   	
	return reg_val;
}	

/**
  * @brief   ������NRF�ļĴ�����д��һ������
  * @param   
  *		@arg reg : NRF������+�Ĵ�����ַ
  *		@arg pBuf�����ڴ洢���������ļĴ������ݵ����飬�ⲿ����
  * 	@arg bytes: pBuf�����ݳ���
  * @retval  NRF��status�Ĵ�����״̬
  */
u8 NRF24L01_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
	u8 status, byte_cnt;

	NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
	NRF_CSN_LOW();

	/*���ͼĴ�����*/		
	status = NRF24L01_WriteReadByte(reg); 

	/*��ȡ����������*/
	for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
	{	
		pBuf[byte_cnt] = NRF24L01_WriteReadByte(NOP); //��NRF24L01��ȡ����  
	}

	/*CSN���ߣ����*/
	NRF_CSN_HIGH();	

	return status;		//���ؼĴ���״ֵ̬
}

/**
  * @brief   ������NRF�ļĴ�����д��һ������
  * @param   
  *		@arg reg : NRF������+�Ĵ�����ַ
  *		@arg pBuf���洢�˽�Ҫд��д�Ĵ������ݵ����飬�ⲿ����
  * 	@arg bytes: pBuf�����ݳ���
  * @retval  NRF��status�Ĵ�����״̬
  */
u8 NRF24L01_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
	u8 status,byte_cnt;
	NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
	NRF_CSN_LOW();			

	/*���ͼĴ�����*/	
	status = NRF24L01_WriteReadByte(reg); 

	/*�򻺳���д������*/
	for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
	{
		NRF24L01_WriteReadByte(*pBuf++);	//д���ݵ������� 	 
	}

	/*CSN���ߣ����*/
	NRF_CSN_HIGH();			

	return (status);	//����NRF24L01��״̬ 		
}

/**
  * @brief  ���ò��������ģʽ
  * @param  ��
  * @retval ��
  */
void NRF24L01_RxMode(void)
{
	NRF_CE_LOW();	

	NRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ

	NRF24L01_WriteReg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    

	NRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ    

	NRF24L01_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);      //����RFͨ��Ƶ��    

	NRF24L01_WriteReg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��      

	NRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��   

	NRF24L01_WriteReg(NRF_WRITE_REG+CONFIG1, 0x0f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 

	/*CE���ߣ��������ģʽ*/	
	NRF_CE_HIGH();
}



/**
  * @brief  ���÷���ģʽ
  * @param  ��
  * @retval ��
  */
void NRF24L01_TxMode(void)
{  
	NRF_CE_LOW();		

	NRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 

	NRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK   

	NRF24L01_WriteReg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    

	NRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  

	NRF24L01_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��

	NRF24L01_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);       //����RFͨ��ΪCHANAL

	NRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   

	NRF24L01_WriteReg(NRF_WRITE_REG+CONFIG1,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�

	/*CE���ߣ����뷢��ģʽ*/	
	NRF_CE_HIGH();
	NRF24L01_Delay(0xffff); //CEҪ����һ��ʱ��Ž��뷢��ģʽ
}

/**
  * @brief  ��Ҫ����NRF��MCU�Ƿ���������
  * @param  ��
  * @retval SUCCESS/ERROR ��������/����ʧ��
  */
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 

	/*д��5���ֽڵĵ�ַ.  */  
	NRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*����д��ĵ�ַ */
	NRF24L01_ReadBuf(TX_ADDR,buf1,5); 

	/*�Ƚ�*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		{
			break;
		}
	} 
	   
	if(i==5)
	{
		return SUCCESS ;        //MCU��NRF�ɹ����� 
	}
	else
	{
		return ERROR ;        //MCU��NRF����������
	}
}

/**
  * @brief   ������NRF�ķ��ͻ�������д������
  * @param   
  *		@arg txBuf���洢�˽�Ҫ���͵����ݵ����飬�ⲿ����	
  * @retval  ���ͽ�����ɹ�����TXDS,ʧ�ܷ���MAXRT��ERROR
  */
u8 NRF24L01_SendData(u8 *txbuf)
{
	u8 state;  

	/*ceΪ�ͣ��������ģʽ1*/
	NRF_CE_LOW();

	/*д���ݵ�TX BUF ��� 32���ֽ�*/						
	NRF24L01_WriteBuf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);

	/*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */   
	NRF_CE_HIGH();

	/*�ȴ���������ж� */                            
	while(NRF_Read_IRQ()!=0); 	

	/*��ȡ״̬�Ĵ�����ֵ */                              
	state = NRF24L01_ReadReg(STATUS);

	/*���TX_DS��MAX_RT�жϱ�־*/                  
	NRF24L01_WriteReg(NRF_WRITE_REG+STATUS,state); 	

	NRF24L01_WriteReg(FLUSH_TX,NOP);    //���TX FIFO�Ĵ��� 

	/*�ж��ж�����*/    
	if(state&MAX_RT)                     //�ﵽ����ط�����
	{
		return MAX_RT; 
	}
	else if(state&TX_DS)                  //�������
	{
		return TX_DS;
	}
	else	
	{		
		return ERROR;                 //����ԭ����ʧ��
	}
}

/**
  * @brief   ���ڴ�NRF�Ľ��ջ������ж�������
  * @param   
  *		@arg rxBuf �����ڽ��ո����ݵ����飬�ⲿ����	
  * @retval 
  *		@arg ���ս��
  */
u8 NRF24L01_ReceiveData(u8 *rxbuf)
{
	u8 state; 
	NRF_CE_HIGH();	 //�������״̬
	 /*�ȴ������ж�*/
	while(NRF_Read_IRQ()!=0); 
	
	NRF_CE_LOW();  	 //�������״̬
	/*��ȡstatus�Ĵ�����ֵ  */               
	state=NRF24L01_ReadReg(STATUS);
	 
	/* ����жϱ�־*/      
	NRF24L01_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*�ж��Ƿ���յ�����*/
	if(state&RX_DR)                                 //���յ�����
	{
		NRF24L01_ReadBuf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_WriteReg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ���
		return RX_DR; 
	}
	else  
	{		
		return ERROR;                    //û�յ��κ�����
	}
}

void NRF24L01_SPIInit(void)
{
	NRF24L01_GPIOConfigure();
	NRF24L01_SPIConfigure();
}
