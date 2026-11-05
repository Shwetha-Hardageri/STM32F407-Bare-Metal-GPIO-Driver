# Bare-Metal GPIO Driver and LED/Button Controller on STM32F407G-DISC1

HAL-free, register-level GPIO driver in Embedded C, with a SysTick-based LED/button controller, on the STM32F407G-DISC1 (ARM Cortex-M4).

Demo: all eight modes cycled by short press (including one-at-a-time stepping), then a 2 s long-press reset to all off.
(https://github.com/user-attachments/assets/d2bb92d5-678a-4441-9fc5-25716ec78614)

## Overview
A reusable GPIO driver built from direct register access (no HAL, no LL), plus an application that uses it to run eight LED modes controlled by the user button. The driver knows nothing about LEDs or buttons. The application never touches a register.

## Hardware and tools
- STM32F407G-DISC1 (STM32F407VGT6), on-board ST-LINK
- STM32CubeIDE, arm-none-eabi-gcc, GDB via ST-LINK
- Clock: default 16 MHz HSI (`RCC->CFGR` read as `0x00000000`)

## Pin map
| Function | Pin |
|---|---|
| LED 0 green | PD12 |
| LED 1 orange | PD13 |
| LED 2 red | PD14 |
| LED 3 blue | PD15 |
| User button (active-high) | PA0 |

Button polarity was measured: IDR bit 0 reads 1 when pressed and 0 when released, with no internal pull enabled. An external pull-down is inferred from this, not confirmed from the schematic.

## Architecture
```
main.c -> app.c -> led_patterns.c -> board.c -> gpio.c -> gpio_regs.h (registers)
              \-> button.c (debounce) -> board.c
              \-> systick.c (1 ms tick)
```
| File | Role |
|---|---|
| `gpio_regs.h` | GPIO and RCC register structs, base addresses, `_Static_assert` offset checks |
| `gpio.c/.h` | Init, write (BSRR), read, toggle, with parameter validation |
| `board.c/.h` | LED and button mapping, `Board_LedWriteMask` |
| `led_patterns.c/.h` | One function per pattern |
| `systick_regs.h`, `systick.c` | SysTick registers, 1 ms tick (LOAD = 15999) |
| `button.c/.h` | 20 ms debounce, press and release events |
| `app.c/.h` | Mode state machine, short and long press |

## Features
- Register-level GPIO driver: MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR
- Port clock enabled from the port address, with all validation done before any register write
- BSRR for single-write pin set and reset
- Eight modes: all off, all on, running forward, running reverse, alternating pairs, binary count, blink all, one-at-a-time
- Short press: next mode (in one-at-a-time mode it advances the LED). Long press (2 s): reset to all off
- Non-blocking timing from a SysTick millisecond counter

## Build and flash
1. Open STM32CubeIDE and import the project (File, Import, Existing Projects into Workspace).
2. Build (hammer icon).
3. Connect the board's mini-USB (ST-LINK) port, then Run, Debug As, STM32 C/C++ Application.

## Test results (measured on hardware)
| Area | Test | Result |
|---|---|---|
| GPIO_Init | PD12 push-pull, medium, no pull | OTYPER 0x0, OSPEEDR 0x01000000, PUPDR 0x0, MODER 0x01000000, returns OK |
| GPIO_Init | PD13 open-drain, very high, pull-up | OTYPER 0x2000, OSPEEDR 0x0C000000, PUPDR 0x04000000, MODER 0x04000000 |
| GPIO_Init | Two pins configured | Second pin did not disturb the first |
| GPIO_Init | Invalid pin 16, NULL port, Pull = 3, misaligned port | All return error; registers 0; RCC 0x00100000 |
| Clock gating | Write to GPIOD_MODER with clock off | Read back 0; with clock on, 0x01000000 |
| Output | WritePin then TogglePin on PD12 | ODR 0x5000 then 0x4000 |
| Output | WritePin with pin 16 | ODR unchanged |
| Input | PA0 pressed / released | SET / RESET |
| Board | Mask 0x5, 0xA | Green+red, orange+blue |
| Board | Mask 0x10 | Returns error, LEDs unchanged |
| Patterns | All seven | Matched expected sequence (visual) |
| Controller | Eight modes, wrap, mode 7 stepping, 2 s long press | Matched design |
| Vector table | `0x0800003C` vs `SysTick_Handler` | `0x08000601` = handler `0x08000600` with Thumb bit set |
| Timing | 20 running-forward steps, hand stopwatch | 10.56 s (about 528 ms per step vs 500 ms target) |
| Fault injection | Clock enable removed | AHB1ENR 0x00100000, MODER 0, no LEDs |
| Fault injection | MODER clear mask 3U changed to 1U, field starts at 10 | 0xFF000000 instead of 0x55000000 |
| Build | -O0 | text 2604, data 0, bss 1592 |
| Build | -O2 | text 1764, data 0, bss 1600; same behavior |

**Not tested:** NULL config pointer, aligned port address outside GPIOA to GPIOI.

## Known limitations
- `GPIO_TogglePin` is read-then-write, so it is not safe if the same pin is also written from an ISR
- Step interval measured about 5.6% slow by hand stopwatch; the system runs from the uncalibrated HSI oscillator and no PLL is configured
- Button is polled, with no EXTI
- `Board_LedWriteMask` writes the LEDs one by one, not in a single BSRR write
- Tests were debugger and visual checks, not an automated test suite
- FPU is unused

## Possible extensions
EXTI button input, single-write multi-pin BSRR function, PLL clock setup, UART driver.






