/*      Author: ffan005
 *  Partner(s) Name: 
 *      Lab Section:
 *      Assignment: Lab #  Exercise #
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 *      Demo: https://www.youtube.com/watch?v=Ly7yjuXQrNQ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States {Start, Initial, Press, Increment, Decrement, Reset, temp1, temp2} state;
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
TCCR1B = 0x0B;
OCR1A = 125;
TIMSK1 = 0x02;
TCNT1 = 0;
_avr_timer_cntcurr = _avr_timer_M;
SREG |= 0x80;
}


void TimerOff(){
TCCR1B = 0x00;
}

void TimerISR(){
TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
_avr_timer_cntcurr--;
if(_avr_timer_cntcurr == 0){
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
}
}

void TimerSet(unsigned long M){
_avr_timer_M = M;
_avr_timer_cntcurr = _avr_timer_M;
}

void Tick(){

  unsigned char temp = ~PINA & 0x03;
  switch(state){
    case Start:
      state = Initial;
      break;

    case Initial:
      state = Press;

    case Press:
      if(temp == 0x03){
        state = Reset;
      } else if (temp == 0x02){
        state = temp2;
      } else if (temp == 0x01){
        state = temp1;
      }
     break;

    case Increment:
      if(temp == 0x01){
        state = Increment;
      } else {
        state = Press;
      }
     break;

    case Decrement:
      if(temp == 0x02){
        state = Decrement;
      } else {
        state = Press;
      }
     break;

    case temp1:
      state = Increment;
      break;

    case temp2:
      state = Decrement;
      break;

    case Reset:
      if(temp == 0x03){
        state = Reset;
      } else {
        state = Press;
      }
     break;

    default:
      state = Start;
      break;
  }

  switch(state){
    case Start:
      break;

    case Initial:
      PORTB = 0x07;
      break;

    case Press:
      break;

    case Increment:
      break;

    case Decrement:
      break;

    case temp1:
      if(PORTB < 0x09){
        PORTB = PORTB + 1;
      }
      break;

    case temp2:
      if(PORTB > 0x00){
        PORTB = PORTB - 1;
      }
     break;

    case Reset:
      PORTB = 0x00;
      break;

    default:
      PORTB = 0x07;
      break;
}
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    TimerSet(50);
    TimerOn();
//    unsigned char tmpB = 0x00;
    while (1) {
//      tmpB = ~tmpB;
//      PORTB = tmpB;
        Tick();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}
