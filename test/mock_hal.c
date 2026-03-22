/* test/mock_hal.c
 * Stub implementations of STM32 HAL functions.
 * Tracks calls and state so the test suite can make assertions.
 */

#include "stm32g4xx_hal.h"
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Simulated hardware state                                           */
/* ------------------------------------------------------------------ */

/* Simulated GPIOA peripheral */
GPIO_TypeDef _mock_GPIOA = {0};

/* Clock-enable flag set by __HAL_RCC_GPIOA_CLK_ENABLE() macro */
int _mock_gpioa_clk_enabled = 0;

/* Call counters */
int mock_HAL_Init_calls                     = 0;
int mock_HAL_Delay_calls                    = 0;
int mock_HAL_RCC_OscConfig_calls            = 0;
int mock_HAL_RCC_ClockConfig_calls          = 0;
int mock_HAL_PWREx_ControlVoltageScaling_calls = 0;
int mock_HAL_GPIO_Init_calls                = 0;
int mock_HAL_GPIO_WritePin_calls            = 0;
int mock_HAL_GPIO_TogglePin_calls           = 0;

/* Captured parameters from the most recent calls */
uint32_t mock_last_delay_ms   = 0;
uint16_t mock_last_toggle_pin = 0;
GPIO_TypeDef *mock_last_toggle_port = NULL;
GPIO_PinState mock_last_write_state = GPIO_PIN_RESET;
uint16_t mock_last_write_pin  = 0;

/* Captured GPIO init parameters */
GPIO_InitTypeDef mock_last_gpio_init = {0};

/* ------------------------------------------------------------------ */
/*  Stub implementations                                               */
/* ------------------------------------------------------------------ */

HAL_StatusTypeDef HAL_Init(void)
{
    mock_HAL_Init_calls++;
    return HAL_OK;
}

void HAL_Delay(uint32_t delay_ms)
{
    mock_HAL_Delay_calls++;
    mock_last_delay_ms = delay_ms;
}

HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct)
{
    (void)RCC_OscInitStruct;
    mock_HAL_RCC_OscConfig_calls++;
    return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct,
                                      uint32_t FLatency)
{
    (void)RCC_ClkInitStruct;
    (void)FLatency;
    mock_HAL_RCC_ClockConfig_calls++;
    return HAL_OK;
}

HAL_StatusTypeDef HAL_PWREx_ControlVoltageScaling(uint32_t VoltageScaling)
{
    (void)VoltageScaling;
    mock_HAL_PWREx_ControlVoltageScaling_calls++;
    return HAL_OK;
}

void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    (void)GPIOx;
    mock_HAL_GPIO_Init_calls++;
    mock_last_gpio_init = *GPIO_Init;
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
                       GPIO_PinState PinState)
{
    mock_HAL_GPIO_WritePin_calls++;
    mock_last_write_pin   = GPIO_Pin;
    mock_last_write_state = PinState;

    if (PinState == GPIO_PIN_SET)
        GPIOx->ODR |= GPIO_Pin;
    else
        GPIOx->ODR &= ~(uint32_t)GPIO_Pin;
}

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    mock_HAL_GPIO_TogglePin_calls++;
    mock_last_toggle_pin  = GPIO_Pin;
    mock_last_toggle_port = GPIOx;
    GPIOx->ODR ^= GPIO_Pin;
}
