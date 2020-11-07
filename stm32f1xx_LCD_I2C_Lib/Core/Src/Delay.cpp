/*
 * Delay.cpp
 *
 *  Created on: Aug 29, 2020
 *      Author: Macbook
 */

#include "Delay.h"


Delay::Delay( TIM_TypeDef *_timer )
{
    // TODO Auto-generated constructor stub
    timer.Instance = _timer;
}

void Delay::start()
{
    clock_enable();
    calc_prescaler();

    timer.Init.Prescaler = val_prescaler;
    timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if ( HAL_TIM_Base_Init( &timer ) != HAL_OK )
    {

    }

    HAL_TIM_Base_Start(&timer);
}

void Delay::clock_enable()
{
    if ( timer.Instance == TIM2 )
       {
        __HAL_RCC_TIM2_CLK_ENABLE( );
       }
    else if ( timer.Instance == TIM3 )
       {
        __HAL_RCC_TIM3_CLK_ENABLE( );
       }
    else if ( timer.Instance == TIM4 )
       {
        __HAL_RCC_TIM4_CLK_ENABLE( );
       }

#ifdef TIM5
    else if ( timer.Instance == TIM5 )
           {
            __HAL_RCC_TIM5_CLK_ENABLE( );
           }
#endif

#ifdef TIM6
    else if ( timer.Instance == TIM6 )
           {
            __HAL_RCC_TIM6_CLK_ENABLE( );
           }
#endif

#ifdef TIM7
    else if ( timer.Instance == TIM7 )
           {
            __HAL_RCC_TIM7_CLK_ENABLE( );
           }
#endif

#ifdef TIM12
    else if ( timer.Instance == TIM12 )
           {
            __HAL_RCC_TIM12_CLK_ENABLE( );
           }
#endif

#ifdef TIM13
    else if ( timer.Instance == TIM13 )
           {
            __HAL_RCC_TIM13_CLK_ENABLE( );
           }
#endif

#ifdef TIM14
    else if ( timer.Instance == TIM14 )
           {
            __HAL_RCC_TIM14_CLK_ENABLE( );
           }
#endif
}

void Delay::micros( uint16_t time )
{
    timer.Instance->CNT = 0;
    timer.Instance->ARR = time - 1;
    __HAL_TIM_CLEAR_FLAG(&timer, TIM_FLAG_UPDATE);
    while(__HAL_TIM_GET_FLAG(&timer,TIM_FLAG_UPDATE)!=true);
}

void Delay::calc_prescaler()
{
    HAL_RCC_GetClockConfig( &clock_config, &latency );
    if ( clock_config.APB1CLKDivider == RCC_HCLK_DIV1 )
    {
        freq_pclk1 = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
        freq_pclk1 = HAL_RCC_GetPCLK1Freq() * 2;
    }
    val_prescaler = ( freq_pclk1 / 1000000 ) - 1;
}


Delay::~Delay()
{
    // TODO Auto-generated destructor stub

}








