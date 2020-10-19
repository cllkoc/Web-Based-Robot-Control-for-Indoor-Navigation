/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <string.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
 
WiFiMulti wifiMulti;
#define motor1_1 4
#define motor1_2 21
#define motor2_1 2
#define motor2_2 15
#define pwm 5
#define pwm2 12
#define RXD2 16
#define TXD2 17
#define motor1_hiz 180
#define motor2_hiz 180
#define yon_hiz 175
#define max_ileri 4000
DynamicJsonDocument doc(200);
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE externalMux = portMUX_INITIALIZER_UNLOCKED;
hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
const int ping = 1000;
char sent[100]; 
int newPointX;
int newPointY;
int jobDone=1;
int job = 0;
boolean kontrol_motor = 1;
volatile int op=6; //1+0-
volatile int angle=0; //1+0-
volatile int angle1=0; //1+0-
volatile int tik=0;
volatile int tik2=0;
volatile int pulse=motor1_hiz+7;
uint32_t  start_ileri;
uint32_t  stop_ileri;
int Ypulse=yon_hiz;
volatile int pulse2=motor2_hiz;
volatile int Gpulse=motor1_hiz+1;
volatile int Gpulse2=motor2_hiz;
int eski_yon;
int yeni_yon;
volatile int pos;
volatile int oldpos;
volatile int donaci;
volatile int olddonaci;
boolean Mode=0; // 0 manuel 1 otomatik.
int volt=50;
float batteryLevel = 0;
String payload;
uint32_t zaman = 0;
uint32_t gec = 0;
float hypoNew=0;
float hypoOld=0;
float Xnew=0;
float Xold=0;
float Ynew=0;
float RAngle=0;
float Yold=0;
float posy=0;
boolean cont_aci=0;
int x;
int y;
int k;
int r=0;
float batSum=0;
int distance;
int jsonSize;
const int freq = 22000;
const int ledChannel = 0;const int ledChannel2 = 1;
const int resolution = 8;
volatile float ang;
int conn=1;
String inString = "";
HTTPClient http;
int httpCode;
int positionsX[5] = {};
int positionsY[5] = {};
int oldPositionsX[5] = {};
int oldPositionsY[5] = {};
boolean busyFlag=0;
void IRAM_ATTR isr2() {
   //motor1
    portENTER_CRITICAL_ISR(&externalMux);
     if(op==1)
     {
      if(tik2%20==0){
      if(pos < 190 && pos > 170 )
      {
        
      }else{
      if(donaci > olddonaci)
      {
       pulse2--;     
       pulse++;
      }else if(donaci < olddonaci)
      {
       pulse--;
       pulse2++;  
      }else{
       
        }
      }
     tik2++;
     
     }
     }
     else if(op==0)
     {
     if((tik2*-1)%20==0){
      if(pos < 355 && pos > 5){
      if(donaci > olddonaci)
       {
        Gpulse2++ ;     
        Gpulse--;
        }else if(donaci < olddonaci)
        {
        Gpulse++;
        Gpulse2--;  
        }else{}
        
     }else{
      Gpulse = pulse;
      Gpulse2= pulse2;
      }
     }
     tik2--;
     }
     else
     {
      
     }
     if(pulse < (motor1_hiz - 20) || pulse > (motor1_hiz + 20))
         pulse = motor1_hiz+7;
     if(pulse2 <(motor2_hiz - 20) || pulse2 >(motor2_hiz + 20))
         pulse2 = motor2_hiz;
        portEXIT_CRITICAL_ISR(&externalMux);
}



void IRAM_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  dur();op=6;
  portEXIT_CRITICAL_ISR(&timerMux);
}



void setup() {
    //pinMode(2,OUTPUT);
    
    pinMode(33,INPUT);
    pinMode(23, INPUT_PULLUP);
    pinMode(18,OUTPUT);
    pinMode(27, INPUT_PULLUP);
    digitalWrite(18,HIGH);
    pinMode(motor1_1,OUTPUT);
    pinMode(motor1_2,OUTPUT);
    pinMode(motor2_1,OUTPUT);
    pinMode(motor2_2,OUTPUT);
    ledcSetup(ledChannel, freq, resolution);
    ledcAttachPin(pwm, ledChannel);
    ledcAttachPin(pwm2, ledChannel2);
    ledcWrite(ledChannel, 0);
    ledcWrite(ledChannel2, 0);
    digitalWrite(motor1_1,LOW);
    digitalWrite(motor1_2,LOW);
    digitalWrite(motor2_1,LOW);
    digitalWrite(motor2_2,LOW);
    wifiMulti.addAP("ZyXEL NBG-418N v2","PJMME77744");
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer,ping*1000, true);
    attachInterrupt(digitalPinToInterrupt(23), isr2, FALLING);
    tik2 =0;
    batteryLevel = analogRead(32)*0.00088;
    batteryLevel =(batteryLevel - 1.535)*239.23;
    volt = (int)batteryLevel;
    }

 

void loop() {
    /////
    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {
         portENTER_CRITICAL(&externalMux); 
         batteryLevel = analogRead(32)*0.00088;
         batSum = batSum + (batteryLevel - 1.535)*239.23;
         r++;
         if(r==100)
         { 
          volt = (int)(batSum/100);
          r = 0; batSum=0;
         }
         if(volt < 2)
              {
                esp_deep_sleep_start(); //Robotu uyut. 
              }
         if(digitalRead(33))
           volt = volt + 100;
         readPosition();
         readMPU(); 
         sprintf(sent,"http://cllkoc.com/fetch.php?x=%d&y=%d&p=%d&a=%d&v=%d&f=%d",x,y,gec,pos,volt,busyFlag);
         portEXIT_CRITICAL(&externalMux); 
         http.setReuse(true);
         http.begin(sent); //HTTP
         timerAlarmEnable(timer);
         zaman = millis();
       // USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        
        httpCode = http.GET();
        if(httpCode > 0) {
         //file found at server.
         if(httpCode == HTTP_CODE_OK) {
                timerAlarmDisable(timer);
                payload = "";
                payload =  http.getString();
                //Serial.print(payload);
               
                if(payload.length() > 12){ Mode = 1;}else{Mode=0;}
                 if(Mode==1 && !(payload.substring(7,11)=="null"))
                 {   Ypulse = 170;
                     
                     DeserializationError error = deserializeJson(doc, payload);
                    // Test if parsing succeeds.
                     if (error) {
                    // Serial.print(F("deserializeJson() failed: "));
                    // Serial.println(error.c_str());
                    
                     }else{ 
                           jsonSize = doc["x"].size();
                           for(k=0;k<jsonSize;k++)
                           {
                             positionsX[k] = (int)doc["x"][k];
                             positionsY[k] = (int)doc["y"][k];
                           }
                           if(!comparePositions())
                           {
                             job=0; //New job.
                           }
                        
                     }
                     if(job==0)
                      { 
                       busyFlag=1;
                       newPointX = positionsX[jobDone]-positionsX[jobDone-1]; 
                      newPointY = positionsY[jobDone]-positionsY[jobDone-1];
                      int region = findRegion(newPointX,newPointY);    
                      newPointX = abs(newPointX); newPointY = abs(newPointY);
                      float RequestAngle; float hypo = sqrt(sq(newPointX)+sq(newPointY)); int request;
                      if(region==0) RequestAngle = 0;
                      if(region==1) RequestAngle = 57.29*asin(newPointX/hypo);
                      if(region==2) RequestAngle = 359-(57.29*asin(newPointX/hypo));
                      if(region==3) RequestAngle = 359-90-57.29*acos(newPointX/hypo);
                      if(region==4) RequestAngle = 57.29*acos(newPointX/hypo)+90;
                      if(region==5) RequestAngle = 0;
                      if(region==6) RequestAngle = 180;
                      if(region==7) RequestAngle = 90;
                      if(region==8) RequestAngle = 270;
                      request = (int)RequestAngle;
                      Serial.println(request);
                      updateAngle(request,(int)hypo);
                    }
                 
                   }
                  if(Mode==0) // Manuel
                 { 
                  Ypulse = yon_hiz;
                  if(bul(payload,'1'))
                  {
                   dur();busyFlag = 1;
                  
                  }
                  if(bul(payload,'2'))
                  {
                  ileri();busyFlag = 1;
                  
                  }
                  else if(bul(payload,'3'))
                  {geri();busyFlag = 1;
                  
                  }
                  else if(bul(payload,'4'))
                  {
                  sag();busyFlag = 1;
                  
                  }
                  else if(bul(payload,'5'))
                  {sol();busyFlag = 1;}
                  else
                  {dur();busyFlag = 0;}
               }
           }else{dur();}
        }
        gec = millis()- zaman;
      
      
        // http.end();
        
         
       }
      
}

int bul(String x , char y)
{
  for(int i=0;i<sizeof(x)+1;i++)
  {
    if(x[i]==y)  
     {return 1;break;}
  }  
  return 0;
  
}
void ileri()
{   portENTER_CRITICAL(&externalMux); 
    op=1;
    digitalWrite(motor1_1,LOW);
    digitalWrite(motor1_2,HIGH);
    digitalWrite(motor2_1,LOW);
    digitalWrite(motor2_2,HIGH);
    ledcWrite(ledChannel, pulse);
    ledcWrite(ledChannel2, pulse2);
    olddonaci = donaci;
    portEXIT_CRITICAL(&externalMux); 
}
void geri()
{   portENTER_CRITICAL(&externalMux);  
    op=0;
    digitalWrite(motor1_1,HIGH);
    digitalWrite(motor1_2,LOW);
    digitalWrite(motor2_1,HIGH);
    digitalWrite(motor2_2,LOW);
    ledcWrite(ledChannel, Gpulse);
    ledcWrite(ledChannel2, Gpulse2);
    olddonaci = donaci;
    portEXIT_CRITICAL(&externalMux); 
    
}
void dur()
{   
    portENTER_CRITICAL(&externalMux); 
    digitalWrite(motor1_1,LOW);
    digitalWrite(motor1_2,LOW);
    digitalWrite(motor2_1,LOW);
    digitalWrite(motor2_2,LOW);
    op=6;
    distance = 0;
    hypoNew = 0;
    hypoOld = 0;
    portEXIT_CRITICAL(&externalMux); 
}
void sag()
{   
    portENTER_CRITICAL(&externalMux); 
    digitalWrite(motor1_1,HIGH);
    digitalWrite(motor1_2,LOW);
    digitalWrite(motor2_1,LOW);
    digitalWrite(motor2_2,HIGH);
    ledcWrite(ledChannel, Ypulse+7);
    ledcWrite(ledChannel2, Ypulse);
    op=3;
    portEXIT_CRITICAL(&externalMux); 
   
}
void sol()
{   
    portENTER_CRITICAL(&externalMux); 
    digitalWrite(motor1_1,LOW);
    digitalWrite(motor1_2,HIGH);
    digitalWrite(motor2_1,HIGH);
    digitalWrite(motor2_2,LOW);
    ledcWrite(ledChannel, Ypulse+7);
    ledcWrite(ledChannel2, Ypulse);
    op=4;
    portEXIT_CRITICAL(&externalMux); 
}
int findRegion(int p1 , int p2){
  if(p1==0 && p2>0)
     return 5;
  else if(p1==0 && p2<0)
     return 6;
  else if(p2==0 && p1>0)
     return 7;
  else if(p2==0 && p1<0)
     return 8;
  else if(p1>0 && p2>0)
     return 1; 
  else if(p1<0 && p2 > 0)
     return 2;
  else if(p1<0 && p2 < 0)
     return 3;
  else if(p1 > 0 && p2 < 0)
     return 4;
  
}
void updateAngle(int angle,int todrive)
{
   dur(); int dene; int angle_180 = 0;
   dene = 0;
   if(angle < 200 && angle > 150)
       { 
        while(1)
       {
       if(angle < pos)
         sol();
       else if(angle > pos)
         sag();
       else 
         dur();
       delay(5);
      readMPU();
      dene = dene + 1;  
      if(dene == 600)
        break;
      }}
   else{
   if(angle > 180)
           angle_180 = angle - 360;
         else
           angle_180 = angle;   
   while(1)
   {
     if(angle_180 < donaci)
         sol();
     else if(angle_180 > donaci)
         sag();
     else 
         dur();
     delay(5);
      readMPU();
      dene = dene + 1;  
      if(dene == 600)
        break;
   }
   }
   
   dur();
   delay(1000);
   readMPU();
  
   start_ileri = millis();
   while(distance < todrive)
   { ileri();
     stop_ileri = millis();
     if(stop_ileri - start_ileri > max_ileri)
      break;
     readPosition();
     readMPU();
     delay(20);
   }
   dur(); 
  
   if(jobDone==jsonSize-1)
   {
    job=1; jobDone=0;busyFlag=0;
    for(int h=0;h<jsonSize;h++)
    {
       oldPositionsX[h] = positionsX[h];
       oldPositionsY[h] = positionsY[h];  
    }
    
   }
   jobDone++;
}
void readMPU()
{     
       digitalWrite(18,LOW);
       digitalWrite(18,HIGH);
       while(Serial2.available()>0)
       {
         int inChar = Serial2.read();
         if (isDigit(inChar)) {
         //convert the incoming byte to a char and add it to the string:
         inString += (char)inChar;
         }
         if (inChar == '\n') {
         pos = inString.toInt();
         inString = "";
         }
        }   
        if(pos > 180)
           donaci = pos - 360;
         else
           donaci = pos;       
}
void readPosition(){
         if(op==1)
         {ang = tik2*1.8;}
         else if(op==0)
         {ang = tik2*0.6;}
         else{}
         tik2=0;
         hypoNew = hypoOld + ang;
         Xnew = Xold + ang * sin(pos*0.0175); x = (int)Xnew;
         Ynew = Yold + ang * cos(pos*0.0175); y = (int)Ynew; distance = (int)hypoNew;
         Xold=Xnew;Yold=Ynew; hypoOld = hypoNew; 
         ang=0.0;
  }
int comparePositions()
{
   int cmp = 1;
  for(int j=0;j<5;j++)
  {
     if(oldPositionsX[j]!=positionsX[j])
        cmp = 0;
     if(oldPositionsY[j]!=positionsY[j])  
        cmp = 0;
  }
 return cmp;
}
