#include "board.h"

#define BOARD_LED_PORT       GPIOD
#define BOARD_BUTTON_PORT    GPIOA
#define BOARD_BUTTON_PIN     0U

/*
 * STM32F407G-DISC1 LED pin mapping:
 * LED 0 -> PD12 (Green)
 * LED 1 -> PD13 (Orange)
 * LED 2 -> PD14 (Red)
 * LED 3 -> PD15 (Blue)
 *
 * User button -> PA0
 */

static const uint8_t ledPins[BOARD_LED_COUNT] =
{
    12U,
    13U,
    14U,
    15U
};

GPIO_Status_t Board_Init(void)
{
    GPIO_Config_t ledConfig =
    {
    	.PinNumber = ledPins[BOARD_LED_0],
        .Mode = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OTYPE_PUSHPULL,
        .Speed = GPIO_SPEED_LOW,
        .Pull = GPIO_PULL_NONE
    };

    GPIO_Config_t buttonConfig =
    {
    	.PinNumber = BOARD_BUTTON_PIN,
        .Mode = GPIO_MODE_INPUT,
        .OutputType = GPIO_OTYPE_PUSHPULL,
        .Speed = GPIO_SPEED_LOW,
        .Pull = GPIO_PULL_NONE
    };

    GPIO_Status_t status;

    /* Configure the four LEDs. */
    for (uint8_t i = 0U; i < BOARD_LED_COUNT; i++)
    {
        ledConfig.PinNumber = ledPins[i];

        status = GPIO_Init(BOARD_LED_PORT, &ledConfig);

        if (status != GPIO_STATUS_OK)
        {
            return GPIO_STATUS_ERROR;
        }
    }

    /* Configure the user button on PA0. */
    status = GPIO_Init(BOARD_BUTTON_PORT, &buttonConfig);

    if (status != GPIO_STATUS_OK)
    {
        return GPIO_STATUS_ERROR;
    }

    /* Start with all LEDs OFF. */
    for (uint8_t i = 0U; i < BOARD_LED_COUNT; i++)
    {
        GPIO_WritePin(GPIOD, ledPins[i], GPIO_PIN_RESET);
    }

    return GPIO_STATUS_OK;
}

GPIO_Status_t Board_LedWrite(Board_LedIndex_t led,
                             GPIO_PinState_t state)
{
    if ((led >= BOARD_LED_COUNT) ||
        ((state != GPIO_PIN_RESET) && (state != GPIO_PIN_SET)))
    {
        return GPIO_STATUS_ERROR;
    }

    GPIO_WritePin(GPIOD, ledPins[led], state);

    return GPIO_STATUS_OK;
}


GPIO_Status_t Board_LedWriteMask(uint8_t mask)
{
    /* Only the lowest four bits are valid. */
    if ((mask & 0xF0U) != 0U)
    {
        return GPIO_STATUS_ERROR;
    }

    for (uint8_t i = 0U; i < BOARD_LED_COUNT; i++)
    {
        GPIO_PinState_t state;

        if ((mask & (1U << i)) != 0U)
        {
            state = GPIO_PIN_SET;
        }
        else
        {
            state = GPIO_PIN_RESET;
        }

        GPIO_WritePin(GPIOD, ledPins[i], state);
    }

    return GPIO_STATUS_OK;
}


GPIO_PinState_t Board_ButtonRead(void)
{
    return GPIO_ReadPin(GPIOA, 0U);
}


