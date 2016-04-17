/**********************************************************************
* Filename    : pwmLed.c
* Description : Make a breathing led.
* Author      : Robot
* E-mail      : support@sunfounder.com
* website     : www.sunfounder.com
* Date        : 2014/08/27
**********************************************************************/

#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>

#define LedPin 6


void myexit()
{
	softPwmWrite(LedPin, 0);
	printf("Exiting...\n");
}


int main(void)
{
	int i;
	int j;
	//Schlussfunktion festlegen
	atexit(myexit);

	if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
		printf("setup wiringPi failed !");
		return 1; 
	}
	
	softPwmCreate(LedPin, 0, 100);//pwm output mode

	j=0;
	while(j<9){
		for(i=10;i<100;i++){
			softPwmWrite(LedPin, i);
			delay(7);
		}
		delay(500);
		for(i=100;i>=10;i--){
			softPwmWrite(LedPin, i);
			delay(7);
		}
	j=j+1;
	}

	return 0;
}
