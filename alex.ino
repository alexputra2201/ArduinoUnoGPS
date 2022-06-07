#include <SoftwareSerial.h>
#include <TinyGPS++.h> //Libary TinyGPS
#include <AltSoftSerial.h>
#include <String.h>

TinyGPSPlus gps; 
SoftwareSerial SIM800L(3,4);                //Serial SIM800L pin
//GPS Module RX pin to Arduino 9
//GPS Module TX pin to Arduino 8
AltSoftSerial neogps;



String Write_API_key = "LO6TDWO8M1OG6ZM5";  //Thingspeak Write API Key
String apn = "M2MAUTOTRONIC";   
unsigned long previousMillis = 0;
long interval = 6000;
float longitude;
float latitude; 
String url;
void setup(){
  Serial.begin(115200); 
  SIM800L.begin(9600);  
  neogps.begin(9600); 
  
}
 
void loop(){
  
  SetupModule();
  SIM800L.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  
  //SIM800L.println("AT+CIPSTART=\"TCP\",\"farikariauperkasa.com\",\"80\"");
  delay(1500);
  ShowSerialData();
 
 
  SIM800L.println("AT+CIPSEND");
  //delay(2000);
  Serial.println();
  ShowSerialData();
   while(Serial.available())  {
    SIM800L.println(Serial.readString());
  }
   unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
       previousMillis = currentMillis;
       sendGpsToServer();
    }
  //String str="GET https://api.thingspeak.com/update?api_key="+Write_API_key+"&field1=" + String(longitude) +"&field2="+String(latitude);
  Serial.println(url);  delay(2000);
  SIM800L.println(url); delay(2000);
  ShowSerialData();
 
  SIM800L.println((char)26); delay(4000); 
  SIM800L.println();
  ShowSerialData();
  SIM800L.println("AT+CIPSHUT");//close the connection
  delay(500); 
  ShowSerialData();
  url="";
  
  //total delay looping 50s
  delay(1000); //add 10s for 60s total delay looping 
} 

int sendGpsToServer()
{
    //Can take up to 60 seconds
    boolean newData = false;
    for (unsigned long start = millis(); millis() - start < 2000;){
      while (neogps.available()){
        if (gps.encode(neogps.read())){
          newData = true;
          break;
        }
      }
    }
  
    //If newData is true
    if(true){
      newData = false;
      
      String latitude, longitude;
      float altitude;
      unsigned long date, time, speed, satellites;
  
      latitude = String(gps.location.lat(), 6); // Latitude in degrees (double)
      longitude = String(gps.location.lng(), 6); // Longitude in degrees (double)
      altitude = gps.altitude.meters(); // Altitude in meters (double)
      date = gps.date.value(); // Raw date in DDMMYY format (u32)
      time = gps.time.value(); // Raw time in HHMMSSCC format (u32)
      speed = gps.speed.kmph();
      
      Serial.print("Latitude= "); 
      Serial.print(latitude);
      Serial.print(" Longitude= "); 
      Serial.println(longitude);
  
      //if (latitude == 0) {return 0;}
      
      String temp;
//      url = "http://farikariauperkasa.com/gps_project/gpsdata.php?lat=";
//      url += latitude;
//      url += "&lng=";
//      url += longitude;

      url = "GET https://api.thingspeak.com/update?api_key="+Write_API_key+"&field1=" + longitude +"&field2="+latitude;

      //url = "http://ahmadssd.000webhostapp.com/gpsdata.php?lat=222&lng=222";

      Serial.println(url);    
      delay(300);
    }
    return 1;
}

void SetupModule(){  
  if (SIM800L.available())Serial.write(SIM800L.read());
  SIM800L.println("AT"); delay(1000);
  SIM800L.println("AT+CPIN?"); delay(1000);
  SIM800L.println("AT+CREG?"); delay(1000);
  SIM800L.println("AT+CGATT?"); delay(1000);
  SIM800L.println("AT+CIPSHUT");delay(1000);
  SIM800L.println("AT+CIPSTATUS"); delay(1000);
  SIM800L.println("AT+CIPMUX=0");  delay(1000);

 //setting the APN,
  SIM800L.println("AT+CSTT=\""+apn+"\"");delay(1000);
  ShowSerialData();
  SIM800L.println("AT+CIICR");    delay(1000);
  ShowSerialData();
  
  //get local IP adress
  SIM800L.println("AT+CIFSR");    delay(1000);
  ShowSerialData();
  
  SIM800L.println("AT+CIPSPRT=0");delay(1000);
  ShowSerialData();
}

void ShowSerialData(){
  while(SIM800L.available()!=0)
  Serial.write(SIM800L.read());
  delay(1000); 
}
 
