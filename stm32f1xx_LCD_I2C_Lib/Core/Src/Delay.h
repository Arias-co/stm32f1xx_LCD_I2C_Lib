/*
 * Delay.h
 *
 *  Created on: Aug 29, 2020
 *      Author: Macbook
 */

#ifndef SRC_DELAY_H_
#define SRC_DELAY_H_

#include "stm32f1xx_hal.h"
#include <stdio.h>

class Delay
{
private:
    RCC_ClkInitTypeDef clock_config;
    TIM_HandleTypeDef timer;

    uint32_t freq_pclk1;
    uint32_t latency;
    uint16_t val_prescaler;
    uint16_t val_period;

    void clock_enable();
    void calc_prescaler();

public:
    Delay( TIM_TypeDef  *_timer );
    void start();
    void micros( uint16_t time );
    virtual ~Delay();
};

#endif /* SRC_DELAY_H_ */
