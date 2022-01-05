#include <avr/io.h>
#include <avr/interrupt.h>
#define CPU_CLOCK 2000000 // 16Mhz -> / 8 2Mhz
#define DELAY_TIME 150

void	wait_x_cpu_clocks(int32_t cpu_clocks)
{
	while (cpu_clocks > 0)
	{
		cpu_clocks-=3;
	}
}

void	custom_delay(int32_t milli)
{
	//milli = 0,001s
	milli = milli *	2000;
	wait_x_cpu_clocks(milli - 5);
}

void draw_animation()
{
	 	PORTB = 0b00000001;
	 	PORTC = 0b00000010;
		custom_delay(DELAY_TIME);
	 	PORTB = 0b00000010;
		custom_delay(DELAY_TIME);
	 	PORTB = 0b00000100;
		custom_delay(DELAY_TIME);
	 	PORTB = 0b00001000;
	 	PORTC = 0b00000000;
		custom_delay(DELAY_TIME);
	 	PORTB = 0b00000100;
		custom_delay(DELAY_TIME);
	 	PORTB = 0b0000010;
		custom_delay(DELAY_TIME);
}

void setupTimer1() {
  cli();
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  // 100 Hz (16000000/((624+1)*256))
  OCR1A = 624;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 256
  TCCR1B |= (1 << CS12);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

ISR(TIMER1_COMPA_vect) {
	static uint16_t counter = 0;

	if (counter <= 0xFF)
	{
		OCR2B = 0xFF;
		OCR0A = counter;
		OCR0B = 0xFF - counter;
	}
	else if (counter <= 0x1FF)
	{
		OCR0A = 0xFF;
		OCR0B = counter - 0x100;
		OCR2B = 0xFF - (counter - 0x100);
	}
	else if (counter <= 0x2FF)
	{
		OCR0B = 0xFF;
		OCR2B = counter - 0x200;
		OCR0A = 0xFF - (counter - 0x200);
	}
	if (counter >= 0x300)
	{
		counter = 0;
	}
	else 
	{
		counter++;
	}

}

int		main()
{
	DDRB |= 0b00001111;
	DDRD |= 0b01101000;
	DDRC |= 0b00000010;
	PORTB &= !0b00001111;
	PORTD |= 0b01101000;
	PORTC &= !0b00000010;
	DDRD |= 0b01101000;
	PORTD = 0b01101000;

	TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
	TCCR0B |= (1 << CS10);

	TCCR0A |= (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
	TCCR0B |= (1 << CS10) ;

	TCCR2A |= (1 << COM2B1) | (1 << WGM00) | (1 << WGM01);
	TCCR2B |= (1 << CS10) ;

	OCR0A = 0xFF;
	OCR0B = 0xFF;
	OCR2B = 0xFF;

	setupTimer1();

	for(;;)
	{
		draw_animation();
	}
	return (0);
}