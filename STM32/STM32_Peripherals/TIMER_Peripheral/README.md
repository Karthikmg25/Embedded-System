



# STM32F401RE General Purpose Timers -- Baremetal
  
## Overview  
  
General Purpose Timers in STM32F401RE are used for:  
  
- Delay generation  
- Periodic interrupts  
- Non-blocking timing  
- PWM generation  
- Input capture  
- Output compare  
- Event counting  
  
### Available General Purpose Timers  
  
| Timer | Type | Counter Size |  
|---|---|---|  
| TIM2 | General Purpose | 32-bit |  
| TIM3 | General Purpose | 16-bit |  
| TIM4 | General Purpose | 16-bit |  
| TIM5 | General Purpose | 32-bit |  
  
---
## Timer Clock Basics

**Timer counter frequency**:
`F_timer = F_CPU/ Prescaler+1​`

**Overflow / update event frequency:**
`F_overflow = F_CPU/ (TOP+1)(Prescaler+1​)`
  
# Important Timer Registers  
  
| Register | Purpose |  
|---|---|  
| CR1 | Timer control register |  
| CNT | Current counter value |  
| PSC | Prescaler register |  
| ARR | Auto reload register (TOP value) |  
| SR | Status register |  
| DIER | DMA / Interrupt enable register |  
| EGR | Event generation register |  
| CCMR1 | Capture Compare Mode Register |  
| CCER | Capture Compare Enable Register |  
| CCRx | Compare register for channels |  
  
---  
  
# Important Bits  
  
## CR1 Register  
  
| Bit | Purpose |  
|---|---|  
| CEN | Enable timer |  
| ARPE | Auto reload preload enable |  
  
---  
  
## SR Register  
  
| Bit | Purpose |  
|---|---|  
| UIF | Update interrupt flag |  
  
---  
  
## DIER Register  
  
| Bit | Purpose |  
|---|---|  
| UIE | Update interrupt enable |  
  
---  
  
## CCMR1 Register  
  
| Bits | Purpose |  
|---|---|  
| OCxM | PWM mode selection |  
| OCxPE | CCR preload enable |  
  
---  
  
## CCER Register  
  
| Bit | Purpose |  
|---|---|  
| CCxE | Enable channel output |  
  
---
  
# Basic Timer Workflow  
  
Typical timer configuration order:  
  
```text  
1. Enable timer clock  
2. Configure PSC  
3. Configure ARR  
4. Reset CNT  
5. Configure timer mode  
6. Enable interrupt/PWM if needed  
7. Enable timer  
```
---  
  
# 1. Blocking Delay Using Timer Polling  
  
## Idea  
  
- Timer generates update event every 1ms  
- Poll UIF flag  
- Clear flag after overflow  
  
## General Steps  
  
```text  
1. Enable timer clock  
2. Set PSC  
3. Set ARR  
4. Enable timer  
5. Wait for UIF flag  
6. Clear UIF flag  
7. Repeat for required milliseconds  
```  
  
# 2. Timer Interrupt Generation  
  
## Idea  
  
- Timer generates periodic interrupts  
- ISR executes automatically after overflow  
  
## General Steps  
  
```text  
1. Enable timer clock  
2. Configure PSC and ARR  
3. Enable update interrupt (UIE)  
4. Enable NVIC interrupt  
5. Enable timer  
6. Write ISR  
7. Clear UIF inside ISR  
```
## NVIC Basics  
  
### Enable Interrupt  
 NVIC has multiple ISER registers:  
- ISER[0] to ISER[7]  
- Each ISER register controls 32 interrupt lines
```c  
NVIC->ISER[x]  
```  
  
Each bit represents one interrupt line:  
- bit 0 → IRQ 0  
- bit 1 → IRQ 1  
- ...  
- bit 31 → IRQ 31  

  
### Set Priority  
Each interrupt has an 8-bit priority field in the `IP[]` array:
- Each `IP[x]` is 8 bits wide  
- Lower value = higher priority (0 = highest priority)
Upper nibble of IP[x] is used for setting priority level.
  
| Value | Meaning |  
|------|--------|  
| 0x0 | Highest priority |  
| 0x1 | Lower priority |  
| 0xF | Lowest priority |
  
```c  
NVIC->IP[IRQn]  
```
# 3. Non-Blocking Delay Using Timer Interrupts  
  
## Idea  
  
- Generate 1ms interrupt periodically  
- Increment software tick counter  
- Use counter difference for delays  
  
## Advantages  
  
```text  
- CPU free for other tasks  
- More scalable  
- Better multitasking  
- Used in RTOS systems  
```  
  
## General Steps  
  
```text  
1. Configure timer for 1ms interrupt  
2. Increment global tick inside ISR  
3. Store current tick  
4. Compare elapsed ticks in main loop  
5. Execute task after required delay  
```  
  
## Overflow-Safe Delay Logic  
  
```c  
if((current_tick - previous_tick) >= delay)  
```  
  
This works correctly even after counter overflow.  
  
---  
  
# 4. PWM Generation  
  
## PWM Basics  
  
PWM = Pulse Width Modulation.  
  
Used for:  
  
- LED dimming  
- Motor speed control  
- Servo control  
- Audio generation  
  
---
## PWM Frequency
`f_PWM = f_CPU/(PSC+1)(ARR+1)`
## Duty Cycle
`Duty Cycle = Compare value/(TOP+1) * 100`
## PWM Generation Steps  
  
```text  
1. Enable timer clock  
2. Configure PSC and ARR  
3. Configure PWM mode in CCMR  
4. Set compare value in CCRx  
5. Enable output channel in CCER  
6. Configure GPIO Alternate Function  
7. Enable timer  
```
---  
  
## PWM Modes  
  
| OCxM Value | Mode |  
|---|---|  
| 110 | PWM Mode 1 |  
| 111 | PWM Mode 2 |  
---
## Alternate Function Configuration  
  
Timer outputs require:  
  
```text  
1. GPIO Alternate Function mode  
2. Correct AF selection  
```  
  
Example:  
  
| Pin | Function | AF |  
|---|---|---|  
| PB6 | TIM4_CH1 | AF2 |  
---
  
### GPIO Configuration for TIM4_CH1  
  
#### Set Alternate Function Mode  
  
```c  
GPIOB->MODER |= (0x2 << (6*2));  
```  
  
#### Select AF2  
  
```c  
GPIOB->AFR[0] |= (0x2 << (6*4));  
```
## Important Preload Features  
  
### ARPE  
  
Auto Reload Preload Enable.  
  
- Buffers ARR updates  
- Prevents glitches  
- Useful for dynamic frequency changes  
  
---  
  
### OCxPE  
  
Capture Compare Preload Enable.  
- Buffers CCR updates  
- Smooth duty cycle updates  
  
---
# Common Beginner Mistakes  
  
## Forgetting Timer Enable  
  
```c  
TIMx->CR1 |= (1<<CEN);  
```  
  
---  
  
## Forgetting GPIO Alternate Function  
  
Timer runs internally but no signal appears on pin.  
  
---  
  
## CCR Value Greater Than ARR  
  
```text  
CCR > ARR → Output becomes fully ON  
```  
  
---  
  
## Forgetting NVIC Enable  
  
Interrupt never reaches CPU.  
  
---
  
# Useful Mental Model  
  
| Register | Meaning |  
|---|---|  
| PSC | Controls timer speed |  
| ARR | Controls overflow period / PWM resolution |  
| CCR | Controls compare point / duty cycle |  
| CNT | Current running counter value |  
  
---  
  
# Typical Embedded Uses  
  
| Application | Timer Feature |  
|---|---|  
| LED blinking | Update event |  
| RTOS tick | Interrupt |  
| LED brightness | PWM |  
| Motor control | PWM |  
| Frequency measurement | Input capture |  
| Event timing | CNT register |  
| Servo control | PWM |