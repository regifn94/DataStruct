#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <pms.h>

// ================= CONNECT TO WIFI ================
const char ssid[] = "wifi-name";
const char pass[] = "pass";

const char* host = "api.thingspeak.com"; // connect to thingspeak
const char* APIKey = "thingspeakkey"; // api key to thingspeak

char auth[] = "blynkkey"; // blynk key  
WiFiClient client;
int pm10ug, pm25ug;
int pm1=0, CO2PPM;
float COPPM;

// Deklarasi variabel mq-7
#define co2Zero     55                        //calibrated CO2 0 level

long RL = 1000; // 1000 Ohm
long Ro = 8592; // 830 ohm ( SILAHKAN DISESUAIKAN)

Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

// Deklarasi variabel sensor debu
Pmsx003 pms(0, 2);

void setup(void)
{
  Serial.begin(9600);
  
    while (!Serial) {};
  Serial.println("Pmsx003");

  pms.begin();
  pms.waitForData(Pmsx003::wakeupTime);
  pms.write(Pmsx003::cmdModeActive);
  
  ads.begin();
  //connect to WiFi
  Serial.print("Connecting to: "); Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected\n");
  Blynk.begin(auth, ssid, pass);
}

auto lastRead = millis();

void loop(void)
{
  int16_t adc0, adc1, adc2, adc3;

  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  //membaca sensor MQ-135
  int co2now[10];                               //int array for co2 readings
  int co2raw = 0;                               //int for raw value of co2
  int co2ppm = 0;                               //int for calculated ppm
  int zzz = 0;


  // membaca sensor MQ-7 -> CO
  int sensorvalue = ads.readADC_SingleEnded(1); // membaca nilai ADC dari sensor
  float VRL = sensorvalue * 5.00 / 1024; // mengubah nilai ADC ( 0 - 1023 ) menjadi nilai voltase ( 0 - 5.00 volt )
  //Serial.print("VRL : "); Serial.print(VRL); Serial.println(" volt");

  float Rs = ( 5.00 * RL / VRL ) - RL;
  //Serial.print("Rs : "); Serial.print(Rs); Serial.println(" Ohm");

  float ppm = 100 * pow(Rs / Ro, -1.53); // ppm = 100 * ((rs/ro)^-1.53);
  COPPM = ppm-11;
  Serial.print("CO : "); Serial.print(ppm); Serial.println(" ppm");
  Serial.println();

  // Sensor MQ-135 -> CO2
  for (int x = 0; x < 10; x++) //samplpe co2 10x over 2 seconds
  {
    co2now[x] = ads.readADC_SingleEnded(2);
  }

  for (int x = 0; x < 10; x++) //add samples together
  {
    zzz = zzz + co2now[x];
  }
  co2raw = zzz / 10;                          //divide samples by 10
  co2ppm = co2raw - co2Zero;                 //get calculated ppm
  CO2PPM = co2ppm;
  Serial.print("Sensor MQ-7 ="); Serial.print(CO2PPM); Serial.println(" PPM");
  
  // membaca sensor debu
  const auto n = Pmsx003::Reserved;
  Pmsx003::pmsData data[n];
  Pmsx003::PmsStatus status = pms.read(data, n);
  switch (status) {
    case Pmsx003::OK:
    {
      Serial.println("_______");
      auto newRead = millis();
      Serial.print("Wait time ");
      Serial.println(newRead - lastRead);
      lastRead = newRead;

      for (int i = Pmsx003::PM1dot0; i < n; ++i) { 
        if(Pmsx003::dataNames[i] == "PM10."){
          pm10ug = data[i];
          Serial.print(pm10ug);
          Serial.print("\t");
          Serial.print(Pmsx003::dataNames[i]);Serial.print(" [");Serial.print(Pmsx003::metrics[i]);Serial.print("]");Serial.println();
        }
        Serial.println();
        if(Pmsx003::dataNames[i] == "PM2.5"){ 
          pm25ug = data[i];
          Serial.print(pm25ug);
          Serial.print("\t");
          Serial.print(Pmsx003::dataNames[i]);Serial.print(" [");Serial.print(Pmsx003::metrics[i]);Serial.print("]");Serial.println();
         }
      }
      break;
    }
    case Pmsx003::noData:
      break;
    default:
      Serial.println("_______");
      Serial.println(Pmsx003::errorMsg[status]);
  };

  Blynk.virtualWrite(V1, pm25ug);// PM 2.5
  Blynk.virtualWrite(V2, pm10ug);// PM 10    
  Blynk.virtualWrite(V3, COPPM); // CO    
  Blynk.virtualWrite(V4, CO2PPM);// CO2
  
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }
    // We now create a URI for the request
    String url = "/update";
    url += "?api_key=";
    url += APIKey;
    url += "&field1=";
    url += pm1;
    url += "&field2=";
    url += pm25ug;
    url += "&field3=";
    url += pm10ug;
    url += "&field4=";
    url += COPPM;//co    
    url += "&field5=";
    url += CO2PPM;//co2 
    
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }
    
  notificationSystem();
  delay(3000);
  Blynk.run();
  
}

void notificationSystem(){
  Blynk.setProperty(V4, "color", "#E1C340");//orange -> CO2
  Blynk.notify("CO2 : Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
}

void notificationTest(){
  int pm10priv=pm10ug;
  int pm25priv=pm25ug;
  int co2priv = CO2PPM;
  float copriv = COPPM;
  Serial.print("PM 10 : ");Serial.println(pm10priv);
  Serial.print("PM 25 : ");Serial.println(pm25priv);
  Serial.print("CO2 : ");Serial.println(co2priv);
  Serial.print("CO : ");Serial.println(copriv);
  if((pm10ug > 151 && pm10ug < 350) || (pm25ug > 66 && pm25ug < 150) || (COPPM > 35 && COPPM < 800) || (CO2PPM > 1000 && CO2PPM < 2000)){
      // 1 start
      if(copriv > 35 && copriv < 800){              // CO
        if(co2priv > 1000 && co2priv < 2000){       // CO, CO2
          if(pm10priv > 151 && pm10priv < 230){     // CO, CO2, PM10
            if(pm25priv > 66 && pm25priv < 150){    // CO, CO2, PM10, PM2.5
              Blynk.setProperty(V1, "color", "#E1C340");//orange -> pm2.5
              Blynk.setProperty(V2, "color", "#E1C340");//orange -> pm10
              Blynk.setProperty(V3, "color", "#E1C340");//orange -> CO
              Blynk.setProperty(V4, "color", "#E1C340");//orange -> CO2
              Blynk.notify("PM 10 & PM 2.5 : Unhealthy! \nCO : Max exposure for 8 hour work day. \nCO2 : Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
            }
            else{
              Blynk.setProperty(V2, "color", "#E1C340");//orange -> pm10
              Blynk.setProperty(V3, "color", "#E1C340");//orange -> CO
              Blynk.setProperty(V4, "color", "#E1C340");//orange -> CO2
              Blynk.notify("PM 10 : Unhealthy! \nCO : Max exposure for 8 hour work day. \nCO2 : Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
            }
          }
          else if(pm25priv > 66 && pm25priv < 150){ // CO, CO2 PM2.5
            Blynk.setProperty(V1, "color", "#E1C340");//orange -> pm2.5
            Blynk.setProperty(V3, "color", "#E1C340");//orange -> CO
            Blynk.setProperty(V4, "color", "#E1C340");//orange -> CO2

            Blynk.notify("PM 2.5 : Unhealthy! \nCO : Max exposure for 8 hour work day. \nCO2 : Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
          }
          else{                                       // CO, CO2
            Blynk.setProperty(V3, "color", "#E1C340");//orange -> CO
            Blynk.setProperty(V4, "color", "#E1C340");//orange -> CO2
            Blynk.notify("CO : Max exposure for 8 hour work day. \nCO2 : Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
          }
        }
        else 
        if(pm10priv > 151 && pm10priv < 230){                        // CO, PM10
            if(pm25priv > 66 && pm25priv < 150){                          // CO, PM10, PM2.5
              Blynk.setProperty(V1, "color", "#E1C340");//orange -> pm2.5
              Blynk.setProperty(V2, "color", "#E1C340");//orange -> pm10
             Blynk.notify("PM 10 & PM 2.5 : Unhealthy! \nCO : Max exposure for 8 hour work day. \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
            }
            else{                                                         
              Blynk.setProperty(V2, "color", "#E1C340");//orange -> pm10
              Blynk.setProperty(V3, "color", "#E1C340");//orange -> CO
              Blynk.notify("PM 10 : Unhealthy! \nCO : Max exposure for 8 hour work day. \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
            }
        }
        else 
        if(pm25priv > 66 && pm25priv < 150){
            Blynk.setProperty(V1, "color", "#E1C340");//orange -> pm2.5
            Blynk.setProperty(V3, "color", "#E1C340");//orange -> CO
            Blynk.notify("PM 2.5 : Unhealthy! \nCO : Max exposure for 8 hour work day. \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
        }
        else{
            Blynk.setProperty(V3, "color", "#E1C340");//orange -> CO
            Blynk.notify("CO : Max exposure for 8 hour work day. \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
        } 
      }
      else if(pm10priv > 151 && pm10priv < 230){     // CO, PM10
        if(pm25priv > 66 && pm25priv < 150){    // CO, PM10, PM2.5
           Blynk.setProperty(V1, "color", "#E1C340");//orange -> pm2.5
           Blynk.setProperty(V2, "color", "#E1C340");//orange -> pm10
           Blynk.notify("PM 10 & PM 2.5 : Unhealthy! \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
        }
        else{
           Blynk.setProperty(V2, "color", "#E1C340");//orange -> pm10
           Blynk.setProperty(V3, "color", "#E1C340");//orange -> CO
           Blynk.notify("PM 10 : Unhealthy! \nCO : Max exposure for 8 hour work day. \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
        }
      }
      else if(pm25priv > 66 && pm25priv < 150){    // CO, PM2.5
           Blynk.setProperty(V1, "color", "#E1C340");//orange -> pm2.5
           Blynk.setProperty(V3, "color", "#E1C340");//orange -> CO
           Blynk.notify("PM 2.5 : Unhealthy! \nCO : Max exposure for 8 hour work day. \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
      }
      else{
           Blynk.setProperty(V4, "color", "#E1C340");//orange -> CO2
           Blynk.notify("CO2 : Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system!");
        }
    }
    else if((pm10priv>=351 && pm10priv<=420) || (pm25priv>=151 && pm25priv<=250) || (copriv >=800 && copriv < 12800) || (co2priv >= 2000 && co2priv <5000)){
      // 1 start
      if(copriv > 800 && copriv < 12800){              // CO
        if(co2priv > 2000 && co2priv < 5000){       // CO, CO2
          if(pm10priv > 351 && pm10priv < 420){     // CO, CO2, PM10
            if(pm25priv > 151 && pm25priv < 250){    // CO, CO2, PM10, PM2.5
              Blynk.setProperty(V1, "color", "#F51720");//orange -> pm2.5
              Blynk.setProperty(V2, "color", "#F51720");//orange -> pm10
              Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
              Blynk.setProperty(V4, "color", "#F51720");//orange -> CO2
              Blynk.notify("PM 10 & PM 2.5 : Very Unhealthy! \nCO : Fatal within 2 - 3 hours. \nCO2 : Very Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
            }
            else{
              Blynk.setProperty(V2, "color", "#F51720");//orange -> pm10
              Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
              Blynk.setProperty(V4, "color", "#F51720");//orange -> CO2
              Blynk.notify("PM 10 : Very Unhealthy! \nCO : Fatal within 2 - 3 hours. \nCO2 : Very Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
            }
          }
          else if(pm25priv > 151 && pm25priv < 250){ // CO, CO2 PM2.5
            Blynk.setProperty(V1, "color", "#F51720");//orange -> pm2.5
            Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
            Blynk.setProperty(V4, "color", "#F51720");//orange -> CO2
            Blynk.notify("PM 2.5 : Very Unhealthy! \nCO : Fatal within 2 - 3 hours. \nCO2 : Very Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
          }
          else{                                       // CO, CO2
            Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
            Blynk.setProperty(V4, "color", "#F51720");//orange -> CO2
            Blynk.notify("CO : Fatal within 2 - 3 hours. \nCO2 : Very Unhealty! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
          }
        }                                                                 // CO, CO2
        else 
        if(pm10priv > 351 && pm10priv < 420){                        // CO, PM10
            if(pm25priv > 151 && pm25priv < 250){                          // CO, PM10, PM2.5
              Blynk.setProperty(V1, "color", "#F51720");//orange -> pm2.5
              Blynk.setProperty(V2, "color", "#F51720");//orange -> pm10
              Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
              Blynk.notify("PM 10 & PM 2.5 : Very Unhealthy! \nCO : Fatal within 2 - 3 hours.\nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
            else{                                                         // CO, PM10
              Blynk.setProperty(V2, "color", "#F51720");//orange -> pm10
              Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
              Blynk.notify("PM 10 : Very Unhealthy! \nCO : Fatal within 2 - 3 hours. \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
            }
        }
        else 
        if(pm25priv > 151 && pm25priv < 250){                     // CO, PM2.5
            Blynk.setProperty(V1, "color", "#F51720");//orange -> pm2.5
            Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
            Blynk.notify("PM 2.5 : Very Unhealthy! \nCO : Fatal within 2 - 3 hours. \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
        }
        else{                                                         // CO
            Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
            Blynk.notify("CO : Fatal within 2 - 3 hours. \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
        } 
      }
      // 1 end

      // 2 start
      else if(pm10priv > 351 && pm10priv < 420){     // CO, PM10
        if(pm25priv > 151 && pm25priv < 250){    // CO, PM10, PM2.5
           Blynk.setProperty(V1, "color", "#F51720");//orange -> pm2.5
           Blynk.setProperty(V2, "color", "#F51720");//orange -> pm10
           Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
           Blynk.notify("PM 10 & PM 2.5 : Very Unhealthy! \nCO : Fatal within 2 - 3 hours. \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
        }
        else{
           Blynk.setProperty(V2, "color", "#F51720");//orange -> pm10
           Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
           Blynk.notify("PM 10 : Very Unhealthy! \nCO : Fatal within 2 - 3 hours. \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
        }
      }
      // 2 end

      // 3 start
      else if(pm25priv > 151 && pm25priv < 250){    // CO, PM2.5
           Blynk.setProperty(V1, "color", "#F51720");//orange -> pm2.5
           Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
            Blynk.notify("PM 2.5 : Very Unhealthy! \nCO : Fatal within 2 - 3 hours. \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
      }
      // 4 start
      else{
        Blynk.setProperty(V3, "color", "#F51720");//orange -> CO
         Blynk.notify("CO : Fatal within 2 - 3 hours. \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
      }
      // 4 end
    }
    
    else if((pm10priv> 420) || (pm25priv > 250) || (copriv >= 12800) || (co2priv >= 40000)){
      // 1 start
      if(copriv >= 12800){              // CO
        if(co2priv >= 40000){       // CO, CO2
          if(pm10priv> 420){     // CO, CO2, PM10
            if(pm25priv > 250){    // CO, CO2, PM10, PM2.5
              Blynk.setProperty(V1, "color", "#0F0E0E");//orange -> pm2.5
              Blynk.setProperty(V2, "color", "#0F0E0E");//orange -> pm10
              Blynk.setProperty(V3, "color", "#0F0E0E");//orange -> CO
              Blynk.setProperty(V4, "color", "#0F0E0E");//orange -> CO2
              Blynk.notify("PM 10 & PM 2.5 : Hazardous & Extreme! \nCO : Hazardous & Extreme! \nCO2 : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
            }
            else{
              //Blynk.setProperty(V1, "color", "#50C878");//orange -> pm2.5
              Blynk.setProperty(V2, "color", "#0F0E0E");//orange -> pm10
              Blynk.setProperty(V3, "color", "#0F0E0E");//orange -> CO
              Blynk.setProperty(V4, "color", "#0F0E0E");//orange -> CO2
              Blynk.notify("PM 10 : Hazardous & Extreme! \nCO : Hazardous & Extreme! \nCO2 : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
            }
          }
          else if(pm25priv > 250){ // CO, CO2 PM2.5
            Blynk.setProperty(V1, "color", "#0F0E0E");//orange -> pm2.5
            Blynk.setProperty(V3, "color", "#0F0E0E");//orange -> CO
            Blynk.setProperty(V4, "color", "#0F0E0E");//orange -> CO2
            Blynk.notify("PPM 2.5 : Hazardous & Extreme! \nCO : Hazardous & Extreme! \nCO2 : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
          }
          else{                                       // CO, CO2
            Blynk.setProperty(V3, "color", "#0F0E0E");//orange -> CO
            Blynk.setProperty(V4, "color", "#0F0E0E");//orange -> CO2
            Blynk.notify("CO : Hazardous & Extreme! \nCO2 : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
          }
        }                                                                 // CO, CO2
        else 
        if(pm10priv > 420){                        // CO, PM10
            if(pm25priv > 250){                          // CO, PM10, PM2.5
              Blynk.setProperty(V1, "color", "#0F0E0E");//orange -> pm2.5
              Blynk.setProperty(V2, "color", "#0F0E0E");//orange -> pm10
              Blynk.setProperty(V3, "color", "#0F0E0E");//orange -> CO
              Blynk.notify("PM 10 & PM 2.5 : Hazardous & Extreme! \nCO : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
              Blynk.notify("PM 10, PM 2.5 & CO Tidak sehat!. Segera melakukan sirkulasi udara, seperti buka jendela atau nyalakan AC!");
            }
            else{                                                         // CO, PM10
              Blynk.setProperty(V2, "color", "#0F0E0E");//orange -> pm10
              Blynk.setProperty(V3, "color", "#0F0E0E");//orange -> CO
              Blynk.notify("PM 10 : Hazardous & Extreme! \nCO : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
            }
        }
        else 
        if(pm25priv > 250){                     // CO, PM2.5
            Blynk.setProperty(V1, "color", "#0F0E0E");//orange -> pm2.5
            Blynk.setProperty(V3, "color", "#0F0E0E");//orange -> CO
            Blynk.notify("PM 2.5 : Hazardous & Extreme! \nCO : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
        }
        else{                                                         // CO
            Blynk.setProperty(V4, "color", "#50C878");//orange -> CO2
            Blynk.notify("CO : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
        } 
      }
      // 1 end

      // 2 start
      else if(pm10priv > 420){     // CO, PM10
        if(pm25priv > 250){    // CO, PM10, PM2.5
           Blynk.setProperty(V1, "color", "#0F0E0E");//orange -> pm2.5
           Blynk.setProperty(V2, "color", "#0F0E0E");//orange -> pm10
           Blynk.setProperty(V3, "color", "#0F0E0E");//orange -> CO
           Blynk.notify("PM 10 & PM 2.5 : Hazardous & Extreme! \nCO : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
        }
        else{
           //Blynk.setProperty(V1, "color", "#50C878");//orange -> pm2.5
           Blynk.setProperty(V2, "color", "#0F0E0E");//black -> pm10
           Blynk.setProperty(V3, "color", "#0F0E0E");//black -> CO
           Blynk.notify("PM 10 : Hazardous & Extreme! \nCO : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
           Blynk.notify("PM 10 & CO Tidak sehat!. Segera melakukan sirkulasi udara, seperti buka jendela atau nyalakan AC!");
        }
      }
      // 2 end

      // 3 start
      else if(pm25priv > 250){    // CO, PM2.5
           Blynk.setProperty(V1, "color", "#0F0E0E");//black -> pm2.5
           Blynk.setProperty(V3, "color", "#0F0E0E");//black -> CO
           Blynk.notify("PM 2.5 : Hazardous & Extreme! \nCO : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
      }
      // 3 end

      // 4 start
      else{
        Blynk.setProperty(V3, "color", "#0F0E0E");//black -> CO
        Blynk.notify("CO : Hazardous & Extreme! \nNote : Please choosing the recirculation setting of the car’s ventilation system or get out from the car!");
      }
      // 4 end
    }
    else{
       Blynk.setProperty(V1, "color", "#50C878");//green
       Blynk.setProperty(V2, "color", "#50C878");//green
       Blynk.setProperty(V3, "color", "#50C878");//green
       Blynk.setProperty(V4, "color", "#50C878");//green
    }
 }
