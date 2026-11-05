#ifndef SYSTICK_REGS_H_
#define SYSTICK_REGS_H_

#include <stdint.h>
#include <stddef.h>

/* Cortex-M4 SysTick register structure. */
typedef struct
{
    volatile uint32_t CTRL;    /* 0x00: Control and status */
    volatile uint32_t LOAD;    /* 0x04: Reload value */
    volatile uint32_t VAL;     /* 0x08: Current value */
    volatile uint32_t CALIB;   /* 0x0C: Calibration */
} SysTick_RegDef_t;

/* SysTick peripheral base address. */
#define SYSTICK_BASE_ADDR    (0xE000E010UL)

#define SYSTICK              ((SysTick_RegDef_t *)SYSTICK_BASE_ADDR)

/* Verify register offsets at compile time. */
_Static_assert(offsetof(SysTick_RegDef_t, CTRL) == 0x00U,
               "SysTick CTRL offset incorrect");

_Static_assert(offsetof(SysTick_RegDef_t, LOAD) == 0x04U,
               "SysTick LOAD offset incorrect");

_Static_assert(offsetof(SysTick_RegDef_t, VAL) == 0x08U,
               "SysTick VAL offset incorrect");

_Static_assert(offsetof(SysTick_RegDef_t, CALIB) == 0x0CU,
               "SysTick CALIB offset incorrect");

#endif /* SYSTICK_REGS_H_ */
