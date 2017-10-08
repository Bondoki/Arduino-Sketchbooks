#include "SD.h"
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"

// A simple data logger for the Arduino analog pins
// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL 300000  //5min 10000 //30000 // mills between entries (reduce to take more/faster data)
// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 1200000 // 20min 20000 //300000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()
#define ECHO_TO_SERIAL 0 // echo data to serial port
#define WAIT_TO_START 0 // Wait for serial input in setup()
// the digital pins that connect to the LEDs
//efine redLEDpin 2
//fine greenLEDpin 3

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
#define DHTPINSENSORONE 2     // what digital pin we're connected to
#define DHTPINSENSORTWO 3     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dhtOne(DHTPINSENSORONE, DHTTYPE);
DHT dhtTwo(DHTPINSENSORTWO, DHTTYPE);


// The analog pins that connect to the sensors
#define photocellPin A9 // analog 0
#define tempPin A8 // analog 1
//#define BANDGAPREF 14 // special indicator that we want to measure the bandgap
//#define aref_voltage 5.0 // we tie 3.3V to ARef and measure it with a multimeter!
//#define bandgap_voltage 1.1 // this is not super guaranteed but its not -too- off
RTC_DS1307 RTC; // define the Real Time Clock object
// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;
// the logging file
File logfile;
void error(char *str)
{
Serial.print("error: ");
Serial.println(str);
// red LED indicates error
//digitalWrite(redLEDpin, HIGH);
while(1);
}
void setup(void)
{
Serial.begin(9600);
Serial.println();
// use debugging LEDs
//pinMode(redLEDpin, OUTPUT);
///inMode(greenLEDpin, OUTPUT);
#if WAIT_TO_START
Serial.println("Type any character to start");
while (!Serial.available());
#endif //WAIT_TO_START
// initialize the SD card
Serial.print("Initializing SD card...");
// make sure that the default chip select pin is set to
// output, even if you don't use it:
pinMode(10, OUTPUT);
// see if the card is present and can be initialized:
if (!SD.begin(10, 11, 12, 13)) {
error("Card failed, or not present");
}
Serial.println("card initialized.");
// create a new file
char filename[] = "LOGGER00.CSV";
for (uint8_t i = 0; i < 100; i++) {
filename[6] = i/10 + '0';
filename[7] = i%10 + '0';
if (! SD.exists(filename)) {
// only open a new file if it doesn't exist
logfile = SD.open(filename, FILE_WRITE);
break; // leave the loop!
}
}
if (! logfile) {
error("couldnt create file");
}
Serial.print("Logging to: ");
Serial.println(filename);
// connect to RTC
Wire.begin();
if (!RTC.begin()) {
logfile.println("RTC failed");
#if ECHO_TO_SERIAL
Serial.println("RTC failed");
#endif //ECHO_TO_SERIAL
}
logfile.println("millis,stamp,datetime,light,temp, humDHT1, humDHT2, meanHumDHT, tempDHT1, tempDHT2, meanTempDHT, ");
#if ECHO_TO_SERIAL
Serial.println("millis,stamp,datetime,light,temp,vcc");
#endif //ECHO_TO_SERIAL
// If you want to set the aref to something other than 5v
//analogReference(EXTERNAL);

dhtOne.begin();
dhtTwo.begin();
}
void loop(void)
{
DateTime now;
// delay for the amount of time we want between readings
delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
//digitalWrite(greenLEDpin, HIGH);
// log milliseconds since starting
uint32_t m = millis();
logfile.print(m); // milliseconds since start
logfile.print(", ");
#if ECHO_TO_SERIAL
Serial.print(m); // milliseconds since start
Serial.print(", ");
#endif
// fetch the time
now = RTC.now();
// log time
logfile.print(now.unixtime()); // seconds since 1/1/1970
logfile.print(", ");
logfile.print('"');
logfile.print(now.year(), DEC);
logfile.print("/");
logfile.print(now.month(), DEC);
logfile.print("/");
logfile.print(now.day(), DEC);
logfile.print(" ");
logfile.print(now.hour(), DEC);
logfile.print(":");
logfile.print(now.minute(), DEC);
logfile.print(":");
logfile.print(now.second(), DEC);
logfile.print('"');
#if ECHO_TO_SERIAL
Serial.print(now.unixtime()); // seconds since 1/1/1970
Serial.print(", ");
Serial.print('"');
Serial.print(now.year(), DEC);
Serial.print("/");
Serial.print(now.month(), DEC);
Serial.print("/");
Serial.print(now.day(), DEC);
Serial.print(" ");
Serial.print(now.hour(), DEC);
Serial.print(":");
Serial.print(now.minute(), DEC);
Serial.print(":");
Serial.print(now.second(), DEC);
Serial.print('"');
#endif //ECHO_TO_SERIAL
analogRead(photocellPin);
delay(10);
int photocellReading = analogRead(photocellPin);
//analogRead(tempPin);
delay(10);
//getting the voltage reading from the temperature sensor
float voltageMean=0.0;
float temperatureC=0.0;

int reading = analogRead(tempPin);
    
for(int z=0; z<10; z++)
{
    // converting that reading to voltage, for 3.3v arduino use 3.3
    float voltage = reading * 5.0;
    voltage /= 1024.0;
    
    voltageMean += voltage;
    
    temperatureC += (voltage - 0.5) * 100 ; //converting from 10 mv per degree wit 500 mV offset
    delay(30);
}
voltageMean /= 10.0;
temperatureC /= 10.0;
    
// Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h1 = dhtOne.readHumidity();
  // Read temperature as Celsius (the default)
  float t1 = dhtOne.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dhtOne.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  //if (isnan(h) || isnan(t) || isnan(f)) {
  //  Serial.println("Failed to read from DHT sensor!");
  //  return;
  //}
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h2 = dhtTwo.readHumidity();
  // Read temperature as Celsius (the default)
  float t2 = dhtTwo.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
//  float f2 = dhtTwo.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  //if (isnan(h2) || isnan(t2) || isnan(f2)) {
    //Serial.println("Failed to read from DHT sensor!");
   // return;
  //}

  float meanDHTtemperature = 0.5*(t1+t2);
  float meanDHThumidity = 0.5*(h1+h2);
  
//int tempReading = analogRead(tempPin);
// converting that reading to voltage, for 3.3v arduino use 3.3, for 5.0, use 5.0
/*voltage =0;// tempReading * aref_voltage / 1024;
//float temperatureC = (voltage - 0.5) * 100 ;

int temperatureC = map(analogRead(tempPin), 0, 410, -50, 150);
delay(50);
temperatureC += map(analogRead(tempPin), 0, 410, -50, 150);
delay(50);
temperatureC += map(analogRead(tempPin), 0, 410, -50, 150);
temperatureC /= 3;
*/
//float temperatureF = (temperatureC * 9 / 5) + 32;
logfile.print(", ");
logfile.print(photocellReading);
logfile.print(", ");
logfile.print(temperatureC);
logfile.print(", ");
logfile.print(h1);
logfile.print(", ");
logfile.print(h2);
logfile.print(", ");
logfile.print(meanDHThumidity);
logfile.print(", ");
logfile.print(t1);
logfile.print(", ");
logfile.print(t2);
logfile.print(", ");
logfile.print(meanDHTtemperature);
#if ECHO_TO_SERIAL
Serial.print(", ");
Serial.print(photocellReading);
Serial.print(", ");
Serial.print(temperatureC);
Serial.print(", ");
Serial.print(voltageMean);
#endif //ECHO_TO_SERIAL
// Log the estimated 'VCC' voltage by measuring the internal 1.1v ref
//analogRead(BANDGAPREF);
delay(10);
//int refReading = analogRead(BANDGAPREF);
//float supplyvoltage = (bandgap_voltage * 1024) / refReading;
//logfile.print(", ");
//logfile.print(supplyvoltage);
#if ECHO_TO_SERIAL
Serial.print(", ");
//Serial.print(supplyvoltage);
#endif // ECHO_TO_SERIAL
logfile.println();
#if ECHO_TO_SERIAL
Serial.println();
#endif // ECHO_TO_SERIAL
//digitalWrite(greenLEDpin, LOW);
// Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
// which uses a bunch of power and takes time
if ((millis() - syncTime) < SYNC_INTERVAL) return;
syncTime = millis();
// blink LED to show we are syncing data to the card & updating FAT!
//digitalWrite(redLEDpin, HIGH);
logfile.flush();
//digitalWrite(redLEDpin, LOW);
}

