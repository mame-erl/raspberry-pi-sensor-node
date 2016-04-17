/*
 *      dht22.c:
 *	Simple test program to test the wiringPi functions
 *	Based on the existing dht11.c
 *	Amended by technion@lolware.net
 */

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

//#include "locking.h"

#define MAXTIMINGS 85
static int DHTPIN = 7;


static int dht22_dat[5] = {0,0,0,0,0};




float temp;
int timer;
bool heiz_an = false;
int sicher=0;
bool dsicher = false;

int check(float t, float h){
	
	
	if(t<=24.5){
	printf("Zu kalt, Heizung wird eingeschaltet!\n\n");
	
	digitalWrite(0, 0);	
	digitalWrite(2, 1);


	}else if(t>26){
	printf("Zu warm, Heizung wird aus- und Luefter eingeschaltet!\n\n");
	
	digitalWrite(2, 0);
	digitalWrite(0, 1 );
	

	}else if (t>24.5 && t< 26){
	
	printf("Das Ideal wurde erreicht, Temperatur halten.\n\n");
	digitalWrite(0, 0);
	digitalWrite(2, 0);


	}

}



static uint8_t sizecvt(const int read)
{
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0)
  {
    printf("Invalid data from wiringPi library\n");
    exit(EXIT_FAILURE);
  }
  return (uint8_t)read;
}

static int read_dht22_dat()
{


uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;

  dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

  // pull pin down for 18 milliseconds
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, HIGH);
  delay(10);
  digitalWrite(DHTPIN, LOW);
  delay(18);
  // then pull it up for 40 microseconds
  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(40); 
  // prepare to read the pin
  pinMode(DHTPIN, INPUT);

  // detect change and read data
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (sizecvt(digitalRead(DHTPIN)) == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    laststate = sizecvt(digitalRead(DHTPIN));

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      dht22_dat[j/8] <<= 1;
      if (counter > 16)
        dht22_dat[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ((j >= 40) && 
      (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
        float t, h;
        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
        h /= 10;
        t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
        t /= 10.0;
        if ((dht22_dat[2] & 0x80) != 0)  t *= -1;
	sicher = 0;	
	dsicher = false;
	temp = t;
    printf("Humidity = %.2f %% Temperature = %.2f *C \n", h, t );



	//check(t, h);
		
	
    return 1;

		
  }
  else
  {
	
    //printf("Data not goody, skip\n");

	if (dsicher == true){
		sicher++;
	}
	dsicher = true;
    
    return 0;
	
  }
}



int main (int argc, char *argv[])
{
 	

	
  if (argc != 2)
    printf ("usage: %s <pin>\ndescription: pin is the wiringPi pin number\nusing 7 (GPIO 4)\n",argv[0]);
  else
    DHTPIN = atoi(argv[1]);
   

	
 printf ("Raspberry Pi wiringPi DHT22 reader\nwww.lolware.net\n") ;

 

  if (wiringPiSetup () == -1)
    exit(EXIT_FAILURE) ;
	
  if (setuid(getuid()) < 0)
  {
    perror("Dropping privileges failed\n");
    exit(EXIT_FAILURE);
  }

//	pinMode(0, OUTPUT);	//Luefter
//	pinMode(2, OUTPUT);	//Heizungen

	while(1){
	   delay(2000);
	   while (read_dht22_dat() == 0) 
	      {
		if (sicher >= 6){
		printf("10s warten!\n");
		delay(10000);
		sicher = 0;
		}	
	      delay(1000); // wait 1sec to refresh
              }
	}
  delay(1500);
  return 0 ;
}
