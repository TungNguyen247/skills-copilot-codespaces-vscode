/* Nucleo-G474RE: Blink LD2 (PA5) using STM32 HAL
 *
 * Board  : NUCLEO-G474RE
 * MCU    : STM32G474RET6
 * LED    : LD2 (green) on PA5
 * Toolchain: STM32CubeIDE / arm-none-eabi-gcc
 */

#include "stm32g4xx_hal.h"

/* ------------------------------------------------------------------ */
/*  Forward declarations                                               */
/* ------------------------------------------------------------------ */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void Error_Handler(void);

/* ------------------------------------------------------------------ */
/*  Main                                                               */
/* ------------------------------------------------------------------ */
int main(void)
{
    /* Reset all peripherals, initialize Flash and SysTick */
    HAL_Init();

    /* Configure the system clock to 170 MHz (PLL from HSI16) */
    SystemClock_Config();

    /* Initialize GPIO for the user LED on PA5 */
    GPIO_Init();

    /* Infinite loop: toggle LED every 500 ms */
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(500); /* 500 ms delay */
    }
}

/* ------------------------------------------------------------------ */
/*  System Clock Configuration                                         */
/*  HSI16 -> PLL -> SYSCLK = 170 MHz                                  */
/* ------------------------------------------------------------------ */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

    /** Initializes the RCC Oscillators */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM       = RCC_PLLM_DIV4;
    RCC_OscInitStruct.PLL.PLLN       = 85;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR       = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK  |
                                       RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_PCLK1  |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    /* Flash latency 4 wait-states required at 170 MHz / VOS1 boost */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

/* ------------------------------------------------------------------ */
/*  GPIO Initialization                                                */
/*  PA5 -> output push-pull, no pull, medium speed (LD2)              */
/* ------------------------------------------------------------------ */
static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIOA peripheral clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Set PA5 initial state to OFF (low) */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    /* Configure PA5 as push-pull output */
    GPIO_InitStruct.Pin   = GPIO_PIN_5;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* ------------------------------------------------------------------ */
/*  Error Handler                                                      */
/* ------------------------------------------------------------------ */
static void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        /* Stay here on unrecoverable error */
    }
}
