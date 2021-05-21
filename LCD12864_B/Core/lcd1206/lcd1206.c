//
// Created by jiqiren on 2021/4/28.
//

#include "lcd1206.h"
//
// Created by jiqiren on 2021/4/28.
//

uint16_t temp;


//��ʱ
void delay(uint32_t m)
{
    uint32_t i,j;
    for(i=0;i<m;i++)
        for(j=0;j<10;j++);
}
/**
  * @brief �����ã�������cubemx����
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
//LCD��ʼ��
void LCD_Init(void)
{
    LCD_wcmd(0x30);//�����趨������ָ�
    HAL_Delay(10);
    //LCD_wcmd(0x08);
    //HAL_Delay(10);
    //LCD_wcmd(0x10);
    //HAL_Delay(10);
    LCD_wcmd(0x0C);//��ʾ�����ع��
    HAL_Delay(10);
    LCD_wcmd(0x01);//�����ʾ
    HAL_Delay(10);
    //LCD_wcmd(0x06);
    //HAL_Delay(10);
}
//æ�ж�
void CheckBusy(void)
{
    uint8_t status;
    HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_SET);	//�������ݣ�RW=1
    HAL_GPIO_WritePin(D_pin,D0_Pin|D1_Pin|D2_Pin|D3_Pin|D4_Pin|D5_Pin|D6_Pin|D7_Pin,GPIO_PIN_SET);
    do
    {
        HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);
        HAL_Delay(5);
        status = HAL_GPIO_ReadPin(D_pin, D7_Pin);//����BFλ���ж��Ƿ�æ
        HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);
    }while(status & 0x80);

}
//LCDд����
void LCD_wcmd(uint8_t cmd)
{
    CheckBusy();
    HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_RESET);	//�������ݣ�RW=0
    HAL_Delay(5);
    temp=(temp&0xff00)|cmd;//temp�ĵͰ�λ�����cmd�Ž�ȥ
    Write_data(temp);
    //GPIO_Write(D_pin,temp);//ͨ��GPIO_Write�������������ݷ���A�˿ڣ�Ҳ����LCD�����ݿڣ�
    HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);//ʹ��λ����
    HAL_Delay(10);//10msӦ���ܷ�������
    HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);//ʹ��λ�ر�
}
//LCDд����
void LCD_wdat(uint8_t dat)
{
    CheckBusy();
    HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_RESET);
    HAL_Delay(5);
    temp=(temp&0xff00)|dat; //temp�ĵͰ�λ�����dat�Ž�ȥ
    Write_data(temp);
    //GPIO_Write(D_pin,temp);//ͨ��GPIO_Write�������������ݷ���A�˿ڣ�Ҳ����LCD�����ݿڣ�
    HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);//ʹ��λ����
    HAL_Delay(10);//10msӦ���ܷ�������
    HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);//ʹ��λ�ر�
}
//��LCD12864��д��һ�����ݣ���Ϊ�㲻����ÿ��ֻ����һ�ֽ����ݣ�
void LCD_Wmessage(uint8_t y,uint8_t x,uint8_t* message)
{
    switch(y)
    {
        case 1:LCD_wcmd(0x7f+x);break;
        case 2:LCD_wcmd(0x8f+x);break;
        case 3:LCD_wcmd(0x87+x);break;
        case 4:LCD_wcmd(0x97+x);break;

    }
    //LCD_wcmd(address);//Ҫ��ʾ��λ�ã�������������ʾ��LCD����һ�У��ͰѸ��е���ʼ��ַͨ��д����ķ�ʽ���ͳ�ȥ
    while(*message>0)//����жϺܹؼ����ж����������û�з���
    {
        LCD_wdat(*message); //�ں˻��Ƿ��ֽں���
        message++; //ָ��ͦ���õġ���
    }
}

void  Write_data(uint16_t temp)
{
    /* ��8λֵ�ж� */
    if((temp&0X0080) == 0X0080)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D7_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D7_Pin,GPIO_PIN_RESET);
    }
    /* ��7λֵ�ж� */
    if((temp&0X0040) == 0X0040)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D6_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D6_Pin,GPIO_PIN_RESET);
    }
    /* ��6λֵ�ж� */
    if((temp&0X0020) == 0X0020)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D5_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D5_Pin,GPIO_PIN_RESET);
    }
    /* ��5λֵ�ж� */
    if((temp&0X0010) == 0X0010)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D4_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D4_Pin,GPIO_PIN_RESET);
    }
    /* ��4λֵ�ж� */
    if((temp&0X0008) == 0X0008)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D3_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D3_Pin,GPIO_PIN_RESET);
    }
    /* ��3λֵ�ж� */
    if((temp&0X0004) == 0X0004)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D2_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D2_Pin,GPIO_PIN_RESET);
    }
    /* ��2λֵ�ж� */
    if((temp&0X0002) == 0X0002)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D1_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D1_Pin,GPIO_PIN_RESET);
    }
    /* ��1λֵ�ж� */
    if((temp&0X0001) == 0X0001)			//	**** **** 1000 0000
    {
        HAL_GPIO_WritePin(D_pin,D0_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(D_pin,D0_Pin,GPIO_PIN_RESET);
    }


}



