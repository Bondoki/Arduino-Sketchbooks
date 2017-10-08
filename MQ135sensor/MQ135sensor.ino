// Arduino code and library available to download - link below the video

/* Connection pins:

Arduino       MQ-135
  A0            A0
 +5V            VCC
 GND            GND
 
*/

#include "MQ135.h"

int analogPin = 0;

MQ135 gasSensor = MQ135(analogPin);


int i = 0;

double result;
double rzero;

void setup() {
  Serial.begin(9600);
}

void loop() {
 if (i==0) {
   rzero = gasSensor.getRZero(); // float
 }
 if (i>0) {  
   result = gasSensor.getRZero();
   rzero = (rzero + result)/2;
  }
  
  float ppm = gasSensor.getPPM();
  //float correctedppm = gasSensor.getPPM();
  
  Serial.print(rzero);
  Serial.print(", ");
  Serial.print(result);
  Serial.print(", ");
  Serial.print(ppm);
  Serial.println(" ppm");
  i++;
  delay(10000);
}
