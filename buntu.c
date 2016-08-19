/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：buntu.h
*
* 文件标识：
* 摘    要：
*
*
* 当前版本：v1.1
* 作    者：lhb Steven
* 完成日期：2016/3/14
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "buntu.h"
#include "Delay.h"
#include "Time.h"

#define BU_X1 PD_ODR_ODR3
#define BU_X2 PD_ODR_ODR2
#define BU_X3 PD_ODR_ODR0  
#define BU_X4 PC_ODR_ODR7
#define BU_X5 PC_ODR_ODR6

#define BU_Y1 PC_IDR_IDR5
#define BU_Y2 PC_IDR_IDR4
#define BU_Y3 PC_IDR_IDR3
#define BU_Y4 PC_IDR_IDR2
#define BU_Y5 PC_IDR_IDR1

#define BU_DE_Q 60000
#define BU_DE_H 1500

#define BU_EN 1

/**********************************************函数定义***************************************************** 
* 函数名称: void BuntuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 按键初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/14
************************************************************************************************************/ 
void BuntuInit(void) {     
    PD_DDR_DDR3 = 1;
    PD_CR1_C13 = 1;
    PD_CR2_C23 = 0;
    
    PD_DDR_DDR2 = 1;
    PD_CR1_C12 = 1;
    PD_CR2_C22 = 0;
    
    PD_DDR_DDR0 = 1;
    PD_CR1_C10 = 1;
    PD_CR2_C20 = 0;
    
    PC_DDR_DDR6 = 1;
    PC_CR1_C16 = 1;
    PC_CR2_C26 = 0;
    
    PC_DDR_DDR7 = 1;
    PC_CR1_C17 = 1;
    PC_CR2_C27 = 0;
    //----------------------------------
    PC_DDR_DDR5 = 0;
    PC_CR1_C15 = 0;
    PC_CR2_C25 = 0;
    
    PC_DDR_DDR4 = 0;
    PC_CR1_C14 = 0;
    PC_CR2_C24 = 0;
    
    PC_DDR_DDR3 = 0;
    PC_CR1_C13 = 0;
    PC_CR2_C23 = 0;
    
    PC_DDR_DDR2 = 0;
    PC_CR1_C12 = 0;
    PC_CR2_C22 = 0;
    
    PC_DDR_DDR1 = 0;
    PC_CR1_C11 = 0;
    PC_CR2_C21 = 0;
    
    BU_X1 = 0;
    BU_X2 = 0;
    BU_X3 = 0;
    BU_X4 = 0;
    BU_X5 = 0;
}
/**********************************************函数定义***************************************************** 
* 函数名称: BuntuSetX static u8(u8 num) 
* 输入参数: u8 num 
* 返回参数: BuntuSetX  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/11
************************************************************************************************************/ 
static void BuntuSetX(u8 num) { 
    switch(num) {
        case 0:
            BU_X5 = 0;
            BU_X1 = 1;
        break;
        case 1:
            BU_X1 = 0;
            BU_X2 = 1;
        break;
        case 2:
            BU_X2 = 0;
            BU_X3 = 1;
        break;
        case 3:
            BU_X3 = 0;
            BU_X4 = 1;
        break;
        case 4:
            BU_X4 = 0;
            BU_X5 = 1;
        break;
        default:
            BU_X1 = 0;
            BU_X2 = 0;
            BU_X3 = 0;
            BU_X4 = 0;
            BU_X5 = 0;
            break;
    }
}

u8 buntu_bit[25];
u8 buntu_bit_h[25];
/**********************************************函数定义***************************************************** 
* 函数名称: u8 BuntuRead(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能: 读取按键状态  
* 作    者: by lhb_steven
* 日    期: 2016/3/14
************************************************************************************************************/ 
u8 BuntuRead(void) { 
    static u16 bun_count[25];
    for(u8 i = 0;i < 5;i++ ) {
        u8 bit = 0;
        bit = i*5;
        BuntuSetX(i);
        //按键1
        if(BU_Y1 == BU_EN) {
            if(bun_count[0+bit] < BU_DE_Q) {
                bun_count[0+bit]++;
                if(bun_count[0+bit] == BU_DE_H) {
                    buntu_bit[0+bit] = 1;
                }
            }
        } else {
            if(bun_count[0+bit] > BU_DE_H) {
                bun_count[0+bit] = 0;
                return (0+bit);
            }
            bun_count[0+bit] = 0;
        }
        //按键2
        if(BU_Y2 == BU_EN) {
            if(bun_count[1+bit] < BU_DE_Q) {
                bun_count[1+bit]++;
                if(bun_count[1+bit] == BU_DE_H) {
                    buntu_bit[1+bit] = 1;
                }
                if((1+bit) == 21) {
                    if(bun_count[21] == 50000) {
                        return 50;
                    }
                } else if((1+bit) == 16) {
                    if(bun_count[16] == 5000) {
                        return 60;
                    }
                }
            }
        } else {
            if( (bun_count[1+bit] > BU_DE_H) && (bun_count[1+bit] < 50000) ) {
                if((1+bit) == 16) {
                    if(bun_count[19] > BU_DE_H) {
                    
                    } else if(bun_count[18] > BU_DE_H) {
                    
                    } else if(bun_count[12] > BU_DE_H) {
                    
                    } else if(bun_count[9] > BU_DE_H) {
                    
                    } else if(bun_count[8] > BU_DE_H) {
                    
                    } else if(bun_count[13] > BU_DE_H) {
                    
                    } else if(bun_count[12] > BU_DE_H) {
                    
                    } else if(bun_count[23] > BU_DE_H) {
                    
                    } else {}
                }
                bun_count[1+bit] = 0;
                return (1+bit);
            }
            bun_count[1+bit] = 0;
        }
        //按键3
        if(BU_Y3 == BU_EN) {
            if(bun_count[2+bit] < BU_DE_Q) {
                bun_count[2+bit]++;
                if(bun_count[2+bit] == BU_DE_H) {
                    buntu_bit[2+bit] = 1;
                }
            }
        } else {
            if(bun_count[2+bit] > BU_DE_H) {
                bun_count[2+bit] = 0;
                return (2+bit);
            }
            bun_count[2+bit] = 0;
        }
        //按键4
        if(BU_Y4 == BU_EN) {
            if(bun_count[3+bit] < BU_DE_Q) {
                bun_count[3+bit]++;
                if(bun_count[3+bit] == BU_DE_H) {
                    buntu_bit[3+bit] = 1;
                }
            }
        } else {
            if(bun_count[3+bit] > BU_DE_H) {
                bun_count[3+bit] = 0;
                return (3+bit);
            }
            bun_count[3+bit] = 0;
        }
        //按键5
        if(BU_Y5 == BU_EN) {
            if(bun_count[4+bit] < BU_DE_Q) {
                bun_count[4+bit]++;
                if(bun_count[4+bit] == BU_DE_H) {
                    buntu_bit[4+bit] = 1;
                }
            }
        } else {
            if(bun_count[4+bit] > BU_DE_H) {
                bun_count[4+bit] = 0;
                return (4+bit);
            }
            bun_count[4+bit] = 0;
        }
    }
    return 0x30;
}


