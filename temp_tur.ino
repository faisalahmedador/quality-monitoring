#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5 //digital output for temp sensor
#define SensorPin A5          // the pH meter Analog output is connected with the Arduino’s Analog

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float Celcius = 0;
float Fahrenheit = 0;

//End of tempurature sensor declaring

int sensorPin = A0;

float volt;
float ntu;

//End of turbidity sensor declaring

unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10], temp;


String message = ""; //for web server
bool messageReady = false; //for web server

void setup() {
  Serial.begin(115200);
  sensors.begin();
}
void loop() {

  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celcius);
  //End of tempurature sensor calculation



  volt = 0;
  for (int i = 0; i < 800; i++)
  {

    volt += ((float)analogRead(sensorPin) / 1023) * 5;
  }

  volt = volt / 800;
  volt = round_to_dp(volt, 1);

  if (volt < 3.15) {
    ntu = 3000;
  } else {
    ntu = -1120.4 * square(volt) + 5742.3 * volt - 5908.234; //21820.74 16178.576 5642.164
    if (ntu < 1) {
      ntu = -ntu;
    }
  }


  //End of turbidity sensor calculation


  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(SensorPin);

    delay(10);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++) { //take the average value of 6 center sample

    avgValue += buf[i];
  }
  float phValue = (float)avgValue * 5.0;
  phValue = phValue / 1024;
  phValue = phValue / 6;
  //Serial.println(phValue);
  phValue = -5.70 * phValue + 30.94;
  //convert the analog into millivolt
  // phValue=3.5*phValue;

  //End of PH sensor calculation









  //monitor serial communication

  while (Serial.available()) {
    message = Serial.readString();
    messageReady = true;
  }
  if (messageReady) {
    DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, message);

    if (error) {
      Serial.print(F("deserialization() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }
    if (doc["type"] == "request") {
      doc["type"] = "response";
      doc["Celcius"] = Celcius;
      doc["Fahrenheit"] = Fahrenheit;
      //doc["volt"] = volt;
      doc["ntu"] = ntu;
      doc["phValue"] = phValue;
      /*  doc["temp"] = analogRead(A5);
        doc["temp"] = analogRead(A5);
        doc["temp"] = analogRead(A5);
        doc["temp"] = analogRead(A5);
        doc["temp"] = analogRead(A5);*/






      serializeJson(doc, Serial);
    }
    messageReady = false;

  }
}




float round_to_dp( float in_value, int decimal_place) {
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}
