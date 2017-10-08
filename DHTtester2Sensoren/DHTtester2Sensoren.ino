// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain



// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11

//#define DHTTYPE DHT21   // DHT 21 (AM2301)



void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

 
}

void loop() {
  // Wait a few seconds between measurements.
  delay(10000);

  

  // Compute heat index in Fahrenheit (the default)
  float hif = dhtOne.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dhtOne.computeHeatIndex(t, h, false);

  Serial.print("One: Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h2 = dhtTwo.readHumidity();
  // Read temperature as Celsius (the default)
  float t2 = dhtTwo.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f2 = dhtTwo.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h2) || isnan(t2) || isnan(f2)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif2 = dhtTwo.computeHeatIndex(f2, h2);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic2 = dhtTwo.computeHeatIndex(t2, h2, false);

  Serial.print("Two: Humidity: ");
  Serial.print(h2);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t2);
  Serial.print(" *C ");
  Serial.print(f2);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic2);
  Serial.print(" *C ");
  Serial.print(hif2);
  Serial.println(" *F");
  
  
}
