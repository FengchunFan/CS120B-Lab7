/*      Author: ffan005
 *  Partner(s) Name: 
 *      Lab Section:
 *      Assignment: Lab #  Exercise #
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 *      Demo: 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "io.h"
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
//        right here, I chose Display over write data, because when I write data, it keeps writing numbers onto the LCD screen, 
//        but when I use display, it only display once (at least shown as only one digit)
        if (PORTB == 0x00) {
                LCD_DisplayString(1, "0");
        }
        if (PORTB == 0x01) {
                LCD_DisplayString(1, "1");
        }
        if (PORTB == 0x02) {
                LCD_DisplayString(1, "2");
        }
        if (PORTB == 0x03) {
                LCD_DisplayString(1, "3");
        }
        if (PORTB == 0x04) {
                LCD_DisplayString(1, "4");
        }
        if (PORTB == 0x05) {
                LCD_DisplayString(1, "5");
        }
        if (PORTB == 0x06) {
                LCD_DisplayString(1, "6");
        }
        if (PORTB == 0x07) {
                LCD_DisplayString(1, "7");
        }
        if (PORTB == 0x08) {
                LCD_DisplayString(1, "8");
        }
        if (PORTB == 0x09) {
                LCD_DisplayString(1, "9");
        }

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
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    TimerSet(50);
    TimerOn();
//    unsigned char tmpB = 0x00;
    LCD_init();
    while (1) {
//      tmpB = ~tmpB;
//      PORTB = tmpB;
        Tick();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}

