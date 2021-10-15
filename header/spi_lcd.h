/*
 * spi_lcd.h
 *
 *  Created on: 2021��7��31��
 *      Author: sundm
 */

#ifndef SPI_LCD_H_
#define SPI_LCD_H_

#include <string.h>
#include <stdint.h>
#include <stddef.h>

/* POSIX Header files */
#include <pthread.h>
#include <unistd.h>

#ifdef WHITE_LCD
#undef WHITE_LCD
#endif
#define WHITE_LCD 0xff

#ifdef BLACK_LCD
#undef BLACK_LCD
#endif
#define BLACK_LCD 0

//����д�ֱʵ���ɫ
#define TFT_PENCOLOR    RED

//���屳����ɫ
#define TFT_BGCOLOR     WHITE_LCD

void lcd_init(void);
void lcd_clear(uint16_t color);
void lcd_drawpoint(uint16_t x,uint16_t y,uint16_t color);
void lcd_showchar(uint16_t x,uint16_t y,const int8_t dat);
void lcd_showstr(uint16_t x,uint16_t y, int8_t dat[]);
void lcd_showint8(uint16_t x,uint16_t y,int8_t dat);
void lcd_showuint8(uint16_t x,uint16_t y,uint8_t dat);
void lcd_showint16(uint16_t x,uint16_t y,int16_t dat);
void lcd_showuint16(uint16_t x,uint16_t y,uint16_t dat);
void lcd_showint32(uint16_t x,uint16_t y,int32_t dat,uint8_t num);
void lcd_showfloat(uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum);
void showimage(const unsigned char *p);
void lcd_displayimage032(uint8_t *p, uint16_t width, uint16_t height);
void lcd_displayccd(uint8_t *p, uint16_t height);
void lcd_displayimage032_zoom(uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);
void lcd_displayimage8660_zoom(uint16_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);
void lcd_displayimage7725(uint8_t *p, uint16_t width, uint16_t height);
void lcd_display_chinese(uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number, uint16_t color);
void lcd_clear_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

#endif /* SPI_LCD_H_ */
