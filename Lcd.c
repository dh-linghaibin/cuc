/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：
*
* 文件标识：
* 摘    要：
*
*
* 当前版本：
* 作    者：lhb Steven
* 完成日期：2016/7/11
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Lcd.h"
#include "Delay.h"
#include "Materal.h"

#define CS_H    PB_ODR_ODR0 = 1     //PH4 E
#define CS_L    PB_ODR_ODR0 = 0
#define SID_H   PB_ODR_ODR1 = 1     //PH5 SID
#define SID_L   PB_ODR_ODR1 = 0
#define SCLK_H  PB_ODR_ODR2 = 1     //PH6 SCLK
#define SCLK_L  PB_ODR_ODR2 = 0

/*
*send 8 bit data to lcd
*/
static void SendByte(u8 zdata)
{
    u16 i;
    for(i=0; i<8; i++)
    {
        if((zdata << i) & 0x80)
        {
            SID_H;
        }
        else
        {
            SID_L;
        }
        SCLK_H;
        SCLK_L;
    }
}

/*
*Write Commed 
*/
static void WriteCom(u8 cmdcode)
{
   CS_H;
   SendByte(0xf8);
   SendByte(cmdcode & 0xf0);
   SendByte((cmdcode << 4) & 0xf0);
   DelayUs(20);
   CS_L;
}

/*
*Write Data
*
*/
static void WriteData(u8 Dispdata)
{  
  CS_H;
  SendByte(0xfa);	  //11111,RW(0),RS(1),0
  SendByte(Dispdata & 0xf0);
  SendByte((Dispdata << 4) & 0xf0);
  DelayUs(20);
  CS_L;
}

void LcdInit(void) {   
    PB_DDR_DDR0 = 1;
    PB_CR1_C10 = 1;
    PB_CR2_C20 = 0;
    
    PB_DDR_DDR1 = 1;
    PB_CR1_C11 = 1;
    PB_CR2_C21 = 0;
    
    PB_DDR_DDR2 = 1;
    PB_CR1_C12 = 1;
    PB_CR2_C22 = 0;
    
    DelayMs(250);
    WriteCom(0x30);  //30-- basic command operation
    DelayMs(5);
    WriteCom(0x0c);  //The screen does not move the cursor right
    DelayMs(5);
    WriteCom(0x01);  //clear
    DelayMs(5);    //Clear lcd long time
    WriteCom(0x06);  //The lcd Open, the cursor open, anti-white Off
    DelayMs(10);
    LcdDrawClear();
}

/*********************************************************
Function name: Display LCD Pos
Returns: None
Function: Set the display position
*********************************************************/
void LCDPos(u8 x,u8 y) 
{
	u8 pos;
	switch(x)
	{
		case 0: x=0x80;break;
		case 1: x=0x90;break;
		case 2: x=0x88;break;
		case 3: x=0x98;break;
	}
	pos=x+y;
	WriteCom(pos);
}

/*********************************************************
Function name: Display LCD String ()
Returns: None
Function: Display string
*********************************************************/
void LCDString(u8 x,u8 y,u8 *p,u8 time)
{
	LCDPos(x,y);
	for(;*p!='\0';p++)
	{
		WriteData(*p);
		DelayMs(time);
	}
}

void LCDNum(u8 num)
{
    WriteData(0x30+num);
}
void LcdFolt(u8 num) {
    WriteData(0x20+num);
};
/**********************************************函数定义***************************************************** 
* 函数名称: void LcdDrawClear(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/12
************************************************************************************************************/ 
void LcdDrawClear(void) { 
    u8 i,j,k;
    WriteCom(0x34);        //打开扩展指令集
    WriteCom(0x36);        //打开绘图显示

    i = 0x80;            
    for(j = 0;j < 32;j++)//32
    {
        WriteCom(i++);
        WriteCom(0x80);
        for(k = 0;k < 16;k++)//16
        {
            WriteData(0x00);
        }
    }
    
    i = 0x80;
    for(j = 0;j < 32;j++)
    {
        WriteCom(i++);
        WriteCom(0x88);	   
        for(k = 0;k < 16;k++)//16
        {
            WriteData(0x00);
        } 
    }  
    WriteCom(0x30);        //回到基本指令集
}
/**********************************************函数定义***************************************************** 
* 函数名称: void LcdDraw16Clear(u8 line, u8 row) 
* 输入参数: u8 line, u8 row 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/15
************************************************************************************************************/ 
void LcdDraw16Clear(u8 line, u8 row) { 
    u8 i,j,k;
    u16 line_bit = 0;
    
    WriteCom(0x34);        //打开扩展指令集
    WriteCom(0x36);        //打开绘图显示
    
    if(line < 32) {
        line_bit = 0x80;
        i = 0x80+line;   
    } else {
        line_bit = 0x88;
        i = 0x80+line-32;   
    }
    
    
    line_bit += row;
    for(j = 0;j < 16;j++)//32
    {
        WriteCom(i++);
        WriteCom(line_bit);
        for(k = 0;k < 2;k++)//16
        {
            WriteData(0x00);
        }
    }
    WriteCom(0x30);        //回到基本指令集
}
/**********************************************函数定义***************************************************** 
* 函数名称: void LcdDraw16(u8 line,u8 row) 
* 输入参数: u8 line,u8 row 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/12
************************************************************************************************************/ 
void LcdDraw16(u8 line,u8 row,const u8 *ptr) {
    u8 i,j,k;
    u16 line_bit = 0;
    
    WriteCom(0x34);        //打开扩展指令集
    WriteCom(0x36);        //打开绘图显示
    
    if(line < 32) {
        line_bit = 0x80;
        i = 0x80+line;   
    } else {
        line_bit = 0x88;
        i = 0x80+line-32;   
    }
    
    
    line_bit += row;
    for(j = 0;j < 16;j++)//32
    {
        WriteCom(i++);
        WriteCom(line_bit);
        for(k = 0;k < 2;k++)//16
        {
            WriteData(*ptr++);
        }
    }
    WriteCom(0x30);        //回到基本指令集
}
/**********************************************函数定义***************************************************** 
* 函数名称: void LcdDraw18(u8 line,u8 row,const u8 *ptr)) 
* 输入参数: u8 line,u8 row,const u8 *ptr) 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/12
************************************************************************************************************/ 
void LcdDraw18(u8 line,u8 row,const u8 *ptr) { 
    u8 i,j,k;
    u16 line_bit = 0;
    
    WriteCom(0x34);        //打开扩展指令集
    WriteCom(0x36);        //打开绘图显示
    
    if(line < 32) {
        line_bit = 0x80;
        i = 0x80+line;   
    } else {
        line_bit = 0x88;
        i = 0x80+line-32;   
    }
    
    
    line_bit += row;
    for(j = 0;j < 16;j++)//32
    {
        WriteCom(i++);
        WriteCom(line_bit);
        for(k = 0;k < 4;k++)//16
        {
            WriteData(*ptr++);
        }
    }
    WriteCom(0x30);        //回到基本指令集
}
/**********************************************函数定义***************************************************** 
* 函数名称: void LcdDrawNum8(u8 line, u8 row, const u8 *ptr) 
* 输入参数: u8 line, u8 row, const u8 *ptr 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/12
************************************************************************************************************/ 
void LcdDrawNum8(u8 line, u8 row, u8 num, u8 num2) { 
    u8 i,j,k;
    u16 line_bit = 0;
    u8 num_bit1 = 0;
    u8 num_bit2 = 0;
    num_bit1 = num*8;
    num_bit2 = num2*8;
    WriteCom(0x34);        //打开扩展指令集
    WriteCom(0x36);        //打开绘图显示
    
    if(line < 32) {
        line_bit = 0x80;
        i = 0x80+line;   
    } else {
        line_bit = 0x88;
        i = 0x80+line-32;   
    }
    
    
    line_bit += row;
    for(j = 0;j < 8;j++)//32
    {
        WriteCom(i++);
        WriteCom(line_bit);
        for(k = 0;k < 1;k++)//16
        {
            WriteData(num_s[num_bit1++]);
            WriteData(num_s[num_bit2++]);
        }
    }
    WriteCom(0x30);        //回到基本指令集
}
/**********************************************函数定义***************************************************** 
* 函数名称: void LcdDraeBq8x16(u8 line, u8 row, u8 num) 
* 输入参数: u8 line, u8 row, u8 num 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/12
************************************************************************************************************/ 
void LcdDraeBq8x16(u8 line, u8 row, u8 num) { 
    u8 i,j,k;
    u16 line_bit = 0;
    u8 bit = 0;
    bit = num*16;
    WriteCom(0x34);        //打开扩展指令集
    WriteCom(0x36);        //打开绘图显示

    if(line < 32) {
            line_bit = 0x80;
            i = 0x80+line;   
        } else {
            line_bit = 0x88;
            i = 0x80+line-32;   
    }


    line_bit += row;
    for(j = 0;j < 8;j++)//32
    {
        WriteCom(i++);
        WriteCom(line_bit);
        for(k = 0;k < 2;k++)//16
        {
            WriteData(f_e[bit++]);
        }
    }
    WriteCom(0x30);        //回到基本指令集
}







