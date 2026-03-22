/* test/stm32g4xx_hal.h
 * Mock HAL header for host-side unit tests of main.c (Nucleo-G474RE blink).
 * Provides all types, macros, and function prototypes referenced by main.c so
 * that the source can be compiled with a standard x86 gcc toolchain.
 */

#ifndef STM32G4XX_HAL_H
#define STM32G4XX_HAL_H

#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Basic types                                                        */
/* ------------------------------------------------------------------ */
typedef enum { HAL_OK = 0, HAL_ERROR = 1, HAL_BUSY = 2, HAL_TIMEOUT = 3 } HAL_StatusTypeDef;
typedef enum { GPIO_PIN_RESET = 0, GPIO_PIN_SET = 1 } GPIO_PinState;

/* ------------------------------------------------------------------ */
/*  GPIO                                                               */
/* ------------------------------------------------------------------ */
#define GPIO_PIN_5            ((uint16_t)0x0020U)
#define GPIO_MODE_OUTPUT_PP   0x00000001U
#define GPIO_NOPULL           0x00000000U
#define GPIO_SPEED_FREQ_MEDIUM 0x00000002U

typedef struct {
    uint16_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
} GPIO_InitTypeDef;

/* Simulated GPIO peripheral */
typedef struct {
    uint32_t ODR;   /* output data register (bitmask per pin) */
    uint32_t MODER; /* mode register */
    uint32_t PUPDR; /* pull-up/pull-down register */
    uint32_t OSPEEDR;
} GPIO_TypeDef;

/* Single simulated GPIOA instance (defined in mock_hal.c) */
extern GPIO_TypeDef _mock_GPIOA;
#define GPIOA (&_mock_GPIOA)

/* ------------------------------------------------------------------ */
/*  RCC                                                                */
/* ------------------------------------------------------------------ */
#define RCC_OSCILLATORTYPE_HSI       0x00000002U
#define RCC_HSI_ON                   0x00000001U
#define RCC_HSICALIBRATION_DEFAULT   64U
#define RCC_PLL_ON                   0x00000002U
#define RCC_PLLSOURCE_HSI            0x00000002U
#define RCC_PLLM_DIV4                4U
#define RCC_PLLP_DIV2                1U
#define RCC_PLLQ_DIV2                1U
#define RCC_PLLR_DIV2                1U

#define RCC_CLOCKTYPE_HCLK   0x00000001U
#define RCC_CLOCKTYPE_SYSCLK 0x00000002U
#define RCC_CLOCKTYPE_PCLK1  0x00000004U
#define RCC_CLOCKTYPE_PCLK2  0x00000008U

#define RCC_SYSCLKSOURCE_PLLCLK 0x00000003U
#define RCC_SYSCLK_DIV1         0x00000000U
#define RCC_HCLK_DIV1           0x00000000U

#define FLASH_LATENCY_4  4U

typedef struct {
    uint32_t PLLState;
    uint32_t PLLSource;
    uint32_t PLLM;
    uint32_t PLLN;
    uint32_t PLLP;
    uint32_t PLLQ;
    uint32_t PLLR;
} RCC_PLLInitTypeDef;

typedef struct {
    uint32_t            OscillatorType;
    uint32_t            HSIState;
    uint32_t            HSICalibrationValue;
    RCC_PLLInitTypeDef  PLL;
} RCC_OscInitTypeDef;

typedef struct {
    uint32_t ClockType;
    uint32_t SYSCLKSource;
    uint32_t AHBCLKDivider;
    uint32_t APB1CLKDivider;
    uint32_t APB2CLKDivider;
} RCC_ClkInitTypeDef;

/* ------------------------------------------------------------------ */
/*  PWR                                                                */
/* ------------------------------------------------------------------ */
#define PWR_REGULATOR_VOLTAGE_SCALE1_BOOST 1U

/* ------------------------------------------------------------------ */
/*  RCC clock-enable macro (no-op stub)                               */
/* ------------------------------------------------------------------ */
extern int _mock_gpioa_clk_enabled;
#define __HAL_RCC_GPIOA_CLK_ENABLE() (_mock_gpioa_clk_enabled = 1)

/* ------------------------------------------------------------------ */
/*  Interrupt control (no-op on host)                                  */
/* ------------------------------------------------------------------ */
#define __disable_irq() ((void)0)

/* ------------------------------------------------------------------ */
/*  HAL function prototypes (implemented in mock_hal.c)               */
/* ------------------------------------------------------------------ */
HAL_StatusTypeDef HAL_Init(void);
void              HAL_Delay(uint32_t delay_ms);
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct,
                                      uint32_t FLatency);
HAL_StatusTypeDef HAL_PWREx_ControlVoltageScaling(uint32_t VoltageScaling);
void              HAL_GPIO_Init(GPIO_TypeDef *GPIOx,
                                GPIO_InitTypeDef *GPIO_Init);
void              HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
                                   GPIO_PinState PinState);
void              HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif /* STM32G4XX_HAL_H */
