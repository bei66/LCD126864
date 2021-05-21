//
// Created by jiqiren on 2021/4/28.
//

#include "lcd1206.h"
//
// Created by jiqiren on 2021/4/28.
//

uint16_t temp;


//延时
void delay(uint32_t m)
{
    uint32_t i,j;
    for(i=0;i<m;i++)
        for(j=0;j<10;j++);
}
/**
  * @brief （弃用）引脚由cubemx定义
  * @param None
  * @retval None
  */
void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
//    HAL_GPIO_WritePin(D_pin, D0_Pin|D1_Pin|D2_Pin|D3_Pin
//                             |D4_Pin|D5_Pin|D6_Pin|D7_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, RS_Pin|RW_Pin|EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : PA0 PA1 PA2 PA3
                             PA4 PA5 PA6 PA7 */
//    GPIO_InitStruct.Pin = D0_Pin|D1_Pin|D2_Pin|D3_Pin
//                          |D4_Pin|D5_Pin|D6_Pin|D7_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(D_pin, &GPIO_InitStruct);

    /*Configure GPIO pins : RS_Pin RW_Pin EN_Pin */
    GPIO_InitStruct.Pin = RS_Pin|RW_Pin|EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
//LCD初始化
void LCD_Init(void)
{
    LCD_wcmd(0x30);//功能设定：基本指令集
    HAL_Delay(10);
    //LCD_wcmd(0x08);
    //HAL_Delay(10);
    //LCD_wcmd(0x10);
    //HAL_Delay(10);
    LCD_wcmd(0x0C);//显示开，关光标
    HAL_Delay(10);
    LCD_wcmd(0x01);//清除显示
    HAL_Delay(10);
    //LCD_wcmd(0x06);
    //HAL_Delay(10);
}
//忙判断
void CheckBusy(void)
{
    uint8_t status;
    HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_SET);	//读出数据，RW=1
    HAL_GPIO_WritePin(D_pin,D0_Pin|D1_Pin|D2_Pin|D3_Pin|D4_Pin|D5_Pin|D6_Pin|D7_Pin,GPIO_PIN_SET);
    do
    {
        HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);
        HAL_Delay(5);
        status = HAL_GPIO_ReadPin(D_pin, D7_Pin);//接收BF位，判断是否忙
        HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);
    }while(status & 0x80);

}
//LCD写命令
void LCD_wcmd(uint8_t cmd)
{
    CheckBusy();
    HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_RESET);	//读出数据，RW=0
    HAL_Delay(5);
    temp=(temp&0xff00)|cmd;//temp的低八位清零后将cmd放进去
    Write_data(temp);
    //GPIO_Write(D_pin,temp);//通过GPIO_Write（）函数将数据发到A端口（也就是LCD的数据口）
    HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);//使能位开启
    HAL_Delay(10);//10ms应该能发送完了
    HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);//使能位关闭
}
//LCD写数据
void LCD_wdat(uint8_t dat)
{
    CheckBusy();
    HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_RESET);
    HAL_Delay(5);
    temp=(temp&0xff00)|dat; //temp的低八位清零后将dat放进去
    Write_data(temp);
    //GPIO_Write(D_pin,temp);//通过GPIO_Write（）函数将数据发到A端口（也就是LCD的数据口）
    HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);//使能位开启
    HAL_Delay(10);//10ms应该能发送完了
    HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);//使能位关闭
}
//向LCD12864中写入一行数据（因为你不可能每次只发送一字节数据）
void LCD_Wmessage(uint8_t y,uint8_t x,uint8_t* message)
{
    switch(y)
    {
        case 1:LCD_wcmd(0x7f+x);break;
        case 2:LCD_wcmd(0x8f+x);break;
        case 3:LCD_wcmd(0x87+x);break;
        case 4:LCD_wcmd(0x97+x);break;

    }
    //LCD_wcmd(address);//要显示的位置，你想让内容显示在LCD的哪一行，就把该行的起始地址通过写命令的方式发送出去
    while(*message>0)//这个判断很关键，判断你的内容有没有发完
    {
        LCD_wdat(*message); //内核还是发字节函数
        message++; //指针挺好用的。。
    }
}

void  Write_data(uint16_t temp)
{
    /* 高8位值判断 */
    if((temp&0X0080) == 0X0080)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D7_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D7_Pin,GPIO_PIN_RESET);
    }
    /* 高7位值判断 */
    if((temp&0X0040) == 0X0040)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D6_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D6_Pin,GPIO_PIN_RESET);
    }
    /* 高6位值判断 */
    if((temp&0X0020) == 0X0020)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D5_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D5_Pin,GPIO_PIN_RESET);
    }
    /* 高5位值判断 */
    if((temp&0X0010) == 0X0010)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D4_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D4_Pin,GPIO_PIN_RESET);
    }
    /* 高4位值判断 */
    if((temp&0X0008) == 0X0008)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D3_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D3_Pin,GPIO_PIN_RESET);
    }
    /* 高3位值判断 */
    if((temp&0X0004) == 0X0004)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D2_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D2_Pin,GPIO_PIN_RESET);
    }
    /* 高2位值判断 */
    if((temp&0X0002) == 0X0002)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D1_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D1_Pin,GPIO_PIN_RESET);
    }
    /* 高1位值判断 */
    if((temp&0X0001) == 0X0001)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D0_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D0_Pin,GPIO_PIN_RESET);
    }


}



