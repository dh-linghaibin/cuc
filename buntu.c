/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�buntu.h
*
* �ļ���ʶ��
* ժ    Ҫ��
*
*
* ��ǰ�汾��v1.1
* ��    �ߣ�lhb Steven
* ������ڣ�2016/3/14
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "buntu.h"
#include "Delay.h"
#include "Time.h"

#define BU_X1 PD_ODR_ODR4
#define BU_X2 PD_ODR_ODR3
#define BU_X3 PD_ODR_ODR2
#define BU_X4 PC_ODR_ODR7
#define BU_X5 PC_ODR_ODR6

#define BU_Y1 PC_IDR_IDR5
#define BU_Y2 PC_IDR_IDR4
#define BU_Y3 PC_IDR_IDR3
#define BU_Y4 PB_IDR_IDR4
#define BU_Y5 PB_IDR_IDR5

#define BU_DE_Q 2000
#define BU_DE_H 1990

/**********************************************��������***************************************************** 
* ��������: void BuntuInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��: ������ʼ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/14
************************************************************************************************************/ 
void BuntuInit(void) { 
    PD_DDR_DDR2 = 1;
    PD_CR1_C12 = 1;
    PD_CR2_C22 = 0;
    
    PD_DDR_DDR3 = 1;
    PD_CR1_C13 = 1;
    PD_CR2_C23 = 0;
    
    PD_DDR_DDR4 = 1;
    PD_CR1_C14 = 1;
    PD_CR2_C24 = 0;
    
    PC_DDR_DDR6 = 1;
    PC_CR1_C16 = 1;
    PC_CR2_C26 = 0;
    
    PC_DDR_DDR7 = 1;
    PC_CR1_C17 = 1;
    PC_CR2_C27 = 0;
    //----------------------------------
    PB_DDR_DDR4 = 0;
    PB_CR1_C14 = 0;
    PB_CR2_C24 = 0;
    
    PB_DDR_DDR5 = 0;
    PB_CR1_C15 = 0;
    PB_CR2_C25 = 0;
    
    PC_DDR_DDR3 = 0;
    PC_CR1_C13 = 0;
    PC_CR2_C23 = 0;
    
    PC_DDR_DDR4 = 0;
    PC_CR1_C14 = 0;
    PC_CR2_C24 = 0;
    
    PC_DDR_DDR5 = 0;
    PC_CR1_C15 = 0;
    PC_CR2_C25 = 0;
    
    BU_X1 = 1;
    BU_X2 = 1;
    BU_X3 = 1;
    BU_X4 = 1;
    BU_X5 = 1;
}
/**********************************************��������***************************************************** 
* ��������: BuntuSetX static u8(u8 num) 
* �������: u8 num 
* ���ز���: BuntuSetX  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/11
************************************************************************************************************/ 
static void BuntuSetX(u8 num) { 
    switch(num) {
        case 0:
            BU_X5 = 1;
            BU_X1 = 0;
        break;
        case 1:
            BU_X1 = 1;
            BU_X2 = 0;
        break;
        case 2:
            BU_X2 = 1;
            BU_X3 = 0;
        break;
        case 3:
            BU_X3 = 1;
            BU_X4 = 0;
        break;
        case 4:
            BU_X4 = 1;
            BU_X5 = 0;
        break;
        default:
            BU_X1 = 1;
            BU_X2 = 1;
            BU_X3 = 1;
            BU_X4 = 1;
            BU_X5 = 1;
            break;
    }
}

u8 buntu_bit[25];
u8 buntu_bit_h[25];
/**********************************************��������***************************************************** 
* ��������: u8 BuntuRead(void) 
* �������: void 
* ���ز���: u8  
* ��    ��: ��ȡ����״̬  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/14
************************************************************************************************************/ 
u8 BuntuRead(void) { 
    static u16 bun_count[25];
    for(u8 i = 0;i < 5;i++ ) {
        u8 bit = 0;
        bit = i*5;
        BuntuSetX(i);
        //����1
        if(BU_Y1 == 0) {
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
        //����2
        if(BU_Y2 == 0) {
            if(bun_count[1+bit] < BU_DE_Q) {
                bun_count[1+bit]++;
                if(bun_count[1+bit] == BU_DE_H) {
                    buntu_bit[1+bit] = 1;
                }
            }
        } else {
            if(bun_count[1+bit] > BU_DE_H) {
                bun_count[1+bit] = 0;
                return (1+bit);
            }
            bun_count[1+bit] = 0;
        }
        //����3
        if(BU_Y3 == 0) {
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
        //����4
        if(BU_Y4 == 0) {
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
        //����5
        if(BU_Y5 == 0) {
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


