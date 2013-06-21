/*
 * Authors (alphabetical order)
 * - Andre Bernet <bernet.andre@gmail.com>
 * - Andreas Weitl
 * - Bertrand Songis <bsongis@gmail.com>
 * - Bryan J. Rentoul (Gruvin) <gruvin@gmail.com>
 * - Cameron Weeks <th9xer@gmail.com>
 * - Erez Raviv
 * - Gabriel Birkus
 * - Jean-Pierre Parisy
 * - Karl Szmutny
 * - Michael Blandford
 * - Michal Hlavinka
 * - Pat Mackenzie
 * - Philip Moss
 * - Rob Thomson
 * - Romolo Manfredini <romolo.manfredini@gmail.com>
 * - Thomas Husterer
 *
 * opentx is based on code named
 * gruvin9x by Bryan J. Rentoul: http://code.google.com/p/gruvin9x/,
 * er9x by Erez Raviv: http://code.google.com/p/er9x/,
 * and the original (and ongoing) project by
 * Thomas Husterer, th9x: http://code.google.com/p/th9x/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "../opentx.h"

uint16_t TrainerPpmStream[20] =
{
  2000,
  2200,
  2400,
  2600,
  2800,
  3000,
  3200,
  3400,
  45000-21600,
  0,0,0,0,0,0,0,0,0,0,0
};

uint16_t * TrainerPulsePtr;

void setupTrainerPulses()
{
  int16_t PPM_range = g_model.extendedLimits ? 640 * 2 : 512 * 2; //range of 0.7..1.7msec

  uint16_t *ptr ;
  uint32_t p=8+g_model.ppmNCH*2; //Channels *2

  ptr = TrainerPpmStream ;

  uint32_t rest = 22500u * 2; //Minimum Framelen=22.5 ms
  rest += int32_t(g_model.ppmFrameLength) * 1000;

  for (uint32_t i = 0 ; i < p ; i += 1 ) {
    uint32_t pulse = limit((int16_t)-PPM_range, channelOutputs[i], (int16_t)PPM_range) + 2*PPM_CH_CENTER(i);
    rest -= pulse ;
    *ptr++ = pulse ;
  }
  *ptr++ = rest ;
  *ptr = 0 ;

  TIM8->CCR2 = rest - 1000 ;             // Update time
  TIM8->CCR4 = (g_model.ppmDelay*50+300)*2 ;
}

// Trainer PPM oputput PC9, Timer 8 channel 4
void init_trainer_ppm()
{
  setupTrainerPulses() ;
  TrainerPulsePtr = TrainerPpmStream ;

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN ;           // Enable portC clock
  configure_pins( 0x0200, PIN_PERIPHERAL | PIN_PORTC | PIN_PER_3 | PIN_OS25 | PIN_PUSHPULL ) ;
  RCC->APB2ENR |= RCC_APB2ENR_TIM8EN ;            // Enable clock

  TIM8->ARR = *TrainerPulsePtr++ ;
  TIM8->PSC = (PERI2_FREQUENCY * TIMER_MULT_APB2) / 2000000 - 1 ;               // 0.5uS from 30MHz
  TIM8->CCER = TIM_CCER_CC4E ;
  TIM8->CCMR2 = TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE ;                   // PWM mode 1
  TIM8->CCR4 = 600 ;              // 300 uS pulse
  TIM8->BDTR = TIM_BDTR_MOE ;
  TIM8->EGR = 1 ;
  // TIM8->DIER = TIM_DIER_UDE ;

  TIM8->SR &= ~TIM_SR_UIF ;                               // Clear flag
  TIM8->SR &= ~TIM_SR_CC2IF ;                             // Clear flag
  TIM8->DIER |= TIM_DIER_CC2IE ;
  TIM8->DIER |= TIM_DIER_UIE ;

  TIM8->CR1 = TIM_CR1_CEN ;
  NVIC_EnableIRQ(TIM8_CC_IRQn) ;
  NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn) ;
}

// TODO - testing
void stop_trainer_ppm()
{
  configure_pins( 0x0200, PIN_INPUT | PIN_PORTC ) ;
  TIM8->CR1 &= ~TIM_CR1_CEN ;                             // Stop counter
  NVIC_DisableIRQ(TIM8_CC_IRQn) ;                         // Stop Interrupt
  NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn) ; // Stop Interrupt
}

extern "C" void TIM8_CC_IRQHandler()
{
  TIM8->DIER &= ~TIM_DIER_CC2IE ;         // stop this interrupt
  TIM8->SR &= ~TIM_SR_CC2IF ;                             // Clear flag

  setupTrainerPulses() ;

  TrainerPulsePtr = TrainerPpmStream ;
  TIM8->DIER |= TIM_DIER_UDE ;

  TIM8->SR &= ~TIM_SR_UIF ;                                       // Clear this flag
  TIM8->DIER |= TIM_DIER_UIE ;                            // Enable this interrupt
}

extern "C" void TIM8_UP_TIM13_IRQHandler()
{
  TIM8->SR &= ~TIM_SR_UIF ;                               // Clear flag

  TIM8->ARR = *TrainerPulsePtr++ ;
  if ( *TrainerPulsePtr == 0 )
  {
          TIM8->SR &= ~TIM_SR_CC2IF ;                     // Clear this flag
          TIM8->DIER |= TIM_DIER_CC2IE ;  // Enable this interrupt
  }
}

// Trainer capture, PC8, Timer 3 channel 3
void init_trainer_capture()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN ; // Enable portC clock
  configure_pins (PIN_TR_PPM_IN, PIN_PERIPHERAL | PIN_PORTC | PIN_PER_2) ;
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN ; // Enable clock

  TIM3->ARR = 0xFFFF ;
  TIM3->PSC = (PERI1_FREQUENCY * TIMER_MULT_APB1) / 2000000 - 1 ;               // 0.5uS
  TIM3->CR2 = 0 ;
  TIM3->CCMR2 = TIM_CCMR2_IC3F_0 | TIM_CCMR2_IC3F_1 | TIM_CCMR2_CC3S_0 ;
  TIM3->CCER = TIM_CCER_CC3E ;
  TIM3->SR &= ~TIM_SR_CC3IF ;                             // Clear flag
  TIM3->DIER |= TIM_DIER_CC3IE ;
  TIM3->CR1 = TIM_CR1_CEN ;
  NVIC_EnableIRQ(TIM3_IRQn) ;
}


extern "C" void TIM3_IRQHandler()
{
  uint16_t capture ;
  static uint16_t lastCapt ;
  uint16_t val ;

  capture = TIM3->CCR3 ;

  val = (uint16_t)(capture - lastCapt) / 2 ;
  lastCapt = capture;

  // We prcoess g_ppmInsright here to make servo movement as smooth as possible
  //    while under trainee control
  if ((val>4000) && (val < 19000)) { // G: Prioritize reset pulse. (Needed when less than 8 incoming pulses)
    ppmInState = 1; // triggered
  }
  else {
    if(ppmInState && (ppmInState<=8)) {
      if ((val>800) && (val<2200)) {
        g_ppmIns[ppmInState++ - 1] = (int16_t)(val - 1500)*(g_eeGeneral.PPM_Multiplier+10)/10; //+-500 != 512, but close enough.
      }
      else {
        ppmInState = 0; // not triggered
      }
    }
  }
}
