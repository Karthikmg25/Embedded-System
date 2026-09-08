# STM32F401RE SysTick Driver

A lightweight, register-level SysTick driver for the STM32F401RE (Cortex-M4), providing a configurable time base, a blocking millisecond delay, and a non-blocking timeout/elapsed-time check.

## Features

- Configurable tick frequency (Hz) and clock source (AHB or AHB/8)
- Software tick counter driven by the SysTick interrupt
- Blocking delay in milliseconds (`SysTick_Delay_ms`)
- Non-blocking elapsed-tick check for polling-style timeouts (`SysTick_HasElapsedTicks`)
- Input validation for frequency and clock source, with clear error codes

## Files

| File | Description |
|---|---|
| `stm32f401re_systick_driver.h` | Public types, error codes, and API declarations |
| `stm32f401re_systick_driver.c` | Driver implementation |

## Dependencies

- `STM32F401RE.h` — device register definitions
- `stm32f401re_rcc_driver.h` — used to read the current HCLK frequency via `RCC_GetHCLKFreq()`

## API Reference

### `SysTick_Status_t SysTick_Init(uint32_t tick_frequency, SysTick_ClkSrc_t clk_src)`

Configures and starts the SysTick timer.

**Parameters**
- `tick_frequency` — desired number of SysTick interrupts per second (e.g. `1000` for 1 ms ticks)
- `clk_src` — `SYSTICK_CLK_AHB` or `SYSTICK_CLK_AHB_DIV8`

**Returns**
- `SYSTICK_OK` — configured and running
- `SYSTICK_ERROR_INVALID_FREQUENCY` — frequency is `0`, or the resulting reload value exceeds the 24-bit `LOAD` register range
- `SYSTICK_ERROR_INVALID_CLKSRC` — unrecognized clock source

**Behavior**
- Reads the current HCLK from the RCC driver and derives the SysTick input clock (`HCLK` or `HCLK/8`)
- Computes the reload value as `(systick_clk / tick_frequency) - 1`
- Disables SysTick, loads `LOAD`, clears `VAL`, sets the clock source bit, then enables the counter and interrupt

### `void SysTick_Handler(void)`

SysTick interrupt service routine. Increments the internal tick counter. This must be linked as the `SysTick_Handler` in the interrupt vector table (typically done automatically if this symbol name matches the startup file's weak alias).

### `uint32_t SysTick_GetTick(void)`

Returns the number of SysTick interrupts that have occurred since initialization, in **ticks** (not milliseconds).

### `void SysTick_Delay_ms(uint32_t ms)`

Blocking delay for approximately `ms` milliseconds. Converts `ms` to ticks using the frequency passed to `SysTick_Init()`, then busy-waits until that many ticks have elapsed.

Uses unsigned subtraction (`SysTick_GetTick() - start`) so the delay behaves correctly even if `tick_count` wraps around.

### `bool SysTick_HasElapsedTicks(uint32_t start, uint32_t timeout)`

Non-blocking check for whether `timeout` ticks have elapsed since `start`.

- Returns `true` once elapsed, `false` otherwise
- Does not modify `start` — the caller is responsible for updating it once the timeout is handled (e.g. `start = SysTick_GetTick();` to restart the interval)
- Also wraparound-safe via unsigned subtraction

## Usage Example

```c
#include "stm32f401re_systick_driver.h"

int main(void)
{
    // 1000 ticks/sec (1 ms resolution), clocked from AHB
    SysTick_Status_t status = SysTick_Init(1000, SYSTICK_CLK_AHB);
    if (status != SYSTICK_OK)
    {
        // handle init failure
        while (1);
    }

    while (1)
    {
        // Blocking delay
        SysTick_Delay_ms(500);
        // toggle_led();

        // Non-blocking periodic task example
        static uint32_t last = 0;
        if (SysTick_HasElapsedTicks(last, 1000))
        {
            last = SysTick_GetTick();
            // poll_sensor();
        }
    }
}
```

## Notes / Known Limitations

- `SysTick_Init()` stores `tick_frequency` into the internal `tick_f` variable *before* validating it against `0`; a failed init still leaves `tick_f` set. Avoid calling `SysTick_Delay_ms()` if `SysTick_Init()` returned an error.
- `LOAD` is a 24-bit register; requested tick frequencies that would require a reload value ≥ `0xFFFFFF` are rejected as `SYSTICK_ERROR_INVALID_FREQUENCY`.
- `SysTick_Delay_ms()` casts the `ms * tick_f` product to `uint64_t` before dividing, to avoid 32-bit overflow for large delay/frequency combinations.
- This driver assumes a single, ungated tick counter (no start/stop/reset API beyond `SysTick_Init()`).

## Possible Future Improvements

- Add a `SysTick_DeInit()` / stop function
- Add microsecond-resolution delay support
- Add overflow-safe accessor or explicit 64-bit tick counter option for very long-running applications
