#ifndef BOARD_H_
#define BOARD_H_

#include <stdint.h>
#include "gpio.h"

/*
 * Board hardware abstraction layer.
 *
 * This layer maps logical LED indices and the user button
 * to the physical GPIO pins on the STM32F407G-DISC1 board.
 */

/* Logical LED identifiers; application code need not know pin numbers. */
typedef enum
{
    BOARD_LED_0 = 0U,
    BOARD_LED_1,
    BOARD_LED_2,
    BOARD_LED_3,
    BOARD_LED_COUNT
} Board_LedIndex_t;

/* Initialize the board's LEDs and user button. */
GPIO_Status_t Board_Init(void);

/* Set or reset one LED using its logical index. */
GPIO_Status_t Board_LedWrite(Board_LedIndex_t led,
                             GPIO_PinState_t state);

GPIO_Status_t Board_LedWriteMask(uint8_t mask);

/* Read the user button's current state. */
GPIO_PinState_t Board_ButtonRead(void);

#endif /* BOARD_H_ */
