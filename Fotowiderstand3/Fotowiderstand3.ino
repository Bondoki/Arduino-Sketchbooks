int eingang=A0;

int piezo=7;
int sensorWert=0;

const int minDuration=1;
const int maxDuration=10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(piezo, OUTPUT);
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
  digitalWrite(piezo, HIGH);
  delay(sensorWert);
    digitalWrite(piezo, LOW);
  delay(sensorWert);
}
