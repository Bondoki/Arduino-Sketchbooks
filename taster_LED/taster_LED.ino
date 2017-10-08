int LEDblau=6;
int taster =7;
int tasterstatus=0;

void setup() {
  // put your setup code here, to run once:
pinMode(LEDblau, OUTPUT);
pinMode(taster, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  tasterstatus=digitalRead(taster);
  
  if(tasterstatus==HIGH)
  {
    digitalWrite(LEDblau, HIGH);
    delay(5000);
    digitalWrite(LEDblau, LOW);
  }
  else
  {
    digitalWrite(LEDblau, LOW);
  }
}
