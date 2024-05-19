#ifndef _HAL_OLED_H
#define _HAL_OLED_H


void hal_OledInit(void);
void hal_Oled_Color_Turn(unsigned char i);
void hal_Oled_Display_Turn(unsigned char i);
void hal_Oled_Display_on(void);
void hal_Oled_Display_Off(void);
void hal_Oled_Refresh(void);
void hal_Oled_Clear(void);
static void hal_Oled_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void hal_Oled_DrawLine(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char mode);
void hal_Oled_DrawCircle(unsigned char x,unsigned char y,unsigned char r);
void hal_Oled_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1,unsigned char mode);
void hal_Oled_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char size1,unsigned char mode);
void hal_Oled_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size1,unsigned char mode);
void hal_Oled_ShowChinese(unsigned char x,unsigned char y,unsigned char num,unsigned char size1,unsigned char mode);
void hal_Oled_ScrollDisplay(unsigned char num,unsigned char space,unsigned char mode);
void hal_Oled_ShowPicture(unsigned char x,unsigned char y,unsigned char sizex,unsigned char sizey,unsigned char BMP[],unsigned char mode);
void hal_Oled_ClearArea(unsigned char x,unsigned char y,unsigned char sizex,unsigned char sizey);
extern unsigned char BMP1[];
extern unsigned char BMP_IconWifiSignal[4][32];
#endif
