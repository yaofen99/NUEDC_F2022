/*
 * ui.h
 *
 *  Created on: 2021年8月1日
 *      Author: sundm
 */

#ifndef UI_H_
#define UI_H_

#define INT_TY 0
#define FLOAT_TY 1


#define NOT_JUMP 255//非跳转

#define VK_ENTER 1//确认
#define VK_CANCEL 2//取消
#define VK_INC 3//加
#define VK_DEC 4//减


#define ITEM_SIZE 5

typedef uint8_t Index;
typedef void(*Interface_f)(void);
typedef void(*Interface_t)(uint8_t action_type, uint8_t pos);

union var_ty{
    int i;
    float f;
};

union p_var_ty{
    int * i;
    float * f;
};

typedef struct MenuItems
{
        uint8_t x;
        uint8_t y;
        Index EnterIndex;
        uint8_t *title;
        union p_var_ty var_ptr;
        union var_ty step;
        int var_ty;
        union var_ty var_max;
        union var_ty var_min;
}MenuItem;//菜单项


typedef struct MenuState
{
    Index CurIndex;//当前索引
    Index BackIndex;//返回索引
    Interface_f onDraw;//菜单绘图函数
    Interface_t onHandle;//菜单处理函数
}MenuState;//菜单界面

extern int motor_start_flag;

void ui_init(void);//初始化函数
void Draw(void);//绘图API
void Invalidate(void);//强制刷新界面
void UpdateMenuItems(uint8_t view_id);//更新菜单项
void goBack(void);//返回上一级菜单


void onEnter(void);//确认键回调
void onCancel(void);//取消键回调
void onIncrease(void);//增加键回调
void onDecrease(void);//减小键回调
void onPageUp(void);//上一页键回调
void onPageDown(void);//下一页键回调

#endif /* UI_H_ */
