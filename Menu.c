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
typedef struct DataNode
{
    u8 mode;//��ǰģʽ 0���Զ�ģʽ 1���ֶ�ģʽ 2�����ģʽ
    u8 e_line;
    u8 cnt;//һ�ο��ص�Դ���еĴ�����ÿ�ιػ��󼴹�0
    u8 ttl;//�����ܵ���
    u8 push_book;//�Ƿ���Ҫ����
    u16 knife_front_size;//��ǰ�ߴ�
    u16 knife_rear_size;//����ߴ�
    u16 group_no;//�����������
    
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
    menu.mode = 0;
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
            if(menu.mode == 0) {
                menu.mode = 1;
                LcdDraw16(32,0,tubiao_a);//�Զ�
                LcdDraw18(32,1,tubiao_auto);
            } else if(menu.mode == 1) {
                menu.mode = 2;
                LcdDraw16(32,0,tubiao_s);//�Զ�
                LcdDraw18(32,1,tubiao_siuare);
            } else if(menu.mode == 2) {
                menu.mode = 0;
                LcdDraw16(32,0,tubiao_m);//�Զ�
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
        case 6://���䰴����
        
        break;
        
        
        default:
        break;
    }
}






