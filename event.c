/*
 * event thread
 *
 * */

#include "includes.h"


RingBuf_Object   ringObj;

/*
 *  ======== doEventLogs ========
 */
void doEventLogs(void)
{
    uint8_t event;
#ifdef NORMAL_DEBUG
    while(RingBuf_get(&ringObj, &event) >= 0)
    {
        if(event & Button_EV_CLICKED)
        {
            DEBUG_printf("Button:Click");
        }
        if(event & Button_EV_DOUBLECLICKED)
        {
            DEBUG_printf("Button:Double Click");
        }
        if(event & Button_EV_LONGPRESSED)
        {
            DEBUG_printf("Button:Long Pressed");
        }
    }
#endif
}
