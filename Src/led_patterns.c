#include "led_patterns.h"
#include "board.h"

void Pattern_AllOff(void)
{
    (void)Board_LedWriteMask(0x00U);
}

void Pattern_AllOn(void)
{
    (void)Board_LedWriteMask(0x0FU);
}

static void Pattern_ShowSingleLed(uint32_t step)
{
    uint8_t mask = (uint8_t)(1U << (step % 4U));
    (void)Board_LedWriteMask(mask);
}

void Pattern_OneAtATime(uint32_t step)
{
    Pattern_ShowSingleLed(step);
}

void Pattern_RunningForward(uint32_t step)
{
    Pattern_ShowSingleLed(step);
}

void Pattern_RunningReverse(uint32_t step)
{
    uint8_t ledIndex = (uint8_t)(3U - (step % 4U));
    uint8_t mask = (uint8_t)(1U << ledIndex);
    (void)Board_LedWriteMask(mask);
}

void Pattern_AlternatingPairs(uint32_t step)
{
    if ((step % 2U) == 0U)
    {
        (void)Board_LedWriteMask(0x05U);
    }
    else
    {
        (void)Board_LedWriteMask(0x0AU);
    }
}

void Pattern_BinaryCount(uint32_t step)
{
    uint8_t mask = (uint8_t)(step % 16U);
    (void)Board_LedWriteMask(mask);
}

void Pattern_BlinkAll(uint32_t step)
{
    if ((step % 2U) == 0U)
    {
        (void)Board_LedWriteMask(0x00U);
    }
    else
    {
        (void)Board_LedWriteMask(0x0FU);
    }
}


