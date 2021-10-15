/*
 * ui.h
 *
 *  Created on: 2021��8��1��
 *      Author: sundm
 */

#ifndef UI_H_
#define UI_H_

#define INT_TY 0
#define FLOAT_TY 1


#define NOT_JUMP 255//����ת

#define VK_ENTER 1//ȷ��
#define VK_CANCEL 2//ȡ��
#define VK_INC 3//��
#define VK_DEC 4//��


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
}MenuItem;//�˵���


typedef struct MenuState
{
    Index CurIndex;//��ǰ����
    Index BackIndex;//��������
    Interface_f onDraw;//�˵���ͼ����
    Interface_t onHandle;//�˵�������
}MenuState;//�˵�����

extern int motor_start_flag;

void ui_init(void);//��ʼ������
void Draw(void);//��ͼAPI
void Invalidate(void);//ǿ��ˢ�½���
void UpdateMenuItems(uint8_t view_id);//���²˵���
void goBack(void);//������һ���˵�


void onEnter(void);//ȷ�ϼ��ص�
void onCancel(void);//ȡ�����ص�
void onIncrease(void);//���Ӽ��ص�
void onDecrease(void);//��С���ص�
void onPageUp(void);//��һҳ���ص�
void onPageDown(void);//��һҳ���ص�

#endif /* UI_H_ */
