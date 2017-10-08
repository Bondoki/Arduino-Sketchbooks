int eingang=A0;

int LEDblue=7;
int sensorWert=0;

const int minDuration=100;
const int maxDuration=1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(LEDblue, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  sensorWert = analogRead(eingang);
  
  sensorWert = map(sensorWert,200,800,minDuration, maxDuration);
  sensorWert = constrain(sensorWert, minDuration, maxDuration);
  
  Serial.print("Sensorwert=");
  Serial.println(sensorWert);
/*  
  if(sensorWert > 512)
  {
    digitalWrite(LEDblue, HIGH);
  }
  else
  {
    digitalWrite(LEDblue, LOW);
  }
  delay(100);
*/
  digitalWrite(LEDblue, HIGH);
  delay(sensorWert);
    digitalWrite(LEDblue, LOW);
  delay(sensorWert);
}
