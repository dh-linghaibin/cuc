/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：Menu.c
*
* 文件标识：V
* 摘    要：菜单内核
*
*
* 当前版本：V1.0
* 作    者：lhb Steven
* 完成日期：2016/3/17
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Menu.h"
#include "Led.h"
#include "Com.h"
#include "Lcd.h"
#include "Materal.h"
/*
功能+1：自动推书
功能+2：工制转英制
功能+6：调节刀深浅
功能+7：调结压纸时间
功能+8：调节尺寸补偿
功能+10：最小尺寸
功能+11：最大尺寸
功能+14：老化测试
功能+下：进入刀切微调界面
*/
/*
确认键：输入确认，保存，推书
记忆键：调出数据   -------------------按下记忆键可调出数据，并进入自动模式，单次按下M键释放反色后可修改M值（0-100）组 直接按 后可修改M值（0-100组），直接按数字选择组数，或按上下键切换组数
推书键：选择推书模式/复位
清楚键：修改清楚尺寸
模式键：模式切换键 -------------------按模式键可以切换
上下键：前进，后退/选择键
1   键：数字键
功能键：功能键
推书键：按下复位键马上释放后程序自动推纸，如果按下时间超过2秒钟，程序则进行推纸器复位
*/
typedef struct DataNode
{
    u8 mode;//当前模式 0：自动模式 1：手动模式 2：差分模式
    u8 e_line;
    u8 cnt;//一次开关电源所切的次数，每次关机后即归0
    u8 ttl;//裁切总刀数
    u8 push_book;//是否需要推书
    u16 knife_front_size;//刀前尺寸
    u16 knife_rear_size;//刀后尺寸
    u16 group_no;//保存数据组号
    
} tDataNode;
/***********************************************变量声明*****************************************************
* 功    能: caidan  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
static tDataNode menu;
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 菜单参数初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
void MenuInit(void) { 
    menu.cnt = 0;
    menu.mode = 0;
}
/**********************************************函数定义***************************************************** 
* 函数名称: u8 MenuGetMode(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能: 模式状态  
* 作    者: by lhb_steven
* 日    期: 2016/3/26
************************************************************************************************************/ 
u8 MenuGetMode(void) { 
    return menu.mode;
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuModeSet(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 更新相应模式进入相应功能  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
void MenuModeSet(u8 cmd) { 
    switch( cmd ) {
        case 11://模式按键
            if(menu.mode == 0) {
                menu.mode = 1;
                LcdDraw16(32,0,tubiao_a);//自动
                LcdDraw18(32,1,tubiao_auto);
            } else if(menu.mode == 1) {
                menu.mode = 2;
                LcdDraw16(32,0,tubiao_s);//自动
                LcdDraw18(32,1,tubiao_siuare);
            } else if(menu.mode == 2) {
                menu.mode = 0;
                LcdDraw16(32,0,tubiao_m);//自动
                LcdDraw18(32,1,tubiao_manual);
            }
        break;
        case 5:
            if(menu.e_line < 5) {
                menu.e_line++;
            } else {
                menu.e_line = 0;
            }
            switch( menu.e_line ) {
                case 0:
                    LcdDrawNum8(16,7,0,12);
                    LcdDrawNum8(56,7,11,11);
                break;
                case 1:
                    LcdDrawNum8(16,7,11,11);
                    LcdDrawNum8(24,7,11,12);
                break;
                case 2:
                    LcdDrawNum8(24,7,11,11);
                    LcdDrawNum8(32,7,11,12);
                break;
                case 3:
                    LcdDrawNum8(32,7,11,11);
                    LcdDrawNum8(40,7,11,12);
                break;
                case 4:
                    LcdDrawNum8(40,7,11,11);
                    LcdDrawNum8(48,7,11,12);
                break;
                case 5:
                     LcdDrawNum8(48,7,11,11);
                    LcdDrawNum8(56,7,11,12);
                break;
                default:
                    break;
            }
        break;
        case 0:
            if(menu.e_line > 0) {
                menu.e_line--;
            } else {
                menu.e_line = 5;
            }
            switch( menu.e_line ) {
                case 0:
                    LcdDrawNum8(16,7,0,12);
                    LcdDrawNum8(56,7,11,11);
                break;
                case 1:
                    LcdDrawNum8(16,7,11,11);
                    LcdDrawNum8(24,7,11,12);
                break;
                case 2:
                    LcdDrawNum8(24,7,11,11);
                    LcdDrawNum8(32,7,11,12);
                break;
                case 3:
                    LcdDrawNum8(32,7,11,11);
                    LcdDrawNum8(40,7,11,12);
                break;
                case 4:
                    LcdDrawNum8(40,7,11,11);
                    LcdDrawNum8(48,7,11,12);
                break;
                case 5:
                     LcdDrawNum8(48,7,11,11);
                    LcdDrawNum8(56,7,11,12);
                break;
                default:
                    break;
            }
        break;
        case 6://记忆按键键
        
        break;
        
        
        default:
        break;
    }
}






