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
#include "Eeprom.h"
#include "Delay.h"
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
typedef enum {
    best_arrive = 3000,//最大位置
    best_sep_aaive = 47700,
}menudata;

typedef struct DataNode
{
    u8 language;//语言                                        42
    u8 mode;//当前模式 0：自动模式 1：手动模式 2：差分模式    10
    u8 e_line;
    u16 cnt;//一次开关电源所切的次数，每次关机后即归0        
    u16 ttl;//裁切总刀数                                       8 9
    u8 push_book;//是否需要推书                               11   
    u16 knife_front_size;//刀前尺寸                           12 13
    u16 knife_rear_size;//刀后尺寸                            14 15
    u16 knife_rear_size_setp;//步进尺寸                           
    u16 group_no;//保存数据组号     
    u8 aut_m;//保存是否可以设置数据
    u16 aut_e[8];//保存自动模式下第一组数据                 (16,17) (18,19) (20,21) (22,23) (24,25) (26,27) (28,29) (30,31)
    u16 aut_group;//对第几组数据操作
    u8 aut_m_num;//对第几位进行更改
    u16 siuare_data[2];//等分模式 数据                     (35,36) (37,38)
    
    u8 program;//编程位                                    
    u8 moto_vigor;//电机力度 0-10                          39 
    u8 mtot_cut_place;//电机位置 0-20                      40
    u8 size_sys;//尺寸制式                                 41
    u8 clear_zero;//清零                                  
    
    u8 set_pag1;//普通设置
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
    
    menu.aut_group = 1;
    menu.mode = EepromRead(10);
    menu.push_book = EepromRead(11);
    menu.knife_rear_size = EepromRead(14);
    menu.knife_rear_size |= (u16)(EepromRead(15) << 8);
    
    menu.ttl = EepromRead(8);
    menu.ttl |= (u16)(EepromRead(9) << 8);
    
    menu.language = EepromRead(42);//中文模式
    menu.moto_vigor = EepromRead(39);//读取电机力度
    menu.mtot_cut_place = EepromRead(40);//电机刀位置微调
    menu.size_sys = EepromRead(41);//尺寸
    
    for(int save_i = 0; save_i < 2; save_i++) {
        u8 save_adr = 0;
        save_adr = 35+save_i*2;
        menu.siuare_data[save_i] = EepromRead(save_adr);
        menu.siuare_data[save_i] |= (u16)(EepromRead(save_adr+1) << 8);
    }
    
    for(int save_i = 0; save_i < 8; save_i++) {
        u8 save_adr = 0;
        save_adr = 16+save_i*2;
        menu.aut_e[save_i] = EepromRead(save_adr);
        menu.aut_e[save_i] |= (u16)(EepromRead(save_adr+1) << 8);
    }
   
    menu.aut_m = 8;//不修改数据
    
    LcdDraw16(0,0,tubiao1);//图标1
    LCDPos(0,1);
    LCDNum(menu.knife_rear_size/1000);
    LCDNum(menu.knife_rear_size/100%10);
    LCDNum(menu.knife_rear_size%100/10);
    LcdFolt(14);
    LCDNum(menu.knife_rear_size%10);  
        
    LcdDraw16(16,0,tubiao2);//箭头
    LCDString(1,1,"  0.0",1);
   
    MenuShowMode(menu.mode);//显示当前模式
    
    LcdDraw16(48,0,tubiao_m);//M
    MenuShowPushFlag(menu.push_book);//显示是否需要推纸
    
    MenuShowData(menu.mode);//显示数据
   
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuSaveData(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/20
************************************************************************************************************/ 
void MenuSaveData(void) { 
    EepromWrite(10,menu.mode);
    EepromWrite(11,menu.push_book);
    EepromWrite(14,(u8)menu.knife_rear_size);
    EepromWrite(15,(u8)(menu.knife_rear_size >> 8));
    for(int save_i = 0; save_i < 8; save_i++) {
        u8 save_adr = 0;
        save_adr = 16+save_i*2;
        EepromWrite(save_adr,(u8)menu.aut_e[save_i]);
        EepromWrite(save_adr+1,(u8)(menu.aut_e[save_i] >> 8));
    }
    
    for(int save_i = 0; save_i < 2; save_i++) {
        u8 save_adr = 0;
        save_adr = 35+save_i*2;
        EepromWrite(save_adr,(u8)menu.siuare_data[save_i]);
        EepromWrite(save_adr+1,(u8)(menu.siuare_data[save_i] >> 8));
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuShowMode(u8 md) 
* 输入参数: u8 md 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/15
************************************************************************************************************/ 
void MenuShowMode(u8 md) { 
    switch( md ) {
        case 0://自动模式
            LcdDraw16(32,0,tubiao_a);
            if(menu.language == 0) {
                LcdDraw18(32,1,tubiao_auto);
            } else {
                LCDString(2,1,"自动",1);
            }
            break;
        case 1://手动模式
            LcdDraw16(32,0,tubiao_yuanquan);
            if(menu.language == 0) {
                LcdDraw18(32,1,tubiao_manual);
            } else {
                LCDString(2,1,"手动",1);
            }
            break;
        case 2://等分模式
            LcdDraw16(32,0,tubiao_s);
            if(menu.language == 0) {
                LcdDraw18(32,1,tubiao_siuare);
            } else {
                LCDString(2,1,"等分",1);
            }
            break;
        case 3://设置模式
            if(menu.language == 0) {
                LCDString(2,1,"Set",1);
            } else {
                LCDString(2,1,"设置",1);
            }
            break;
        case 4://系统设置
            if(menu.language == 0) {
                LCDString(2,1,"Sys_Set",1);
            } else {
                LCDString(2,1,"设置",1);
            }
        case 5://老化模式
            if(menu.language == 0) {
                LCDString(2,1,"Age  ",1);
            } else {
                LCDString(2,1,"老化",1);
            }
            break;
        default:
            break;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuShowPushFlag(u8 pf) 
* 输入参数: u8 pf 
* 返回参数: void  
* 功    能: 判断是否需要推纸  
* 作    者: by lhb_steven
* 日    期: 2016/7/15
************************************************************************************************************/ 
void MenuShowPushFlag(u8 pf) { 
    if(pf == 0) {
        //不需要推纸
        LcdDraw18(48,1,tubiao3);
    } else {
        //需要推纸
        LcdDraw18(48,1,tubiao3_arrow);
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuShowData(u8 md) 
* 输入参数: u8 md 
* 返回参数: void  
* 功    能: 根据当点模式显示对应数据  
* 作    者: by lhb_steven
* 日    期: 2016/7/15
************************************************************************************************************/ 
void MenuShowData(u8 md) {
    switch( md ) {
        case 0://自动模式
            LcdDraw16Clear(0,4);
            LCDString(0,4,"M ",1);
            LCDNum(0);
            LCDNum(1);
            
            LcdDraeBq8x16(16,4,0+menu.aut_m);//e
            LcdDraeBq8x16(24,4,1+menu.aut_m);
            LcdDraeBq8x16(32,4,2+menu.aut_m);
            LcdDraeBq8x16(40,4,3+menu.aut_m);
            LcdDraeBq8x16(48,4,4+menu.aut_m);
            LcdDraeBq8x16(56,4,5+menu.aut_m);
            
            for(int show_i = 0; show_i < 6; show_i++) {
                u8 show_x = 0;
                show_x = 16+show_i*8;
                if(menu.aut_group == show_i) {//第一组
                    if(menu.aut_m == 0) {//修改模式
                        if(menu.aut_m_num == 0) {
                            LcdDrawNum8(show_x,5,11,menu.aut_e[menu.aut_group]/100%10);
                            LcdDrawNum8(show_x,6,menu.aut_e[menu.aut_group]%100/10,10);
                            LcdDrawNum8(show_x,7,menu.aut_e[menu.aut_group]%10,12);
                        } else if(menu.aut_m_num == 1) {
                           LcdDrawNum8(show_x,5,menu.aut_e[menu.aut_group]/1000,11);
                            LcdDrawNum8(show_x,6,menu.aut_e[menu.aut_group]%100/10,10);
                            LcdDrawNum8(show_x,7,menu.aut_e[menu.aut_group]%10,12);
                        } else if(menu.aut_m_num == 2) {
                            LcdDrawNum8(show_x,5,menu.aut_e[menu.aut_group]/1000,menu.aut_e[menu.aut_group]/100%10);
                            LcdDrawNum8(show_x,6,11,10);
                            LcdDrawNum8(show_x,7,menu.aut_e[menu.aut_group]%10,12);
                        } else if(menu.aut_m_num == 3) {
                            LcdDrawNum8(show_x,5,menu.aut_e[menu.aut_group]/1000,menu.aut_e[menu.aut_group]/100%10);
                            LcdDrawNum8(show_x,6,menu.aut_e[menu.aut_group]%100/10,10);
                            LcdDrawNum8(show_x,7,11,12);
                        } 
                    } else {
                        LcdDrawNum8(show_x,5,menu.aut_e[menu.aut_group]/1000,menu.aut_e[menu.aut_group]/100%10);
                        LcdDrawNum8(show_x,6,menu.aut_e[menu.aut_group]%100/10,10);
                        LcdDrawNum8(show_x,7,menu.aut_e[menu.aut_group]%10,12);
                    }
                } else {
                    LcdDrawNum8(show_x,5,menu.aut_e[show_i]/1000,menu.aut_e[show_i]/100%10);
                    LcdDrawNum8(show_x,6,menu.aut_e[show_i]%100/10,10);
                    LcdDrawNum8(show_x,7,menu.aut_e[show_i]%10,13);
                }
            }
            break;
        case 1://手动模式
            LCDString(0,4," ",1);
            LcdDraw16Clear(32,4);
            LcdDraw16Clear(48,4);
            LcdDraw16Clear(32,5);
            LcdDraw16Clear(48,5);
            LcdDraw16Clear(32,6);
            LcdDraw16Clear(48,6);
            LcdDraw16Clear(32,7);
            LcdDraw16Clear(48,7);
            LcdDraw16(0,4,tubiao_ruler);

            LcdDraeBq8x16(16,4,6);//cnt
            LcdDraeBq8x16(24,4,7);//ttl
           
            LcdDrawNum8(16,5,0,menu.cnt/10000);
            LcdDrawNum8(16,6,menu.cnt/1000%10,menu.cnt/100%10);
            LcdDrawNum8(16,7,menu.cnt%100/10,menu.cnt%100%10);

            LcdDrawNum8(24,5,0,menu.ttl/10000);
            LcdDrawNum8(24,6,menu.ttl/1000%10,menu.ttl/100%10);
            LcdDrawNum8(24,7,menu.ttl%100/10,menu.ttl%100%10);  
            break;
        case 2://等分模式
            LcdDraw16Clear(0,4);
            LCDString(0,4,"S 23",1);
            LcdDraeBq8x16(16,4,0+8);//e
            LcdDraeBq8x16(24,4,1+8);
            
//            LcdDrawNum8(16,5,3,0);
//            LcdDrawNum8(16,6,0,10);
//            LcdDrawNum8(16,7,0,12);
//
//            LcdDrawNum8(24,5,11,11);
//            LcdDrawNum8(24,6,11,11);
//            LcdDrawNum8(24,7,11,11);
            for(int show_i = 0; show_i < 2; show_i++) {
                u8 show_x = 0;
                show_x = 16+show_i*8;
                if(menu.aut_group == show_i) {//第一组
                    if(menu.aut_m == 0) {//修改模式
                        if(menu.aut_m_num == 0) {
                            LcdDrawNum8(show_x,5,11,menu.siuare_data[menu.aut_group]/100%10);
                            LcdDrawNum8(show_x,6,menu.siuare_data[menu.aut_group]%100/10,10);
                            LcdDrawNum8(show_x,7,menu.siuare_data[menu.aut_group]%10,12);
                        } else if(menu.aut_m_num == 1) {
                           LcdDrawNum8(show_x,5,menu.siuare_data[menu.aut_group]/1000,11);
                            LcdDrawNum8(show_x,6,menu.siuare_data[menu.aut_group]%100/10,10);
                            LcdDrawNum8(show_x,7,menu.siuare_data[menu.aut_group]%10,12);
                        } else if(menu.aut_m_num == 2) {
                            LcdDrawNum8(show_x,5,menu.siuare_data[menu.aut_group]/1000,menu.siuare_data[menu.aut_group]/100%10);
                            LcdDrawNum8(show_x,6,11,10);
                            LcdDrawNum8(show_x,7,menu.siuare_data[menu.aut_group]%10,12);
                        } else if(menu.aut_m_num == 3) {
                            LcdDrawNum8(show_x,5,menu.siuare_data[menu.aut_group]/1000,menu.siuare_data[menu.aut_group]/100%10);
                            LcdDrawNum8(show_x,6,menu.siuare_data[menu.aut_group]%100/10,10);
                            LcdDrawNum8(show_x,7,11,12);
                        } 
                    } else {
                        LcdDrawNum8(show_x,5,menu.siuare_data[menu.aut_group]/1000,menu.siuare_data[menu.aut_group]/100%10);
                        LcdDrawNum8(show_x,6,menu.siuare_data[menu.aut_group]%100/10,10);
                        LcdDrawNum8(show_x,7,menu.siuare_data[menu.aut_group]%10,12);
                    }
                } else {
                    LcdDrawNum8(show_x,5,menu.siuare_data[show_i]/1000,menu.siuare_data[show_i]/100%10);
                    LcdDrawNum8(show_x,6,menu.siuare_data[show_i]%100/10,10);
                    LcdDrawNum8(show_x,7,menu.siuare_data[show_i]%10,13);
                }
            }
            break;
        case 3://参数设置模式
            LcdDrawClear();
            if(menu.language == 0) {
                LCDString(0,0,"Language Set:   ",1);
                LCDString(2,0,"Size Set:",1);
                
                LCDString(1,2,"Englsih         ",1);
                
                if(menu.size_sys == 0) {
                    LCDString(3,0,"China(Mm)     ",1);
                } else {
                    LCDString(3,0,"Englsih(Mile) ",1);
                }
            } else {
                LCDString(0,0,"语言设置:       ",1);
                LCDString(2,0,"尺寸制式:",1);
                
                LCDString(1,2,"中文",1);
                
                if(menu.size_sys == 0) {
                    LCDString(3,1,"公制（MM）  ",1);
                } else {
                    LCDString(3,1,"英制（Mile）",1);
                }
            }

            switch(menu.set_pag1) {
            case 0:
                LCDString(1,7,"○",1);
                LCDString(3,7,"  ",1);
                break;
            case 1:
                LCDString(1,7,"  ",1);
                LCDString(3,7,"○",1);
                break;
            case 2:
                LCDString(1,7,"●",1);
                break;
            case 3:
                LCDString(3,7,"●",1);
                break;
            default:
                break;
            }
       
            break;
        case 4://特殊设置
            LcdDrawClear();
            if(menu.language == 0) {
                LCDString(0,0," System Settings",1);
                
                LCDString(1,0,"MotoMaud:",1);
                LCDNum(menu.moto_vigor/10%10);
                LCDNum(menu.moto_vigor%10);
                
                LCDString(2,0,"CutPlace:",1);
                LCDNum(menu.mtot_cut_place/10%10);
                LCDNum(menu.mtot_cut_place%10);
                
                LCDString(3,0,"CutClearn:",1);
                if(menu.clear_zero == 0) {
                    LCDString(3,5,"Yes",1);
                } else {
                    LCDString(3,5,"No",1);
                }
            } else {
                LCDString(0,0,"    系统设置",1);
                
                LCDString(1,0,"电机力度:",1);
                LCDNum(menu.moto_vigor/10%10);
                LCDNum(menu.moto_vigor%10);
                
                LCDString(2,0,"刀位置调:",1);
                LCDNum(menu.mtot_cut_place/10%10);
                LCDNum(menu.mtot_cut_place%10);
                
                LCDString(3,0,"刀计数清:",1);
                if(menu.clear_zero == 0) {
                    LCDString(3,5,"是",1);
                } else {
                    LCDString(3,5,"否",1);
                }
            }
            
            switch(menu.set_pag1) {
            case 0:
                LCDString(3,7,"  ",1);
                LCDString(2,7,"  ",1);
                LCDString(1,7,"○",1);
                break;
            case 1:
                LCDString(1,7,"  ",1);
                LCDString(3,7,"  ",1);
                LCDString(2,7,"○",1);
                break;
            case 2:
                LCDString(1,7,"  ",1);
                LCDString(2,7,"  ",1);
                LCDString(3,7,"○",1);
                break;
            case 3:
                LCDString(1,7,"●",1);
                break;
            case 4:
                LCDString(2,7,"●",1);
                break;
            case 5:
                LCDString(3,7,"●",1);
                break;
            default:
                break;
            }
            break;
        case 5://老化模式
            LcdDrawClear();
            LCDString(0,0,"            ",1);
            LCDString(1,0,"            ",1);
            LCDString(2,0,"     ......   ",1);
            LCDString(3,0,"            ",1);
            if(menu.language == 0) {
                LCDString(1,0,"    AGING MODE",1);
            } else {
                LCDString(1,0,"    老化模式",1);
            }
            break;
        default:
            break;
    }
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
* 函数名称: void MenuSetAutData(u8 num) 
* 输入参数: u8 num 
* 返回参数: void  
* 功    能: 设置e 的修改数据  
* 作    者: by lhb_steven
* 日    期: 2016/7/20
************************************************************************************************************/ 
void MenuSetAutData(u8 num) { 
    if(menu.mode == 0) {
         if(menu.aut_m == 0) {
            u8 aut_h0 = 0;
            u8 aut_h1 = 0;
            u8 aut_h2 = 0;
            u8 aut_h3 = 0;
            aut_h0 = menu.aut_e[menu.aut_group]/1000;
            aut_h1 = menu.aut_e[menu.aut_group]/100%10;
            aut_h2 = menu.aut_e[menu.aut_group]%100/10;
            aut_h3 = menu.aut_e[menu.aut_group]%10;
            if(menu.aut_m_num == 0) {
                aut_h0 = num;
                menu.aut_m_num = 1;
            } else if(menu.aut_m_num == 1) {
                aut_h1 = num;
                menu.aut_m_num = 2;
            } else if(menu.aut_m_num == 2) {
                aut_h2 = num;
                menu.aut_m_num = 3;
            } else if(menu.aut_m_num == 3) {
                aut_h3 = num;
                menu.aut_m_num = 0;
                menu.aut_m = 8;
                MenuSaveData();//保存数据
            }
            menu.aut_e[menu.aut_group] = aut_h0*1000;
            menu.aut_e[menu.aut_group] += aut_h1*100;
            menu.aut_e[menu.aut_group] += aut_h2*10;
            menu.aut_e[menu.aut_group] += aut_h3;
            if(menu.aut_e[menu.aut_group] > best_arrive) {
                menu.aut_e[menu.aut_group] = best_arrive;
            }
        }
    } else if(menu.mode == 1) {//手动
    
    } else if(menu.mode == 2) {//等分
        if(menu.aut_m == 0) {
            u8 aut_h0 = 0;
            u8 aut_h1 = 0;
            u8 aut_h2 = 0;
            u8 aut_h3 = 0;
            aut_h0 = menu.siuare_data[menu.aut_group]/1000;
            aut_h1 = menu.siuare_data[menu.aut_group]/100%10;
            aut_h2 = menu.siuare_data[menu.aut_group]%100/10;
            aut_h3 = menu.siuare_data[menu.aut_group]%10;
            if(menu.aut_m_num == 0) {
                aut_h0 = num;
                menu.aut_m_num = 1;
            } else if(menu.aut_m_num == 1) {
                aut_h1 = num;
                menu.aut_m_num = 2;
            } else if(menu.aut_m_num == 2) {
                aut_h2 = num;
                menu.aut_m_num = 3;
            } else if(menu.aut_m_num == 3) {
                aut_h3 = num;
                menu.aut_m_num = 0;
                menu.aut_m = 8;
                MenuSaveData();//保存数据
                ComSend(0xb0,(u8)(menu.siuare_data[0]/menu.siuare_data[1]),(u8)(menu.siuare_data[1]),(u8)(menu.siuare_data[1] >> 8));
            }
            menu.siuare_data[menu.aut_group] = aut_h0*1000;
            menu.siuare_data[menu.aut_group] += aut_h1*100;
            menu.siuare_data[menu.aut_group] += aut_h2*10;
            menu.siuare_data[menu.aut_group] += aut_h3;
            if(menu.siuare_data[1] > menu.siuare_data[0]) {
                menu.siuare_data[1] = menu.siuare_data[0];
            }
        }
    } else if(menu.mode == 3) {//普通设置
        
    } else if(menu.mode == 4){//系统设置
    
    }
    MenuShowData(menu.mode);//显示数据
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
            if( (menu.mode == 3)||(menu.mode == 4)||(menu.mode == 5) ) {//清楚数据
                LCDString(0,0,"                ",1);
                LCDString(1,0,"                ",1);
                LCDString(2,0,"                ",1);
                LCDString(3,0,"                ",1);
                
                LcdDraw16(0,0,tubiao1);//图标1
                LCDPos(0,1);
                LCDNum(menu.knife_rear_size/1000);
                LCDNum(menu.knife_rear_size/100%10);
                LCDNum(menu.knife_rear_size%100/10);
                LcdFolt(14);
                LCDNum(menu.knife_rear_size%10);  
                    
                LcdDraw16(16,0,tubiao2);//箭头
                LCDString(1,1,"  0.0",1);
                
                LcdDraw16(48,0,tubiao_m);//M
                MenuShowPushFlag(menu.push_book);//显示是否需要推纸
                ComSend(0xa1,0x00,0x00,0x00);//退出老化模式
            }
            if(menu.mode < 2) {
                menu.mode++;
            } else {
                menu.mode = 0;
            }
            menu.aut_group = 0;
            menu.set_pag1 = 0;
            MenuShowMode(menu.mode);//显示当前模式
            MenuShowData(menu.mode);//显示数据
            menu.knife_rear_size_setp = (u16)(( (best_arrive-menu.knife_rear_size) * (best_sep_aaive/best_arrive)));
            if(menu.push_book == 0) {
                ComSend(0x04,menu.mode,(u8)(menu.knife_rear_size_setp),(u8)(menu.knife_rear_size_setp >> 8));
            } else {
                ComSend(0x05,menu.mode,(u8)(menu.knife_rear_size_setp),(u8)(menu.knife_rear_size_setp >> 8));
            }
            if(menu.mode == 2) {//等分模式
                DelayMs(20);
                ComSend(0xb0,(u8)(menu.siuare_data[0]/menu.siuare_data[1]),(u8)(menu.siuare_data[1]),(u8)(menu.siuare_data[1] >> 8));
            }
            MenuSaveData();
        break;
        case 22://确认按键
            if(menu.mode == 0) {//自动模式
                menu.knife_rear_size = menu.aut_e[menu.aut_group];
                LCDPos(0,1);
                LCDNum(menu.knife_rear_size/1000);
                LCDNum(menu.knife_rear_size/100%10);
                LCDNum(menu.knife_rear_size%100/10);
                LcdFolt(14);
                LCDNum(menu.knife_rear_size%10);  
                
                menu.knife_rear_size_setp = (u16)(( (best_arrive-menu.knife_rear_size) * (15.9)));
                if(menu.push_book == 0) {
                    ComSend(0x04,menu.mode,(u8)(menu.knife_rear_size_setp),(u8)(menu.knife_rear_size_setp >> 8));
                } else {
                    ComSend(0x05,menu.mode,(u8)(menu.knife_rear_size_setp),(u8)(menu.knife_rear_size_setp >> 8));
                }
                MenuSaveData();
            } else if(menu.mode == 3) {//普通设置模式
                switch(menu.set_pag1) {
                case 0:
                    menu.set_pag1 = 2;
                    break;
                case 1:
                    menu.set_pag1 = 3;
                    break;
                case 2:
                    menu.set_pag1 = 0;
                    //保存语言 选择
                    EepromWrite(42,menu.language);
                    break;
                case 3:
                    menu.set_pag1 = 1;
                    //保存 尺寸只是
                    EepromWrite(41,menu.size_sys);
                    break;
                default:
                    break;
                }
                MenuShowData(menu.mode);//显示数据
            } else if(menu.mode == 4) {//系统设置模式
                switch(menu.set_pag1) {
                case 3://电机力度
                    EepromWrite(39,menu.moto_vigor);
                    ComSend(0x08,menu.moto_vigor,0x00,0x00);
                    break;
                case 4://刀位置
                     EepromWrite(40,menu.mtot_cut_place);
                     ComSend(0x0a,menu.mtot_cut_place,0x00,0x00);
                    break;
                case 5://刀计数清零
                    if(menu.clear_zero == 0) {
                        
                    } else {
                        EepromWrite(8,0x00);
                        EepromWrite(9,0x00);
                        menu.ttl = EepromRead(8);
                        menu.ttl |= (u16)(EepromRead(9) << 8);
                    }
                default:
                    break;
                }
                if(menu.set_pag1 > 2) {
                    menu.set_pag1 -= 3;
                } else {
                    menu.set_pag1 += 3;
                }
                MenuShowData(menu.mode);//显示数据
            }
        break;
        case 0://按键 下
            if(menu.mode == 0) {//自动模式
                if(menu.aut_group > 0) {
                    menu.aut_group--;
                } else {
                    menu.aut_group = 5;
                }
                
            } else if(menu.mode == 3) {//普通设置模式
                if(menu.set_pag1 > 1) {
                    if(menu.set_pag1 == 2) {
                        if(menu.language > 0) {
                            menu.language--;
                        } else {
                            menu.language = 1;
                        }   
                    } else {
                        if(menu.size_sys > 0) {
                            menu.size_sys--;
                        } else {
                            menu.size_sys = 1;
                        }  
                    }
                } else {
                    if(menu.set_pag1 > 0) {
                        menu.set_pag1--;
                    } else {
                        menu.set_pag1 = 1;
                    }   
                }
            } else if(menu.mode == 2) {//等分模式
                if(menu.aut_group > 0) {
                    menu.aut_group--;
                } else {
                    menu.aut_group = 1;
                }
            } else if(menu.mode == 4) {//系统设置模式
                switch(menu.set_pag1) {
                case 3://点击力度
                    if(menu.moto_vigor > 1) {
                        menu.moto_vigor--;
                    } else {
                        menu.moto_vigor = 9;
                    }
                    break;
                case 4://刀位置设置
                    if(menu.mtot_cut_place > 1) {
                        menu.mtot_cut_place--;
                    } else {
                        menu.mtot_cut_place = 9;
                    }
                    break;
                case 5://刀计数清零
                    if(menu.clear_zero > 0) {
                        menu.clear_zero--;
                    } else {
                        menu.clear_zero = 1;
                    }
                    break;
                default:
                    if(menu.set_pag1 > 0) {
                        menu.set_pag1--;
                    } else {
                        menu.set_pag1 = 2;
                    }
                    break;
                }
            }
            MenuShowData(menu.mode);//显示数据
        break;
        case 5://按键 上
            if(menu.mode == 0) {//自动模式
                if(menu.aut_group < 5) {
                    menu.aut_group++;
                } else {
                    menu.aut_group = 0;
                }
                
            } else if(menu.mode == 3) {//普通设置模式
                if(menu.set_pag1 > 1) {
                    if(menu.set_pag1 == 2) {
                        if(menu.language < 1) {
                            menu.language++;
                        } else {
                            menu.language = 0;
                        }

                    } else {
                        if(menu.size_sys < 1) {
                            menu.size_sys++;
                        } else {
                            menu.size_sys = 0;
                        }
                    }
                } else {
                    if(menu.set_pag1 < 1) {
                        menu.set_pag1++;
                    } else {
                        menu.set_pag1 = 0;
                    }
                }
            } else if(menu.mode == 2) {//等分模式
                if(menu.aut_group < 1) {
                    menu.aut_group++;
                } else {
                    menu.aut_group = 0;
                }
            } else if(menu.mode == 4) {//系统设置模式
                switch(menu.set_pag1) {
                case 3://点击力度
                    if(menu.moto_vigor < 9) {
                        menu.moto_vigor++;
                    } else {
                        menu.moto_vigor = 1;
                    }
                    break;
                case 4://刀位置设置
                    if(menu.mtot_cut_place < 9) {
                        menu.mtot_cut_place++;
                    } else {
                        menu.mtot_cut_place = 1;
                    }
                    break;
                case 5://刀计数清零
                    if(menu.clear_zero < 1) {
                        menu.clear_zero++;
                    } else {
                        menu.clear_zero = 0;
                    }
                    break;
                default:
                    if(menu.set_pag1 < 2) {
                        menu.set_pag1++;
                    } else {
                        menu.set_pag1 = 0;
                    }
                    break;
                }  
            }
            MenuShowData(menu.mode);//显示数据
        break;
        case 6://记忆按键键
            if(menu.mode == 0) {//自动模式
                if(menu.aut_m == 0) {
                    menu.aut_m = 8;
                } else {
                    menu.aut_m = 0;
                }
                MenuShowData(menu.mode);//显示数据
            } else if(menu.mode == 2) {//等分模式
                if(menu.aut_m == 0) {
                    menu.aut_m = 8;
                } else {
                    menu.aut_m = 0;
                }
                MenuShowData(menu.mode);//显示数据
            }
        break;
        case 21://推书键短按
            if(menu.push_book == 0) {
                menu.push_book = 1;
            } else {
                menu.push_book = 0;
            }
            MenuShowPushFlag(menu.push_book);
        break;
        case 50://推书按键长按
            ComSend(0x06,0x00,0x00,0x00);//复位
        break;
        case 60://功能键按下
            menu.program = 1;//进入编程模式
            break;
        case 16://功能键放开
            if(menu.program == 4) {
                menu.program = 0;
                menu.mode = 3;
                MenuShowMode(menu.mode);//显示当前模式
                MenuShowData(menu.mode);//显示数据
            } else if(menu.program == 5){
                menu.program = 0;
                menu.mode = 4;
                MenuShowMode(menu.mode);//显示当前模式
                MenuShowData(menu.mode);//显示数据
            } else if(menu.program == 22) {//老化模式
                menu.program = 0;
                menu.mode = 5;//老化模式
                MenuShowMode(menu.mode);//显示当前模式
                MenuShowData(menu.mode);//显示数据
                ComSend(0xa1,0x01,0x00,0x00);//老化
            } else {
                menu.program = 0;
            }
            break;
        case 23://数字键0
            MenuSetAutData(0);
        break;
        case 19://数字键1
            MenuSetAutData(1);
        break;
        case 18://数字键2
            if(menu.program == 0) {
                MenuSetAutData(2);
            } else if(menu.program == 3) {
                menu.program = 22;
            }
        break;
        case 17://数字键3
            if(menu.program == 0) {
                MenuSetAutData(3);
            } else if(menu.program == 3) {
                menu.program = 4;
            }
        break;
        case 14://数字键4
            MenuSetAutData(4);
        break;
        case 13://数字键5
            if(menu.program == 0) {
                MenuSetAutData(5);
            } else if(menu.program == 2) {
                menu.program = 3;
            } else {
                
            }
        break;
        case 12://数字键6
            if(menu.program == 0) {
                MenuSetAutData(6);
            } else if(menu.program == 3) {
                menu.program = 5;//特殊设置模式
            }
        break;
        case 9://数字键7
            if(menu.program == 0) {
                MenuSetAutData(7);
            } else if(menu.program == 1) {
                menu.program = 2;
            } else {
                
            }
        break;
        case 8://数字键8
            MenuSetAutData(8);
        break;
        case 7://数字键9
            MenuSetAutData(9);
        break;
        default:
        break;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuShowError(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/19
************************************************************************************************************/ 
void MenuShowError(u8 cmd) {
    LcdDraw16Clear(0,4);
    LcdDraw16Clear(16,4);
    LcdDraw16Clear(16,5);
    LcdDraw16Clear(16,6);
    LcdDraw16Clear(16,7);
    LcdDraw16Clear(32,4);
    LcdDraw16Clear(48,4);
    LcdDraw16Clear(32,5);
    LcdDraw16Clear(48,5);
    LcdDraw16Clear(32,6);
    LcdDraw16Clear(48,6);
    LcdDraw16Clear(32,7);
    LcdDraw16Clear(48,7);
    if(menu.language == 0) {
        LCDString(0,0,"   SystemError",1);
    } else {
        LCDString(0,0,"    系统错误",1);
    }
    switch(cmd) {
    case 0:
        if(menu.language == 0) {
            LCDString(1,4,"PerssMoto",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"  Foreward",1);
        } else {
            LCDString(1,4,"压纸电机",1);
            LCDString(2,4,"  未接",1);
            LCDString(3,4,"霍尔信号",1);
            LCDString(3,4,"  正传",1);
        }
        break;
    case 1:
        if(menu.language == 0) {
            LCDString(1,4,"PerssMoto",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"  reversal",1);
        } else {
            LCDString(1,4,"压纸电机",1);
            LCDString(2,4,"  未接",1);
            LCDString(3,4,"霍尔信号",1);
            LCDString(3,4,"  反传",1);
        }
        break;
    case 2:
        if(menu.language == 0) {
            LCDString(1,4," CutMoto",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"  Foreward",1);
        } else {
            LCDString(1,4,"切纸电机",1);
            LCDString(2,4,"  未接",1);
            LCDString(3,4,"霍尔信号",1);
            LCDString(3,4,"  正传",1);
        }    
        break;
    case 3:
        if(menu.language == 0) {
            LCDString(1,4,"CutMoto",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"  reversal",1);
        } else {
            LCDString(1,4,"切纸电机",1);
            LCDString(2,4,"  未接",1);
            LCDString(3,4,"霍尔信号",1);
            LCDString(3,4,"  反传",1);
        }
        break;
    case 4:
        if(menu.language == 0) {
            LCDString(1,4,"stepping motor",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"BackZero",1);
        } else {
            LCDString(1,4,"步进电机",1);
            LCDString(2,4,"  未接",1);
            LCDString(3,4,"霍尔信号",1);
            LCDString(3,4,"  回零",1);
        }
        break;
    case 5:
        break;
    case 6:
        break;
    default:
        break;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuAsk(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/19
************************************************************************************************************/ 
void MenuAsk(void) { 
    if(ComGetFlag() == 0x01) {
        ComSetFlag(0x00);
        switch( ComGetDate(1) ) {
        case 0x07://退纸电机位置
            if(menu.mode == 1) {
                u16 m_dat = 0;
                m_dat = ComGetDate(2);
                m_dat |= (u16)(ComGetDate(3) << 8);
                m_dat = best_sep_aaive - m_dat;
                menu.knife_rear_size = (u16)(m_dat/15.9);
                LCDPos(0,1);
                LCDNum(menu.knife_rear_size/1000);
                LCDNum(menu.knife_rear_size/100%10);
                LCDNum(menu.knife_rear_size%100/10);
                LcdFolt(14);
                LCDNum(menu.knife_rear_size%10);  
            }
            break;
        case 0x03://切纸
            menu.cnt++;
            menu.ttl++;
            EepromWrite(8,(u8)menu.ttl);
            EepromWrite(9,(u8)(menu.ttl >> 8));
            MenuShowData(menu.mode);//显示数据
            break;
        case 0x44://有错误
            switch( ComGetDate(2) ) {
            case 0x01://压纸电机错误
                if(ComGetDate(3) == 0) {
                    ComGetDate(0);
                } else {
                    ComGetDate(1);
                }
                break;
            case 0x02://切纸电机错误
                if(ComGetDate(3) == 0) {
                    ComGetDate(2);
                } else {
                    ComGetDate(3);
                }
                break;
            case 0x03://步进电机错误
                ComGetDate(4);
                break;
            }
            break;
        default:
            break;
        }
    }
}





