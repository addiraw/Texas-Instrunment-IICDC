#include <DHT.h>
#include <DHT_U.h>

#include <stdio.h>
#include <string.h>
 
//*******for senser **********
float humidity;//humidity
float temperature;//temperature
 
#define DHTPIN 3    //D3
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
 
void(* resetFunc) (void) = 0; //declare reset function at address 0
 
#define DEBUG true
int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;
 
bool ModuleState=false;
unsigned long timeCount;
 
//**********api write key******************
const String apiKey="GW3P9JW5G76ZCVN6";
 
String  showString="";
 
void setup()
{
 
    pinMode(PWR_KEY, OUTPUT);
    pinMode(RST_KEY, OUTPUT);
    pinMode(LOW_PWR_KEY, OUTPUT);
    digitalWrite(RST_KEY, LOW);
    digitalWrite(LOW_PWR_KEY, HIGH);
    digitalWrite(PWR_KEY, HIGH);
    Serial1.begin(115200);
    digitalWrite(PWR_KEY, LOW);
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(10000);
    
    SerialUSB.begin(115200);
    SerialUSB.println("Hello!");
    SerialUSB.println("There need some time to initialize!");
    delay(16000);
 
    dht.begin();//sensor begin
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
 
    Serial1.begin(115200);
    
    //while (!SerialUSB)
    {
        ;// wait for serial port to connect
    }
 
 
    ModuleState=moduleStateCheck();
    if(ModuleState==false)//if it's off, turn on it.
    {
      digitalWrite(PWR_KEY, LOW);
      delay(3000);
      digitalWrite(PWR_KEY, HIGH);
      delay(10000);
      SerialUSB.println("Now turnning the A9/A9G on.");
    }
 
#if 1
    //GPS test
    sendData("AT+GPS=0", 1000, DEBUG);//1: turn on GPS  0:Turn off GPS
    
    //GSM Test
    sendData("AT+CCID", 3000, DEBUG);  //get SIM 
    sendData("AT+CREG?", 3000, DEBUG);
    sendData("AT+CGATT=1", 3000, DEBUG);
    sendData("AT+CGACT=1,1", 3000, DEBUG);
    sendData("AT+CGDCONT=1,\"IP\",\"ntnet\"", 3000, DEBUG);
 
 
    sendData("AT+CSQ", 1000, DEBUG);  //
    //sendData("AT+CIPSTART=\"TCP\",\"www.how2electronics.com\",80", 2000, DEBUG);//
 
 
    String cmdString= "AT+HTTPGET=\"http://api.thingspeak.com/update.json?api_key=" + apiKey + "&field1=" + (String)temperature +"&field2=" + (String)humidity + "&field3=0.0&field4=0.0" + "\"";
    sendData(cmdString,5000,DEBUG);
#endif
  
  timeCount=millis();
  SerialUSB.println("A9/A9G Test Begin!");
}
 
void loop()
{
 
#if 1
  if(millis()-timeCount>30000) //60000ms
  {
    
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    
    SerialUSB.println("humidity is " + (String)humidity);
    SerialUSB.println("temperature is " + (String)temperature);
 
  //if (!isnan(humidity) && !isnan(temperature)) 
  {
    //SerialUSB.println("Read from DHT sensor!");
    showString = "Humidity:" + (String)humidity + "%" + "\r\n" +"Temperature:"+(String)temperature + "C"; 
  }
    SerialUSB.println(showString);
 
    sendData("AT+CGATT=1", 1000, DEBUG);
    sendData("AT+CGACT=1,1", 1000, DEBUG);
    sendData("AT+CGDCONT=1,\"IP\",\"Ntnet\"", 1000, DEBUG);
 
  String cmdString= "AT+HTTPGET=\"http://api.thingspeak.com/update.json?api_key=" + apiKey + "&field1=" + (String)temperature +"&field2=" + (String)humidity + "&field3=0.0&field4=0.0" + "\"";
   SerialUSB.println(cmdString);
    sendData(cmdString,5000,DEBUG);
    delay(5000);
    timeCount=millis();//refresh
  }
#endif
 
  while (Serial1.available() > 0) {
    String cstring = Serial1.readString();
    SerialUSB.print(cstring);//SerialUSB.write(Serial1.read());
    yield();
 
  }
  while (SerialUSB.available() > 0) {
    Serial1.write(SerialUSB.read());
    yield();
  }
}
 
bool moduleStateCheck()
{
    int i = 0;
    bool state=false;
    for (i = 0; i < 10; i++)
    {
        String msg = String("");
        msg = sendData("AT", 1000, DEBUG);
        if (msg.indexOf("OK") >= 0)
        {
            SerialUSB.println("A9/A9G Module had turned on.");
                state=true;
            return state;
        }
        delay(500);
    }
    return state;
}
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    Serial1.println(command);
    long int time = millis();
    while ((time + timeout) > millis())
    {
        while (Serial1.available())
        {
            char c = Serial1.read();
            response += c;
        }
    }
    if (debug)
    {
        SerialUSB.print(response);
    }
    return response;
}
