/*
 * MINI2.c
 *
 *  Created on: Sep 21, 2023
 *      Author: karim
 */

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned char sec1=0,sec2=0,min1=0,min2=0,hour1=0,hour2=0,TICKS;

void initialization(void){
	DDRA|=0X3F;
	DDRC|=0X0F;
	PORTC&=0xF0;
	PORTA&=0x3F;
}
void Timer1_init(void){
	TCNT1=0; //timer initial value
	OCR1A=977;//compare value
	TCCR1A=(1<<FOC1A);
	//enable compare interrupt for channel A
		/* Configure timer1 control registers
		 * 1. Non PWM mode FOC1A=1 and FOC1B=1
		 * 2. No need for OC1A & OC1B in this example so COM1A0=0 & COM1A1=0 & COM1B0=0 & COM1B1=0
		 * 3. CTC Mode and compare value in OCR1A WGM10=0 & WGM11=0 & WGM12=1 & WGM13=0
		 */
	TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);
	/*
		 * 4. Clock = F_CPU/1024 CS10=1 CS11=0 CS12=1
		 */
	TIMSK|=(1<<OCIE1A);
	/*SET THE I_BIT*/
	SREG|=(1<<7);
}void INT0_INIT(void){
	DDRD&=~(1<<PD2);
	GICR|=(1<<INT0);
	PORTD |=(1<<PD2);
	MCUCR|=(1<<ISC01);
	SREG|=(1<<7);
}
// Interrupt Service Routine for timer1 compare mode channel A

ISR(TIMER1_COMPA_vect){
	if(sec1<9)
		{
			sec1++;
		}
		else if(sec1==9 && sec2 < 5)
		{
			sec1=0;
			sec2++;
		}
		else if (sec2 == 5 && sec1 == 9 && min1 < 5)
		{
			sec1=0;
			sec2=0;
			++min1;
		}
		else if(min1==9 && min2 < 5 && sec2 == 5 && sec1 == 9)
		{
			sec1=0;
			sec2=0;
			min1=0;
			++min2;
		}
		else if (min2 == 5 && min1 == 9 && hour1 <9 && sec2 == 5 && sec1 == 9)
		{
			sec1=0;
			sec2=0;
			min1=0;
			min2=0;
			++hour1;
		}
		else if (min2 == 5 && min1 == 9 && hour1 ==9 && sec2 == 5 && sec1 == 9)
		{
			sec1=0;
			sec2=0;
			min1=0;
			min2=0;
			hour1=0;
			++hour2;
		}



}
// External INT0 Interrupt Service Routine
ISR(INT0_vect){
	sec1=0,sec2=0,min1=0,min2=0,hour1=0,hour2=0;
}
void INT1_INIT(void){

	DDRD&=~(1<<3);
	MCUCR|=(1<<ISC10)|(1<<ISC11);
	GICR|=(1<<7);
	SREG|=(1<<7);
}
// External INT1 Interrupt Service Routine
ISR(INT1_vect){
	TCCR1B &=0xF8;
}
void INT2_INIT(void){
	DDRB&=~(1<<PB2);
	GICR|=(1<<INT2);
	PORTB |= (1<<PB2);
    sei();
}
ISR(INT2_vect){
	TCCR1B|=(1<<CS10)|(1<<CS12);
}



int main(void){
	DDRC|=(1<<PC5);

	initialization();
	Timer1_init();
	INT0_INIT();
	INT1_INIT();
	INT2_INIT();
	unsigned char Time[6]={sec1,sec2,min1,min2,hour1,hour2};

	while(1){
		Time[0]=sec1;
		Time[1]=sec2;
		Time[2]=min1;
		Time[3]=min2;
		Time[4]=hour1;
		Time[5]=hour2;
	for(int i=0;i<6;i++){
		PORTA=(1<<i);
		PORTC=(PORTC&0XF0)|(Time[i]&0x0F);
		_delay_ms(1);

	}
 if(min1==1&&hour1==0&&sec1==0&&sec2==0&&min2==0&&hour2==0){
		PORTC&=~(1<<5);

	}
 else{
	 PORTC|=(1<<PC5);

 }




	}
return 0;}
