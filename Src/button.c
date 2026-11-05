#include "button.h"
#include "board.h"

/* Button must remain stable for 20 ms. */
#define BUTTON_DEBOUNCE_MS    20U

/* Last accepted, stable button state. */
static GPIO_PinState_t stableState = GPIO_PIN_RESET;

/* Most recently sampled raw button state. */
static GPIO_PinState_t candidateState = GPIO_PIN_RESET;

/* Time when the candidate state was first observed. */
static uint32_t candidateSince = 0U;


void Button_Init(uint32_t now)
{
    /*
     * If the button is already held during reset,
     * its initial state is treated as stable.
     * Therefore, no new PRESSED event is generated.
     */
    /* Read the initial physical button state. */
    stableState = Board_ButtonRead();

    /* Start candidate state from the same reading. */
    candidateState = stableState;

    candidateSince = now;
}


Button_Event_t Button_Update(uint32_t now)
{
    /* Read the physical PA0 button. */
    GPIO_PinState_t rawState = Board_ButtonRead();

    /* Detect a change in the raw input. */
    if (rawState != candidateState)
    {
        candidateState = rawState;
        candidateSince = now;
    }

    /* Accept the new state only after 20 ms of stability. */
    if ((candidateState != stableState) &&
        ((uint32_t)(now - candidateSince) >= BUTTON_DEBOUNCE_MS))
    {
        stableState = candidateState;

        if (stableState == GPIO_PIN_SET)
        {
            return BUTTON_EVENT_PRESSED;
        }
        else
        {
            return BUTTON_EVENT_RELEASED;
        }
    }

    return BUTTON_EVENT_NONE;
}
