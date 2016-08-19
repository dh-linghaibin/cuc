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
#include "Com.h"
#include "Eeprom.h"

int main( void ) {
    SysInit();//system
    EeepromInit();
    BuntuInit();// 
    LcdInit();//
    ComInit();
    MenuInit();
    INTEN
    while(1) {
        MenuModeSet(BuntuRead());
        MenuAsk();
    }
}
