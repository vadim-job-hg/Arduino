#include <dht11.h>
dht11 DHT;
#define MOISTURE_PIN A2  //soil Moisture sensor
#define DHT11_PIN    9   //DHT11
const int LOOP_DELAY = 360;
const int LOOP_DELAY_OK = 8640;
const int LOOP_DELAY_WARNINGS = 720;
const int LOOP_DELAY_CRITICAL = 180;
const int PUMP_ALLOWED = 8640;
class Enviroment{
  protected:
    int airHumidity;   //environment humidity
    int airTemperature;  // environment temperature
    int soilHumidity;
    int maxHumidity = 6; 
    bool pumpIsOn = false;
    unsigned long int lastPump;

    public:
    void init(){
      pinMode(5, OUTPUT);
      pinMode(6, OUTPUT);
      pumpOff();
    }

    void scanSensors(){
      scanAir();
      scanGround();
    }

    void printToPort(){
      Serial.print("airHumidity:");
      Serial.print(airHumidity);
      Serial.print(",\t");
      Serial.print("airTemperature:");
      Serial.print(airTemperature);      
      Serial.print(",\t");
      Serial.print("soilHumidity:");
      Serial.println(soilHumidity);
        
    }   

    void pumpActions(){
      if (soilHumidity < 1 && abs(millis()-lastPump)/1000<PUMP_ALLOWED && !pumpIsOn) {
        pumpOn();
        delay(3000);
      } else if(pumpIsOn) {
        pumpOff();
      }  
    }

    void pumpTest(){      
       pumpOn();
       delay(3000);
       pumpOff();
       delay(3000);    
    }
    
    int getCurrentSituation(){
      
    }

    protected:

    void scanAir(){
      int chk;
      chk = DHT.read(DHT11_PIN);   //Read Data
      switch (chk){
        case DHTLIB_OK:  
            Serial.print("OK,\t"); 
            break;
        case DHTLIB_ERROR_CHECKSUM: 
            Serial.print("Checksum error,\t"); 
            break;
        case DHTLIB_ERROR_TIMEOUT: 
            Serial.print("Time out error,\t"); 
            break;
        default: 
            Serial.print("Unknown error,\t"); 
            break;
      }
      
      airHumidity=DHT.humidity;
      airTemperature=DHT.temperature; 
    }

    void scanGround(){
      soilHumidity = analogRead(MOISTURE_PIN);
    }
    //open pump
    void pumpOn() 
    {
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      pumpIsOn = true;
      lastPump = millis();
    }
    
    //close pump
    void pumpOff() 
    {
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      pumpIsOn = false;
    }    
};
Enviroment env; 
void setup(){
  Serial.begin(9600);   
  env.init();
}

void loop(){
  env.scanSensors();
  env.printToPort();
  env.pumpActions();
  //env.pumpTest();
  int delaySec = 1;
  delay(delaySec*1000);
}
