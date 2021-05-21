//
// Created by jiqiren on 2021/4/28.
//

#ifndef LCD_TEST_LCD1206_H
#define LCD_TEST_LCD1206_H
#include "main.h"

#define			LINE1		0x80 //第一行起始地址，下同
#define			LINE2 		0x90
#define			LINE3 		0x88
#define			LINE4 		0x98

//#define RS_Pin GPIO_PIN_1
//#define RS_GPIO_Port GPIOB
//#define RW_Pin GPIO_PIN_3
//#define RW_GPIO_Port GPIOB
//#define EN_Pin GPIO_PIN_5
//#define EN_GPIO_Port GPIOB
#define D_pin GPIOA

void GPIO_Init(void);
void LCD_Init(void);
void CheckBusy(void);
void LCD_wcmd(uint8_t cmd);
void LCD_wdat(uint8_t dat);
void LCD_Wmessage(uint8_t y,uint8_t x,uint8_t* message);
void Write_data(uint16_t temp);
#endif //LCD_TEST_LCD1206_H
