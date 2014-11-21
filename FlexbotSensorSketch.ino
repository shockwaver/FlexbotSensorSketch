#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <TimedAction.h>
   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
long baud = 115200L;
String incoming;
// set seaLevelPressure to default
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

TimedAction sensorRead = TimedAction(2000,sensorData);

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(baud);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }*/
  Serial1.begin(baud);

  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    Serial1.println("sensor error");
    while(1);
  }

  //Serial.println("Sending AT");
  //Serial1.print("AT");
  delay(500);
  Serial.println("Ready.");
  Serial1.setTimeout(100);
}

void loop() // run over and over
{
  //sensorRead.check();
  incoming = "";
  if (Serial1.available() > 0) {
    incoming = Serial1.readString();
    incoming.trim();
    //Serial.println(incoming.substring(1,3));
    Serial.print("> ");Serial.println(incoming);
    if (incoming == "read") {
      sensorData();
    } else if (incoming == "temp") {
      sensorTemp();
    } else if (incoming.substring(0,3) == "set") {
      String pressure = incoming.substring(3);
      pressure.trim();
      char pBuff[16]; // string buffer
      pressure.toCharArray(pBuff, sizeof(pBuff));
      seaLevelPressure = atof(pBuff); // convert string to char array to float
      Serial.print("Setting local sea level pressure to: ");
      Serial.println(seaLevelPressure);
      Serial1.print("OK+");Serial1.print(seaLevelPressure);Serial1.print("hPa");
    } else if (incoming == "get") {
      Serial.print("Current local sea level pressure: ");Serial.println(seaLevelPressure);
      Serial1.print(seaLevelPressure);Serial1.print(" hPa");
    } else if (incoming == "pres" {
      getPressure(); 
    } else if (incoming == "elev" {
      getElev(); 
    }else {
      Serial.println("Uknown Command");
      Serial1.print("err");
    }
  }
}

void getPressure(void) {
  sensors_event_t event;
  bmp.getEvent(&event);
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");
    
    Serial1.print(event.pressure);Serial1.print(" hPa");
  }
}

void getElev(void) {
  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
     
    /* First we get the current temperature from the BMP085 */
    float temperature;
    bmp.getTemperature(&temperature);

    /* Then convert the atmospheric pressure, SLP and temp to altitude    */
    /* Update this next line with the current SLP for better results      */
    
    float altitude = bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure,
                                        temperature);
    Serial.print("Altitude: ");Serial.print(altitude);Serial.println("m");
    Serial1.print(altitude);Serial1.print("m");
  }
}
                                        

void sensorTemp(void) {
  float temperature;
  bmp.getTemperature(&temperature);
  float tempF = (temperature * 9.0)/5.0 + 32;
  Serial1.print(temperature);Serial1.print("c/");Serial1.print(tempF);Serial1.print("f");
  Serial.print(temperature);Serial.print("c/");Serial.print(tempF);Serial.println("f");
}

void sensorData(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");
    
    /* Calculating altitude with reasonable accuracy requires pressure    *
     * sea level pressure for your position at the moment the data is     *
     * converted, as well as the ambient temperature in degress           *
     * celcius.  If you don't have these values, a 'generic' value of     *
     * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
     * in sensors.h), but this isn't ideal and will give variable         *
     * results from one day to the next.                                  *
     *                                                                    *
     * You can usually find the current SLP value by looking at weather   *
     * websites or from environmental information centers near any major  *
     * airport.                                                           *
     *                                                                    *
     * For example, for Paris, France you can check the current mean      *
     * pressure and sea level at: http://bit.ly/16Au8ol                   */
     
    /* First we get the current temperature from the BMP085 */
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    /* Then convert the atmospheric pressure, SLP and temp to altitude    */
    /* Update this next line with the current SLP for better results      */
    
    float altitude = bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure,
                                        temperature);
    Serial.print("Altitude:    "); 
    Serial.print(altitude); 
    Serial.println(" m");
    Serial.println("");
    Serial1.print(temperature); Serial1.print(","); 
    Serial1.print(event.pressure); Serial1.print(",");
    Serial1.print(altitude,1); //Serial1.println("");
  }
    else
  {
    Serial.println("Sensor error");
    Serial1.println("err");
  }
  
  //delay(1000);
}
