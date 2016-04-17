/*
# open autostart file
$ sudo nano /etc/rc.local
# add the script (above exit 0)
sudo /home/pi/programme/shutdown_btn </dev/null >/dev/null 2>&1 &
*/

#include <wiringPi.h>
#include <stdio.h>

#define ButtonPin 5

int main(void)
{
	if(wiringPiSetup() == -1){ //when initialize wiring failed,print message to screen
		printf("setup wiringPi failed !");
		return 1; 
	}
	
	pinMode(ButtonPin, INPUT);

	pullUpDnControl(ButtonPin, PUD_UP);  //pull up to 3.3V,make GPIO1 a stable level
	while(1){
		if(digitalRead(ButtonPin) == 0){ //indicate that button has pressed down
			system("sudo shutdown -h now");   //shutdown
		}
	delay(100);
	}

	return 0;
}


