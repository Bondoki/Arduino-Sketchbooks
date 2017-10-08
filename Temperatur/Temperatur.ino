int TMP36 = A0;

int temperatur = 0;

int temp[10];

int time= 20;

void setup() {

Serial.begin(9600);

}

void loop() {
  
  for(int i=0; i < 10; i++)
  {
    temp[i] = map(analogRead(TMP36), 0, 410, -50, 150);

    delay(time);
  }
  
  temperatur=0;
  
  for(int i=0; i < 10; i++)
  {
    temperatur += temp[i];
  }
  
  temperatur /= 10;
  
  Serial.print(temperatur);

  Serial.println(" Grad Celsius");
}

