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

#include "Type.h"
#include "Sys.h"
#include "Time.h"
#include "Lcd.h"
#include "buntu.h"
#include "Materal.h"
#include "menu.h"

int main( void ) {
    SysInit();//system
    BuntuInit();//
    MenuInit();
    LcdInit();//

    LcdDraw16(0,0,tubiao1);//图标1
    LCDString(0,1,"300.0",1);
    
    LcdDraw16(16,0,tubiao2);//箭头
    LCDString(1,1,"  0.0",1);
    
    LcdDraw16(32,0,tubiao_a);//自动
    LcdDraw18(32,1,tubiao_auto);
    
    LcdDraw16(48,0,tubiao_m);//M
    LcdDraw18(48,1,tubiao3_arrow);//大尺子
    
    LCDString(0,4,"Mm M01",1);
    
    LcdDraeBq8x16(16,4,0);
    LcdDraeBq8x16(24,4,1);
    LcdDraeBq8x16(32,4,2);
    LcdDraeBq8x16(40,4,3);
    LcdDraeBq8x16(48,4,4);
    LcdDraeBq8x16(56,4,5);
    
    LcdDrawNum8(16,5,3,0);
    LcdDrawNum8(16,6,0,10);
    LcdDrawNum8(16,7,0,12);
    
    LcdDrawNum8(24,5,11,11);
    LcdDrawNum8(24,6,11,11);
    LcdDrawNum8(24,7,11,11);
    
    LcdDrawNum8(32,5,11,11);
    LcdDrawNum8(32,6,11,11);
    LcdDrawNum8(32,7,11,11);
    
    LcdDrawNum8(40,5,11,11);
    LcdDrawNum8(40,6,11,11);
    LcdDrawNum8(40,7,11,11);
    
    LcdDrawNum8(48,5,11,11);
    LcdDrawNum8(48,6,11,11);
    LcdDrawNum8(48,7,11,11);
    
    LcdDrawNum8(56,5,11,11);
    LcdDrawNum8(56,6,11,11);
    LcdDrawNum8(56,7,11,11);
    
    while(1) {
        MenuModeSet(BuntuRead());
    }
}
