#ifndef EVENT_H_
#define EVENT_H_

#include <ti/drivers/utils/RingBuf.h>

extern RingBuf_Object   ringObj;

extern void doEventLogs(void);


#endif /* EVENT_H_ */
