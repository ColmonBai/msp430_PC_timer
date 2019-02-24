#include <msp430x14x.h>
#include "Config.h"
int count = 0;
int temp = 0;
int Flag;
uchar key;
char timeBuff[10];
//*************************************************************************
//			初始化IO口子程序
//*************************************************************************
void Port_init()
{
    P4SEL = 0x00;
    P4DIR = 0xFF;                   //数据口输出模式
    P5SEL = 0x00;
    P5DIR|= BIT5 + BIT6 + BIT7;     //控制口设置为输出模式

    P1SEL = 0x00;                   //P1普通IO功能
    P1DIR = 0xF0;                   //P10~P13输入模式，外部电路已接上拉电阻
    P6SEL = 0x00;                   //P6口普通IO功能
    P6DIR = 0xFF;                   //P6口输出模式
}
//***********************************************************************
//	显示屏命令写入函数
//***********************************************************************
void LCD_write_com(unsigned char com)
{
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort = com;                 //命令写入端口
    delay_ms(5);
    EN_CLR;
}
//***********************************************************************
//	显示屏数据写入函数
//***********************************************************************
void LCD_write_data(unsigned char data)
{
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort = data;                //数据写入端口
    delay_ms(5);
    EN_CLR;
}
//***********************************************************************
//	显示屏清空显示
//***********************************************************************
void LCD_clear(void)
{
    LCD_write_com(0x01);            //清屏幕显示
    delay_ms(5);
}
//***********************************************************************
//	显示屏字符串写入函数
//***********************************************************************
void LCD_write_str(unsigned char x,unsigned char y,unsigned char *s)
{
    if (y == 0)
    {
        LCD_write_com(0x80 + x);        //第一行显示
    }
    else
    {
        LCD_write_com(0xC0 + x);        //第二行显示
    }
    while (*s)
    {
        LCD_write_data( *s);
        s ++;
    }
}
//***********************************************************************
//	显示屏单字符写入函数
//***********************************************************************
void LCD_write_char(unsigned char x,unsigned char y,unsigned char data)
{
    if (y == 0)
    {
        LCD_write_com(0x80 + x);        //第一行显示
    }
    else
    {
        LCD_write_com(0xC0 + x);        //第二行显示
    }
    LCD_write_data( data);
}
//***********************************************************************
//	显示屏初始化函数
//***********************************************************************
void LCD_init(void)
{
    LCD_write_com(0x38);		//显示模式设置
    delay_ms(5);
    LCD_write_com(0x08);		//显示关闭
    delay_ms(5);
    LCD_write_com(0x01);		//显示清屏
    delay_ms(5);
    LCD_write_com(0x06);		//显示光标移动设置
    delay_ms(5);
    LCD_write_com(0x0C);		//显示开及光标设置
    delay_ms(5);
}
//**********************************************************************
//	键盘扫描子程序，采用逐键扫描的方式
//**********************************************************************
uchar Key_Scan(void)
{
    uchar key_check;
    uchar key_checkin;
    key_checkin=KeyPort;          	    //读取IO口状态，判断是否有键按下
    key_checkin&= 0x0F;          		//读取IO口状态，判断是否有键按下
    if(key_checkin!=0x0F)
    {          	                        //IO口值发生变化则表示有键按下
        delay_ms(20);                  	//键盘消抖，延时20MS
        key_checkin=KeyPort;
        if(key_checkin!=0x1F)
        {
            key_check=KeyPort;          //获取按键状态
            switch (key_check & 0x0F)   //记录哪个键被按下
            {
                case 0x0E:key=1;break;
                case 0x0D:key=2;break;
                case 0x0B:key=3;break;
                case 0x07:key=4;break;
            }
        }
    }
    else
    {
        key=0xFF;
    }
    return key;
}
char * translate(int time)
{
    char sTime = time % 60;
    char mTime = time % 3600 / 60;
    char hTime = time / 3600;
    timeBuff[0] = (hTime / 10) + '0';
    timeBuff[1] = (hTime % 10) + '0';
    timeBuff[2] = ':';
    timeBuff[3] = (mTime / 10) + '0';
    timeBuff[4] = (mTime % 10) + '0';
    timeBuff[5] = ':';
    timeBuff[6] = (sTime / 10) + '0';
    timeBuff[7] = (sTime % 10) + '0';
    timeBuff[8] = '\0';
    return timeBuff;
}
void LEDPort_Init()
{
    LED8DIR  = 0xFF;                        //设置IO口方向为输出
    LED8 = 0xFF;                            //P6口初始设置为FF
}
//*************************************************************************
//               MSP430串口初始化
//*************************************************************************
void UART_Init()
{
    U0CTL|=SWRST + CHAR;                    //复位SWRST，8位数据模式
    U0TCTL|=SSEL1;                          //SMCLK为串口时钟
    U0BR1=baud_h;                           //BRCLK=8MHZ,Baud=BRCLK/N
    U0BR0=baud_l;                           //N=UBR+(UxMCTL)/8
    U0MCTL=0x00;                            //微调寄存器为0，波特率9600bps
    ME1|=UTXE0;                             //UART0发送使能
    ME1|=URXE0;                             //UART0接收使能
    U0CTL&=~SWRST;
    IE1|=URXIE0;                            //接收中断使能位
    P3SEL|= BIT4 + BIT5;                    //设置IO口为第二功能模式，启用UART功能
    P3DIR|= BIT4;                           //设置TXD0口方向为输出
}
//*************************************************************************
//              串口0发送数据函数
//*************************************************************************
void Send_Byte(uchar data)
{
    while(!(IFG1&UTXIFG0));                 //发送寄存器空的时候发送数据
    U0TXBUF=data;
}
//*************************************************************************
//              串口0发送字符串函数
//*************************************************************************
void Print_Str(uchar *s)
{
    while(*s != '\0')
    {
        Send_Byte(*s++);
    }
}
//*************************************************************************
//               处理来自串口0的接收中断
//*************************************************************************
#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
    int data=0;
    data=U0RXBUF;                       //接收到的数据存起来
    LED8 = data;                        //将收到的数据显示在8个LED灯上显示，仅限16进制
    data *= 60;
    temp = data;
    Send_Byte(data);                    //将接收到的数据再发送出去
}
void main(void)
{
    TACTL = TASSEL0 + TACLR;            // 定时器A设置为ACLK, 清除 TAR计数器
    CCTL0 = CCIE;                       //打开捕获比较模块1 的中断功能
    CCR0 = 4096;                        //设置比较值，中断时间：4096*1000/32768=125ms
    TACTL |= MC1;                       // Timer_连续计数
    _EINT();                            //开CPU总中断
    WDT_Init();                         //看门狗设置
    Clock_Init();                       //系统时钟设置
    Port_init();                        //系统初始化，设置IO口属性
    LCD_init();                         //液晶参数初始化设置
    LCD_clear();                        //清屏
    delay_ms(100);                      //延时100ms
    Flag = 1;
    UART_Init();                        //串口设置初始化
    LEDPort_Init();
    Print_Str("DM430-L Board UART Test...\n");             //发送字符串测试
    while(1)
    {
        if((temp > 0) && (temp%60 == 0))
        {
              LED8 &= 0x0f;
              delay_ms(100);
              LED8 |= 0xf0;
              delay_ms(100);
              LED8 &= 0x0f;
              delay_ms(100);
              LED8 |= 0xf0;
              delay_ms(100);
        }
        Key_Scan();                      //键盘扫描，看是否有按键按下
        if(key!=0xff)
        {                                //如果有按键按下，则显示该按键键值1～4
            switch(key)
            {
                case 1:LED8 = 0xFE;Flag = !Flag;break;     //给不同的键赋键值，键值1，亮2个LED灯，时钟暂停
                case 2:LED8 = 0xFD;temp = 0;break;         //给不同的键赋键值，键值2，亮2个LED灯，时钟清零
                case 3:LED8 = 0xFB;break;	           //给不同的键赋键值，键值3，亮2个LED灯
                case 4:LED8 = 0xF7;break;	           //给不同的键赋键值，键值4，亮2个LED灯
            }
        }
    }
}
//************************************************************************
// Timer A0中断服务程序
//************************************************************************
#pragma vector= TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
    CCR0 = CCR0+4096;                           //再次加入值，则4096以后中断---125ms  可以将最大误差减小到125ms
    if(Flag){                                   //Flag == 1表示暂停键未按下
        count++;                                //每次中断count++
        if(count == 8)                          //每次8次中断为一秒
        {
            temp++;
            count = 0;
        }
    }
    LCD_write_str(0,0,translate(temp));
}
