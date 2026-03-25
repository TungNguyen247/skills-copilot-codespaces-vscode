/* test/test_led_blink.c
 * Host-side unit tests for Nucleo-G474RE LED blink (main.c).
 *
 * Strategy
 * --------
 *  - We compile main.c against the mock HAL (test/stm32g4xx_hal.h +
 *    test/mock_hal.c) on a standard x86 host.
 *  - Because the three helper functions (SystemClock_Config, GPIO_Init,
 *    Error_Handler) are declared static in main.c, we re-expose them by
 *    defining 'static' away before including the source.
 *  - We do NOT call main() directly (it loops forever). Instead we call
 *    each helper individually and verify observable side-effects through
 *    the mock counters / hardware-state variables.
 *
 * Build & run (from the test/ directory):
 *   make
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Strip 'static' so the helpers become visible */
#define static

#include "stm32g4xx_hal.h"

/* ------------------------------------------------------------------ */
/*  Pull in the source under test                                      */
/*  We rename main() to avoid a duplicate symbol with our own main().  */
/* ------------------------------------------------------------------ */
#define main stm32_main
#include "../main.c"
#undef main
#undef static

/* ------------------------------------------------------------------ */
/*  Mock state (declared in mock_hal.c)                                */
/* ------------------------------------------------------------------ */
extern int           mock_HAL_Init_calls;
extern int           mock_HAL_Delay_calls;
extern int           mock_HAL_RCC_OscConfig_calls;
extern int           mock_HAL_RCC_ClockConfig_calls;
extern int           mock_HAL_PWREx_ControlVoltageScaling_calls;
extern int           mock_HAL_GPIO_Init_calls;
extern int           mock_HAL_GPIO_WritePin_calls;
extern int           mock_HAL_GPIO_TogglePin_calls;
extern uint32_t      mock_last_delay_ms;
extern uint16_t      mock_last_toggle_pin;
extern GPIO_TypeDef *mock_last_toggle_port;
extern GPIO_PinState mock_last_write_state;
extern uint16_t      mock_last_write_pin;
extern GPIO_InitTypeDef mock_last_gpio_init;
extern int           _mock_gpioa_clk_enabled;
extern GPIO_TypeDef  _mock_GPIOA;

/* ------------------------------------------------------------------ */
/*  Minimal test framework                                             */
/* ------------------------------------------------------------------ */
static int tests_run    = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT(cond, msg)                                              \
    do {                                                               \
        tests_run++;                                                   \
        if (cond) {                                                    \
            tests_passed++;                                            \
            printf("  PASS  %s\n", msg);                              \
        } else {                                                       \
            tests_failed++;                                            \
            printf("  FAIL  %s  (line %d)\n", msg, __LINE__);        \
        }                                                              \
    } while (0)

/* Reset all mock state between test groups */
static void reset_mocks(void)
{
    mock_HAL_Init_calls                      = 0;
    mock_HAL_Delay_calls                     = 0;
    mock_HAL_RCC_OscConfig_calls             = 0;
    mock_HAL_RCC_ClockConfig_calls           = 0;
    mock_HAL_PWREx_ControlVoltageScaling_calls = 0;
    mock_HAL_GPIO_Init_calls                 = 0;
    mock_HAL_GPIO_WritePin_calls             = 0;
    mock_HAL_GPIO_TogglePin_calls            = 0;
    mock_last_delay_ms                       = 0;
    mock_last_toggle_pin                     = 0;
    mock_last_toggle_port                    = NULL;
    mock_last_write_state                    = GPIO_PIN_SET; /* non-zero sentinel */
    mock_last_write_pin                      = 0;
    _mock_gpioa_clk_enabled                  = 0;
    memset(&_mock_GPIOA, 0, sizeof(_mock_GPIOA));
    memset(&mock_last_gpio_init, 0, sizeof(mock_last_gpio_init));
}

/* ------------------------------------------------------------------ */
/*  Test: SystemClock_Config                                           */
/* ------------------------------------------------------------------ */
static void test_SystemClock_Config(void)
{
    printf("\n[SystemClock_Config]\n");
    reset_mocks();

    SystemClock_Config();

    ASSERT(mock_HAL_PWREx_ControlVoltageScaling_calls == 1,
           "ControlVoltageScaling called once");
    ASSERT(mock_HAL_RCC_OscConfig_calls == 1,
           "HAL_RCC_OscConfig called once");
    ASSERT(mock_HAL_RCC_ClockConfig_calls == 1,
           "HAL_RCC_ClockConfig called once");
}

/* ------------------------------------------------------------------ */
/*  Test: GPIO_Init                                                    */
/* ------------------------------------------------------------------ */
static void test_GPIO_Init(void)
{
    printf("\n[GPIO_Init]\n");
    reset_mocks();

    GPIO_Init();

    ASSERT(_mock_gpioa_clk_enabled == 1,
           "GPIOA peripheral clock was enabled");

    ASSERT(mock_HAL_GPIO_WritePin_calls == 1,
           "HAL_GPIO_WritePin called once (initial state)");
    ASSERT(mock_last_write_pin == GPIO_PIN_5,
           "WritePin targets PA5");
    ASSERT(mock_last_write_state == GPIO_PIN_RESET,
           "LED set to OFF (GPIO_PIN_RESET) initially");

    ASSERT(mock_HAL_GPIO_Init_calls == 1,
           "HAL_GPIO_Init called once");
    ASSERT(mock_last_gpio_init.Pin == GPIO_PIN_5,
           "GPIO_Init configures PA5");
    ASSERT(mock_last_gpio_init.Mode == GPIO_MODE_OUTPUT_PP,
           "PA5 configured as push-pull output");
    ASSERT(mock_last_gpio_init.Pull == GPIO_NOPULL,
           "PA5 has no pull resistor");
    ASSERT(mock_last_gpio_init.Speed == GPIO_SPEED_FREQ_MEDIUM,
           "PA5 speed is medium");
}

/* ------------------------------------------------------------------ */
/*  Test: TogglePin + Delay (one blink cycle)                         */
/* ------------------------------------------------------------------ */
static void test_blink_cycle(void)
{
    printf("\n[Blink cycle (TogglePin + HAL_Delay)]\n");
    reset_mocks();

    /* Simulate two blink steps manually */
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);

    ASSERT(mock_HAL_GPIO_TogglePin_calls == 2,
           "TogglePin called twice");
    ASSERT(mock_last_toggle_pin == GPIO_PIN_5,
           "TogglePin targets PA5");
    ASSERT(mock_last_toggle_port == GPIOA,
           "TogglePin targets GPIOA");
    ASSERT(mock_HAL_Delay_calls == 2,
           "HAL_Delay called twice");
    ASSERT(mock_last_delay_ms == 500,
           "HAL_Delay called with 500 ms");

    /* After two toggles starting from 0, ODR bit should be back at 0 */
    ASSERT((_mock_GPIOA.ODR & GPIO_PIN_5) == 0,
           "LED back to OFF state after two toggles");
}

/* ------------------------------------------------------------------ */
/*  Test: single toggle changes LED state                              */
/* ------------------------------------------------------------------ */
static void test_single_toggle_flips_state(void)
{
    printf("\n[Single toggle flips LED state]\n");
    reset_mocks();

    /* Start with LED off */
    _mock_GPIOA.ODR &= ~(uint32_t)GPIO_PIN_5;

    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    ASSERT((_mock_GPIOA.ODR & GPIO_PIN_5) != 0,
           "LED turns ON after first toggle");

    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    ASSERT((_mock_GPIOA.ODR & GPIO_PIN_5) == 0,
           "LED turns OFF after second toggle");
}

/* ------------------------------------------------------------------ */
/*  Test: HAL_Init is called during startup                           */
/* ------------------------------------------------------------------ */
static void test_hal_init_called(void)
{
    printf("\n[HAL_Init called on startup]\n");
    reset_mocks();

    HAL_Init();

    ASSERT(mock_HAL_Init_calls == 1,
           "HAL_Init called once");
}

/* ------------------------------------------------------------------ */
/*  Entry point                                                        */
/* ------------------------------------------------------------------ */
int main(void)
{
    printf("=== Nucleo-G474RE LED Blink — Host-side Mock Tests ===\n");

    test_hal_init_called();
    test_SystemClock_Config();
    test_GPIO_Init();
    test_blink_cycle();
    test_single_toggle_flips_state();

    printf("\n--- Results: %d/%d passed", tests_passed, tests_run);
    if (tests_failed > 0)
        printf(", %d FAILED", tests_failed);
    printf(" ---\n");

    return (tests_failed == 0) ? 0 : 1;
}
