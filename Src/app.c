#include "app.h"
#include "board.h"
#include "button.h"
#include "led_patterns.h"

/* Application timing constants. */
#define PATTERN_INTERVAL_MS    500U
#define LONG_PRESS_MS          2000U

/* Current application mode. */
static App_Mode_t currentMode = APP_MODE_ALL_OFF;

/* Current step within a pattern. */
static uint32_t patternStep = 0U;

/* Time of the last automatic pattern update. */
static uint32_t lastPatternUpdate = 0U;

/* Button press tracking. */
static uint32_t pressStartTime = 0U;
static uint8_t buttonIsPressed = 0U;
static uint8_t longPressHandled = 0U;


/* Select and display the current pattern. */
static void App_ShowPattern(void)
{
    switch (currentMode)
    {
        case APP_MODE_ALL_OFF:
            Pattern_AllOff();
            break;

        case APP_MODE_ALL_ON:
            Pattern_AllOn();
            break;

        case APP_MODE_RUNNING_FORWARD:
            Pattern_RunningForward(patternStep);
            break;

        case APP_MODE_RUNNING_REVERSE:
            Pattern_RunningReverse(patternStep);
            break;

        case APP_MODE_ALTERNATING_PAIRS:
            Pattern_AlternatingPairs(patternStep);
            break;

        case APP_MODE_BINARY_COUNT:
            Pattern_BinaryCount(patternStep);
            break;

        case APP_MODE_BLINK_ALL:
            Pattern_BlinkAll(patternStep);
            break;

        case APP_MODE_ONE_AT_A_TIME:
            Pattern_OneAtATime(patternStep);
            break;

        default:
            currentMode = APP_MODE_ALL_OFF;
            Pattern_AllOff();
            break;
    }
}


/* Move to the next application mode. */
static void App_NextMode(uint32_t now)
{
    currentMode++;

    if (currentMode >= APP_MODE_COUNT)
    {
        currentMode = APP_MODE_ALL_OFF;
    }

    patternStep = 0U;
    lastPatternUpdate = now;

    App_ShowPattern();
}


void App_Init(uint32_t now)
{
    currentMode = APP_MODE_ALL_OFF;
    patternStep = 0U;

    lastPatternUpdate = now;

    pressStartTime = 0U;
    buttonIsPressed = 0U;
    longPressHandled = 0U;

    Button_Init(now);

    Pattern_AllOff();
}


void App_Update(uint32_t now)
{
    Button_Event_t event = Button_Update(now);

    /* Handle a newly confirmed button press. */
    if (event == BUTTON_EVENT_PRESSED)
    {
        buttonIsPressed = 1U;
        pressStartTime = now;
        longPressHandled = 0U;
    }

    /* Detect a long press while the button is held. */
    if ((buttonIsPressed != 0U) &&
        (longPressHandled == 0U) &&
        ((uint32_t)(now - pressStartTime) >= LONG_PRESS_MS))
    {
        longPressHandled = 1U;

        currentMode = APP_MODE_ALL_OFF;
        patternStep = 0U;
        lastPatternUpdate = now;

        Pattern_AllOff();
    }

    /* Handle a confirmed button release. */
    if (event == BUTTON_EVENT_RELEASED)
    {
        buttonIsPressed = 0U;

        if (longPressHandled == 0U)
        {
            if (currentMode == APP_MODE_ONE_AT_A_TIME)
            {
                patternStep++;
                Pattern_OneAtATime(patternStep);
            }
            else
            {
                App_NextMode(now);
            }
        }
    }

    /* Advance automatic patterns every 500 ms. */
    if ((currentMode != APP_MODE_ALL_OFF) &&
        (currentMode != APP_MODE_ALL_ON) &&
        (currentMode != APP_MODE_ONE_AT_A_TIME) &&
        ((uint32_t)(now - lastPatternUpdate) >= PATTERN_INTERVAL_MS))
    {
        lastPatternUpdate = now;
        patternStep++;

        App_ShowPattern();
    }
}
