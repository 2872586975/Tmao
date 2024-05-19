#include "stm32F10x.h"
#include "hal_time.h"
#include "hal_beep.h"

#define BEEP_PORT			GPIOB
#define BEEP_PIN			GPIO_Pin_4

#define BEEP_EN_PORT	GPIOA
#define BEEP_EN_PIN	GPIO_Pin_0

#define BEEP_110_STEP_FREQ 58

//#define BP_FRQ1	500
//#define BP_FRQ2	BP_FRQ1+33
//#define BP_FRQ3	BP_FRQ2+33
//#define BP_FRQ4	BP_FRQ3+33
//#define BP_FRQ5	BP_FRQ4+33
//#define BP_FRQ6	BP_FRQ5+33
//#define BP_FRQ7	BP_FRQ6+33
//#define BP_FRQ8	BP_FRQ7+33
//#define BP_FRQ9	BP_FRQ8+33
//#define BP_FRQ10	BP_FRQ9+33
//#define BP_FRQ11	BP_FRQ10+33
//#define BP_FRQ12	BP_FRQ11+33
//#define BP_FRQ13	BP_FRQ12+33
//#define BP_FRQ14	BP_FRQ13+33
//#define BP_FRQ15	BP_FRQ14+33

#define BP_FRQ1	260
#define BP_FRQ2	BP_FRQ1+49
#define BP_FRQ3	BP_FRQ2+49
#define BP_FRQ4	BP_FRQ3+49
#define BP_FRQ5	BP_FRQ4+49
#define BP_FRQ6	BP_FRQ5+49
#define BP_FRQ7	BP_FRQ6+49
#define BP_FRQ8	BP_FRQ7+49
#define BP_FRQ9	BP_FRQ8+49
#define BP_FRQ10	BP_FRQ9+49
#define BP_FRQ11	BP_FRQ10+49
#define BP_FRQ12	BP_FRQ11+49
#define BP_FRQ13	BP_FRQ12+49
#define BP_FRQ14	BP_FRQ13+49
#define BP_FRQ15	BP_FRQ14+49

//unsigned short NoteFreqAry[12] = {955,901,851,803,758,715,675,637,601,568,536,506};
//unsigned short NoteFreqAry[12] = {253,268,284,300,318,337,357,379,401,425,450,477};

//unsigned short NoteFreqAry[2] = {253,268,284,300,318,337,357,379,401,425,450,477};
unsigned short NoteFreqAry[30] = {
		
		BP_FRQ1,BP_FRQ2,BP_FRQ3,BP_FRQ4,BP_FRQ5,BP_FRQ6,BP_FRQ7,BP_FRQ8,BP_FRQ9,BP_FRQ10,BP_FRQ11,BP_FRQ12,BP_FRQ13,BP_FRQ14,BP_FRQ15,
		BP_FRQ15,BP_FRQ14,BP_FRQ13,BP_FRQ12,BP_FRQ11,BP_FRQ10,BP_FRQ9,BP_FRQ8,BP_FRQ7,BP_FRQ6,BP_FRQ5,BP_FRQ4,BP_FRQ3,BP_FRQ2
};

static void hal_BeepConfig(void);
 
static void hal_BeepPwmHandle(void);


static void hal_BeepConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA , ENABLE); 	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��	PB4	
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; ; 
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BEEP_EN_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; ; 
	GPIO_Init(BEEP_EN_PORT, &GPIO_InitStructure);
	GPIO_SetBits(BEEP_EN_PORT,BEEP_EN_PIN);
	
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = 100; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =SystemCoreClock/1000000 - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	//����Ƚ�ͨ��1
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	
	
	
 
}


void hal_BeepInit(void)
{
	hal_BeepConfig();
	hal_CreatTimer(T_BEEP,hal_BeepPwmHandle,120,T_STA_START);
	hal_BeepPwmCtrl(1);
}

void hal_BeepProc(void)
{
	
}

void hal_BeepPwmCtrl(unsigned char cmd)
{
 
	if(cmd)
	{
		GPIO_ResetBits(BEEP_EN_PORT,BEEP_EN_PIN);
	}else
	{
		GPIO_SetBits(BEEP_EN_PORT,BEEP_EN_PIN);
	}

}

static void hal_BeepPwmHandle(void)
{
	static unsigned char i=0;
	//hal_SetBeepFreq(NoteFreqAry[i]);
	TIM_SetAutoreload(TIM3,NoteFreqAry[i]);
	TIM_SetCompare1(TIM3,NoteFreqAry[i]/2);
	TIM_SetCounter(TIM3,0);

	i++;
	if(i>28)
	{
		i=0;
	}
	
	hal_ResetTimer(T_BEEP,T_STA_START);
	 
}
 
