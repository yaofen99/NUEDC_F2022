/*
 * lcd_font.h
 *
 *  Created on: 2021年7月31日
 *      Author: sundm
 */

#ifndef LCD_FONT_H_
#define LCD_FONT_H_
//-------常用颜色----------
#define RED             0xF800  //红色
#define BLUE            0x001F  //蓝色
#define YELLOW          0xFFE0  //黄色
#define GREEN           0x07E0  //绿色
#define WHITE           0xFFFF  //白色
#define BLACK           0x0000  //黑色
#define GRAY            0X8430  //灰色
#define BROWN           0XBC40  //棕色
#define PURPLE          0XF81F  //紫色
#define PINK            0XFE19  //粉色


extern const uint8_t tft_ascii[95][16];

extern const uint8_t gImage_qq[3200];

extern const uint8_t chinese_test[8][16];

extern const uint8_t oled_6x8[][6];

extern const uint8_t oled_8x16[];

extern const uint8_t oled_16x16_chinese[][16];



#endif /* LCD_FONT_H_ */
