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
* 完成日期：2016/7/12
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Materal.h"


const u8 tubiao1[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x16  --*/
0x00,0x00,0x3F,0xFC,0x40,0x02,0x5F,0x72,0x4F,0x7A,0x5F,0x7A,0x4F,0x7A,0x5F,0x7A,
0x4F,0x7A,0x5F,0x72,0x5F,0x62,0x4F,0x62,0x5F,0x42,0x40,0x02,0x3F,0xFC,0x00,0x00,
};
const u8 tubiao2[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x16  --*/
0x00,0x00,0x3F,0xFC,0x40,0x02,0x40,0x02,0x42,0x02,0x46,0x02,0x4E,0x02,0x5F,0xFA,
0x5F,0xFA,0x4E,0x02,0x46,0x02,0x42,0x02,0x40,0x02,0x40,0x02,0x3F,0xFC,0x00,0x00,
};
const u8 tubiao_s[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x16  --*/
0x00,0x00,0x3F,0xFC,0x40,0x02,0x43,0x82,0x47,0xC2,0x46,0x02,0x46,0x02,0x43,0x82,
0x41,0x82,0x40,0xC2,0x46,0xC2,0x46,0xC2,0x43,0x82,0x40,0x02,0x3F,0xFC,0x00,0x00,
};
const u8 tubiao_a[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x16  --*/
0x00,0x00,0x3F,0xFC,0x40,0x02,0x41,0x82,0x41,0x82,0x42,0x42,0x42,0x42,0x46,0x62,
0x47,0xE2,0x47,0xE2,0x4C,0x32,0x4C,0x32,0x4C,0x32,0x40,0x02,0x3F,0xFC,0x00,0x00,
};
const u8 tubiao_m[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x16  --*/
0x00,0x00,0x3F,0xFC,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x58,0x1A,0x54,0x2A,
0x52,0x4A,0x51,0x8A,0x51,0x8A,0x40,0x02,0x40,0x02,0x40,0x02,0x3F,0xFC,0x00,0x00,
};
const u8 tubiao_yuanquan[] = {
/*--  调入了一幅图像：D:\01-Project\35-Cutter_Count\data\标签2.bmp  --*/
/*--  宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x3F,0xFC,0x41,0x82,0x41,0x82,0x43,0xC2,0x45,0xA2,0x49,0x92,
0x49,0x92,0x45,0xA2,0x43,0xC2,0x41,0x82,0x41,0x82,0x3F,0xFC,0x00,0x00,0x00,0x00,
};
const u8 tubiao3[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=32x16  --*/
0x00,0x00,0x00,0x00,0x3E,0x00,0x0F,0x00,0x0E,0x00,0x0F,0x00,0x3E,0x00,0x0F,0x00,
0x0E,0x00,0x0F,0x00,0x3E,0x00,0x0F,0x00,0x0E,0x00,0x0F,0x00,0x3E,0xFF,0xEF,0x3F,
0x0E,0xA2,0x2F,0x61,0x3E,0x91,0x2E,0x92,0x0E,0x88,0xAD,0x0C,0x3E,0xFF,0xEB,0xF8,
0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,
};
const u8 tubiao3_arrow[] = {
/*--  调入了一幅图像：D:\01-Project\35-Cutter_Count\data\标签3.bmp  --*/
/*--  宽度x高度=32x16  --*/
0x00,0x00,0x00,0x00,0x3E,0x00,0x0F,0x00,0x0E,0x01,0x0F,0x00,0x3E,0x01,0x8F,0x00,
0x0E,0x03,0xCF,0x00,0x3E,0xFF,0xEF,0x00,0x0E,0x00,0x0F,0x00,0x3E,0xFF,0xEF,0x3F,
0x0E,0xA2,0x2F,0x61,0x3E,0x91,0x2E,0x92,0x0E,0x88,0xAD,0x0C,0x3E,0xFF,0xEB,0xF8,
0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFE,0x40,0x00,0x00,0x02,0x7F,0xFF,0xFF,0xFE,
};
const u8 tubiao_auto[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=32x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x08,0x00,0x00,0x00,0x14,0x42,0xFC,0x70,0x14,0x42,0x30,0x88,0x22,0x42,0x30,0x88,
0x3E,0x42,0x30,0x88,0x3E,0x24,0x30,0x88,0x22,0x24,0x30,0x88,0x63,0x18,0x30,0x70,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const u8 tubiao_ruler[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x1F,0xE0,0x1F,0x00,0x1F,0xE0,0x1F,0xC0,0x1F,0xE0,0x1F,0x00,
0x1F,0xE0,0x1F,0x80,0x1F,0xE0,0x1F,0x00,0x1F,0xE0,0x1F,0x80,0x1F,0xE0,0x00,0x00,
};
const u8 tubiao_manual[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=32x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x44,0xA4,0x44,0x38,0xA4,0xA4,0xA4,
0x54,0xE6,0xA4,0xE4,0x55,0x15,0xA5,0x14,0x55,0x14,0x99,0x17,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const u8 tubiao_siuare[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=32x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x65,0x22,0x39,0xE0,0x95,0x22,0x25,0x00,0x85,0x22,0x29,0x00,
0x65,0x25,0x31,0xE0,0x15,0x27,0x31,0x00,0x95,0x28,0xA9,0x00,0x64,0xC8,0xA5,0xE0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const u8 num_s[] = {
/*--  调入了一幅图像：这是您新建的图像  0--*/
/*--  宽度x高度=8x8  --*/
0x00,0x18,0x24,0x24,0x24,0x24,0x18,0x00,
/*--  调入了一幅图像：这是您新建的图像  1--*/
/*--  宽度x高度=8x8  --*/
0x00,0x08,0x38,0x18,0x18,0x18,0x3C,0x00,
/*--  调入了一幅图像：这是您新建的图像  2--*/
/*--  宽度x高度=8x8  --*/
0x00,0x38,0x44,0x08,0x10,0x20,0x7C,0x00,
/*--  调入了一幅图像：这是您新建的图像  3--*/
/*--  宽度x高度=8x8  --*/
0x00,0x78,0x6C,0x0C,0x38,0x0C,0x6C,0x7C,
/*--  调入了一幅图像：这是您新建的图像  4--*/
/*--  宽度x高度=8x8  --*/
0x00,0x18,0x38,0x28,0x68,0x7C,0x08,0x3C,
/*--  调入了一幅图像：这是您新建的图像  5--*/
/*--  宽度x高度=8x8  --*/
0x00,0x7C,0x60,0x7C,0x6C,0x0C,0x6C,0x78,
/*--  调入了一幅图像：这是您新建的图像  6--*/
/*--  宽度x高度=8x8  --*/
0x00,0x3C,0x60,0x7C,0x6C,0x6C,0x6C,0x3C,
/*--  调入了一幅图像：这是您新建的图像  7--*/
/*--  宽度x高度=8x8  --*/
0x00,0x3E,0x36,0x0C,0x0C,0x18,0x18,0x18,
/*--  调入了一幅图像：这是您新建的图像  8--*/
/*--  宽度x高度=8x8  --*/
0x00,0x7C,0x6C,0x6C,0x78,0x6C,0x6C,0x7C,
/*--  调入了一幅图像：这是您新建的图像  9--*/
/*--  宽度x高度=8x8  --*/
0x00,0x78,0x6C,0x6C,0x6C,0x7C,0x1C,0x78,
/*--  调入了一幅图像：这是您新建的图像  .--*/
/*--  宽度x高度=8x8  --*/
0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=8x8  --*/
0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=8x8  --*/
0x00,0x06,0x1E,0x7E,0x7E,0x1E,0x06,0x00,//箭头

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//空白
};
const u8 f_e[] = {
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x8  --*/
0xF8,0x00,0x8B,0x90,0xBA,0xB0,0x8A,0x90,0xBA,0x90,0x8A,0x90,0xFB,0xB8,0x00,0x00,
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x8  --*/
0xF8,0x00,0x8B,0x98,0xBA,0xA4,0x8A,0x88,0xBA,0x90,0x8A,0xA4,0xFB,0xBC,0x00,0x00,
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x8  --*/
0xF8,0x00,0x8B,0x98,0xBA,0xA4,0x8A,0x88,0xBA,0x84,0x8A,0xA4,0xFB,0x98,0x00,0x00,
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x8  --*/
0xF8,0x00,0x8B,0x88,0xBA,0x98,0x8A,0xA8,0xBA,0xBC,0x8A,0x88,0xFB,0x9C,0x00,0x00,
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x8  --*/
0xF8,0x00,0x8B,0xBC,0xBA,0xA0,0x8A,0xB8,0xBA,0x84,0x8A,0xA4,0xFB,0xB8,0x00,0x00,
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x8  --*/
0xF8,0x00,0x8B,0x98,0xBA,0xA0,0x8A,0xB8,0xBA,0xA4,0x8A,0xA4,0xFB,0x98,0x00,0x00,
/*--  调入了一幅图像：D:\01-Project\35-Cutter_Count\data\E01.bmp  --*/
/*--  宽度x高度=16x8  --*/
0x00,0x00,0x64,0xBE,0x96,0x88,0x86,0x88,0x85,0x88,0x95,0x88,0x65,0x88,0x00,0x00,
/*--  调入了一幅图像：这是您新建的图像  --*/
/*--  宽度x高度=16x8  --*/
0x00,0x00,0x77,0x40,0x22,0x40,0x22,0x40,0x22,0x40,0x22,0x70,0x00,0x00,0x00,0x00,
/*--  调入了一幅图像：D:\01-Project\35-Cutter_Count\data\E01.bmp  --*///从这里开始是反选
/*--  宽度x高度=16x8  --*/
0x00,0x00,0x7B,0x90,0x42,0xB0,0x72,0x90,0x42,0x90,0x7A,0x90,0x03,0xB8,0x00,0x00,
/*--  调入了一幅图像：D:\01-Project\35-Cutter_Count\data\E02.bmp  --*/
/*--  宽度x高度=16x8  --*/
0x00,0x00,0x73,0x98,0x42,0xA4,0x72,0x88,0x42,0x90,0x72,0xA4,0x03,0xBC,0x00,0x00,
/*--  调入了一幅图像：D:\01-Project\35-Cutter_Count\data\E03.bmp  --*/
/*--  宽度x高度=16x8  --*/
0x00,0x00,0x73,0x98,0x42,0xA4,0x72,0x88,0x42,0x84,0x72,0xA4,0x03,0x98,0x00,0x00,
/*--  调入了一幅图像：D:\01-Project\35-Cutter_Count\data\E04.bmp  --*/
/*--  宽度x高度=16x8  --*/
0x00,0x00,0x73,0x98,0x42,0x98,0x72,0xA8,0x42,0xBC,0x72,0x88,0x03,0x9C,0x00,0x00,
/*--  调入了一幅图像：D:\01-Project\35-Cutter_Count\data\E05.bmp  --*/
/*--  宽度x高度=16x8  --*/
0x00,0x00,0x73,0xBC,0x42,0xA0,0x72,0xB8,0x42,0x84,0x72,0xA4,0x03,0xB8,0x00,0x00,
/*--  调入了一幅图像：D:\01-Project\35-Cutter_Count\data\E06.bmp  --*/
/*--  宽度x高度=16x8  --*/
0x00,0x00,0x73,0x98,0x42,0xA0,0x72,0xB8,0x42,0xA4,0x72,0xA4,0x03,0x98,0x00,0x00,
};

