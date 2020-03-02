
#include <ThingSpeak.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>

unsigned long myChannelNumber = 896598;  // Replace the 0 with your channel number
const char * myWriteAPIKey = "IK48GHVB1L1ND9JM";    // Paste your ThingSpeak Write API Key between the quotes
//------------------------------------------------------------------

WiFiClient client;

#define HOSTIFTTT "maker.ifttt.com"
#define EVENTO "ESP"
#define IFTTTKEY "cbPx5g-V0gy3_qnKm9dA3eRhSEnFhduKttLAFnk-8jm"


ESP8266WebServer server;
uint8_t pin_led = 4;
bool led_status = LOW;
/*char* ssid = "Lake Touch ";
  char* password = "csenetwork";*/



char* ssid = "CSE Network - 2.4G";
char* password = "csenetwork";


/*char* ssid = "fpsr";
  char* password = "Bismillah";*/

void setup()
{
  pinMode(pin_led, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //server.on("/",[](){server.send(200,"text/plain","Hello World!");});
  server.on("/", handleIndex);
  server.begin();
  ThingSpeak.begin(client);
}

void loop()
{
  server.handleClient();
}

void handleIndex() {
  DynamicJsonDocument doc(1024);
  double Celcius = 0;
  double Fahrenheit = 0;
  double ntu = 0;
  double phValue = 0;
  doc["type"] = "request";
  serializeJson(doc, Serial);

  boolean messageReady = false;
  String message = "";

  while (messageReady == false) {
    if (Serial.available()) {
      message = Serial.readString();
      Serial.println(message);
      messageReady = true;
    }
  }
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print(F("deserializeJson() failed"));
    Serial.println(error.c_str());
    return;
  }
  //  temp = doc["temp"];

  Celcius = doc["Celcius"];
  Fahrenheit = doc["Fahrenheit"];
  //doc["volt"] = volt;
  ntu = doc["ntu"];
  phValue = doc["phValue"];


  ThingSpeak.setField(1, int(Celcius));
  ThingSpeak.setField(2, int(Fahrenheit));
  ThingSpeak.setField(3, int(ntu));
  ThingSpeak.setField(4, int(phValue));
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  server.send(200, "text/html", SendHTML(Celcius, Fahrenheit, ntu, phValue));



  if ((phValue > 8.0 || phValue < 6.0) || Celcius > 30.0 || (ntu != 3000 && ntu > 500))  {
    led_status = HIGH;
    if (client.connected())
    {
      client.stop();
    }

    client.flush();
    if (client.connect(HOSTIFTTT, 80)) {
      //Serial.println("Connected");
      // build the HTTP request
      String toSend = "GET /trigger/";
      toSend += EVENTO;
      toSend += "/with/key/";
      toSend += IFTTTKEY;
      toSend += "?value1=";
      toSend += Celcius;
      toSend += "&value2=";
      toSend += ntu;
      toSend += "&value3=";
      toSend += phValue;
      toSend += " HTTP/1.1\r\n";
      toSend += "Host: ";
      toSend += HOSTIFTTT;
      toSend += "\r\n";
      toSend += "Connection: close\r\n\r\n";
      client.print(toSend);
      //Serial.println(toSend);
    }

    client.flush();
    client.stop();
    delay(500);
  }



  else {
    led_status = LOW;
  }

  delay(20000);
}



String SendHTML(double temperature1, double temperature2, double turbidity, double phvalue) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Water Quality Monitoring</title>\n";
  ptr += "<style>html { font-family: Times New Roman, Times, serif; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 30px;color: #0775f0;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>IoT Based Water Quality Monitoring System</h1>\n";
  ptr += "<p>Celcius: ";
  ptr += temperature1;
  ptr += "&deg;C</p>";
  ptr += "<p>Farenheit: ";
  ptr += temperature2;
  ptr += "&deg;F</p>";
  ptr += "<p>Turbidity: ";
  ptr += turbidity;
  ptr += "ntu</p>";
  ptr += "<p>Potential of Hydrogen(pH): ";
  ptr += phvalue;
  ptr += "</p>";
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
