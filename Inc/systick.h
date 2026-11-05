#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

void SysTick_Init(void);
uint32_t SysTick_GetTick(void);

#endif /* SYSTICK_H_ */
