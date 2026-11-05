#ifndef APP_H_
#define APP_H_

#include <stdint.h>

/* Initialize application controller state. */
void App_Init(uint32_t now);

/* Update application logic using the current millisecond tick. */
void App_Update(uint32_t now);

#endif /* APP_H_ */
