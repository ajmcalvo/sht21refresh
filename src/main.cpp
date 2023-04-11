
#include <Arduino.h>
#include <Wire.h>
#include "SHT2x.h"

uint16_t address = 0x40;
uint16_t freq = 1000;
// SHT21...
SHT2xClass SHT2x;

float temperature = 0;
float humidity = 0;  

void leeSHT21(void);
void testSensores();

void setup() {
  Wire.begin();
  delay(100);
  Serial.begin(115200);  // start serial for output
}


void loop() {
  testSensores();
  delay(2000);

}

void testSensores(){
   // Lectura del sensor SHT21...
  leeSHT21();  
  Serial.println("SHT21...");
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" ºC");
  Serial.print("Humedad    : ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.println();
}

void leeSHT21(void){
  unsigned long timeOutSensor = 0; 
  float t;
  float h;
  boolean temperature_ok = false;
  boolean humidity_ok = false;
 
  SHT2x.PrepareTemperatureNoHold();
  timeOutSensor = millis();  
  do{ 
    if(SHT2x.GetTemperatureNoHold(&t)){
      temperature_ok = true;     
    }
  }while(temperature_ok == false && (millis() - timeOutSensor) < 100L);
  
  SHT2x.PrepareHumidityNoHold();
  timeOutSensor = millis();  
  do{ 
    if(SHT2x.GetHumidityNoHold(&h)){
      humidity_ok = true;   
    }
  }while(humidity_ok == false && (millis() - timeOutSensor) < 100L);

  if(temperature_ok && humidity_ok){
    temperature = t;
    humidity = h;
  }else{
    temperature = 0;
    humidity = 0;
  }
}
