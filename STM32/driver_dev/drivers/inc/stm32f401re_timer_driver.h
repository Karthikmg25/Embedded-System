
/**
 * @file    stm32f401re_timer_driver.h
 *
 * @brief   Timer driver: timebase, update events, output compare, input capture.
 *
 * @version 1.0
 *
 * Created on: Oct 4, 2026
 *
 * Author: Karthik
 *
 */


#ifndef INC_STM32F401RE_TIMER_DRIVER_H_
#define INC_STM32F401RE_TIMER_DRIVER_H_

#include <STM32F401RE.h>
#include <stddef.h>
#include <stdint.h>


/**
 * @brief   Timebase configuration.
 * @details Sets count direction, tick rate and overflow (update event) rate.
 */
typedef struct
{
	/*< Timer mode  - count direction and alignment >*/
	uint8_t CounterMode;

	/*  PSC Register value, timer frequency =  timer_clksrc / (prescaler + 1)
	 *  - Range : 16bit (0 - 65535)
	 */
	uint16_t Prescaler;

	/* ARR Register value, overflow frequency = timer_frequency / (period + 1)
	 * - Range : 32-bit on TIM2/TIM5
	 *         16-bit on TIM3/TIM4
	 */
	uint32_t Period;

	/*
	 * ARR preload enable.
	 * - Enabled: new ARR value is latched at the next update event.
	 * - Disabled: new ARR value takes effect immediately.
	 */
	uint8_t AutoReloadPreload;

	/*
	 * One-pulse mode enable.
	 * - When enabled, the counter stops at the next update event.
	 */
	uint8_t OnePulseMode;

}TIM_TimeBase_Config_t;


/**
 *  Forward declaration of the timer handle, needed by the callback typedef.
 */
struct TIM_Handle_t;

/**
 *  Timer event callback prototype.
 *  -  @breif     :  Used for both the update event and the capture events.
 *  -  @param[in] :  pTIMHandle  Handle of the timer that raised the event.
 *  -  @note         Called from ISR context. Keep it short and non-blocking.
 */
typedef void (*TIM_Callback_t) (struct TIM_Handle_t *pTIMHandle);

/**
 *  Timer handle.
 *  - @brief :  Holds the timer instance, its timebase configuration and the
 *              registered event callbacks. One handle per timer instance.
 */

typedef struct TIM_Handle_t
{
	/* Pointer to the timer register block (e.g. TIM2, TIM3) */
	TIM_Reg_t *pTIMx;

	/* Timebase configurations */
	TIM_TimeBase_Config_t TIM_Base;

	/* < Interrupt Mode parameters > */
	/********************************/

	/**
	 * Update event callback.
	 *  - Called on counter overflow/underflow when the update interrupt is enabled.
	 *  - @note : NULL means no callback is invoked.
	 */
	TIM_Callback_t UpdateCallback;

	/**
	 * Capture event callbacks, one per channel.
	 * - Index 0 = CH1, 1 = CH2, 2 = CH3, 3 = CH4.
	 * - Each channel has its own independent callback, so all four can be used on the same timer.
	 * - @note :  NULL means no callback is invoked for that channel.
	 */
	TIM_Callback_t IC_Callback[4];

}TIM_Handle_t;

/**
 *  Output compare channel configuration.
 *  @brief - Selects the channel, its compare value, output mode and polarity.
 */
typedef struct
{
	/**
	 * Channel number.
	 *  - TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4
	 */
	uint8_t  Channel;


	/**
	 * CCRx register value.
	 * - A compare event occurs when CCRx == CNT.
	 * - @note - 32-bit on TIM2/TIM5 (0 to 0xFFFFFFFF), 16-bit on TIM3/TIM4 (0 to 0xFFFF).
	 * - @note - In PWM modes this sets the duty cycle relative to the Period
	 *       (duty = CompareValue / (Period + 1)     in edge-aligned mode up counting).
	 *       (duty = CompareValue + 1 / (Period + 1) in edge-aligned mode down counting).
	 *       (duty = CompareValue / Period           in centre-aligned mode).
	 */
	uint32_t CompareValue;

	/**
	 * Output compare mode.
	 *  -  OutputMode modes: Frozen, Toggle, PWM1, PWM2, etc.
	 */
	uint8_t  OutputMode;

	/**
	 * Output polarity.
	 * - OutputPolarity : active high / active low
	 */
	uint8_t  OutputPolarity;

}TIM_OC_Config_t;

/**
 *  Input capture channel configuration.
 *   - @brief    Selects the channel, which input pin feeds it, and how the
 *               input signal is filtered, divided and edge-detected.
 */
typedef struct
{
	/**
	 * Channel number.
	 * - - TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4
	 */
	uint8_t Channel;

	/**
	 * Input mapping.
	 * - Direct  : the channel captures from its own input (e.g. CH1 from TI1).
	 * - Indirect: the channel captures from the paired channel's input (e.g. CH2 from TI1), as used for PWM input measurement.
	 * - TRC     : the channel captures from the internal trigger input TRC.
	 *
	 */
	uint8_t InputMode;

	/**
	 * Input filter.
	 * - Digital filter applied to the input to reject glitches.
	 * - Higher values filter more but delay edge detection.
	 * - @note - Maps to the ICxF field, valid range 0 to 15 (0 = no filter).
	 *
	 */
	uint8_t Filtering;

	/**
	 * Input capture prescaler.
	 * - Divides the number of input events before a capture is triggered
	 * - (capture every 1, 2, 4 or 8 events).
	 * - @note - This is separate from the timebase Prescaler in TIM_TimeBase_Config_t,
	 *           which divides the timer clock.
	 *
	 */
	uint8_t Prescaler;

	/**
	 * Active edge.
	 * - Rising, falling or both edges trigger a capture.
	 */
	uint8_t InputPolarity;

}TIM_IC_Config_t;


/*
 * Define an enum for Timer status
 */
typedef enum{

	TIM_OK,
	TIM_ERROR

}TIM_Status_t;

/*
 *  Define configuration macros - TimeBase configurations
 */

// Counter modes
#define TIM_COUNTERMODE_UPCOUNTING       	    0		/* EDGE ALIGNED MODE - Counts up                                   */
#define TIM_COUNTERMODE_DOWNCOUNTING       		1		/* EDGE ALIGNED MODE - Counts down                                 */
#define TIM_COUNTERMODE_CENTRE_ALIGNED_1   		2		/* CENTRE ALIGNED MODE 1 - Compare flag set only on DownCounting   */
#define TIM_COUNTERMODE_CENTRE_ALIGNED_2   		3		/* CENTRE ALIGNED MODE 2 - Compare flag set only on UpCounting     */
#define TIM_COUNTERMODE_CENTRE_ALIGNED_3   		4		/* CENTRE ALIGNED MODE 3 - Compare flag set on both                */

// AutoReloadPreload
#define TIM_AUTORELOADPRELOAD_DISABLE			0       /* NO BUFFERING : A new value takes effect immediately             */
#define TIM_AUTORELOADPRELOAD_ENABLE			1       /* BUFFERING    : A new value takes effect at the next update event*/

// OnePulseMode
#define TIM_ONEPULSEMODE_DISABLE				0		/* COUNTER RUNS CONTINUOUSLY                                       */
#define TIM_ONEPULSEMODE_ENABLE					1       /* COUNTER STOPS AFTER FIRST UPDATE EVENT                          */


/*
 *   Declare public APIs for time-base configurations
 *
 *
 */


/**
 *
 * API for Peripeheral clock control
 */
void TIM_PeriClockControl(TIM_Reg_t *pTIMx, uint8_t EnorDi);


/**
 * API to Initialize Time base configurations
 */
TIM_Status_t TIM_BaseInit(TIM_Handle_t* pTIMHandle);

/*
 *  Status Flag APIs
 */
uint8_t TIM_GetFlagStatus(TIM_Handle_t *pTIMHandle, uint8_t TIM_Flag);

void TIM_ClearFlag(TIM_Handle_t *pTIMHandle, uint8_t TIM_Flag);

/*
 * Timer start and stop functions
 */
void TIM_BaseStart(TIM_Handle_t* pTIMHandle);

void TIM_BaseStart_IT(TIM_Handle_t* pTIMHandle);

void TIM_BaseStop(TIM_Handle_t* pTIMHandle);

void TIM_BaseStop_IT(TIM_Handle_t* pTIMHandle);

TIM_Status_t  TIM_IRQEnable(TIM_Handle_t* pTIMHandle);

TIM_Status_t TIM_IRQPriorityConfig(TIM_Handle_t *pTIMHandle, uint8_t Priority);

void TIM_RegisterUpdateCallback(TIM_Handle_t* pTIMHandle, TIM_Callback_t Callback);


/*
 *  Define configuration macros - channel configurations : output compare
 */

//channels
#define TIM_CHANNEL_1              			1
#define TIM_CHANNEL_2              			2
#define TIM_CHANNEL_3              			3
#define TIM_CHANNEL_4              			4
// Output Compare Modes
#define TIM_OCMODE_FROZEN         			0    /* COMPARE MATCH HAS NO EFFECT*/
#define TIM_OCMODE_ACTIVE         			1    /* OUTPUT SET HIGH ON COMPARE MATCH*/
#define TIM_OCMODE_INACTIVE         		2    /* OUTPUT SET LOW ON COMPARE MATCH*/
#define TIM_OCMODE_TOGGLE         			3    /* OUTPUT TOGGLES ON COMPARE MATCH*/
#define TIM_OCMODE_PWM1           			6                  /* PWM MODE 1 */
                                     	 	 	 /* IN UPCOUNTING,   OUTPUT ACTIVE WHEN   CNT <  CCRX   */
									 	 	 	 /* IN DOWNCOUNTING, OUTPUT INACTIVE WHEN CNT <= CCRX   */
#define TIM_OCMODE_PWM2           			7                  /* PWM MODE 2 */
									 	 	 	 /* IN UPCOUNTING,   OUTPUT INACTIVE WHEN CNT <  CCRX   */
									 	 	 	 /* IN DOWNCOUNTING, OUTPUT ACTIVE WHEN   CNT <= CCRX   */
//Output Polarity
#define TIM_OCPOLARITY_HIGH           		0    /* ACTIVE HIGH*/
#define TIM_OCPOLARITY_LOW            		1    /* ACTIVE LOW*/



/*
 *  Declare public APIs for output compare
 *
 */

TIM_Status_t TIM_OC_Init(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *pOCHandle);

void TIM_OC_SetCompareValue(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *pOCHandle, uint32_t CompareValue);

void TIM_OC_Start(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *pOCHandle);

void TIM_OC_Stop(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *pOCHandle);


/*
 *  Define configuration macros - channel configurations - input capture
 */

// Input Capture Modes
#define TIM_ICMODE_DIRECT                   1 /* INPUT MODE : DIRECT INPUT   - CH1 -> TI1, CH2 -> TI2*/
#define TIM_ICMODE_INDIRECT                 2 /* INPUT MODE : INDIRECT INPUT - CH1 -> TI2, CH2 -> TI1*/
#define TIM_ICMODE_TRC                      3 /* INPUT MODE : INPUT MAPPED TO TRC*/
// Input Filtering
#define TIM_ICFILTER_0                      0  /* NO FILTER       */
#define TIM_ICFILTER_1                      1  /* DIGITAL FILTER  */
#define TIM_ICFILTER_2                      2  /* DIGITAL FILTER  */
#define TIM_ICFILTER_3                      3  /* DIGITAL FILTER  */
#define TIM_ICFILTER_4                      4  /* DIGITAL FILTER  */
#define TIM_ICFILTER_5                      5  /* DIGITAL FILTER  */
#define TIM_ICFILTER_6                      6  /* DIGITAL FILTER  */
#define TIM_ICFILTER_7                      7  /* DIGITAL FILTER  */
#define TIM_ICFILTER_8                      8  /* DIGITAL FILTER  */
#define TIM_ICFILTER_9                      9  /* DIGITAL FILTER  */
#define TIM_ICFILTER_10                     10 /* DIGITAL FILTER  */
#define TIM_ICFILTER_11                     11 /* DIGITAL FILTER  */
#define TIM_ICFILTER_12                     12 /* DIGITAL FILTER  */
#define TIM_ICFILTER_13                     13 /* DIGITAL FILTER  */
#define TIM_ICFILTER_14                     14 /* DIGITAL FILTER  */
#define TIM_ICFILTER_15                     15 /* DIGITAL FILTER  */
// Input Capture Prescaler
#define TIM_ICPSC_DIV1                      0  /* NO PRESCALER*/
#define TIM_ICPSC_DIV2                      1  /* CAPTURE ON ONCE EVERY 2 EVENTS */
#define TIM_ICPSC_DIV4                      2  /* CAPTURE ON ONCE EVERY 4 EVENTS */
#define TIM_ICPSC_DIV8                      3  /* CAPTURE ON ONCE EVERY 8 EVENTS */
// Input Capture Polarity
#define TIM_ICPOLARITY_RISING               0  /* CAPTURE RISING EDGE           */
#define TIM_ICPOLARITY_FALLING              1  /* CAPTURE FALLING EDGE          */
#define TIM_ICPOLARITY_BOTHEDGE             2  /* CAPTURE BOTH EDGEs            */


/*
 *  Declare public APIs for input capture
 *
 */
TIM_Status_t TIM_IC_Init(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig);

uint32_t TIM_IC_GetCaptureValue(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig);

void TIM_IC_Start(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig);

void TIM_IC_Stop(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig);

void TIM_IC_Start_IT(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig);

void TIM_IC_Stop_IT(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig);

void TIM_IC_RegisterCallback(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig, TIM_Callback_t Callback);




#endif /* INC_STM32F401RE_TIMER_DRIVER_H_ */
