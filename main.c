#include "sra.c"

/*
on/off : 069
0 : 022
1 : 019
2 : 024
3 : 085
4 : 008
5 : 019
6 : 081
7 : 066
8 : 082
9 : 081
right : 067
left : 064
vol+ : 009
vol- : 021
mute : 071
pause : 068
mode : 070
EQ : 007
RPT : 025
SCN : 020
*/

char sbuffer[10], ch, toggle, last_toggle;
unsigned char pos;
unsigned int necdata;

void initPort()
{
	DDRD = 0xFE;
	PORTD = 00;
	DDRC = 0xff;
	PORTC = 0x00;
}


void forward()
{
	sbi(PORTD,2);			//Motor
	cbi(PORTD,3);			//Connected to D,2 and D,3
}


void main()
{

initPort();
usart_init();
pwm1_init();
pos = 0;
last_toggle = -1;
forward();
set_pwm1a(399);
while(1)
	{
				ch = usart_receive_char(); //loop till character received
				PORTC=ch;
				if(ch==0x0A) // if received character is <LF> end of line, time to display
				{
					pos = 0; // buffer position reset for next reading
					
					// extract nec data from serial buffer to 16 bit value
					necdata = ((sbuffer[2]-'0')<<4)|(sbuffer[3]-'0'); // convert data from ASCII to low byte
					necdata |= (((sbuffer[0]-'0')<<12)|(sbuffer[1]-'0'))<<8; // convert data from ASCII to high byte					
										
										
				}
				else { //store serial data to buffer
					sbuffer[pos] = ch;
					pos++;
				}
				char code = necdata&0x00FF;
				PORTC=code;

				switch(code)
				{
					case 22: set_pwm1a(350);break;//0
					case 19: set_pwm1a(300);break;//1
					case 24: set_pwm1a(250);break;//2
					case 85: set_pwm1a(200);break;//3
					case 8: set_pwm1a(150);break;//4
					case 66: set_pwm1a(200);break;//7
					case 82: set_pwm1a(250);break;//8
					case 81: set_pwm1a(300);break;//9
 					default: break;
				}
				
	   } // end while
}
