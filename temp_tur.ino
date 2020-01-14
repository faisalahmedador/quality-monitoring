#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

 float Celcius=0;
 float Fahrenheit=0;

 int sensorPin = A0;

  float volt;
  float ntu;
void setup(void)
{
  
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{ 
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);

  volt =0;
  for(int i=0; i<800; i++)
  {
    
    volt +=((float)analogRead(sensorPin)/1023)*5;
  }

    volt = volt/800;
    volt = round_to_dp(volt,1);

    if(volt <2.5){
      ntu = 3000;
    }else{
      ntu = -1120.4*square(volt)+5742.3*volt-4353.8;
      }
  

  
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);

  Serial.println(volt);
    Serial.println(ntu);
  delay(1000);
}



    float round_to_dp( float in_value, int decimal_place){
       float multiplier = powf( 10.0f, decimal_place );
       in_value = roundf( in_value * multiplier ) / multiplier;
        return in_value;
}
