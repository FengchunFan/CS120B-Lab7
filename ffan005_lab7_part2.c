/*      Author: ffan005
 *  Partner(s) Name: 
 *      Lab Section:
 *      Assignment: Lab #  Exercise #
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 *      Demo: https://www.youtube.com/watch?v=3PX1CMqoxZU
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned char Mask = 0x00;
unsigned char score = 0x05;
enum States {Start, Init, Action, Pause, Paused, Resume} state;
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

        if (score == 0x00) {
                LCD_DisplayString(1, "0");
        }
        if (score == 0x01) {
                LCD_DisplayString(1, "1");
        }
        if (score == 0x02) {
                LCD_DisplayString(1, "2");
        }
        if (score == 0x03) {
                LCD_DisplayString(1, "3");
        }
        if (score == 0x04) {
                LCD_DisplayString(1, "4");
        }
        if (score == 0x05) {
                LCD_DisplayString(1, "5");
        }
        if (score == 0x06) {
                LCD_DisplayString(1, "6");
        }
        if (score == 0x07) {
                LCD_DisplayString(1, "7");
        }
        if (score == 0x08) {
                LCD_DisplayString(1, "8");
        }
        if (score == 0x09) {
                LCD_DisplayString(1, "Victory!!!");
        }

  switch(state){
        case Start:
                state = Init;
               break;
        case Init:
                state = Action;
                break;
        case Action:
                if((~PINA & 0x01) == 0x01){
                state = Pause;
                }else{
                state = Action;
                }
                break;
                break;
        case Pause:
                if((~PINA & 0x01) == 0x01){
                state = Pause;
                }else{
                state = Action;
                }
                break;

        case Paused:
                if((~PINA & 0x01) == 0x01){
                state = Resume;
                }else{
                state = Paused;
                }
                break;
        case Resume:
                if((~PINA & 0x01) == 0x01){
                state = Resume;
                }else{
                state = Action;
                }
                break;

        default:
                state = Start;
                break;
  }

  switch(state){
        case Start:
                break;
        case Init:
                PORTB = 0x01;
               break;
        case Action:
                if(PORTB == 0x01){
                        Mask = 0x00;
                        PORTB = 0x02;
                }else if(PORTB == 0x02){
                        if(Mask == 0x00){
                        PORTB = 0x04;
                        }else{
                        PORTB = 0x01;
                        }
                }else if(PORTB == 0x04){
                        Mask = 0x01;
                        PORTB = 0x02;
                }
                break;
        case Paused:
                break;
          case Pause:
                if(PORTB == 0x02){
                score = score + 1;
                }else {
                score = score - 1;
                }
                  break;
          case Resume:
                  break;
        default:
                break;
  }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    TimerSet(50);
    TimerOn();
    LCD_init();
//    unsigned char tmpB = 0x00;
    while (1) {
//      tmpB = ~tmpB;
//      PORTB = tmpB;
       Tick();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}
