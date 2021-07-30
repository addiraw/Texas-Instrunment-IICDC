// code to check A(G working or not )

#include <stdio.h>
#include <string.h>
#include <SoftwareSerial.h>
 
#define DEBUG true
int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;
 
bool ModuleState=false;
SoftwareSerial Serial1(2, 3); // RX, TX
 
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
    
    Serial.begin(115200);
    //while (!Serial)
    {
        ; // wait for serial port to connect
    }
 
        ModuleState=moduleStateCheck();
        if(ModuleState==false)//if it's off, turn on it.
        {
            digitalWrite(PWR_KEY, LOW);
            delay(3000);
            digitalWrite(PWR_KEY, HIGH);
            delay(10000);
      Serial.println("Now turnning the A9/A9G on.");
        }
 
    //sendData("AT+CCID", 3000, DEBUG);
    //sendData("AT+CREG?", 3000, DEBUG);
    //sendData("AT+CGATT=1", 1000, DEBUG);
    //sendData("AT+CGACT=1,1", 1000, DEBUG);
    //sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"", 1000, DEBUG);
    
    //sendData("AT+CIPSTART=\"TCP\",\"www.mirocast.com\",80", 2000, DEBUG);
    Serial.println("Maduino A9/A9G Test Begin!");
}
 
void loop()
{
  while (Serial1.available() > 0) {
    Serial.write(Serial1.read());
    yield();
  }
  while (Serial.available() > 0) {
    Serial1.write(Serial.read());
    yield();
  }
}
 
bool moduleStateCheck()
{
    int i = 0;
    bool moduleState=false;
    for (i = 0; i < 5; i++)
    {
        String msg = String("");
        msg = sendData("AT", 1000, DEBUG);
        if (msg.indexOf("OK") >= 0)
        {
            Serial.println("A9/A9G Module had turned on.");
                moduleState=true;
            return moduleState;
        }
        delay(1000);
    }
    return moduleState;
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
        Serial.print(response);
    }
    return response;
}