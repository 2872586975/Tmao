#ifndef _RFD_H
#define _RFD_H




//定义一个clk的时间长(单位us)
#define RFD_CLK_SENDLEN		400

//定时器中断频率x(x个us中断一次)
#define RFD_INT_FRQ		50

#define RFDCLKEND	0xFFFF


 //引导码脉宽允许误差范围
#define  RFD_TITLE_CLK_MINL  20
#define  RFD_TITLE_CLK_MAXL  44

 //数据码脉宽允许误差范围
#define  RFD_DATA_CLK_MINL   2
#define  RFD_DATA_CLK_MAXL   5

//Rfd发送重复次数
#define RFD_TX_NUM		15

#define RFD_RX_PORT	GPIOA
#define RFD_RX_PIN	GPIO_Pin_11



#define RFD_NORMAL_DELDOUBLE_TIME  (T500MS+T50MS)




typedef enum
{
	RFDT_CLKSTEP0=0,
	RFDT_CLKSTEP1,            
	RFDT_CLKSTEP2,            
}RFD_SENDCLKTypeDef;

 
 typedef void (*RFD_RcvCallBack_t)(unsigned char *pBuff);

void hal_RFDInit(void);
void hal_RFDProc(void);
 

void RFD_SampTimerInterrupt_Handle(void);

void hal_RFCRcvCBSRegister(RFD_RcvCallBack_t pCBS);





#endif
