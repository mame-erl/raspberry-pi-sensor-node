#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAX_TIME 85
#define DHT11PIN 7
#define ATTEMPTS 5
int dht11_val[5]={0,0,0,0,0};

typedef unsigned char uchar;
typedef unsigned int  uint;
 
#define     ADC_CS    0
#define     ADC_CLK   1
#define     ADC_DIO   2
 
uchar get_ADC_Result(void)
{
    //10:CH0
    //11:CH1
    uchar i;
    uchar dat1=0, dat2=0;
 
    digitalWrite(ADC_CS, 0);
 
    digitalWrite(ADC_CLK,0);
    digitalWrite(ADC_DIO,1);    delayMicroseconds(2);
    digitalWrite(ADC_CLK,1);    delayMicroseconds(2);
    digitalWrite(ADC_CLK,0);
 
    digitalWrite(ADC_DIO,1);    delayMicroseconds(2); //CH0 10
    digitalWrite(ADC_CLK,1);    delayMicroseconds(2);
    digitalWrite(ADC_CLK,0);
 
    digitalWrite(ADC_DIO,0);    delayMicroseconds(2); //CH0 0
     
    digitalWrite(ADC_CLK,1);   
    digitalWrite(ADC_DIO,1);    delayMicroseconds(2);
    digitalWrite(ADC_CLK,0);   
    digitalWrite(ADC_DIO,1);    delayMicroseconds(2);
     
    for(i=0;i<8;i++)
    {
        digitalWrite(ADC_CLK,1);    delayMicroseconds(2);
        digitalWrite(ADC_CLK,0);    delayMicroseconds(2);
 
        pinMode(ADC_DIO, INPUT);
        dat1=dat1<<1 | digitalRead(ADC_DIO);
    }
     
    for(i=0;i<8;i++)
    {
        dat2 = dat2 | ((uchar)(digitalRead(ADC_DIO))<<i);
        digitalWrite(ADC_CLK,1);    delayMicroseconds(2);
        digitalWrite(ADC_CLK,0);    delayMicroseconds(2);
    }
 
    digitalWrite(ADC_CS,1);
 
    pinMode(ADC_DIO, OUTPUT);
     
    return(dat1==dat2) ? dat1 : 0;
}

float dht11_read_val(int mode)
{
  uint8_t lststate=HIGH;
  uint8_t counter=0;
  uint8_t j=0,i;
  float farenheit;
  for(i=0;i<5;i++)
     dht11_val[i]=0;
  pinMode(DHT11PIN,OUTPUT);
  digitalWrite(DHT11PIN,LOW);
  delay(18);
  digitalWrite(DHT11PIN,HIGH);
  delayMicroseconds(40);
  pinMode(DHT11PIN,INPUT);
  for(i=0;i<MAX_TIME;i++)
  {
    counter=0;
    while(digitalRead(DHT11PIN)==lststate){
      counter++;
      delayMicroseconds(1);
      if(counter==255)
        break;
    }
    lststate=digitalRead(DHT11PIN);
    if(counter==255)
       break;
    // top 3 transistions are ignored
    if((i>=4)&&(i%2==0)){
      dht11_val[j/8]<<=1;
      if(counter>16)
        dht11_val[j/8]|=1;
      j++;
    }
  }
  // verify checksum and print the verified data
  if((j>=40)&&(dht11_val[4]==((dht11_val[0]+dht11_val[1]+dht11_val[2]+dht11_val[3])& 0xFF)))
  {
    if(mode == 1) //Temperatur
    {
      return((float)dht11_val[2]+(float)(dht11_val[3]/100));
    }
    else if(mode == 0) //Feuchtigkeit
    {
      return((float)dht11_val[0]+(float)(dht11_val[1]/100));
    }
  } 
}
 
int main(void)
{
    if(wiringPiSetup()==-1)
    {
      exit(1);
    }

    uchar adcVal;
    pinMode(ADC_CS,  OUTPUT);
    pinMode(ADC_CLK, OUTPUT);
    pinMode(ADC_DIO, OUTPUT);
    float temp;
    float hum;

    //Luftfeuchte einlesen, bis valider Wert
    while(1)
    {
      hum=dht11_read_val(0);
      if(hum>9.9)
      {
        //printf("Luftfeuchte: %4.1f\n",hum);
        break;
      }
    }

    delay(800);

    //Temperatur einlesen, bis valider Wert
    while(1)
    {
      temp=dht11_read_val(1);
      if(temp!=0.0)
      {
        //printf("Temperatur: %4.1f\n",temp);
        break;
      }
    }    

    //Lichtsensor einlesen
    adcVal = (get_ADC_Result())-80;

    //Ergebnis ausgeben
    printf("%4.1f/%4.1f/%d\n", temp, hum, adcVal);
  	return 0;
}