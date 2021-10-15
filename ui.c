/*
 * ui.c
 *
 *  Created on: 2021年8月1日
 *      Author: sundm
 */

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/display/Display.h>
#include <ti/drivers/utils/RingBuf.h>
#include <ti/drivers/apps/LED.h>
#include <ti/drivers/apps/Button.h>

/* Driver Configuration */
#include "ti_drivers_config.h"

#include "includes.h"

#define MaxItemsNum 30
#define MaxLine 10

int motor_start_flag = 0;
void gotoViewById(uint8_t view_id);

//flash_data fd;//flash
param_ty param = {
    .bend_steer_kp = 9.0,
    .bend_steer_ki = 0.0,
    .bend_steer_kd = 42.0,
    .motor_kp = 24.0,
    .motor_ki = 0.0,
    .motor_kd = 0.0,
    .circle_pid_ratio = 1.0,
    .velocity = 30,
    .circle_size = {170, 260, 170, 260},
    .circle_turndis = {50, 50, 50, 50},
    .circle_ad = {0, 0, 0, 0},
    .circle_in_angle = {-30,33,-30,33},
    .circle_exit_angle = {-35,35,-35,35},
    .circle_num = 0,
    .circle_en = 1,
};

uint8_t CurView = 0;//当前菜单界面
int8_t CurItem = 1;//当前菜单子项
uint8_t ItemNums = 0;//菜单子项总数
uint8_t isEditing = 0;//是否正在编辑
union var_ty old_val = {0};//保存调参时原先的值，如果按取消后可恢复成原来的值
int8_t ItemFirstLine = 1;//当前起始行显示的菜单子项

uint8_t last_item[20] = {1} ;//上次一、二、三级菜单子项  i= 0 1 2
int8_t item_index= 0;//当前

MenuItem Items[25] ;//菜单项指针，保存当前菜单界面的所有项
Interface_f CurDraw;//当前菜单界面的绘图函数
Interface_t CurHandle;//当前菜单界面的处理函数，可以处理 确认键，增加键，减少键

void DrawMainMenuView()
{

}

void DrawSensorView(void)
{
    int i;
    for(i = 0; i < ItemNums; i++)
    {
        lcd_showstr(Items[i].x, Items[i].y, Items[i].title);
    }

    uint8_t str1[21];
    uint8_t str2[21];
    uint8_t str3[21];
    uint8_t str4[21];
    uint8_t str5[21];
    sprintf(str1, "lua: %d", lua);
    sprintf(str2, "%.1f %.1f %.1f", imu3.acc[0], imu3.acc[1],imu3.acc[2]);
    sprintf(str3, "%.1f %.1f %.1f", imu3.gyro[0],imu3.gyro[1],imu3.gyro[2]);
    sprintf(str4, "%.1f %.1f %.1f", imu3.agl[0], imu3.agl[1],imu3.agl[2]);
    sprintf(str5, "temp: %.2f", imu.temp);

    lcd_showstr(0, 1, str1);
    lcd_showstr(0, 2, "acc");
    lcd_showfloat(64,2,imu3.acc[0],2,1); lcd_showfloat(0,3,imu3.acc[1],2,1); lcd_showfloat(64,3,imu3.acc[2],2,1);
    lcd_showstr(0, 4, "gyro");
    lcd_showfloat(64,4,imu3.gyro[0],2,1);  lcd_showfloat(0,5,imu3.gyro[1],2,1);  lcd_showfloat(64,5,imu3.gyro[2],2,1);
    lcd_showstr(0, 6, "agl");
    lcd_showfloat(64,6,imu3.agl[0],2,1);  lcd_showfloat(0,7,imu3.agl[1],2,1); lcd_showfloat(64,7,imu3.agl[2],2,1);

    lcd_showstr(0, 9, str5);

}

void DrawCCDView()
{
    int i;
    uint16_t *p , temp;
    p = gPixel;
    for(i = 0; i < ItemNums; i++)
    {
        lcd_showstr(Items[i].x, Items[i].y, Items[i].title);
    }
    lcd_displayccd(gPixel, 50);
    //串口3发送图像到上位机
//    ccd_send_data(gPixel);
//    ccd_send_data2(gPixel);

    /*计算中线*/
}

void DrawEncoderView()
{
    int i;
    for(i = 0; i < ItemNums; i++)
    {
        lcd_showstr(Items[i].x, Items[i].y, Items[i].title);
    }
    uint8_t str1[21];
    int encoder1_speed ,encoder2_speed, encoder3_speed, encoder4_speed;
    encoder1_speed = get_encoder1();
    encoder2_speed = get_encoder2();
    encoder3_speed = get_encoder3();
    encoder4_speed = get_encoder4();
    lcd_showint32(2,3,encoder1_speed,5);
    lcd_showint32(66,3,encoder2_speed,5);
    lcd_showint32(2,5,encoder3_speed,5);
    lcd_showint32(66,5,encoder4_speed,5);

}


void gotoMainMenuView()
{
        gotoViewById(0);
}

void DrawBootView()
{
    int i;
    for(i = 0; i < ItemNums; i++)
    {
        lcd_showstr(Items[i].x, Items[i].y, Items[i].title);
    }
    lcd_showstr(3, 5, "start?");
}

void HandleBootView(uint8_t action_type, uint8_t pos)
{
        if(action_type == VK_ENTER) {
        switch(pos) {
        case 1:
//            alarm_times(25, 200);
                        motor_start_flag = 1;
                        gotoMainMenuView();
            break;
        case 2:
//            alarm_once(10);
                        motor_start_flag = 0;
                        gotoMainMenuView();
            break;
        }
    }
}

void DrawFeatureView()
{
    int i;
    for(i = 0; i < ItemNums; i++)
    {
        lcd_showstr(Items[i].x, Items[i].y, Items[i].title);
    }
//        uint8_t str1[21];
//        uint8_t str2[21];
//        uint8_t str3[21];
//        uint8_t str4[21];
//        uint8_t str5[21];
//        uint8_t str6[21];
//        uint8_t str7[21];
//        uint8_t str8[21];
//        gradient_variance_ty edge_grad_var = get_edge_gradient_variance(18, 40);
//        sprintf(str1, "variance:"); sprintf(str5, "%07.3f", get_variance());
//        sprintf(str2, "curvity_l:");    sprintf(str6, "%07.3f", edge_grad_var.left_edge);
//        sprintf(str3, "curvity_r:");  sprintf(str7, "%07.3f", edge_grad_var.right_edge);
//        sprintf(str4, "all_write:");    sprintf(str8, "%02d", col_scan_all_white(18));
//
//        lcd_showstr(8, 2, str1);
//        lcd_showstr(8, 3, str5);
//        lcd_showstr(8, 4, str2);
//        lcd_showstr(8, 5, str6);
//        lcd_showstr(8, 6, str3);
//        lcd_showstr(8, 7, str7);
//        lcd_showstr(8, 8, str4);
//        lcd_showstr(8, 9, str8);
}

void DrawFlagView()
{
    int i;
    for(i = 0; i < ItemNums; i++)
    {
        lcd_showstr(Items[i].x, Items[i].y, Items[i].title);
    }

//    uint8_t str1[21];
//    uint8_t str2[21];
//    uint8_t str3[21];
//    uint8_t str4[21];
//    switch(road_type)
//    {
//        case START:
//        sprintf(str1, "road:");sprintf(str2, "START");
//        break;
//    case BEND:
//        sprintf(str1, "road:");sprintf(str2, "BEND     ");
//        break;
//    case OUT_BOUND:
//        sprintf(str1, "road:");sprintf(str2, "OUT_BOUND");
//        break;
//    case LONG_STRAIGHT:
//        sprintf(str1, "road:");sprintf(str2, "LONG_STRAIGHT");
//        break;
//    case CROSS:
//        sprintf(str1, "road: ");sprintf(str2, "CROSS");
//        break;
//    case CIRCLE_ENTRY:
//        sprintf(str1, "road:");sprintf(str2, "CIRCLE_ENTRY");
//        break;
//    case IN_CIRCLE:
//        sprintf(str1, "road: ");sprintf(str2, "IN_CIRCLE");
//        break;
//        case CIRCLE_READY_EXIT:
//        sprintf(str1, "road:");sprintf(str2, "CIRCLE_RDY_EXIT");
//        break;
//    case CIRCLE_EXIT:
//        sprintf(str1, "road:");sprintf(str2, "CIRCLE_EXIT ");
//        break;
//    case FINISH_ENTRY:
//        sprintf(str1, "road:");sprintf(str2, "FINISH_ENTRY");
//        break;
//    case FINISH_STOP:
//        sprintf(str1, "road: ");sprintf(str2, "FINISH_STOP");
//        break;
//    case RAMP:
//        sprintf(str1, "road:");sprintf(str2, "RAMP ");
//        break;
//    case FORK_ENTRY:
//        sprintf(str1, "road:");sprintf(str2, "Fork_ENTRY ");
//        break;
//    case FORK_IN:
//        sprintf(str1, "road:");sprintf(str2, "FORK_IN   ");
//        break;
//    case FORK_EXIT:
//        sprintf(str1, "road:");sprintf(str2, "FORK_EXIT  ");
//        break;
//    }
//        switch(ele_lock)
//    {
//        case 2:         //-->_START
//        sprintf(str3, "lock:");sprintf(str4, "_START");
//        break;
//    case _BEND:
//        sprintf(str3, "lock:");sprintf(str4, "_BEND ");
//        break;
//    case _OUT_BOUND:
//        sprintf(str3, "lock:");sprintf(str4, "_OUT_BOUND");
//        break;
//    case _CROSS:
//        sprintf(str3, "lock:");sprintf(str4, "_CROSS ");
//        break;
//        case _CIRCLE:
//        sprintf(str3, "lock:");sprintf(str4, "_CIRCLE ");
//        break;
//    case _BARN:
//        sprintf(str3, "lock:");sprintf(str4, "_BARN ");
//        break;
//    case _RAMP:
//        sprintf(str3, "lock:");sprintf(str4, "_RAMP ");
//        break;
//    case _FORK:
//        sprintf(str3, "lock:");sprintf(str4, "_FORK ");
//        break;
//    }
//
//        lcd_showstr(8, 1, str1);
//        lcd_showstr(15, 2, str2);
//        lcd_showstr(8, 3, str3);
//        lcd_showstr(15, 4, str4);
//
//        sprintf(str1, "angle:%.2f", car_goal_status.goal_angle);
//        lcd_showstr(2, 5, str1);
//        sprintf(str1, "mid:%.2f", get_image_middle_line());
//        lcd_showstr(2, 6, str1);
//        sprintf(str1, "dis:%.2f", get_distance());
//        lcd_showstr(2, 7, str1);
//        extern uint8_t circle_times;
//        sprintf(str1, "circle:%d", circle_times);
//        lcd_showstr(2, 8, str1);
}

#ifndef PI
  #define PI               3.14159265358979f
#endif
#define swap(a, b) { uint16_t t = a; a = b; b = t; }
float* matconv(float* a,float b[3][3]){//计算矩阵乘法
  float res[3];
  int i;
  for(i=0;i<3;i++)
    res[i]=b[i][0]*a[0]+b[i][1]*a[1]+b[i][2]*a[2];
  for(i=0;i<3;i++)a[i]=res[i];
  return a;
}
void rotate(float* obj,float x,float y,float z)
{//旋转该向量
  x/=PI;y/=PI;z/=PI;
  float rz[3][3]={{cos(z),-sin(z),0},{sin(z),cos(z),0},{0,0,1}};
  float ry[3][3]={{1,0,0},{0,cos(y),-sin(y)},{0,sin(y),cos(y)}};
  float rx[3][3]={{cos(x),0,sin(x)},{0,1,0},{-sin(x),0,cos(x)}};
  matconv(matconv(matconv(obj,rz),ry),rx);
}

void lcd_drawline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  uint16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  uint16_t  dx;
  uint16_t dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int err = dx / 2;
  int ystep;

  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }

  for (; x0<x1; x0++)
  {
    if (steep)
    {
      lcd_drawpoint(y0,x0,BLACK);
    }
    else
    {
        lcd_drawpoint(x0,y0,BLACK);
    }
    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

int lineid[]={1,2,2,3,3,4,4,1,5,6,6,7,7,8,8,5,8,4,7,3,6,2,5,1};//记录点之间连接顺序
void DrawIMUView()
{
    int i;
    for(i = 0; i < ItemNums; i++)
    {
        lcd_showstr(Items[i].x, Items[i].y, Items[i].title);
    }

      float cube[8][3]={{-7,-3,-25},{-7,3,-25},{7,3,-25},{7,-3,-25},{-15,-10,25},{-15,10,25},{15,10,25},{15,-10,25}};//立方体各点坐标
      lcd_clear_rectangle(32, 4, 100, 48, WHITE);

            for(i=0;i<8;i++)
            {
              rotate(cube[i],-imu.rol/180*PI,imu.pit/180*PI,0);//imu.yaw/180*PI);//旋转每个点
            }

            for(i=0;i<24;i+=2)
            {//绘制立方体
              lcd_drawline(64+cube[lineid[i]-1][0],25+cube[lineid[i]-1][1],64+cube[lineid[i+1]-1][0],25+cube[lineid[i+1]-1][1]);
            }

        uint8_t str0[21];
        uint8_t str1[21];
        uint8_t str2[21];
        uint8_t str3[21];
        uint8_t str4[21];
        uint8_t str5[21];
        uint8_t str6[21];
        sprintf(str0, "temp:%.2f ", imu.temp);
        sprintf(str1, "gyro_x:%.3f ",imu3.gyro[0]);
        sprintf(str2, "gyro_y:%.3f ", imu3.gyro[1]);
        sprintf(str3, "gyro_z:%.3f ", imu3.gyro[2]);

        sprintf(str4, "YAW:%.2f ",imu.yaw);
        sprintf(str5, "PITCH: %.2f ", imu.pit);
        sprintf(str6, "ROLL:  %.2f ", imu.rol);

        lcd_showstr(0, 4, str1);
        lcd_showstr(0, 5, str2);
        lcd_showstr(0, 6, str3);
        lcd_showstr(0, 7, str4);
        lcd_showstr(0, 8, str5);
        lcd_showstr(0, 9, str6);
}

void DrawPIDView()
{
    int i;
    for(i = 0; i < ItemNums; i++)
    {
        lcd_showstr(Items[i].x, Items[i].y, Items[i].title);
    }
    uint8_t str1[21];
    uint8_t str2[21];
    uint8_t str3[21];
//    sprintf(str1, "steerKp:%.2f", steer_pid.K_p);
//    sprintf(str2, "steerKi:%.2f", steer_pid.K_i);
//    sprintf(str3, "steerKd:%.2f", steer_pid.K_d);

    lcd_showstr(8, 3, str1);
    lcd_showstr(8, 5, str2);
    lcd_showstr(8, 7, str3);
 }

void DrawParamView()
{

}

void HandleFlashView(uint8_t action_type, uint8_t pos)
{
//  根据位置，做出不同处理，以及给予当前编辑项上锁，保存，发送到上位机等操作

    if(action_type == VK_ENTER) {
        switch(pos) {
        case 2:
//            flash_save_data(&fd);//当前 参数存储到 flash
            break;
        case 3:
                  {
            static int reset_cnt = 0;
            if(reset_cnt < 3)
                reset_cnt++;
            else
            {
                reset_cnt = 0;
//                flash_reset_data(&fd);//flash????
            }
                  }
            break;
        }
    }
}

void DrawAdjustParamView()
{
    char str[50];
        if(ItemNums > 1)
        {
            uint8_t i,j;
            for(i = ItemFirstLine,j = 1; j <= ((ItemNums-1 >= (MaxLine - 1)) ? (MaxLine - 1) : ItemNums-1); i++,j++)
            {
                uint8_t str[21];
                if((NULL != Items[i].var_ptr.f)||(NULL != Items[i].var_ptr.i))
                {
                    if(FLOAT_TY == Items[i].var_ty)  sprintf(str, "%.2f", *(Items[i].var_ptr.f));
                    else  sprintf(str,"%03d",*(Items[i].var_ptr.i));
                    lcd_showstr(72, j, str);
                }
            }
        }
        if(isEditing)
            lcd_showstr(0, (CurItem - ItemFirstLine + 1) , "*");
        else
            lcd_showstr(0, (CurItem - ItemFirstLine + 1) , ">");


        { //显示名称
            uint8_t i,j;
            for(i = ItemFirstLine,j = 1; j <= ((ItemNums-1 >= (MaxLine - 1)) ? (MaxLine - 1) : ItemNums-1); i++,j++)
            {
                lcd_showstr(Items[i].x, j, Items[i].title);
            }
        }
}


static int is_number(double d){return (d==d);}
void HandleAdjustParamView(uint8_t action_type, uint8_t pos)
{
    //根据位置，做出不同处理，以及给予当前编辑项上锁，保存，发送到上位机等操作
    if(isEditing) {
        if(action_type == VK_ENTER) {
            isEditing = !isEditing;
            //执行保存，发送上位机
        } else if(action_type == VK_INC) {
            if(FLOAT_TY == Items[pos].var_ty)
            {
                if(fabs(Items[pos].step.f) < 0.0001) {
                    ;
                } else {
                    *(Items[pos].var_ptr.f) += Items[pos].step.f;
                                        if(is_number(*(Items[pos].var_ptr.f)) == 0)
                                          *(Items[pos].var_ptr.f) = Items[pos].var_min.f;
                                        if(*(Items[pos].var_ptr.f) < Items[pos].var_min.f || *(Items[pos].var_ptr.f) > Items[pos].var_max.f)
                                          *(Items[pos].var_ptr.f) = Items[pos].var_min.f;
#ifdef NORMAL_DEBUG
                                           DEBUG_printf("%f,%f\n\r", *(Items[pos].var_ptr.f), Items[pos].step.f);
#endif
                }
            }
            else
            {
                if(Items[pos].step.i == 0) {
                    *(Items[pos].var_ptr.i) = ! (*(Items[pos].var_ptr.i));
                } else {
                    *(Items[pos].var_ptr.i) += Items[pos].step.i;
                                        if(*(Items[pos].var_ptr.i) < Items[pos].var_min.i || *(Items[pos].var_ptr.i) > Items[pos].var_max.i)
                                          *(Items[pos].var_ptr.i) = Items[pos].var_min.i;
                }
            }
        }
        else if(action_type == VK_DEC) {
            if(FLOAT_TY == Items[pos].var_ty)
            {
                if(fabs(Items[pos].step.f) < 0.0001) {
                    ;
                } else {
                    *(Items[pos].var_ptr.f) -= Items[pos].step.f;
                                        if(is_number(*(Items[pos].var_ptr.f)) == 0)
                                          *(Items[pos].var_ptr.f) = Items[pos].var_min.f;
                                        if(*(Items[pos].var_ptr.f) < Items[pos].var_min.f || *(Items[pos].var_ptr.f) > Items[pos].var_max.f)
                                          *(Items[pos].var_ptr.f) = Items[pos].var_min.f;
                }
            }
            else
            {
                if(Items[pos].step.i == 0) {
                    *(Items[pos].var_ptr.i) = ! (*(Items[pos].var_ptr.i));
                } else {
                    *(Items[pos].var_ptr.i) -= Items[pos].step.i;
                                        if(*(Items[pos].var_ptr.i) < Items[pos].var_min.i || *(Items[pos].var_ptr.i) > Items[pos].var_max.i)
                                          *(Items[pos].var_ptr.i) = Items[pos].var_min.i;
                }
            }
        } else if(action_type == VK_CANCEL) {
            if(FLOAT_TY == Items[pos].var_ty)
                 *(Items[pos].var_ptr.f) = old_val.f;//恢复成原来的值
            else
                 *(Items[pos].var_ptr.i) = old_val.i;//恢复成原来的值
        }
    } else {
        if(action_type == VK_ENTER) {
            isEditing = !isEditing;//变成编辑状态
            if(FLOAT_TY == Items[pos].var_ty)
                old_val.f = *(Items[pos].var_ptr.f);
            else
                old_val.i = *(Items[pos].var_ptr.i);
        }
    }
}

MenuState AllView[] = {
    {0, NOT_JUMP, DrawMainMenuView, NULL},
    {1, 0, DrawSensorView, NULL},
    {2, 0, DrawCCDView, NULL},
    {3, 0, DrawEncoderView, NULL},
    {4, 0, DrawBootView, HandleBootView},
    {5, 0, DrawFeatureView, NULL},
    {6, 0, DrawFlagView, NULL},
    {7, 0, DrawIMUView, NULL},
    {8, 0, DrawPIDView, NULL},
    {9, 0, DrawParamView, HandleFlashView},
    {10, 9, DrawAdjustParamView, HandleAdjustParamView},//调参界面
};

void UpdateMenuItems(uint8_t view_id)//根据view_id来更新菜单项
{
    Invalidate();//重绘函数


    switch(view_id) {
    case 0:
        {
            MenuItem menuItems[] = {
                {24, 0, 1, ">>Welcome<<", NULL, 0},
                {30, 1, 1, "1.sensor", NULL, 0},
                {30, 2, 2, "2.TSL1401", NULL, 0},
                {30, 3, 3, "3.encoder", NULL, 0},
                {30, 4, 4, "4.boot", NULL, 0},
                {30, 5, 5, "5.feature", NULL, 0},
                {30, 6, 6, "6.flag", NULL, 0},
                {30, 7, 7, "7.IMU", NULL, 0},
                {30, 8, 8, "8.pid", NULL, 0},
                {30, 9, 9, "9.param", NULL, 0},
            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
    case 1:
        {
            MenuItem menuItems[] = {
                {24, 0, NOT_JUMP, ">Sensor<", NULL, 0},

            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
    case 2:
        {
            MenuItem menuItems[] = {
                {24, 0, NOT_JUMP, ">TSL1401<", NULL, 0},
            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
    case 3:
        {
            MenuItem menuItems[] = {
                {24, 0, NOT_JUMP, ">Encoder<", NULL, 0},
            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
    case 4:
        {
            MenuItem menuItems[] = {
                {24, 0, NOT_JUMP, ">Boot<", NULL, 0},
                {30, 1, NOT_JUMP, "YES", NULL, 0},
                {30, 2, NOT_JUMP, "NO", NULL, 0},

            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
    case 5:
        {
            MenuItem menuItems[] = {
                {24, 0, NOT_JUMP, ">Feature<", NULL, 0},
            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
    case 6:
        {
            MenuItem menuItems[] = {
                {24, 0, NOT_JUMP, ">Flag<", NULL, 0},
            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
    case 7:
        {
            MenuItem menuItems[] = {
                {24, 3, NOT_JUMP, ">IMU<", NULL, 0},
            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
    case 8:
        {
            MenuItem menuItems[] = {
                {24, 0, NOT_JUMP, ">CONTROL<", NULL, 0},
            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
    case 9:
        {
            MenuItem menuItems[] = {
                {24, 0, NOT_JUMP, ">PARAM<", NULL, 0},
                {30, 1, 10, "1.AdjustPara", NULL, 0},
                {30, 2, NOT_JUMP, "2.save", NULL, 0},
                {30, 3, NOT_JUMP, "3.restore", NULL, 0},
            };
            memset(Items, 0x00, sizeof(Items));
            memcpy(Items, menuItems, sizeof(menuItems));
            ItemNums = sizeof(menuItems) / sizeof(MenuItem);
        }
        break;
        case 10:
            {
                MenuItem menuItems[] = {
                        {24, 0, NOT_JUMP, ">Para<", NULL, 0},
                        {8, 1, NOT_JUMP,  "moto_kp:", .var_ptr.f = &param.motor_kp, .step.f = 0.5, FLOAT_TY, .var_min.f = 0, .var_max.f = 1000},
                        {8, 2, NOT_JUMP,  "moto_ki:", .var_ptr.f = &param.motor_ki, .step.f = 0.5, FLOAT_TY, .var_min.f = 0, .var_max.f = 1000},
                        {8, 3, NOT_JUMP,  "moto_kd:", .var_ptr.f = &param.motor_kd, .step.f = 0.5, FLOAT_TY, .var_min.f = 0, .var_max.f = 1000},
                        {8, 4, NOT_JUMP,  "bend_kp:", .var_ptr.f = &param.bend_steer_kp, .step.f = 0.5, FLOAT_TY, .var_min.f = 0, .var_max.f = 1000},
                        {8, 5, NOT_JUMP,  "bend_ki:", .var_ptr.f = &param.bend_steer_ki, .step.f = 0.5, FLOAT_TY, .var_min.f = 0, .var_max.f = 1000},
                        {8, 6, NOT_JUMP,  "bend_kd:", .var_ptr.f = &param.bend_steer_kd, .step.f = 0.5, FLOAT_TY, .var_min.f = 0, .var_max.f = 1000},
                        {8, 7, NOT_JUMP,  "velocty:", .var_ptr.i = &param.velocity, .step.i = 1, INT_TY, .var_min.i = 0, .var_max.i = 1000},
                        {8, 8, NOT_JUMP,  "cir1_s :", .var_ptr.i = param.circle_size, .step.i = 1, INT_TY, .var_min.i = 0, .var_max.i = 1000},
                        {8, 9,  NOT_JUMP,   "cir1IA :", .var_ptr.f = param.circle_in_angle, .step.f = 1, FLOAT_TY, .var_min.f = -45, .var_max.f = 45},
                        {8, 10, NOT_JUMP,  "cir1EA :", .var_ptr.f = param.circle_exit_angle, .step.f = 1, FLOAT_TY, .var_min.f = -45, .var_max.f = 45},
                        {8, 11, NOT_JUMP, "cir1TD :", .var_ptr.i = param.circle_turndis, .step.i = 1, INT_TY, .var_min.i = 0, .var_max.i = 100},

                };
                memset(Items, 0x00, sizeof(Items));
                memcpy(Items, menuItems, sizeof(menuItems));
                ItemNums = sizeof(menuItems) / sizeof(MenuItem);
            }
            break;
    default:
        break;
    }

    if(ItemNums > 1)
    {
        lcd_showstr(Items[0].x, 0, Items[0].title);
        int i,j;
        for(i = ItemFirstLine,j = 1; j <= ((ItemNums-1 >= (MaxLine - 1)) ? (MaxLine - 1) : ItemNums-1); i++,j++)
        {
            lcd_showstr(Items[i].x, j, Items[i].title);
        }

        lcd_showstr(10, (CurItem - ItemFirstLine + 1) , "->");
    }
}

void ui_init(void)
{
//    flash_init_data(&fd, 11, &param, sizeof(param));
    CurView = AllView[0].CurIndex;
    CurDraw = AllView[CurView].onDraw;
    CurHandle = AllView[CurView].onHandle;
    UpdateMenuItems(CurView);
}

static void printvalue(void)
{
#ifdef NORMAL_DEBUG
    DEBUG_printf("CurView=%d ItemNums=%d CurItem=%d ItemFirstLine=%d\r\n",CurView,ItemNums,CurItem,ItemFirstLine);
#endif

}

void Draw(void)
{
    static unsigned int counter = 0;
    counter++;
    char str1[32];

    (*CurDraw)();
    sprintf(str1, "%d %d",CurView, CurItem );
    lcd_showstr(80,0,(const int8_t*)str1);
    if(0 == counter%20)
        printvalue();

}

void Invalidate(void)//重绘函数
{
    lcd_clear(WHITE);
}

void gotoViewById(uint8_t view_id)
{
        CurView = view_id;
        CurDraw = AllView[CurView].onDraw;
        CurHandle = AllView[CurView].onHandle;
        UpdateMenuItems(CurView);//根据当前View更新菜单项
}

void onEnter(void)
{
    if(Items[CurItem].EnterIndex != NOT_JUMP) {//是跳转功能，根据状态转移表，进行跳转
        CurView = Items[CurItem].EnterIndex;
        CurDraw = AllView[CurView].onDraw;
        CurHandle = AllView[CurView].onHandle;
        Invalidate();//重绘函数

        item_index ++;
        CurItem = 1;
        ItemFirstLine = 1;

        UpdateMenuItems(CurView);//根据当前View更新菜单项
    } else {//不是跳转，就执行功能
        if(CurHandle != NULL) {
            (*CurHandle)(VK_ENTER, CurItem);//传递ENTER键消息
        }
    }
}

void onCancel(void)
{
    if(!isEditing) {
        if(AllView[CurView].BackIndex != NOT_JUMP) {//是跳转功能，根据状态转移表，进行跳转
            CurView = AllView[CurView].BackIndex;
            CurDraw = AllView[CurView].onDraw;
            CurHandle = AllView[CurView].onHandle;
            Invalidate();//重绘函数

            item_index --;
            if( item_index < 0) item_index = 0;
            CurItem = last_item[item_index];
            ItemFirstLine = 1;

            UpdateMenuItems(CurView);//根据当前View更新菜单项
        }
    } else {
        if(CurHandle != NULL) {
            (*CurHandle)(VK_CANCEL, CurItem);//传递CANCEL键消息
        }
        isEditing = !isEditing;
    }
}

void onIncrease(void)
{
    if(!isEditing) {
        CurItem++;

        if(CurItem > ItemNums - 1) {//根据状态转移表，进行跳转
            CurItem = 1;
            ItemFirstLine = 1;
        }

        if((CurItem > (MaxLine - 1)) &&(CurItem-ItemFirstLine>=(MaxLine - 1)))
            ItemFirstLine++;


        UpdateMenuItems(CurView);//根据当前View更新菜单项
    }
    else if(CurHandle != NULL)//不是跳转，执行功能
        (*CurHandle)(VK_INC, CurItem);

    last_item[item_index] = CurItem;
}

void onDecrease(void)
{
    if(!isEditing) {
        CurItem--;
        if(CurItem < 1) {//根据状态转移表，进行跳转
            CurItem = ItemNums - 1;
            if(ItemNums > MaxLine)
                ItemFirstLine = ItemNums  -(MaxLine - 1);
        }

        if(ItemNums > MaxLine)
        {
            if((CurItem < ItemNums - (MaxLine - 1))&&(CurItem<ItemFirstLine))
                ItemFirstLine --;
        }

        UpdateMenuItems(CurView);//根据当前View更新菜单项
    }
    else if(CurHandle != NULL)//不是跳转，执行功能
        (*CurHandle)(VK_DEC, CurItem);

    last_item[item_index] = CurItem;
}




