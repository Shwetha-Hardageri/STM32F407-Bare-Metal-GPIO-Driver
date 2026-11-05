#ifndef GPIO_REGS_H
#define GPIO_REGS_H

#include <stdint.h>
#include <stddef.h>

typedef struct{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
}GPIO_RegDef_t;

typedef struct{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	uint32_t RESERVED0;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t RESERVED1[2];
	volatile uint32_t AHB1ENR;
}RCC_RegDef_t;


_Static_assert(offsetof(GPIO_RegDef_t, BSRR)== 0x18, "GPIO BSRR offset is incorrect");
_Static_assert(offsetof(RCC_RegDef_t, AHB1ENR)== 0x30, "RCC AHB1ENR offset is incorrect");
_Static_assert(offsetof(GPIO_RegDef_t, AFR)== 0x20, "GPIO AFR offset is incorrect");

// GPIO and RCC base address macros
#define GPIOA_BASE (0x40020000U)
#define GPIOB_BASE (0x40020400U)
#define GPIOC_BASE (0x40020800U)
#define GPIOD_BASE (0x40020C00U)
#define GPIOE_BASE (0x40021000U)
#define GPIOF_BASE (0x40021400U)
#define GPIOG_BASE (0x40021800U)
#define GPIOH_BASE (0x40021C00U)
#define GPIOI_BASE (0x40022000U)

#define RCC_BASE   (0x40023800U)

// GPIO and RCC pointer macros
#define GPIOA ((GPIO_RegDef_t *)GPIOA_BASE)
#define GPIOB ((GPIO_RegDef_t *)GPIOB_BASE)
#define GPIOC ((GPIO_RegDef_t *)GPIOC_BASE)
#define GPIOD ((GPIO_RegDef_t *)GPIOD_BASE)
#define GPIOE ((GPIO_RegDef_t *)GPIOE_BASE)
#define GPIOF ((GPIO_RegDef_t *)GPIOF_BASE)
#define GPIOG ((GPIO_RegDef_t *)GPIOG_BASE)
#define GPIOH ((GPIO_RegDef_t *)GPIOH_BASE)
#define GPIOI ((GPIO_RegDef_t *)GPIOI_BASE)

#define RCC ((RCC_RegDef_t *)RCC_BASE)

#endif /* GPIO_REGS_H */
