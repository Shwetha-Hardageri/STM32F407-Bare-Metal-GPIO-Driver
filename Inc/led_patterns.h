#ifndef LED_PATTERNS_H_
#define LED_PATTERNS_H_

#include <stdint.h>

typedef enum
{
    APP_MODE_ALL_OFF = 0U,
    APP_MODE_ALL_ON,
    APP_MODE_RUNNING_FORWARD,
    APP_MODE_RUNNING_REVERSE,
    APP_MODE_ALTERNATING_PAIRS,
    APP_MODE_BINARY_COUNT,
    APP_MODE_BLINK_ALL,
    APP_MODE_ONE_AT_A_TIME,
    APP_MODE_COUNT
} App_Mode_t;

void Pattern_AllOff(void);
void Pattern_AllOn(void);
void Pattern_OneAtATime(uint32_t step);
void Pattern_RunningForward(uint32_t step);
void Pattern_RunningReverse(uint32_t step);
void Pattern_AlternatingPairs(uint32_t step);
void Pattern_BinaryCount(uint32_t step);
void Pattern_BlinkAll(uint32_t step);

#endif /* LED_PATTERNS_H_ */
