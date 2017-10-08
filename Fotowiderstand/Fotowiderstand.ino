int eingang=A0;

int LEDblue=7;
int sensorWert=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(LEDblue, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  sensorWert = analogRead(eingang);
  Serial.print("Sensorwert=");
  Serial.println(sensorWert);
  
  if(sensorWert > 512)
  {
    digitalWrite(LEDblue, HIGH);
  }
  else
  {
    digitalWrite(LEDblue, LOW);
  }
  
  delay(100);
}
