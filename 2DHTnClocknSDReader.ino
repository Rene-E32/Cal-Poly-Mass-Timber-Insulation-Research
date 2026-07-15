#include <DHT11.h>

#include "RTClib.h"

#include <Wire.h>

#include <SPI.h>

#include <SD.h>


//naming/activating file for sd reader
File sensorLog;

const 
int chipSelect = 10; //for RTC

const 
int internalHumtempPin = 8;       //pin select/initilizaion for sensors

const 
int externalHumtempPin = 9;



RTC_DS3231 rtc;  //something about clock

DHT11 dhtInternal(internalHumtempPin);
                                          //naming functions                     
DHT11 dhtExternal(externalHumtempPin);



void 
setup() {


Serial.begin(9600);



// Initialize SD Card hardware

if (!SD.begin(chipSelect)) {

Serial.println(F("SD initialization failed!"));

return;

}

Serial.println(F("SD initialization done."));



// Create file and write CSV headers if it's a new file

sensorLog = SD.open("logged.txt", FILE_WRITE);

if (sensorLog) {

if (sensorLog.size() ==
0) { //"if file exists"


//setting headers
sensorLog.println(F("Date(M/D/Y),Time(H:M:S),Inside Temperature(C),Inside Temperature(F),Inside Humidity(%),Outside Temperature(C),Outside Temperature(F),Outside Humidity(%)"));

Serial.println(F("New file created. Headers written."));

} else { // if exist, with same name, itll add onto instead of making new

Serial.println(F("File already exists. Appending new data to the end."));

}

sensorLog.close(); // log then close

} else { //error handling

Serial.println(F("Error opening logged.txt for header initialization."));

}


if (!rtc.begin()) { //begins clock

Serial.println(F("Couldn't find RTC")); // if doesn't start

while (1); // halt if RTC isn't wired correctly, kicks out if cant find 

}



if (!rtc.lostPower()) { //lost power case

Serial.println(F("RTC lost power, setting the time!"));

// sets RTC to the date & time this sketch was compiled

//rtc.adjust(DateTime(F(__DATE__), //logs data from pc from upload
//F(__TIME__)));

}

}

void 
loop() {

// initialize variables, gives 0 valuse
//updates over 0, to make sure dont get NaN values

int internalTemperatureC = 0;
int internalTemperatureF = 0;

int internalHumidity = 0;

int externalTemperatureC = 0;
int externalTemperatureF = 0;

int externalHumidity = 0;


DateTime now = rtc.now(); // sets now time, the current moment of RTC


// internal/external temp/hum sensor setup, 
//requests data

int internalResults = 
dhtInternal.readTemperatureHumidity(internalTemperatureC, internalHumidity);

int externalResults = 
dhtExternal.readTemperatureHumidity(externalTemperatureC, externalHumidity);

// internal sensor serial printing

if (internalResults == 
0 ) {

// convert from celsius to farenheight

internalTemperatureF = (internalTemperatureC * 
1.8) + 32;

Serial.print(F("Internal Temperature(C): "));

Serial.print(internalTemperatureC);

Serial.print(F(", Internal Temperature(F): "));

Serial.print(internalTemperatureF);

Serial.print(F(", Internal Humidity: "));

Serial.print(internalHumidity);

Serial.println(F(" %"));

}

else { //function for error handling 

Serial.println(DHT11::getErrorString(internalResults));

}



// external sensor serial printing 

if (externalResults == 
0 ) {

// convert from celsius to farenheight

externalTemperatureF = (externalTemperatureC * 
1.8) + 32;

Serial.print(F("External Temperature(C): "));

Serial.print(externalTemperatureC);

Serial.print(F(", External Temperature(F): "));

Serial.print(externalTemperatureF);

Serial.print(F(", External Humidity: "));

Serial.print(externalHumidity);

Serial.println(F(" %"));

}

else { // error for out sensor

Serial.println(DHT11::getErrorString(externalResults));

}



// Open the file to add the current sensor readings
sensorLog = SD.open("logged.txt", FILE_WRITE); // if exists

if (sensorLog) {
//writes all this
sensorLog.print(now.month(), DEC); //DEC for decimial formating
                              
sensorLog.print('/');

sensorLog.print(now.day(), DEC);

sensorLog.print('/');

sensorLog.print(now.year(), DEC);

sensorLog.print(F(","));

sensorLog.print(now.hour(), DEC);

sensorLog.print(':');

sensorLog.print(now.minute(), DEC);

sensorLog.print(':');

sensorLog.print(now.second(), DEC);

sensorLog.print(",");

sensorLog.print(internalTemperatureC);

sensorLog.print(",");

sensorLog.print(internalTemperatureF);

sensorLog.print(",");

sensorLog.print(internalHumidity);

sensorLog.print(",");

sensorLog.print(externalTemperatureC);

sensorLog.print(",");

sensorLog.print(externalTemperatureF);

sensorLog.print(",");

sensorLog.println(externalHumidity); //very last one has to have LN
//gives nice coloums, makes it easier to import to mat lab
//m
 

sensorLog.close(); // save and close file, 
// avoids corrupt whole file

Serial.println(F("Data successfully logged to SD card.")); //

} else {

Serial.println(F("Error opening logged.txt during loop data save."));

}



delay(5000); // 5 seconds
 
}