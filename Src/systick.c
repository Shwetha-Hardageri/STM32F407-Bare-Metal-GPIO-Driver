#include "systick.h"
#include "systick_regs.h"

/* Incremented every 1 millisecond by the interrupt handler. */
static volatile uint32_t g_msTicks = 0U;

void SysTick_Init(void)
{
    /* Stop SysTick while configuring it. */
    SYSTICK->CTRL = 0U;

    /* 16 MHz clock: 16000 cycles per millisecond, so reload = 15999. */
    SYSTICK->LOAD = 15999U;

    /* Clear the current counter value. */
    SYSTICK->VAL = 0U;

    /* Bit 2: processor clock.
       Bit 1: enable SysTick interrupt.
       Bit 0: enable SysTick counter. */
    SYSTICK->CTRL = (1U << 2U) |
                    (1U << 1U) |
                    (1U << 0U);
}

uint32_t SysTick_GetTick(void)
{
    return g_msTicks;
}

/* Called automatically by the processor every 1 ms. */
void SysTick_Handler(void)
{
    g_msTicks= g_msTicks + 1U;
}

