# STM32_demo
使用的最小系统为stm32F103VET6，从机端用的是野火的板子

打开程序后需要确认所有的头文件已经包含，一些基本的全局变量全部打开
同时打开以下**基本操作**
```c
delay_init();
NVIC_Configuration();
LED_Init();
USART1_Config();
```

### 独立按键
init_key.c init_key.h
```c
KEY1	PA11
KEY2	PA12
KEY3	PA15
KEY4	PB3
KEY5	PB4
KEY6	PB5
```
打开KEY_Init();按照代码中的测试程序进行测试就行，如需改变按键的引脚，只需要改变init_key.h中的宏定义即可，注意一些引脚是不可以配置的。

### 按键中断
exti.c
```c
KEY		PE4
KEY_Configuration();
```
更改按键中断只需更改相应的中断线即可

### 串口
共定义了5个串口，这里用到了usart1，usart2，usart3
```c
115200		usart1  RX--PA10   TX--PA09
9600		usart2  RX--PA03   TX--PA02---用于蓝牙的接收
9600		usart3  RX--PB11   TX--PB10---用于zigbee收发
9600		uart4   RX--PC11   TX--PC10
9600		uart5   RX--PD02   TX--PC12 
```
使用USART1_printf();USART2_printf();USART3_printf();可以把相应的数据传送给串口助手。如果使用printf();则默认使用usart1来发送数据。

### Led & beep
```c
LED0 PE3（靠近下载口的那个）
LED1 PE2
BEEP PB5（需要接跳线帽）
更改引脚在.c和.h文件都要更改。
```
### OLED
```c
GND		GND		GND
VCC		VCC		3V3（2.8v-5.5v）
D0		SCL		PA5
D1		SDA		PA4
RES		RES		PA3
DC		DC		PA2
CS		无引脚
更改oled的引脚仅需更改OLED.h中的一些引脚。

//	OLED_6x8var_ch(0,0,3);
//	OLED_6x8Str(50,5,"ok");//横坐标  纵坐标   要显示的字符
```

### PWM波：使用的是定时器3，四路pwm波,部分重映像会导致下载出问题
```c
TIM2_PWM0_Init();无映像
CH1		PA0		// PA0不可用
CH2		PA1
CH3		PA2
CH4		PA3
TIM3_PWM0_Init();无映像
CH1		PA6
CH2		PA7
CH3		PB0
CH4		PB1
TIM3_PWM2_Init();完全重映像
CH1		PC6
CH2		PC7
CH3		PC8
CH4		PC9

CCR1_Val/TIM_Period=占空比，一般TIM_Period = 999 
```

### 定时器中断
```c
TIM1不准，这里不做考虑
TIMER2_INIT();
TIMER3_INIT();
TIMER4_INIT();
TIMER5_INIT();
TIMER6_INIT();
TIMER7_INIT();
```

### NRF24L01
中断接收方式：SPI1+NRF24L01
```c
GND		GND		VCC		3V3
CE		PB0		CSN		PC13
SCK		PA5		MOSI	PA7
MISO	PA6		IRQ		PA0
```
中断接收是放在TIMER3_INIT();定时器中断里面的，每次都要重新打开NRF24L01_RX_Mode();	这样才能接收到数据，
SPI引脚是在`spi.c`中设置的，NRF引脚是在24l01.h中设置的
轮询方式：
初始化之后一方发送，一方接手即可，引脚没有变化，一般用中断的方式更好吧。

### HC-05蓝牙模块：接收使用
```c
STATE	不接
RX		PA2
TX		PA3
GND		GND
5+		5V	
EN		不接
```
蓝牙使用的是USART2，一般情况下，蓝牙作为接收端使用，bluetooth_usart2的USART_BaudRate =  9600，使能usart2的接收中断(在Bluetooth.c)中，就可以在中断函数中将接收到的数据通过usart1发送到电脑终端，事先无需开启USART2_Config();

### Zigbee
接收：贴有绿色图标
Zigbee_usart3();用来中断接收。
发送：贴有红色图标
直接用USART3_printf();来发送数据
波特率9600		usart3  RX--PB11   TX--PB10---用于zigbee收发
Zigbee中用的是串口3中断，将接收到的数据由串口1发送到电脑串口助手，发送一个字符的时候BEEP尚可工作，发送两个字符的时候BEEP就不工作了。

### HC-SR04超声波
TRIG_Send-----PB5
ECHO_Reci-----PB6
超声波可以用定时器中断读取距离

