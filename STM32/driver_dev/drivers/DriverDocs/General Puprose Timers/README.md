# STM32F401RE Timer Driver

A register-level driver for the general-purpose timers **TIM2, TIM3, TIM4 and TIM5** of the STM32F401RE.
It covers the timebase, update-event interrupts, output compare (including PWM) and input capture with
per-channel interrupt callbacks.

| | |
|---|---|
| **Version** | 1.0 |
| **Target** | STM32F401RE (Cortex-M4) |
| **Timers** | TIM2, TIM3, TIM4, TIM5 |
| **Language** | C |
| **Files** | `stm32f401re_timer_driver.h`, `stm32f401re_timer_driver.c` |

---

## Table of contents

1. [Overview](#1-overview)
2. [Files and dependencies](#2-files-and-dependencies)
3. [How it works](#3-how-it-works)
4. [Quick start](#4-quick-start)
5. [Configuration reference](#5-configuration-reference)
6. [API reference](#6-api-reference)
7. [Usage examples](#7-usage-examples)
8. [Behaviour notes and limitations](#8-behaviour-notes-and-limitations)

---

## 1. Overview

### Features

- **Timebase**: counter mode (up, down, three centre-aligned modes), prescaler, auto-reload period,
  ARR preload, one-pulse mode.
- **Update interrupt**: callback on every counter overflow/underflow.
- **Output compare**: Frozen, Active, Inactive, Toggle, PWM1 and PWM2 on all four channels, with
  selectable polarity and run-time compare-value updates (e.g. duty cycle).
- **Input capture**: all four channels, direct / indirect / TRC mapping, digital filter (0 to 15),
  capture prescaler (/1, /2, /4, /8), rising / falling / both-edge detection.
- **Interrupts**: one independent callback for the update event and one for each of the four capture
  channels, per timer. NVIC enable and priority helpers are included.
- **Input validation**: init functions validate every configuration field and return `TIM_ERROR`
  without touching any register when something is invalid.

### Not covered

- TIM1 and TIM9 to TIM11 (only TIM2 to TIM5).
- Output compare interrupts/callbacks (compare matches drive the pin in hardware).
- DMA, slave/master modes, trigger selection, encoder mode, break/dead-time.
- GPIO and RCC clock-tree configuration (the driver only enables the timer's own peripheral clock).
- Over-capture flag handling (`CCxOF`).

### Timer differences

| Timer | Counter / ARR / CCRx width | Channels |
|---|---|---|
| TIM2, TIM5 | 32-bit | 4 |
| TIM3, TIM4 | 16-bit | 4 |

`Period` and `CompareValue` are range-checked against these widths.

---

## 2. Files and dependencies

| File | Purpose |
|---|---|
| `stm32f401re_timer_driver.h` | Public types, configuration macros, API prototypes |
| `stm32f401re_timer_driver.c` | Implementation and the TIM2 to TIM5 IRQ handlers |

The driver includes the device header `<STM32F401RE.h>`, which must provide:

- `TIM_Reg_t` and the instance pointers `TIM2`, `TIM3`, `TIM4`, `TIM5`
- `TIMx_CLK_EN()` / `TIMx_CLK_DI()` clock macros for each timer
- Bit-position macros for `CR1`, `EGR`, `SR`, `DIER`, `CCMR1/2` and `CCER` (e.g. `TIMx_CR1_CEN`,
  `TIMx_SR_UIF`, `TIMx_SR_CC1IF`, `TIMx_DIER_CC1IE`)
- The `NVIC` register block, per-timer `TIMx_IRQ_NUMBER`, `TIMx_IRQ_ISER_INDEX`, `TIMx_IRQ_ENABLE`
- `ENABLE` / `DISABLE`

**Integration:** add both files to your build and make sure the IRQ handler names in the startup file
(`TIM2_IRQHandler` to `TIM5_IRQHandler`) are not defined anywhere else. The driver defines them.

---

## 3. How it works

### The handle

Each timer in use needs one `TIM_Handle_t`:

```c
typedef struct TIM_Handle_t
{
    TIM_Reg_t            *pTIMx;            /* TIM2, TIM3, TIM4 or TIM5            */
    TIM_TimeBase_Config_t TIM_Base;         /* timebase configuration              */
    TIM_Callback_t        UpdateCallback;   /* update event callback               */
    TIM_Callback_t        IC_Callback[4];   /* capture callbacks, index 0 = CH1... */
} TIM_Handle_t;
```

`TIM_BaseInit()` stores a pointer to your handle inside the driver, so the **handle must stay valid for as
long as the timer is used** (use a global or `static`, never a local variable).

### Interrupt flow

Each timer has a single NVIC line shared by its update and capture events:

```
Timer event ──► TIMx_IRQHandler ──► for each of UIF, CC1IF..CC4IF:
                                      flag set AND its interrupt enable set?
                                        ├─ clear the flag
                                        └─ call the registered callback (if not NULL)
```

- The flag is cleared **before** the callback is called, so callbacks never need to clear it.
- Callbacks run in **ISR context**. Keep them short and non-blocking.
- Callbacks receive the timer handle but **not** the channel number. Register a separate function per
  channel if you need to tell channels apart.

### Callback prototype

```c
typedef void (*TIM_Callback_t)(struct TIM_Handle_t *pTIMHandle);
```

---

## 4. Quick start

Follow this order for an interrupt-driven timer:

1. Fill in `handle.pTIMx` and `handle.TIM_Base`.
2. `TIM_BaseInit(&handle)`: validates, enables the timer clock, programs the timebase.
   **This resets all callbacks to NULL.**
3. Register callbacks (`TIM_RegisterUpdateCallback`, `TIM_IC_RegisterCallback`).
4. (Optional) configure channels: `TIM_OC_Init`, `TIM_IC_Init`.
5. `TIM_IRQPriorityConfig` and `TIM_IRQEnable` to set up the NVIC line.
6. Start: `TIM_BaseStart_IT` (update interrupt) or `TIM_IC_Start_IT`, `TIM_OC_Start`, then
   `TIM_BaseStart_IT`.

> **Remember:** configure the GPIO pin (alternate function) yourself for any channel that uses a pin.
> The driver does not touch GPIO.

---

## 5. Configuration reference

### 5.1 Timebase: `TIM_TimeBase_Config_t`

| Member | Meaning | Values |
|---|---|---|
| `CounterMode` | Count direction / alignment | `TIM_COUNTERMODE_UPCOUNTING`, `_DOWNCOUNTING`, `_CENTRE_ALIGNED_1`, `_CENTRE_ALIGNED_2`, `_CENTRE_ALIGNED_3` |
| `Prescaler` | PSC value. Timer frequency = `timer_clk / (Prescaler + 1)` | 0 to 65535 |
| `Period` | ARR value. Overflow frequency = `timer_freq / (Period + 1)` | 32-bit (TIM2/5), 16-bit (TIM3/4) |
| `AutoReloadPreload` | ARR buffering | `TIM_AUTORELOADPRELOAD_DISABLE` (takes effect immediately), `_ENABLE` (latched at next update) |
| `OnePulseMode` | Stop counter at next update event | `TIM_ONEPULSEMODE_DISABLE`, `_ENABLE` |

Counter modes:

| Macro | Behaviour |
|---|---|
| `TIM_COUNTERMODE_UPCOUNTING` | Edge-aligned, counts up |
| `TIM_COUNTERMODE_DOWNCOUNTING` | Edge-aligned, counts down |
| `TIM_COUNTERMODE_CENTRE_ALIGNED_1` | Centre-aligned, compare flag set only while counting down |
| `TIM_COUNTERMODE_CENTRE_ALIGNED_2` | Centre-aligned, compare flag set only while counting up |
| `TIM_COUNTERMODE_CENTRE_ALIGNED_3` | Centre-aligned, compare flag set in both directions |

### 5.2 Output compare: `TIM_OC_Config_t`

| Member | Meaning | Values |
|---|---|---|
| `Channel` | Channel number | `TIM_CHANNEL_1` to `TIM_CHANNEL_4` |
| `CompareValue` | CCRx value (must be ≤ `Period`) | 32-bit (TIM2/5), 16-bit (TIM3/4) |
| `OutputMode` | Output behaviour on compare match | see below |
| `OutputPolarity` | Active level | `TIM_OCPOLARITY_HIGH`, `TIM_OCPOLARITY_LOW` |

| Output mode | Behaviour |
|---|---|
| `TIM_OCMODE_FROZEN` | Compare match has no effect on the output |
| `TIM_OCMODE_ACTIVE` | Output set active on match |
| `TIM_OCMODE_INACTIVE` | Output set inactive on match |
| `TIM_OCMODE_TOGGLE` | Output toggles on match |
| `TIM_OCMODE_PWM1` | Up: active while `CNT < CCRx`. Down: inactive while `CNT <= CCRx` |
| `TIM_OCMODE_PWM2` | Up: inactive while `CNT < CCRx`. Down: active while `CNT <= CCRx` |

**PWM duty cycle** (PWM1, active-high):

| Counter mode | Duty |
|---|---|
| Edge-aligned, up | `CompareValue / (Period + 1)` |
| Edge-aligned, down | `(CompareValue + 1) / (Period + 1)` |
| Centre-aligned | `CompareValue / Period` |

### 5.3 Input capture: `TIM_IC_Config_t`

| Member | Meaning | Values |
|---|---|---|
| `Channel` | Channel number | `TIM_CHANNEL_1` to `TIM_CHANNEL_4` |
| `InputMode` | Which input feeds the channel | `TIM_ICMODE_DIRECT` (own input), `TIM_ICMODE_INDIRECT` (paired channel's input), `TIM_ICMODE_TRC` |
| `Filtering` | Digital input filter (ICxF) | `TIM_ICFILTER_0` (off) to `TIM_ICFILTER_15` |
| `Prescaler` | Capture prescaler: capture every N events | `TIM_ICPSC_DIV1`, `_DIV2`, `_DIV4`, `_DIV8` |
| `InputPolarity` | Active edge | `TIM_ICPOLARITY_RISING`, `_FALLING`, `_BOTHEDGE` |

> `Prescaler` here divides **input events**. It is unrelated to the timebase `Prescaler`, which divides
> the timer clock.

Channel pairs for indirect mode: CH1 ↔ CH2 and CH3 ↔ CH4.

### 5.4 Status type

```c
typedef enum { TIM_OK, TIM_ERROR } TIM_Status_t;
```

---

## 6. API reference

### 6.1 Timebase and control

| Function | Description |
|---|---|
| `void TIM_PeriClockControl(TIM_Reg_t *pTIMx, uint8_t EnorDi)` | Enable/disable the timer's RCC clock. Called by `TIM_BaseInit`, so normally not needed directly. Unknown instance: ignored. |
| `TIM_Status_t TIM_BaseInit(TIM_Handle_t *h)` | Validate and program the timebase, enable the clock, generate an update event so PSC/ARR take effect (update flag cleared afterwards), reset all callbacks to NULL, store the handle for the IRQ handler. Does **not** start the timer or enable interrupts. Returns `TIM_ERROR` for a NULL handle, invalid instance, invalid mode values, or `Period` > 0xFFFF on TIM3/TIM4. |
| `void TIM_BaseStart(TIM_Handle_t *h)` | Set `CEN`. Counts from the current `CNT`. No interrupt enabled. |
| `void TIM_BaseStop(TIM_Handle_t *h)` | Clear `CEN`. Counter freezes; config, callbacks, `CNT`, interrupt enables and flags are untouched. |
| `void TIM_BaseStart_IT(TIM_Handle_t *h)` | Set `UIE`, then `CEN`. Does not enable the NVIC line. |
| `void TIM_BaseStop_IT(TIM_Handle_t *h)` | Clear `UIE`, then `CEN`. Capture interrupt enables are unchanged. |
| `void TIM_RegisterUpdateCallback(TIM_Handle_t *h, TIM_Callback_t cb)` | Set the update callback (NULL removes it). Call **after** `TIM_BaseInit`. |
| `uint8_t TIM_GetFlagStatus(TIM_Handle_t *h, uint8_t flag)` | Read one `SR` flag. `flag` is a **bit position** (e.g. `TIMx_SR_UIF`), not a mask. Returns 1 or 0. |
| `void TIM_ClearFlag(TIM_Handle_t *h, uint8_t flag)` | Clear one `SR` flag (bit position). |

### 6.2 Interrupt (NVIC) control

| Function | Description |
|---|---|
| `TIM_Status_t TIM_IRQEnable(TIM_Handle_t *h)` | Enable the timer's NVIC line (one line per timer, shared by all its events). |
| `TIM_Status_t TIM_IRQPriorityConfig(TIM_Handle_t *h, uint8_t prio)` | Set NVIC priority, 0 (highest) to 15. Applies to all events of that timer. Default after reset is 0. |

### 6.3 Output compare

| Function | Description |
|---|---|
| `TIM_Status_t TIM_OC_Init(TIM_Handle_t *h, TIM_OC_Config_t *oc)` | Program mode, polarity and compare value for one channel. Does not enable the output. Channel must be disabled while configuring. Returns `TIM_ERROR` on NULL, bad channel/mode/polarity, or `CompareValue` > `Period` (or > 0xFFFF on TIM3/4). |
| `void TIM_OC_SetCompareValue(TIM_Handle_t *h, TIM_OC_Config_t *oc, uint32_t value)` | Change CCRx at run time and update `oc->CompareValue`. Out-of-range values or bad pointers are **silently ignored**. |
| `void TIM_OC_Start(TIM_Handle_t *h, TIM_OC_Config_t *oc)` | Set `CCxE` (enable the output). Does not start the counter. |
| `void TIM_OC_Stop(TIM_Handle_t *h, TIM_OC_Config_t *oc)` | Clear `CCxE`. Configuration kept. Call this before re-running `TIM_OC_Init` on the channel. |

### 6.4 Input capture

| Function | Description |
|---|---|
| `TIM_Status_t TIM_IC_Init(TIM_Handle_t *h, TIM_IC_Config_t *ic)` | Program input mapping, filter, prescaler and edge. Does not enable the channel, interrupt or counter. Channel must be disabled (`TIM_IC_Stop`) before re-init. |
| `uint32_t TIM_IC_GetCaptureValue(TIM_Handle_t *h, TIM_IC_Config_t *ic)` | Read CCRx (also clears the channel's capture flag). Returns 0 on NULL/invalid channel, but 0 is also a valid capture. |
| `void TIM_IC_Start(TIM_Handle_t *h, TIM_IC_Config_t *ic)` | Enable the channel (`CCxE`). Polling mode: use `TIM_GetFlagStatus` on the `CCxIF` flag. |
| `void TIM_IC_Stop(TIM_Handle_t *h, TIM_IC_Config_t *ic)` | Disable the channel. Leaves `CCxIE` unchanged. |
| `void TIM_IC_Start_IT(TIM_Handle_t *h, TIM_IC_Config_t *ic)` | Enable the channel and its capture interrupt (`CCxIE`). Does not enable the NVIC line. |
| `void TIM_IC_Stop_IT(TIM_Handle_t *h, TIM_IC_Config_t *ic)` | Disable the channel and its capture interrupt. |
| `void TIM_IC_RegisterCallback(TIM_Handle_t *h, TIM_IC_Config_t *ic, TIM_Callback_t cb)` | Set the capture callback for `ic->Channel`. Call **after** `TIM_BaseInit` and **before** `TIM_IC_Start_IT`. |

### 6.5 Argument checking summary

| Behaviour | Functions |
|---|---|
| Returns `TIM_ERROR` on bad input | `TIM_BaseInit`, `TIM_OC_Init`, `TIM_IC_Init`, `TIM_IRQEnable`, `TIM_IRQPriorityConfig` |
| Silently ignores a NULL pointer | `TIM_BaseStart_IT`, `TIM_OC_SetCompareValue`, `TIM_IC_Start`, `TIM_IC_Stop`, `TIM_IC_Start_IT`, `TIM_IC_Stop_IT`, `TIM_IC_RegisterCallback`, `TIM_IC_GetCaptureValue` (returns 0) |
| **No check, NULL is undefined behaviour** | `TIM_BaseStart`, `TIM_BaseStop`, `TIM_BaseStop_IT`, `TIM_RegisterUpdateCallback`, `TIM_GetFlagStatus`, `TIM_ClearFlag`, `TIM_OC_Start`, `TIM_OC_Stop` |

---

## 7. Usage examples

The examples assume a **16 MHz timer clock**. Adjust `Prescaler` to your clock configuration.
GPIO setup is not shown.

### 7.1 Periodic update interrupt (1 Hz on TIM2)

```c
#include "stm32f401re_timer_driver.h"

static TIM_Handle_t htim2;               /* global/static: the driver keeps a pointer to it */

static void OnTick(TIM_Handle_t *pTIM)
{
    (void)pTIM;
    /* Runs in ISR context: toggle an LED, set a flag, ... */
}

int main(void)
{
    htim2.pTIMx                      = TIM2;
    htim2.TIM_Base.CounterMode       = TIM_COUNTERMODE_UPCOUNTING;
    htim2.TIM_Base.Prescaler         = 15999;   /* 16 MHz / 16000 = 1 kHz  */
    htim2.TIM_Base.Period            = 999;     /* 1 kHz / 1000   = 1 Hz   */
    htim2.TIM_Base.AutoReloadPreload = TIM_AUTORELOADPRELOAD_ENABLE;
    htim2.TIM_Base.OnePulseMode      = TIM_ONEPULSEMODE_DISABLE;

    if (TIM_BaseInit(&htim2) != TIM_OK) { /* handle error */ }

    TIM_RegisterUpdateCallback(&htim2, OnTick);  /* after TIM_BaseInit */
    TIM_IRQPriorityConfig(&htim2, 5);
    TIM_IRQEnable(&htim2);
    TIM_BaseStart_IT(&htim2);

    while (1) { }
}
```

### 7.2 PWM output (1 kHz, 25% duty on TIM3 CH1)

```c
static TIM_Handle_t    htim3;
static TIM_OC_Config_t pwm1;

void PWM_Setup(void)
{
    /* GPIO: configure the TIM3_CH1 pin (e.g. PA6) in alternate-function mode first */

    htim3.pTIMx                      = TIM3;
    htim3.TIM_Base.CounterMode       = TIM_COUNTERMODE_UPCOUNTING;
    htim3.TIM_Base.Prescaler         = 15;      /* 16 MHz / 16  = 1 MHz tick */
    htim3.TIM_Base.Period            = 999;     /* 1 MHz / 1000 = 1 kHz      */
    htim3.TIM_Base.AutoReloadPreload = TIM_AUTORELOADPRELOAD_ENABLE;
    htim3.TIM_Base.OnePulseMode      = TIM_ONEPULSEMODE_DISABLE;
    TIM_BaseInit(&htim3);

    pwm1.Channel        = TIM_CHANNEL_1;
    pwm1.CompareValue   = 250;                  /* 250 / (999 + 1) = 25 % */
    pwm1.OutputMode     = TIM_OCMODE_PWM1;
    pwm1.OutputPolarity = TIM_OCPOLARITY_HIGH;
    TIM_OC_Init(&htim3, &pwm1);

    TIM_OC_Start(&htim3, &pwm1);                /* enable the output   */
    TIM_BaseStart(&htim3);                      /* then start counting */
}

void PWM_SetDuty(uint32_t ticks)                /* 0 to 999 */
{
    TIM_OC_SetCompareValue(&htim3, &pwm1, ticks);
}
```

### 7.3 Input capture with interrupt (period measurement on TIM5 CH1)

```c
static TIM_Handle_t    htim5;
static TIM_IC_Config_t ic1;                     /* file scope: used by the callback */

static volatile uint32_t lastCapture;
static volatile uint32_t periodTicks;

static void OnCapture1(TIM_Handle_t *pTIM)
{
    uint32_t now = TIM_IC_GetCaptureValue(pTIM, &ic1);
    periodTicks  = now - lastCapture;           /* unsigned math handles 32-bit wrap */
    lastCapture  = now;
}

void Capture_Setup(void)
{
    /* GPIO: configure the TIM5_CH1 pin in alternate-function mode first */

    htim5.pTIMx                      = TIM5;
    htim5.TIM_Base.CounterMode       = TIM_COUNTERMODE_UPCOUNTING;
    htim5.TIM_Base.Prescaler         = 15;           /* 1 MHz tick               */
    htim5.TIM_Base.Period            = 0xFFFFFFFF;   /* free-running 32-bit      */
    htim5.TIM_Base.AutoReloadPreload = TIM_AUTORELOADPRELOAD_DISABLE;
    htim5.TIM_Base.OnePulseMode      = TIM_ONEPULSEMODE_DISABLE;
    TIM_BaseInit(&htim5);

    ic1.Channel       = TIM_CHANNEL_1;
    ic1.InputMode     = TIM_ICMODE_DIRECT;
    ic1.Filtering     = TIM_ICFILTER_0;
    ic1.Prescaler     = TIM_ICPSC_DIV1;
    ic1.InputPolarity = TIM_ICPOLARITY_RISING;
    TIM_IC_Init(&htim5, &ic1);

    TIM_IC_RegisterCallback(&htim5, &ic1, OnCapture1);  /* after BaseInit, before Start_IT */

    TIM_IRQPriorityConfig(&htim5, 4);
    TIM_IRQEnable(&htim5);

    TIM_IC_Start_IT(&htim5, &ic1);
    TIM_BaseStart(&htim5);
}
```

With `Period = 0xFFFFFFFF` the simple subtraction above is wrap-safe. With a smaller `Period`, you must
account for the counter reload yourself.

### 7.4 Capture by polling (no interrupt)

```c
TIM_IC_Start(&htim5, &ic1);
TIM_BaseStart(&htim5);

while (!TIM_GetFlagStatus(&htim5, TIMx_SR_CC1IF)) { }   /* wait for a capture        */
uint32_t t = TIM_IC_GetCaptureValue(&htim5, &ic1);       /* reading CCR1 clears CC1IF */
```

---

## 8. Behaviour notes and limitations

**Initialization**
- `TIM_BaseInit` configures but does **not** start the timer or enable any interrupt.
- Re-initializing a running timer restarts its count, resets all callbacks to NULL, and must not switch
  between edge-aligned and centre-aligned modes while the counter is enabled.
- Only one handle per timer instance is tracked. A second `TIM_BaseInit` on the same instance replaces
  the stored handle.

**Interrupts**
- Enabling an interrupt needs two steps: the timer-level enable (`TIM_BaseStart_IT` / `TIM_IC_Start_IT`)
  and the NVIC line (`TIM_IRQEnable`).
- All events of a timer share one priority because they share one interrupt line.
- If a flag is already set when its interrupt is enabled, the interrupt fires immediately.
- Stopping with `*_Stop_IT` does not clear a pending flag. A pending interrupt may enter the handler once,
  but the callback is not called because its enable bit is cleared.
- The IRQ handler checks both the flag **and** its enable bit before calling a callback.
- The over-capture flags (`CCxOF`) are not handled.

**Output compare**
- `CompareValue` must not exceed `Period`, so **100% PWM duty cannot be set** with this driver.
- CCRx preload (`OCxPE`) is not enabled: a new compare value takes effect immediately and changing it
  mid-period can glitch that period.
- `TIM_OC_Init` does not enable the output. Call `TIM_OC_Start`.
- To reconfigure a channel, stop it first (`TIM_OC_Stop` / `TIM_IC_Stop`), since the `CCxS` bits are only
  writable while the channel is disabled.
- `TIM_OC_SetCompareValue` reports no error. Check your values beforehand.

**Input capture**
- Captured values are raw counter values. Compute periods and pulse widths yourself and handle counter
  overflow between captures.
- TRC mode needs the trigger selection (`TS` bits), which this driver does not configure.
- In indirect mode (e.g. PWM-input measurement), the paired channel must be configured separately.

**General**
- Timer clock: on the STM32F4, timers on an APB bus run at 2× the bus clock when that bus prescaler is
  not 1. Check your clock tree (see the reference manual) before computing `Prescaler`.
- GPIO alternate-function setup is always the application's responsibility.
- Callbacks have no user-data parameter and no channel argument. Use file-scope variables for context
  and one function per channel.

---

*Driver version 1.0. Author: Karthik.*
