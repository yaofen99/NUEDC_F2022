/*
 * uart2.h
 *
 *  Created on: 2021��8��4��
 *      Author: sundm
 */

#ifndef UART1_H_
#define UART1_H_

/*event cfg*/
#define ECO_UART_RECV_EVT        Event_Id_29
#define ECO_UART_WRITE_EVT       Event_Id_28
#define ECO_MOVE_DATA_EVT        Event_Id_27

#define ET_ALL_EVENTS            (ECO_UART_RECV_EVT    |\
                                  ECO_UART_WRITE_EVT   |\
                                  ECO_MOVE_DATA_EVT)


void uart2_WriteData(unsigned char *buf,unsigned short size);
void uart3_WriteData(unsigned char *buf,unsigned short size);
void uart3_SendData(unsigned char dat);
void ccd_send_data2(uint8_t *dat);/*����ΰҵCCD��λ��Э��*/
void ccd_send_data(uint8_t *dat);/*�����λ��CCDЭ��*/

#endif /* UART1_H_ */
