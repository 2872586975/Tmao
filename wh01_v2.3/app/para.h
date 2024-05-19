#ifndef _PARA_H
#define _PARA_H

#define PARA_DTC_SUM		20						//支持探测器的总数量

//探测器类型
typedef enum
{
	DTC_DOOR,		//门磁探测器
	DTC_PIR_MOTION,		//红外探测器
	DTC_REMOTE,			//遥控器
	DTC_TYP_SUM,
}DTC_TYPE_TYPEDEF;	//探测器类型设置值

//探测器防区类型
typedef enum
{
	ZONE_TYP_24HOURS,	//24小时警戒	
	ZONE_TYP_1ST,		//布防警戒
	ZONE_TYP_2ND,		//在家布放警戒
	STG_DEV_AT_SUM
}ZONE_TYPED_TYPEDEF;

//探测器属性
typedef struct
{
	unsigned char ID;								//设备ID
	unsigned char Mark;		 						//0-未学习 1-已学习
	unsigned char NameNum;							//名称序号
	unsigned char Name[16];							//设备名称
	DTC_TYPE_TYPEDEF DTCType;						//设备类型
	ZONE_TYPED_TYPEDEF ZoneType;						//防区类型

	unsigned char Code[3];					//1527/2262  24Bit数据
}Stu_DTC;

#define STU_DTC_SIZE	sizeof(Stu_DTC)

 



#define STU_SYSTEMPARA_SIZE	sizeof(SystemPara_InitTypeDef)	
#define STU_DTC_SIZE	sizeof(Stu_DTC)

#define STU_DEVICEPARA_OFFSET			0
 

void ParaInit(void);
void FactoryReset(void);
unsigned char DtcMatching(unsigned char *pCode);
unsigned char GetDtcNum(void);
void DelSpecifyDtc(Stu_DTC *pdPara);
unsigned char AddDtc(Stu_DTC *pDevPara);
unsigned char CheckPresenceofDtc(unsigned char i);
unsigned char GetDtcID(unsigned char idx);
void SetDtcAbt(unsigned char id,Stu_DTC *psDevPara);

void GetDtcStu(Stu_DTC *pdDevPara, unsigned char id);
#endif
