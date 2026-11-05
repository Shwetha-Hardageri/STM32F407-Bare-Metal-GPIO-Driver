#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include "gpio_regs.h"

/*
 * GPIO driver header file.
 * Defines GPIO configuration types, status codes,
 * and function declarations for GPIO operations.
 */

/*
 * GPIO_Mode_t:
 * Defines the operating mode of a GPIO pin.
 *
 * INPUT     - Pin works as a digital input.
 * OUTPUT    - Pin works as a digital output.
 * ALTERNATE - Pin is connected to an alternate peripheral function.
 * ANALOG    - Pin works in analog mode.
 *
 * These values correspond to the STM32 MODER register encodings.
 */

//enum lets us define a group of named integer constants
typedef enum{
	GPIO_MODE_INPUT = 0,
	GPIO_MODE_OUTPUT = 1,
	GPIO_MODE_ALTERNATE = 2,
	GPIO_MODE_ANALOG = 3
}GPIO_Mode_t;

/*
 * GPIO_OutputType_t:
 * Defines the output driver type of a GPIO pin.
 *
 * PUSHPULL  - Output can drive logic HIGH and LOW.
 * OPENDRAIN - Output can pull LOW; an external or internal
 *             pull-up is generally needed to produce HIGH.
 *
 * These values correspond to the OTYPER register.
 */
typedef enum{
	GPIO_OTYPE_PUSHPULL= 0,
	GPIO_OTYPE_OPENDRAIN = 1
}GPIO_OutputType_t;

/*
 * GPIO_Speed_t:
 * Defines the output transition speed of a GPIO pin.
 *
 * LOW       - Low output speed.
 * MEDIUM    - Medium output speed.
 * HIGH      - High output speed.
 * VERY_HIGH - Very high output speed.
 *
 * These values correspond to the OSPEEDR register.
 * Select a speed appropriate for the application.
 */
typedef enum{
	GPIO_SPEED_LOW= 0,
	GPIO_SPEED_MEDIUM= 1,
	GPIO_SPEED_HIGH= 2,
	GPIO_SPEED_VERY_HIGH= 3
}GPIO_Speed_t;

/*
 * GPIO_Pull_t:
 * Defines the internal pull-up/pull-down configuration.
 *
 * NONE - No internal pull-up or pull-down.
 * UP   - Enables the internal pull-up.
 * DOWN - Enables the internal pull-down.
 *
 * These values correspond to the PUPDR register.
 */
typedef enum{
	GPIO_PULL_NONE= 0,
	GPIO_PULL_UP= 1,
	GPIO_PULL_DOWN= 2
}GPIO_Pull_t;

/*
 * GPIO_PinState_t:
 * Represents the logical state of a GPIO pin.
 *
 * RESET - Logic LOW.
 * SET   - Logic HIGH.
 */
typedef enum{
	GPIO_PIN_RESET= 0,
	GPIO_PIN_SET = 1
}GPIO_PinState_t;

/*
 * GPIO_Status_t:
 * Indicates whether a GPIO driver operation succeeded.
 *
 * OK    - Operation completed successfully.
 * ERROR - Operation failed because of invalid arguments
 *         or another detected error.
 */
typedef enum{
    GPIO_STATUS_OK = 0,
    GPIO_STATUS_ERROR = 1
} GPIO_Status_t;

/*
 * GPIO_Config_t:
 * Stores the configuration settings for one GPIO pin.
 *
 * PinNumber - Pin number from 0 to 15.
 * Mode      - Input, output, alternate function, or analog.
 * OutputType- Push-pull or open-drain.
 * Speed     - Output transition speed.
 * Pull      - Pull-up, pull-down, or no pull.
 *
 * This structure is passed to GPIO_Init().
 */
typedef struct{
	uint8_t PinNumber;
	GPIO_Mode_t Mode;
	GPIO_OutputType_t OutputType;
	GPIO_Speed_t Speed;
	GPIO_Pull_t Pull;
}GPIO_Config_t;

//Configures a GPIO port pin using your configuration structure.
GPIO_Status_t GPIO_Init(GPIO_RegDef_t *pGPIOx, const GPIO_Config_t *pConfig);
//Writes the requested HIGH or LOW state to a pin.
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, GPIO_PinState_t state);
//Reads a pin and returns its state
GPIO_PinState_t GPIO_ReadPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);
//Toggling means reversing the current output state:
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);
#endif /* GPIO_H_ */

/*
* Function: GPIO_Init
* Purpose : Validates the configuration, enables the GPIO port
*           clock, and configures the selected pin's registers.
* Parameters:
*           pGPIOx  - Pointer to the GPIO port.
*           pConfig - Pointer to the pin configuration structure.
* Returns : GPIO_STATUS_OK on success;
*           GPIO_STATUS_ERROR if an error is detected.
*/

/*
 * Function: GPIO_WritePin
 * Purpose : Sets a GPIO output pin HIGH or LOW.
 * Parameters:
 *           pGPIOx    - Pointer to the GPIO port.
 *           pinNumber - Pin number from 0 to 15.
 *           state     - Desired state: GPIO_PIN_SET or
 *                       GPIO_PIN_RESET.
 * Note    : The pin should be configured as an output.
 */

/*
 * Function: GPIO_ReadPin
 * Purpose : Reads the current logic level of a GPIO pin.
 * Parameters:
 *           pGPIOx    - Pointer to the GPIO port.
 *           pinNumber - Pin number from 0 to 15.
 * Returns : GPIO_PIN_SET if the pin reads HIGH;
 *           GPIO_PIN_RESET if the pin reads LOW.
 */

/*
 * Function: GPIO_TogglePin
 * Purpose : Reverses the current output state of a GPIO pin.
 *           HIGH becomes LOW, and LOW becomes HIGH.
 * Parameters:
 *           pGPIOx    - Pointer to the GPIO port.
 *           pinNumber - Pin number from 0 to 15.
 * Note    : Intended for pins configured as outputs.
 */
