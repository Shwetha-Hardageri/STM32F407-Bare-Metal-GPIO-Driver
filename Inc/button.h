#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>

typedef enum
{
    BUTTON_EVENT_NONE = 0U,
    BUTTON_EVENT_PRESSED,
    BUTTON_EVENT_RELEASED
} Button_Event_t;

void Button_Init(uint32_t now);
Button_Event_t Button_Update(uint32_t now);

#endif /* BUTTON_H_ */
