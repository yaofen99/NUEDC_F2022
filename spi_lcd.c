#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* POSIX Header files */
#include <pthread.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/display/Display.h>
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include "includes.h"

#define SPI_MSG_LENGTH  50

unsigned char masterRxBuffer[SPI_MSG_LENGTH];
unsigned char masterTxBuffer[SPI_MSG_LENGTH];

/*=======================LCD  相关定义  ========================*/
#define DC(x)       GPIO_write(CONFIG_TFT_DC, x);
#define REST(x)     GPIO_write(CONFIG_TFT_RES, x);

//定义写字笔的颜色
#define TFT_PENCOLOR    RED

//定义背景颜色
#define TFT_BGCOLOR     WHITE

//定义显示方向
//0 竖屏模式
//1 竖屏模式  旋转180
//2 横屏模式
//3 横屏模式  旋转180
#define TFT_DISPLAY_DIR 0

#if (0==TFT_DISPLAY_DIR || 1==TFT_DISPLAY_DIR)
#define TFT_X_MAX   128 //液晶X方宽度
#define TFT_Y_MAX   160 //液晶Y方宽度

#elif (2==TFT_DISPLAY_DIR || 3==TFT_DISPLAY_DIR)
#define TFT_X_MAX   160 //液晶X方宽度
#define TFT_Y_MAX   128 //液晶Y方宽度

#else
#error "TFT_DISPLAY_DIR 定义错误"

#endif


/* Callback used for toggling the LED. */
void timerCallback(Timer_Handle myHandle);

/*=======================LCD  相关定义 结束  ========================*/

SPI_Handle masterSpi;

/*=======================LCD  函数定义  ========================*/
static void  lcd_writeIndex(uint8_t dat)         //写命令
{
    SPI_Transaction masterTransaction;
    bool transferOK;

    DC(0);

    masterTxBuffer[0] = dat;
    masterTransaction.count = 1;
    masterTransaction.txBuf = (void *)masterTxBuffer;
    masterTransaction.rxBuf = (void *)masterRxBuffer;
    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    if (transferOK) {

    }
    else {
        DEBUG_printf("Unsuccessful master SPI transfer\n");
    }
}

static void lcd_writeData(uint8_t dat)           //写数据
{
    SPI_Transaction masterTransaction;
    bool transferOK;

    DC(1);

    masterTxBuffer[0] = dat;
    masterTransaction.count = 1;
    masterTransaction.txBuf = (void *)masterTxBuffer;
    masterTransaction.rxBuf = (void *)masterRxBuffer;
    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    if (transferOK) {

    }
    else {
        DEBUG_printf("Unsuccessful master SPI transfer\n");
    }
}

static void lcd_writedata_16bit(uint16_t dat)    //向液晶屏写一个16位数据
{
    SPI_Transaction masterTransaction;
    bool transferOK;
    uint8_t dat1[2];
    dat1[0] = dat >> 8;//写入数据  高位在前  低位在后
    dat1[1] = (uint8_t)dat;

    DC(1);

    masterTxBuffer[0] = dat1[0];
    masterTxBuffer[1] = dat1[1];
    masterTransaction.count = 2;
    masterTransaction.txBuf = (void *)masterTxBuffer;
    masterTransaction.rxBuf = (void *)masterRxBuffer;
    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    if (transferOK) {

    }
    else {
        DEBUG_printf("Unsuccessful master SPI transfer\n");
    }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶坐标设置
//  @param      x_start         坐标x方向的起点
//  @param      y_start         坐标y方向的起点
//  @param      x_end           坐标x方向的终点
//  @param      y_end           坐标y方向的终点
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_set_region(0,0,10,10);// x、y的起点都是0，终点都是10
//-------------------------------------------------------------------------------------------------------------------
void lcd_set_region(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{
    if(TFT_X_MAX<x_start){
        DEBUG("lcd_set_region x_start %d > TFT_X_MAX %d", x_start, TFT_X_MAX);
        while(1);
    }
    if(TFT_Y_MAX<y_start){
        DEBUG("lcd_set_region y_start %d > TFT_Y_MAX %d", y_start, TFT_Y_MAX);
        while(1);
    };

    if(TFT_X_MAX<x_end){
        DEBUG("lcd_set_region x_end %d > TFT_X_MAX %d", x_end, TFT_X_MAX);
        while(1);
    };
    if(TFT_Y_MAX<y_end){
        DEBUG("lcd_set_region y_end %d > TFT_Y_MAX %d", y_end, TFT_Y_MAX);
        while(1);
    };

#if (0 == TFT_DISPLAY_DIR || 1 == TFT_DISPLAY_DIR)
    lcd_writeIndex(0x2a);
    lcd_writeData(0x00);
    lcd_writeData(x_start+2);
    lcd_writeData(0x00);
    lcd_writeData(x_end+2);

    lcd_writeIndex(0x2b);
    lcd_writeData(0x00);
    lcd_writeData(y_start+1);
    lcd_writeData(0x00);
    lcd_writeData(y_end+1);

#elif(2 == TFT_DISPLAY_DIR || 3 == TFT_DISPLAY_DIR)
    lcd_writeIndex(0x2a);
    lcd_writeData(0x00);
    lcd_writeData(x_start+1);
    lcd_writeData(0x0);
    lcd_writeData(x_end+1);

    lcd_writeIndex(0x2b);
    lcd_writeData(0x00);
    lcd_writeData(y_start+2);
    lcd_writeData(0x00);
    lcd_writeData(y_end+2);

#endif

    lcd_writeIndex(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶清屏函数
//  @param      color           颜色设置
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_clear(YELLOW);// 全屏设置为黄色
//-------------------------------------------------------------------------------------------------------------------
void lcd_clear(uint16_t color)
{
    uint8_t i,j;
    lcd_set_region(0,0,TFT_X_MAX-1,TFT_Y_MAX-1);
    for (i=0;i<TFT_Y_MAX;i++)
        for (j=0;j<TFT_X_MAX;j++)
            lcd_writedata_16bit(color);
}

void lcd_clear_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint8_t i,j;
    lcd_set_region(x0,y0,x1,y1);
    for (i=0;i<y1-y0;i++)
        for (j=0;j<x1-x0;j++)
            lcd_writedata_16bit(color);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶初始化
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void lcd_init(void)
{
    REST(0);
    usleep(10000);
    REST(1);
    usleep(120000);
    lcd_writeIndex(0x11);
    usleep(120000);

    lcd_writeIndex(0xB1);
    lcd_writeData(0x01);
    lcd_writeData(0x2C);
    lcd_writeData(0x2D);

    lcd_writeIndex(0xB2);
    lcd_writeData(0x01);
    lcd_writeData(0x2C);
    lcd_writeData(0x2D);

    lcd_writeIndex(0xB3);
    lcd_writeData(0x01);
    lcd_writeData(0x2C);
    lcd_writeData(0x2D);
    lcd_writeData(0x01);
    lcd_writeData(0x2C);
    lcd_writeData(0x2D);

    lcd_writeIndex(0xB4);
    lcd_writeData(0x07);

    lcd_writeIndex(0xC0);
    lcd_writeData(0xA2);
    lcd_writeData(0x02);
    lcd_writeData(0x84);
    lcd_writeIndex(0xC1);
    lcd_writeData(0xC5);

    lcd_writeIndex(0xC2);
    lcd_writeData(0x0A);
    lcd_writeData(0x00);

    lcd_writeIndex(0xC3);
    lcd_writeData(0x8A);
    lcd_writeData(0x2A);
    lcd_writeIndex(0xC4);
    lcd_writeData(0x8A);
    lcd_writeData(0xEE);

    lcd_writeIndex(0xC5);
    lcd_writeData(0x0E);

    lcd_writeIndex(0x36);

    if      (TFT_DISPLAY_DIR==0)    lcd_writeData(1<<7 | 1<<6 | 0<<5);  //竖屏模式
    else if (TFT_DISPLAY_DIR==1)    lcd_writeData(0<<7 | 0<<6 | 0<<5);  //竖屏模式  旋转180
    else if (TFT_DISPLAY_DIR==2)    lcd_writeData(1<<7 | 0<<6 | 1<<5);  //横屏模式
    else                            lcd_writeData(0<<7 | 1<<6 | 1<<5);  //横屏模式  旋转180

    lcd_writeIndex(0xe0);
    lcd_writeData(0x0f);
    lcd_writeData(0x1a);
    lcd_writeData(0x0f);
    lcd_writeData(0x18);
    lcd_writeData(0x2f);
    lcd_writeData(0x28);
    lcd_writeData(0x20);
    lcd_writeData(0x22);
    lcd_writeData(0x1f);
    lcd_writeData(0x1b);
    lcd_writeData(0x23);
    lcd_writeData(0x37);
    lcd_writeData(0x00);
    lcd_writeData(0x07);
    lcd_writeData(0x02);
    lcd_writeData(0x10);

    lcd_writeIndex(0xe1);
    lcd_writeData(0x0f);
    lcd_writeData(0x1b);
    lcd_writeData(0x0f);
    lcd_writeData(0x17);
    lcd_writeData(0x33);
    lcd_writeData(0x2c);
    lcd_writeData(0x29);
    lcd_writeData(0x2e);
    lcd_writeData(0x30);
    lcd_writeData(0x30);
    lcd_writeData(0x39);
    lcd_writeData(0x3f);
    lcd_writeData(0x00);
    lcd_writeData(0x07);
    lcd_writeData(0x03);
    lcd_writeData(0x10);

    lcd_writeIndex(0x2a);
    lcd_writeData(0x00);
    lcd_writeData(0x00+2);
    lcd_writeData(0x00);
    lcd_writeData(0x80+2);

    lcd_writeIndex(0x2b);
    lcd_writeData(0x00);
    lcd_writeData(0x00+3);
    lcd_writeData(0x00);
    lcd_writeData(0x80+3);

    lcd_writeIndex(0xF0);
    lcd_writeData(0x01);
    lcd_writeIndex(0xF6);
    lcd_writeData(0x00);

    lcd_writeIndex(0x3A);
    lcd_writeData(0x05);
    lcd_writeIndex(0x29);

    lcd_clear(WHITE);//全白
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶画点
//  @param      x               坐标x方向的起点
//  @param      y               坐标y方向的起点
//  @param      dat             需要显示的颜色
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_drawpoint(0,0,RED);  //坐标0,0画一个红色的点
//-------------------------------------------------------------------------------------------------------------------
void lcd_drawpoint(uint16_t x,uint16_t y,uint16_t color)
{
    lcd_set_region(x,y,x,y);
    lcd_writedata_16bit(color);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符
//  @param      x               坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y               坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat             需要显示的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showchar(0,0,'x');//坐标0,0写一个字符x
//-------------------------------------------------------------------------------------------------------------------
void lcd_showchar(uint16_t x,uint16_t y,const int8_t dat)
{
    uint8_t i,j;
    uint8_t temp;

    for(i=0; i<16; i++)
    {
        lcd_set_region(x,y+i,x+7,y+i);
        temp = tft_ascii[dat-32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
        for(j=0; j<8; j++)
        {
            if(temp&0x01)   lcd_writedata_16bit(TFT_PENCOLOR);
            else            lcd_writedata_16bit(TFT_BGCOLOR);
            temp>>=1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符串
//  @param      x               坐标x方向的起点  参数范围 0 -（TFT_X_MAX-1）
//  @param      y               坐标y方向的起点  参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat             需要显示的字符串
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void lcd_showstr(uint16_t x,uint16_t y,int8_t dat[])
{
    uint16_t j;

    j = 0;
    while(dat[j] != '\0')
    {
        lcd_showchar(x+8*j,y*16,dat[j]);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位有符号
//  @param      x               坐标x方向的起点  参数范围 0 -（TFT_X_MAX-1）
//  @param      y               坐标y方向的起点  参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat             需要显示的变量，数据类型int8
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showint8(0,0,x);//x为int8类型
//-------------------------------------------------------------------------------------------------------------------
void lcd_showint8(uint16_t x,uint16_t y,int8_t dat)
{
    uint8_t a[3];
    uint8_t i;
    if(dat<0)
    {
        lcd_showchar(x,y*16,'-');
        dat = -dat;
    }
    else    lcd_showchar(x,y*16,' ');

    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        lcd_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位无符号
//  @param      x               坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y               坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat             需要显示的变量，数据类型uint8_t
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showuint8_t(0,0,x);//x为uint8_t类型
//-------------------------------------------------------------------------------------------------------------------
void lcd_showuint8(uint16_t x,uint16_t y,uint8_t dat)
{
    uint8_t a[3];
    uint8_t i;

    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        lcd_showchar(x+(8*i),y*16,'0' + a[i]);
        i++;
    }

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位有符号
//  @param      x               坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y               坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat             需要显示的变量，数据类型int16
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showint16(0,0,x);//x为int16类型
//-------------------------------------------------------------------------------------------------------------------
void lcd_showint16(uint16_t x,uint16_t y,int16_t dat)
{
    uint8_t a[5];
    uint8_t i;
    if(dat<0)
    {
        lcd_showchar(x,y*16,'-');
        dat = -dat;
    }
    else    lcd_showchar(x,y*16,' ');

    a[0] = dat/10000;
    a[1] = dat/1000%10;
    a[2] = dat/100%10;
    a[3] = dat/10%10;
    a[4] = dat%10;

    i = 0;
    while(i<5)
    {
        lcd_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位无符号
//  @param      x               坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y               坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat             需要显示的变量，数据类型uint16_t
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showuint16_t(0,0,x);//x为uint16_t类型
//-------------------------------------------------------------------------------------------------------------------
void lcd_showuint16(uint16_t x,uint16_t y,uint16_t dat)
{
    uint8_t a[5];
    uint8_t i;

    a[0] = dat/10000;
    a[1] = dat/1000%10;
    a[2] = dat/100%10;
    a[3] = dat/10%10;
    a[4] = dat%10;

    i = 0;
    while(i<5)
    {
        lcd_showchar(x+(8*i),y*16,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示32位有符号(去除整数部分无效的0)
//  @param      x               坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y               坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat             需要显示的变量，数据类型uint32_t
//  @param      num             需要显示的位数 最高10位  不包含正负号
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showuint32_t(0,0,x,3);//x可以为int32 uint16_t int16 uint8_t int8类型
//  Sample usage:               负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void lcd_showint32(uint16_t x,uint16_t y,int32_t dat,uint8_t num)
{
    char    buff[34];
    uint32_t   length;

    if(10<num)      num = 10;

    num++;
    if(0>dat)   length = sprintf( &buff[0],"%d",dat);//负数
    else
    {
        buff[0] = ' ';
        length = sprintf( &buff[1],"%d",dat);
        length++;
    }
    while(length < num)
    {
        buff[length] = ' ';
        length++;
    }
    buff[num] = '\0';

    lcd_showstr(x, y, buff);    //显示数字
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示浮点数(去除整数部分无效的0)
//  @param      x               坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y               坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat             需要显示的变量，数据类型float或double
//  @param      num             整数位显示长度   最高10位
//  @param      pointnum        小数位显示长度   最高6位
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showfloat(0,0,x,2,3);//显示浮点数   整数显示2位   小数显示三位
//  @note                       特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//                              可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//                              有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//                              负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void lcd_showfloat(uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum)
{
    uint32_t   length;
    char    buff[34];
    int8_t    start,end,point;

    if(6<pointnum)  pointnum = 6;
    if(10<num)      num = 10;

    if(0>dat)   length = sprintf( &buff[0],"%f",dat);//负数
    else
    {
        length = sprintf( &buff[1],"%f",dat);
        length++;
    }
    point = (int8_t)(length - 7); //计算小数点位置
    start = point - num - 1;    //计算起始位
    end = point + pointnum + 1; //计算结束位
    while(0>start)//整数位不够  末尾应该填充空格
    {
        buff[end] = ' ';
        end++;
        start++;
    }

    if(0>dat)   buff[start] = '-';
    else        buff[start] = ' ';

    buff[end] = '\0';

    lcd_showstr(x, y, buff);    //显示数字
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示40*40 QQ图片
//  @param      *p              图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void showimage(const unsigned char *p)
{
    int i,j,k;
    unsigned char picH,picL;
    lcd_clear(WHITE); //清屏

    for(k=0;k<4;k++)
    {
        for(j=0;j<3;j++)
        {
            lcd_set_region(40*j,40*k,40*j+39,40*k+39);      //坐标设置
            for(i=0;i<40*40;i++)
             {
                picL=*(p+i*2);  //数据低位在前
                picH=*(p+i*2+1);
                lcd_writedata_16bit(picH<<8|picL);
             }
         }
    }
}


//-------------------------------------------------------------------------------------------------------------------
// 显示CCD的一条数据 灰度 最长 128 ，拓宽到4象素
//-------------------------------------------------------------------------------------------------------------------
void lcd_displayccd(uint8_t *p, uint16_t height)
{
    uint32_t i,j;

    uint16_t color = 0;
    uint8_t temp;

            for(i=0;i<128;i++)
            {
                temp = *p++ ;//读取像素点
                color=(0x001f&((temp)>>3))<<11;
                color=color|(((0x003f)&((temp)>>2))<<5);
                color=color|(0x001f&((temp)>>3));
                lcd_drawpoint(i, height, color);
                lcd_drawpoint(i, height+1, color);
                lcd_drawpoint(i, height+2, color);
                lcd_drawpoint(i, height+3, color);
                lcd_drawpoint(i, height+4, color);
                lcd_drawpoint(i, height+5, color);
                lcd_drawpoint(i, height+6, color);
            }

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶显示函数
//  @param      *p              图像数组地址
//  @param      width           图像宽度
//  @param      height          图像高度
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_displayimage032(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H)//显示灰度摄像头 图像
//  @note       图像的宽度如果超过液晶的宽度，则自动进行缩放显示。这样可以显示全视野
//-------------------------------------------------------------------------------------------------------------------
void lcd_displayimage032(uint8_t *p, uint16_t width, uint16_t height)
{
    uint32_t i,j;

    uint16_t color = 0;
    uint16_t temp = 0;

    uint16_t coord_x = 0;
    uint16_t coord_y = 0;

    if(2==TFT_DISPLAY_DIR || 3==TFT_DISPLAY_DIR)//竖屏
    {
        coord_x = height>TFT_X_MAX?TFT_X_MAX:height;
        coord_y = width>TFT_Y_MAX?TFT_Y_MAX:width;

        for(j=0;j<coord_y;j++)
        {
            lcd_set_region(0,j,coord_x-1,j);
            for(i=0;i<coord_x;i++)
            {
                temp = *(p+i*width+j*width/coord_y);//读取像素点
                color=(0x001f&((temp)>>3))<<11;
                color=color|(((0x003f)&((temp)>>2))<<5);
                color=color|(0x001f&((temp)>>3));
                lcd_writedata_16bit(color);
            }
        }

    }
    else//横屏
    {
        coord_x = width>TFT_X_MAX?TFT_X_MAX:width;
        coord_y = height>TFT_Y_MAX?TFT_Y_MAX:height;
        lcd_set_region(0,16,coord_x-1,16+coord_y-1);

        for(j=0;j<coord_y;j++)
        {
            for(i=0;i<coord_x;i++)
            {
                temp = *(p+j*width+i*width/coord_x);//读取像素点
                color=(0x001f&((temp)>>3))<<11;
                color=color|(((0x003f)&((temp)>>2))<<5);
                color=color|(0x001f&((temp)>>3));
                lcd_writedata_16bit(color);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶缩放显示函数
//  @param      *p              图像数组地址
//  @param      width           图像宽度
//  @param      height          图像高度
//  @param      dis_width       图像显示宽度  1 -（TFT_X_MAX）
//  @param      dis_height      图像显示高度  1 -（TFT_Y_MAX）
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 0, 0, 160, 120)//显示灰度摄像头 图像
//  @note       图像的宽度如果超过液晶的宽度，则自动进行缩放显示。这样可以显示全视野
//-------------------------------------------------------------------------------------------------------------------
void lcd_displayimage032_zoom(uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    uint32_t i,j;

    uint16_t color = 0;
    uint16_t temp = 0;

    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            temp = *(p+(j*height/dis_height)*width+i*width/dis_width);//读取像素点
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            lcd_writedata_16bit(color);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      凌瞳(彩色摄像头)液晶缩放显示函数
//  @param      *p              图像数组地址
//  @param      width           图像宽度
//  @param      height          图像高度
//  @param      dis_width       图像显示宽度  1 -（TFT_X_MAX）
//  @param      dis_height      图像显示高度  1 -（TFT_Y_MAX）
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_displayimage8660_zoom(scc8660_csi_image[0],SCC8660_CSI_PIC_W,SCC8660_CSI_PIC_H,160,128);
//  @note       图像的宽度如果超过液晶的宽度，则自动进行缩放显示。这样可以显示全视野
//-------------------------------------------------------------------------------------------------------------------
void lcd_displayimage8660_zoom(uint16_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    uint32_t i,j;

    uint16_t color = 0;

    lcd_set_region(0,0,dis_width-1,dis_height-1);//设置显示区域

    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            color = *(p+(j*height/dis_height)*width+i*width/dis_width);//读取像素点
            color = ((color&0xff)<<8) | (color>>8);
            lcd_writedata_16bit(color);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风(二值化摄像头)液晶显示函数
//  @param      *p              图像数组地址
//  @param      width           图像宽度
//  @param      height          图像高度
//  @return     void
//  @since      v1.0
//  @note       图像分辨率需要设置在120(高)*160(宽)以下
//-------------------------------------------------------------------------------------------------------------------
void lcd_displayimage7725(uint8_t *p, uint16_t width, uint16_t height)
{
    int i,j;

    uint16_t temp = 0;
    lcd_set_region(0,0,width-1,height-1);
    for(i=0;i<height * (width/8);i++)
    {
        temp = *p;
        p++;
        for(j=0; j<8; j++)
        {
            if( (temp<<j)&0x80 )    lcd_writedata_16bit(WHITE);
            else                    lcd_writedata_16bit(BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      汉字显示
//  @param      x       横坐标 0-（TFT_X_MAX-1）
//  @param      y       纵坐标 0-（TFT_Y_MAX-1）
//  @param      size    取模的时候设置的汉字字体大小，也就是一个汉字占用的点阵长宽为多少个点，取模的时候需要长宽是一样的。
//  @param      *p      需要显示的汉字数组
//  @param      number  需要显示多少位
//  @param      color   显示颜色
//  @return     void
//  @since      v1.0
//  Sample usage:       lcd_display_chinese(0,0,16,chinese_test[0],4,RED);//显示font文件里面的 示例
//  @Note       使用PCtoLCD2002软件取模           阴码、逐行式、顺向   16*16
//-------------------------------------------------------------------------------------------------------------------
void lcd_display_chinese(uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number, uint16_t color)
{
    int i, j, k;
    uint8_t temp, temp1, temp2;
    const uint8_t *p_data;

    temp2 = size/8;

    lcd_set_region(x,y,number*size-1+x,y+size-1);

    for(i=0;i<size;i++)
    {
        temp1 = number;
        p_data = p+i*temp2;
        while(temp1--)
        {
            for(k=0;k<temp2;k++)
            {
                for(j=8;j>0;j--)
                {
                    temp = (*p_data>>(j-1)) & 0x01;
                    if(temp)    lcd_writedata_16bit(color);
                    else        lcd_writedata_16bit(TFT_BGCOLOR);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2*size;
        }
    }
}
/*=======================LCD  函数定义 结束  ========================*/

void timerCallback(Timer_Handle myHandle)
{
    Encoder_EC11_Analyze(Encoder_EC11_Scan());//EC11检测更新
}

void lcd_init_Thread(void)
{

    SPI_Params masterSpiParams;

    /* Initialize SPI handle as default master */
    SPI_Params_init(&masterSpiParams);
    masterSpiParams.frameFormat = SPI_POL0_PHA0;
    masterSpiParams.bitRate = 10000000;
    masterSpi = SPI_open(CONFIG_SPI_0, &masterSpiParams);  //硬件SPI初始化
    if (masterSpi == NULL) {
        DEBUG_printf("Error initializing master SPI\n");
        while (1);
    }
    else {
        DEBUG_printf("Master SPI initialized\n");
    }

    lcd_init();
    lcd_showstr(0, 1, "Initializing...");
    DEBUG_printf("Done\n");

    EC11_GPIO_Init();
    Encoder_EC11_Init(0);

    /*打开定时器*/
    Timer_Handle timer0;
    Timer_Params params;

    /* Call driver init functions */
    GPIO_init();
    Timer_init();

    /* Setting up the timer in continuous callback mode that calls the callback
     * function every 1,000,000 microseconds, or 1 second.
     */
    Timer_Params_init(&params);
    params.period = 2000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;

    timer0 = Timer_open(CONFIG_TIMER_0, &params);

    if (timer0 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }

    ui_init();
}

void *lcd_Thread(void *arg0)
{
    /* Call driver init functions. */
    GPIO_init();
    SPI_init();
    EC11_GPIO_Init();

    GPIO_setConfig(CONFIG_TFT_RES, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(CONFIG_TFT_DC, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    lcd_init_Thread();

    enum EC11 ec11 = NO_PRESS;
    while(1)
    {
        ec11_read(&ec11);
        switch (ec11)
        {
            case KEY_SHORT:
                onEnter();
                lcd_showint32(0, 0, 0, 2);
                break;
            case KEY_DOUBLE:
                onCancel();
                lcd_showint32(0, 0, 2, 2);
                break;
            case KEY_INCREASE:
                onIncrease();
                lcd_showint32(0, 0, 1, 2);
                break;
            case KEY_DECREASE:
                onDecrease();
                lcd_showint32(0, 0, -1, 2);
                break;
            case KEY_LONG_INCREASE:
//                onDecrease();
                lcd_showint32(0, 0, 3, 2);
                break;
            case KEY_LONG_DECREASE:
//                onDecrease();
                lcd_showint32(0, 0, -3, 2);
                break;
            default:
                break;
        }
        Draw();//绘图函数
        usleep(100000);
    }
    return (NULL);
}
