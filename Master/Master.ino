 #include <SoftwareSerial.h>
#include <Printers.h>
#include <XBee.h>//Software Serial library




XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
Rx64Response rx64 = Rx64Response();
Rx16Response rx16 = Rx16Response();

//int statusLed = 11;
//int errorLed = 12;
int dataLed = 13;

uint8_t option = 0;
uint8_t data = 0;
uint8_t data1 = 0;
uint8_t data2 = 0;
uint8_t data3 = 0;
uint8_t data4 = 0;

void flashLed(int pin, int times, int wait) {
    
    for (int i = 0; i < times; i++) {
      digitalWrite(pin, HIGH);
      delay(wait);
      digitalWrite(pin, LOW);
      
      if (i + 1 < times) {
        delay(wait);
      }
    }
}






SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "Android";       // WiFi SSID
String myPWD = "123456qwe"; // WiFi Password
String myAPI = "TZ5FT0RT841SJI6S";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1"; 
float sendVal;
int value = 0;


void setup()
{
  pinMode(dataLed,  OUTPUT);
  Serial.begin(9600);
  xbee.setSerial(Serial);
  
  espSerial.begin(115200);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  /*while(!esp.find("OK")) 
  {          
      //Wait for connection
  }*/
  delay(1000);
  
}

  void loop()
  {
    /* Here, I'm using the function random(range) to send a random value to the 
     ThingSpeak API. You can change this value to any sensor data
     so that the API will show the sensor data  
    */
    //int aR=analogRead(A0);
    //sendVal = aR*(5/1023); // Send a random number between 1 and 1000
      //value = analogRead(A0);
    // sendVal = value * (5.0/1024);

    int sflag1=0,sflag2=0;

while(true){


xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
      
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE || xbee.getResponse().getApiId() == RX_64_RESPONSE) {
        // got a rx packet
        
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
                xbee.getResponse().getRx16Response(rx16);
          option = rx16.getOption();
          data = rx16.getData(0);
          //Serial.println("1616");
          if(data==255){
            sflag1=1;
            Serial.print("Slave 1 : ");
            data1=rx16.getData(1);
            data2=rx16.getData(2);
            Serial.println(data1);

 /*            sendVal=101;
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(data1);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sendVal);
    Serial.println(sendData); 
    espData("AT+CIPCLOSE=0",1000,DEBUG);
   // delay(16000);
*/
            
            }

             if(data==254){
              sflag2=1;
            Serial.print("Slave 2 : ");
            data3=rx16.getData(1);
            data4=rx16.getData(2);
            Serial.println(data3);
            }
          
        } else {
                xbee.getResponse().getRx64Response(rx64);
          option = rx64.getOption();
          data = rx64.getData(0);
          Serial.println("6464");
        }
        
        // TODO check option, rssi bytes    
        flashLed(dataLed, 1, 10);
        
        // set dataLed PWM to value of the first byte in the data
        //analogWrite(dataLed, data);
        //Serial.println(data);
      } else {
        // not something we were expecting
        flashLed(dataLed, 1, 25);    
      }
    } else if (xbee.getResponse().isError()) {
      //nss.print("Error reading packet.  Error code: ");  
      //nss.println(xbee.getResponse().getErrorCode());
      // or flash error led
    } 


if(sflag1==1&&sflag2==1){break;}

} //end while
     //sendVal=101;
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(data1)+"&field2="+String(data2)+"&field3="+String(data3)+"&field4="+String(data4);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
   // Serial.println(sendVal);
    Serial.println(sendData); 
    espData("AT+CIPCLOSE=0",1000,DEBUG);


   
    delay(60000);
  }



  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
   // Serial.print(response);
  }
  return response;
}
