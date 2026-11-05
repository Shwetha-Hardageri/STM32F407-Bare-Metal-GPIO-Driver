#include "gpio.h"
#include <stddef.h>

GPIO_Status_t GPIO_Init(GPIO_RegDef_t *pGPIOx, const GPIO_Config_t *pConfig)
{
    uint32_t pin;
    uint32_t shift;
    uint32_t portAddress;
    uint32_t clockBit;

//Step 1: Validate pointers and pin number. Do this before accessing any hardware register.
    if ((pGPIOx == NULL) || (pConfig == NULL))
    {
        return GPIO_STATUS_ERROR;
    }

    if (pConfig->PinNumber > 15U)
    {
        return GPIO_STATUS_ERROR;
    }

 // Validate configuration values. Casting to uint32_t also rejects negative enum values.
    if (((uint32_t)pConfig->Mode > GPIO_MODE_ANALOG) ||
        ((uint32_t)pConfig->OutputType > GPIO_OTYPE_OPENDRAIN) ||
        ((uint32_t)pConfig->Speed > GPIO_SPEED_VERY_HIGH) ||
        ((uint32_t)pConfig->Pull > GPIO_PULL_DOWN))
    {
        return GPIO_STATUS_ERROR;
    }

    //Step 2: Validate that pGPIOx represents a GPIO port. from A through I, at a valid 0x400-byte interval.
    portAddress = (uint32_t)pGPIOx;

    if ((portAddress < GPIOA_BASE) ||
        (portAddress > GPIOI_BASE) ||
        (((portAddress - GPIOA_BASE) % 0x400U) != 0U))
    {
        return GPIO_STATUS_ERROR;
    }

    clockBit = (portAddress - GPIOA_BASE) / 0x400U;

    /* Enable the clock for the selected GPIO port. */
    RCC->AHB1ENR |= (1U << clockBit);

    (void)RCC->AHB1ENR;

    pin = pConfig->PinNumber;
    shift = 2U * pin;

    //Step 3: Configure OTYPER.* Each pin uses only one bit, so shift by pin,* not by 2 * pin.
    pGPIOx->OTYPER &= ~(1U << pin);
    pGPIOx->OTYPER |= ((uint32_t)pConfig->OutputType << pin);

    //Step 4: Configure OSPEEDR.
    pGPIOx->OSPEEDR &= ~(3U << shift);
    pGPIOx->OSPEEDR |= ((uint32_t)pConfig->Speed << shift);

    //Step 5: Configure PUPDR. Each pin uses a 2-bit field for pull-up/pull-down.
    pGPIOx->PUPDR &= ~(3U << shift);
    pGPIOx->PUPDR |= ((uint32_t)pConfig->Pull << shift);

    //Step 6: Configure MODER.* Each pin uses a 2-bit field. * First clear the field, then insert the requested mode.
    pGPIOx->MODER &= ~(3U << shift);
    pGPIOx->MODER |= ((uint32_t)pConfig->Mode << shift);

    return GPIO_STATUS_OK;
}

void GPIO_WritePin(GPIO_RegDef_t *pGPIOx,
                   uint8_t pinNumber,
                   GPIO_PinState_t state)
{
    /* Validate the port pointer and pin number. */
    if ((pGPIOx == NULL) || (pinNumber > 15U))
    {
        return;
    }

    /* Reject unsupported pin-state values. */
    if ((state != GPIO_PIN_SET) && (state != GPIO_PIN_RESET))
    {
        return;
    }

    /*
     * BSRR bits 0-15 set pins.
     * BSRR bits 16-31 reset pins.
     * One assignment performs the requested operation.
     */
    pGPIOx->BSRR = (state == GPIO_PIN_SET)
                   ? (1U << pinNumber)
                   : (1U << (pinNumber + 16U));
}


/*
 * Function: GPIO_ReadPin
 * Purpose : Reads a pin's current logic level from IDR.
 * Returns : GPIO_PIN_SET for HIGH, GPIO_PIN_RESET for LOW.
 *           Invalid arguments return GPIO_PIN_RESET.
 */
GPIO_PinState_t GPIO_ReadPin(GPIO_RegDef_t *pGPIOx,
                             uint8_t pinNumber)
{
    /* Validate the port pointer and pin number. */
    if ((pGPIOx == NULL) || (pinNumber > 15U))
    {
        return GPIO_PIN_RESET;
    }

    /* Test the selected bit in the input data register. */
    if ((pGPIOx->IDR & (1U << pinNumber)) != 0U)
    {
        return GPIO_PIN_SET;
    }

    return GPIO_PIN_RESET;
}


/*
 * Function: GPIO_TogglePin
 * Purpose : Reverses the output latch state of a GPIO pin.
 *
 * Note: The read-then-write sequence is not atomic as a whole.
 * Another execution context changing the same pin could cause
 * a race between reading ODR and writing BSRR.
 */
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx,
                    uint8_t pinNumber)
{
    uint32_t pinMask;

    /* Validate the port pointer and pin number. */
    if ((pGPIOx == NULL) || (pinNumber > 15U))
    {
        return;
    }

    pinMask = (1U << pinNumber);

    /* Read the output latch and write the opposite state. */
    if ((pGPIOx->ODR & pinMask) != 0U)
    {
        /* Currently HIGH: reset the pin. */
        pGPIOx->BSRR = (1U << (pinNumber + 16U));
    }
    else
    {
        /* Currently LOW: set the pin. */
        pGPIOx->BSRR = pinMask;
    }
}
