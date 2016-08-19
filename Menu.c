/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�Menu.c
*
* �ļ���ʶ��V
* ժ    Ҫ���˵��ں�
*
*
* ��ǰ�汾��V1.0
* ��    �ߣ�lhb Steven
* ������ڣ�2016/3/17
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Menu.h"
#include "Led.h"
#include "Com.h"
#include "Lcd.h"
#include "Materal.h"
#include "Eeprom.h"
#include "Delay.h"
/*
����+1���Զ�����
����+2������תӢ��
����+6�����ڵ���ǳ
����+7������ѹֽʱ��
����+8�����ڳߴ粹��
����+10����С�ߴ�
����+11�����ߴ�
����+14���ϻ�����
����+�£����뵶��΢������
*/
/*
ȷ�ϼ�������ȷ�ϣ����棬����
���������������   -------------------���¼�����ɵ������ݣ��������Զ�ģʽ�����ΰ���M���ͷŷ�ɫ����޸�Mֵ��0-100���� ֱ�Ӱ� ����޸�Mֵ��0-100�飩��ֱ�Ӱ�����ѡ�������������¼��л�����
�������ѡ������ģʽ/��λ
��������޸�����ߴ�
ģʽ����ģʽ�л��� -------------------��ģʽ�������л�
���¼���ǰ��������/ѡ���
1   �������ּ�
���ܼ������ܼ�
����������¸�λ�������ͷź�����Զ���ֽ���������ʱ�䳬��2���ӣ������������ֽ����λ
*/
typedef enum {
    best_arrive = 3000,//���λ��
    best_sep_aaive = 47700,
}menudata;

typedef struct DataNode
{
    u8 language;//����                                        42
    u8 mode;//��ǰģʽ 0���Զ�ģʽ 1���ֶ�ģʽ 2�����ģʽ    10
    u8 e_line;
    u16 cnt;//һ�ο��ص�Դ���еĴ�����ÿ�ιػ��󼴹�0        
    u16 ttl;//�����ܵ���                                       8 9
    u8 push_book;//�Ƿ���Ҫ����                               11   
    u16 knife_front_size;//��ǰ�ߴ�                           12 13
    u16 knife_rear_size;//����ߴ�                            14 15
    u16 knife_rear_size_setp;//�����ߴ�                           
    u16 group_no;//�����������     
    u8 aut_m;//�����Ƿ������������
    u16 aut_e[8];//�����Զ�ģʽ�µ�һ������                 (16,17) (18,19) (20,21) (22,23) (24,25) (26,27) (28,29) (30,31)
    u16 aut_group;//�Եڼ������ݲ���
    u8 aut_m_num;//�Եڼ�λ���и���
    u16 siuare_data[2];//�ȷ�ģʽ ����                     (35,36) (37,38)
    
    u8 program;//���λ                                    
    u8 moto_vigor;//������� 0-10                          39 
    u8 mtot_cut_place;//���λ�� 0-20                      40
    u8 size_sys;//�ߴ���ʽ                                 41
    u8 clear_zero;//����                                  
    
    u8 set_pag1;//��ͨ����
} tDataNode;
/***********************************************��������*****************************************************
* ��    ��: caidan  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/17
************************************************************************************************************/ 
static tDataNode menu;
/**********************************************��������***************************************************** 
* ��������: void MenuInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��: �˵�������ʼ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/17
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
    
    menu.language = EepromRead(42);//����ģʽ
    menu.moto_vigor = EepromRead(39);//��ȡ�������
    menu.mtot_cut_place = EepromRead(40);//�����λ��΢��
    menu.size_sys = EepromRead(41);//�ߴ�
    
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
   
    menu.aut_m = 8;//���޸�����
    
    LcdDraw16(0,0,tubiao1);//ͼ��1
    LCDPos(0,1);
    LCDNum(menu.knife_rear_size/1000);
    LCDNum(menu.knife_rear_size/100%10);
    LCDNum(menu.knife_rear_size%100/10);
    LcdFolt(14);
    LCDNum(menu.knife_rear_size%10);  
        
    LcdDraw16(16,0,tubiao2);//��ͷ
    LCDString(1,1,"  0.0",1);
   
    MenuShowMode(menu.mode);//��ʾ��ǰģʽ
    
    LcdDraw16(48,0,tubiao_m);//M
    MenuShowPushFlag(menu.push_book);//��ʾ�Ƿ���Ҫ��ֽ
    
    MenuShowData(menu.mode);//��ʾ����
   
}
/**********************************************��������***************************************************** 
* ��������: void MenuSaveData(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/20
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
/**********************************************��������***************************************************** 
* ��������: void MenuShowMode(u8 md) 
* �������: u8 md 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/15
************************************************************************************************************/ 
void MenuShowMode(u8 md) { 
    switch( md ) {
        case 0://�Զ�ģʽ
            LcdDraw16(32,0,tubiao_a);
            if(menu.language == 0) {
                LcdDraw18(32,1,tubiao_auto);
            } else {
                LCDString(2,1,"�Զ�",1);
            }
            break;
        case 1://�ֶ�ģʽ
            LcdDraw16(32,0,tubiao_yuanquan);
            if(menu.language == 0) {
                LcdDraw18(32,1,tubiao_manual);
            } else {
                LCDString(2,1,"�ֶ�",1);
            }
            break;
        case 2://�ȷ�ģʽ
            LcdDraw16(32,0,tubiao_s);
            if(menu.language == 0) {
                LcdDraw18(32,1,tubiao_siuare);
            } else {
                LCDString(2,1,"�ȷ�",1);
            }
            break;
        case 3://����ģʽ
            if(menu.language == 0) {
                LCDString(2,1,"Set",1);
            } else {
                LCDString(2,1,"����",1);
            }
            break;
        case 4://ϵͳ����
            if(menu.language == 0) {
                LCDString(2,1,"Sys_Set",1);
            } else {
                LCDString(2,1,"����",1);
            }
        case 5://�ϻ�ģʽ
            if(menu.language == 0) {
                LCDString(2,1,"Age  ",1);
            } else {
                LCDString(2,1,"�ϻ�",1);
            }
            break;
        default:
            break;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuShowPushFlag(u8 pf) 
* �������: u8 pf 
* ���ز���: void  
* ��    ��: �ж��Ƿ���Ҫ��ֽ  
* ��    ��: by lhb_steven
* ��    ��: 2016/7/15
************************************************************************************************************/ 
void MenuShowPushFlag(u8 pf) { 
    if(pf == 0) {
        //����Ҫ��ֽ
        LcdDraw18(48,1,tubiao3);
    } else {
        //��Ҫ��ֽ
        LcdDraw18(48,1,tubiao3_arrow);
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuShowData(u8 md) 
* �������: u8 md 
* ���ز���: void  
* ��    ��: ���ݵ���ģʽ��ʾ��Ӧ����  
* ��    ��: by lhb_steven
* ��    ��: 2016/7/15
************************************************************************************************************/ 
void MenuShowData(u8 md) {
    switch( md ) {
        case 0://�Զ�ģʽ
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
                if(menu.aut_group == show_i) {//��һ��
                    if(menu.aut_m == 0) {//�޸�ģʽ
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
        case 1://�ֶ�ģʽ
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
        case 2://�ȷ�ģʽ
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
                if(menu.aut_group == show_i) {//��һ��
                    if(menu.aut_m == 0) {//�޸�ģʽ
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
        case 3://��������ģʽ
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
                LCDString(0,0,"��������:       ",1);
                LCDString(2,0,"�ߴ���ʽ:",1);
                
                LCDString(1,2,"����",1);
                
                if(menu.size_sys == 0) {
                    LCDString(3,1,"���ƣ�MM��  ",1);
                } else {
                    LCDString(3,1,"Ӣ�ƣ�Mile��",1);
                }
            }

            switch(menu.set_pag1) {
            case 0:
                LCDString(1,7,"��",1);
                LCDString(3,7,"  ",1);
                break;
            case 1:
                LCDString(1,7,"  ",1);
                LCDString(3,7,"��",1);
                break;
            case 2:
                LCDString(1,7,"��",1);
                break;
            case 3:
                LCDString(3,7,"��",1);
                break;
            default:
                break;
            }
       
            break;
        case 4://��������
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
                LCDString(0,0,"    ϵͳ����",1);
                
                LCDString(1,0,"�������:",1);
                LCDNum(menu.moto_vigor/10%10);
                LCDNum(menu.moto_vigor%10);
                
                LCDString(2,0,"��λ�õ�:",1);
                LCDNum(menu.mtot_cut_place/10%10);
                LCDNum(menu.mtot_cut_place%10);
                
                LCDString(3,0,"��������:",1);
                if(menu.clear_zero == 0) {
                    LCDString(3,5,"��",1);
                } else {
                    LCDString(3,5,"��",1);
                }
            }
            
            switch(menu.set_pag1) {
            case 0:
                LCDString(3,7,"  ",1);
                LCDString(2,7,"  ",1);
                LCDString(1,7,"��",1);
                break;
            case 1:
                LCDString(1,7,"  ",1);
                LCDString(3,7,"  ",1);
                LCDString(2,7,"��",1);
                break;
            case 2:
                LCDString(1,7,"  ",1);
                LCDString(2,7,"  ",1);
                LCDString(3,7,"��",1);
                break;
            case 3:
                LCDString(1,7,"��",1);
                break;
            case 4:
                LCDString(2,7,"��",1);
                break;
            case 5:
                LCDString(3,7,"��",1);
                break;
            default:
                break;
            }
            break;
        case 5://�ϻ�ģʽ
            LcdDrawClear();
            LCDString(0,0,"            ",1);
            LCDString(1,0,"            ",1);
            LCDString(2,0,"     ......   ",1);
            LCDString(3,0,"            ",1);
            if(menu.language == 0) {
                LCDString(1,0,"    AGING MODE",1);
            } else {
                LCDString(1,0,"    �ϻ�ģʽ",1);
            }
            break;
        default:
            break;
    }
}
/**********************************************��������***************************************************** 
* ��������: u8 MenuGetMode(void) 
* �������: void 
* ���ز���: u8  
* ��    ��: ģʽ״̬  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/26
************************************************************************************************************/ 
u8 MenuGetMode(void) { 
    return menu.mode;
}
/**********************************************��������***************************************************** 
* ��������: void MenuSetAutData(u8 num) 
* �������: u8 num 
* ���ز���: void  
* ��    ��: ����e ���޸�����  
* ��    ��: by lhb_steven
* ��    ��: 2016/7/20
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
                MenuSaveData();//��������
            }
            menu.aut_e[menu.aut_group] = aut_h0*1000;
            menu.aut_e[menu.aut_group] += aut_h1*100;
            menu.aut_e[menu.aut_group] += aut_h2*10;
            menu.aut_e[menu.aut_group] += aut_h3;
            if(menu.aut_e[menu.aut_group] > best_arrive) {
                menu.aut_e[menu.aut_group] = best_arrive;
            }
        }
    } else if(menu.mode == 1) {//�ֶ�
    
    } else if(menu.mode == 2) {//�ȷ�
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
                MenuSaveData();//��������
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
    } else if(menu.mode == 3) {//��ͨ����
        
    } else if(menu.mode == 4){//ϵͳ����
    
    }
    MenuShowData(menu.mode);//��ʾ����
}
/**********************************************��������***************************************************** 
* ��������: void MenuModeSet(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��: ������Ӧģʽ������Ӧ����  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/17
************************************************************************************************************/ 
void MenuModeSet(u8 cmd) { 
    switch( cmd ) {
        case 11://ģʽ����
            if( (menu.mode == 3)||(menu.mode == 4)||(menu.mode == 5) ) {//�������
                LCDString(0,0,"                ",1);
                LCDString(1,0,"                ",1);
                LCDString(2,0,"                ",1);
                LCDString(3,0,"                ",1);
                
                LcdDraw16(0,0,tubiao1);//ͼ��1
                LCDPos(0,1);
                LCDNum(menu.knife_rear_size/1000);
                LCDNum(menu.knife_rear_size/100%10);
                LCDNum(menu.knife_rear_size%100/10);
                LcdFolt(14);
                LCDNum(menu.knife_rear_size%10);  
                    
                LcdDraw16(16,0,tubiao2);//��ͷ
                LCDString(1,1,"  0.0",1);
                
                LcdDraw16(48,0,tubiao_m);//M
                MenuShowPushFlag(menu.push_book);//��ʾ�Ƿ���Ҫ��ֽ
                ComSend(0xa1,0x00,0x00,0x00);//�˳��ϻ�ģʽ
            }
            if(menu.mode < 2) {
                menu.mode++;
            } else {
                menu.mode = 0;
            }
            menu.aut_group = 0;
            menu.set_pag1 = 0;
            MenuShowMode(menu.mode);//��ʾ��ǰģʽ
            MenuShowData(menu.mode);//��ʾ����
            menu.knife_rear_size_setp = (u16)(( (best_arrive-menu.knife_rear_size) * (best_sep_aaive/best_arrive)));
            if(menu.push_book == 0) {
                ComSend(0x04,menu.mode,(u8)(menu.knife_rear_size_setp),(u8)(menu.knife_rear_size_setp >> 8));
            } else {
                ComSend(0x05,menu.mode,(u8)(menu.knife_rear_size_setp),(u8)(menu.knife_rear_size_setp >> 8));
            }
            if(menu.mode == 2) {//�ȷ�ģʽ
                DelayMs(20);
                ComSend(0xb0,(u8)(menu.siuare_data[0]/menu.siuare_data[1]),(u8)(menu.siuare_data[1]),(u8)(menu.siuare_data[1] >> 8));
            }
            MenuSaveData();
        break;
        case 22://ȷ�ϰ���
            if(menu.mode == 0) {//�Զ�ģʽ
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
            } else if(menu.mode == 3) {//��ͨ����ģʽ
                switch(menu.set_pag1) {
                case 0:
                    menu.set_pag1 = 2;
                    break;
                case 1:
                    menu.set_pag1 = 3;
                    break;
                case 2:
                    menu.set_pag1 = 0;
                    //�������� ѡ��
                    EepromWrite(42,menu.language);
                    break;
                case 3:
                    menu.set_pag1 = 1;
                    //���� �ߴ�ֻ��
                    EepromWrite(41,menu.size_sys);
                    break;
                default:
                    break;
                }
                MenuShowData(menu.mode);//��ʾ����
            } else if(menu.mode == 4) {//ϵͳ����ģʽ
                switch(menu.set_pag1) {
                case 3://�������
                    EepromWrite(39,menu.moto_vigor);
                    ComSend(0x08,menu.moto_vigor,0x00,0x00);
                    break;
                case 4://��λ��
                     EepromWrite(40,menu.mtot_cut_place);
                     ComSend(0x0a,menu.mtot_cut_place,0x00,0x00);
                    break;
                case 5://����������
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
                MenuShowData(menu.mode);//��ʾ����
            }
        break;
        case 0://���� ��
            if(menu.mode == 0) {//�Զ�ģʽ
                if(menu.aut_group > 0) {
                    menu.aut_group--;
                } else {
                    menu.aut_group = 5;
                }
                
            } else if(menu.mode == 3) {//��ͨ����ģʽ
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
            } else if(menu.mode == 2) {//�ȷ�ģʽ
                if(menu.aut_group > 0) {
                    menu.aut_group--;
                } else {
                    menu.aut_group = 1;
                }
            } else if(menu.mode == 4) {//ϵͳ����ģʽ
                switch(menu.set_pag1) {
                case 3://�������
                    if(menu.moto_vigor > 1) {
                        menu.moto_vigor--;
                    } else {
                        menu.moto_vigor = 9;
                    }
                    break;
                case 4://��λ������
                    if(menu.mtot_cut_place > 1) {
                        menu.mtot_cut_place--;
                    } else {
                        menu.mtot_cut_place = 9;
                    }
                    break;
                case 5://����������
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
            MenuShowData(menu.mode);//��ʾ����
        break;
        case 5://���� ��
            if(menu.mode == 0) {//�Զ�ģʽ
                if(menu.aut_group < 5) {
                    menu.aut_group++;
                } else {
                    menu.aut_group = 0;
                }
                
            } else if(menu.mode == 3) {//��ͨ����ģʽ
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
            } else if(menu.mode == 2) {//�ȷ�ģʽ
                if(menu.aut_group < 1) {
                    menu.aut_group++;
                } else {
                    menu.aut_group = 0;
                }
            } else if(menu.mode == 4) {//ϵͳ����ģʽ
                switch(menu.set_pag1) {
                case 3://�������
                    if(menu.moto_vigor < 9) {
                        menu.moto_vigor++;
                    } else {
                        menu.moto_vigor = 1;
                    }
                    break;
                case 4://��λ������
                    if(menu.mtot_cut_place < 9) {
                        menu.mtot_cut_place++;
                    } else {
                        menu.mtot_cut_place = 1;
                    }
                    break;
                case 5://����������
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
            MenuShowData(menu.mode);//��ʾ����
        break;
        case 6://���䰴����
            if(menu.mode == 0) {//�Զ�ģʽ
                if(menu.aut_m == 0) {
                    menu.aut_m = 8;
                } else {
                    menu.aut_m = 0;
                }
                MenuShowData(menu.mode);//��ʾ����
            } else if(menu.mode == 2) {//�ȷ�ģʽ
                if(menu.aut_m == 0) {
                    menu.aut_m = 8;
                } else {
                    menu.aut_m = 0;
                }
                MenuShowData(menu.mode);//��ʾ����
            }
        break;
        case 21://������̰�
            if(menu.push_book == 0) {
                menu.push_book = 1;
            } else {
                menu.push_book = 0;
            }
            MenuShowPushFlag(menu.push_book);
        break;
        case 50://���鰴������
            ComSend(0x06,0x00,0x00,0x00);//��λ
        break;
        case 60://���ܼ�����
            menu.program = 1;//������ģʽ
            break;
        case 16://���ܼ��ſ�
            if(menu.program == 4) {
                menu.program = 0;
                menu.mode = 3;
                MenuShowMode(menu.mode);//��ʾ��ǰģʽ
                MenuShowData(menu.mode);//��ʾ����
            } else if(menu.program == 5){
                menu.program = 0;
                menu.mode = 4;
                MenuShowMode(menu.mode);//��ʾ��ǰģʽ
                MenuShowData(menu.mode);//��ʾ����
            } else if(menu.program == 22) {//�ϻ�ģʽ
                menu.program = 0;
                menu.mode = 5;//�ϻ�ģʽ
                MenuShowMode(menu.mode);//��ʾ��ǰģʽ
                MenuShowData(menu.mode);//��ʾ����
                ComSend(0xa1,0x01,0x00,0x00);//�ϻ�
            } else {
                menu.program = 0;
            }
            break;
        case 23://���ּ�0
            MenuSetAutData(0);
        break;
        case 19://���ּ�1
            MenuSetAutData(1);
        break;
        case 18://���ּ�2
            if(menu.program == 0) {
                MenuSetAutData(2);
            } else if(menu.program == 3) {
                menu.program = 22;
            }
        break;
        case 17://���ּ�3
            if(menu.program == 0) {
                MenuSetAutData(3);
            } else if(menu.program == 3) {
                menu.program = 4;
            }
        break;
        case 14://���ּ�4
            MenuSetAutData(4);
        break;
        case 13://���ּ�5
            if(menu.program == 0) {
                MenuSetAutData(5);
            } else if(menu.program == 2) {
                menu.program = 3;
            } else {
                
            }
        break;
        case 12://���ּ�6
            if(menu.program == 0) {
                MenuSetAutData(6);
            } else if(menu.program == 3) {
                menu.program = 5;//��������ģʽ
            }
        break;
        case 9://���ּ�7
            if(menu.program == 0) {
                MenuSetAutData(7);
            } else if(menu.program == 1) {
                menu.program = 2;
            } else {
                
            }
        break;
        case 8://���ּ�8
            MenuSetAutData(8);
        break;
        case 7://���ּ�9
            MenuSetAutData(9);
        break;
        default:
        break;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuShowError(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/19
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
        LCDString(0,0,"    ϵͳ����",1);
    }
    switch(cmd) {
    case 0:
        if(menu.language == 0) {
            LCDString(1,4,"PerssMoto",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"  Foreward",1);
        } else {
            LCDString(1,4,"ѹֽ���",1);
            LCDString(2,4,"  δ��",1);
            LCDString(3,4,"�����ź�",1);
            LCDString(3,4,"  ����",1);
        }
        break;
    case 1:
        if(menu.language == 0) {
            LCDString(1,4,"PerssMoto",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"  reversal",1);
        } else {
            LCDString(1,4,"ѹֽ���",1);
            LCDString(2,4,"  δ��",1);
            LCDString(3,4,"�����ź�",1);
            LCDString(3,4,"  ����",1);
        }
        break;
    case 2:
        if(menu.language == 0) {
            LCDString(1,4," CutMoto",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"  Foreward",1);
        } else {
            LCDString(1,4,"��ֽ���",1);
            LCDString(2,4,"  δ��",1);
            LCDString(3,4,"�����ź�",1);
            LCDString(3,4,"  ����",1);
        }    
        break;
    case 3:
        if(menu.language == 0) {
            LCDString(1,4,"CutMoto",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"  reversal",1);
        } else {
            LCDString(1,4,"��ֽ���",1);
            LCDString(2,4,"  δ��",1);
            LCDString(3,4,"�����ź�",1);
            LCDString(3,4,"  ����",1);
        }
        break;
    case 4:
        if(menu.language == 0) {
            LCDString(1,4,"stepping motor",1);
            LCDString(2,4,"  NoLink",1);
            LCDString(3,4,"Hoaer",1);
            LCDString(3,4,"BackZero",1);
        } else {
            LCDString(1,4,"�������",1);
            LCDString(2,4,"  δ��",1);
            LCDString(3,4,"�����ź�",1);
            LCDString(3,4,"  ����",1);
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
/**********************************************��������***************************************************** 
* ��������: void MenuAsk(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/19
************************************************************************************************************/ 
void MenuAsk(void) { 
    if(ComGetFlag() == 0x01) {
        ComSetFlag(0x00);
        switch( ComGetDate(1) ) {
        case 0x07://��ֽ���λ��
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
        case 0x03://��ֽ
            menu.cnt++;
            menu.ttl++;
            EepromWrite(8,(u8)menu.ttl);
            EepromWrite(9,(u8)(menu.ttl >> 8));
            MenuShowData(menu.mode);//��ʾ����
            break;
        case 0x44://�д���
            switch( ComGetDate(2) ) {
            case 0x01://ѹֽ�������
                if(ComGetDate(3) == 0) {
                    ComGetDate(0);
                } else {
                    ComGetDate(1);
                }
                break;
            case 0x02://��ֽ�������
                if(ComGetDate(3) == 0) {
                    ComGetDate(2);
                } else {
                    ComGetDate(3);
                }
                break;
            case 0x03://�����������
                ComGetDate(4);
                break;
            }
            break;
        default:
            break;
        }
    }
}





