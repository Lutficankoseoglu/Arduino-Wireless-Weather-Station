#include <SoftwareSerial.h>     //this library is for wi-fi module's stability.
#include <Adafruit_BMP085.h>  //defines BMP180 sensor library.
#include <LCD_I2C.h>    //defines LCD panel's I2C Module library.
#include <dht11.h>     //defines DHT11 sensor library.
#include <Wire.h>     //defines other I2C modules library.

int E=22; int D=23; int C=24; int P=25;   //These pins are 7-segment display's pins.
int B=26; int A=27; int F=28; int G=29;


String agAdi = "WifiID";                //My Wi-Fi id and password.
String agSifresi = "WifiPassword";

int rxPin = 12;             //These pins are Wi-Fi Module's communication pins.
int txPin = 13;  

String ip = "184.106.153.149";   //This is Thingspeak's ip adress.

SoftwareSerial esp(rxPin, txPin);       //starts our serial commminication for wi-fi module.

 
LCD_I2C lcd(0x3F);        //defines our LCD panel as "lcd".
Adafruit_BMP085 bmp;      //defines BMP180 sensor as "bmp".
dht11 DHT11_sensor;       //defines DHT11 sensor as "DHT11_sensor".
         
int gazdeger;        //defines MQ135 sensor's variable as "gazdeger".                 

int sayac = 0;            //defines an int value for button counter.
int butonDurumu = 0;      //this definition is equates the button status to 0.
int sondurum = 0;         //this definition is for troubleshooting a possible error.


const int buton = 7;          //defines button's pin as "7".
const int capteur_D = 4;      //defines Rain sensor's digital signal pin as "4".
const int capteur_A = A0;     //defines Rain sensor's analog signal pin as "A0".
int DHT11_pin=2;              //defines DHT11 sensor's data pin as "2".

int val_analogique;           //defines an int value for rain sensor.
float sicaklik, nem;          //defines 2 float values for DHT11 sensor.





void setup() {
  Serial.begin(9600);          //it starts serial port communication.
  

  pinMode(E, OUTPUT); pinMode(D, OUTPUT); pinMode(C, OUTPUT);       //define 7-segment display's pins as output.
  pinMode(A, OUTPUT); pinMode(B, OUTPUT); pinMode(F, OUTPUT); 
  pinMode(G, OUTPUT); pinMode(P, OUTPUT); 
  
 
  pinMode(8, OUTPUT);        //defines red led.
  pinMode(9, OUTPUT);         //defines red led.(RGB)
  pinMode(10, OUTPUT);         //defines green led.(RGB)
  pinMode(11, OUTPUT);        //defines blue led.(RGB)
  
  
  pinMode(capteur_D, INPUT);        //defines MQ135 sensor's digital signal pin as "INPUT".
  pinMode(capteur_A, INPUT);        //defines MQ135 sensor's analog signal pin as "INPUT". 

  lcd.begin();                      //it prepares the LCD screen for data exchange.
  lcd.backlight();                  //it turns on the backlight on the LCD screen.

  pinMode(buton, INPUT);            //defines Buton as "INPUT".

  if (!bmp.begin()) {                         //it checks the BMP180 sensor.  
  Serial.println("BMP180 is not accessible\n");   //if the sensor is not working, serial port says "BMP180 is not accessible".
  while(1);
  }

   espSetup();

}
  

void loop() {

  ledInfo();    //This function is gives information with RGB and red led.
  espLoop();

  Serial.print("Temperature from BMP180 sens = ");   //Serial port screen, prints the Temperature value as "*C".
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");          //"*C".
  
  Serial.print("Pressure: ");           //Serial port screen, prints the Pressure value as "pascal".
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");          //"Pascal".   
  
  Serial.print("Altitude: ");           //Serial port screen, prints the altitude value as; 
  Serial.print(bmp.readAltitude());
  Serial.print(" meters, ");          //"meters".
  Serial.print(bmp.readAltitude()*3.28084,0);
  Serial.println(" feet");            //"feet".

  Serial.print("Pressure at sealevel (calculated) = ");    //Serial port screen, prints the Pressure value at the sea level as "pascal"
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");              //"Pascal".

  Serial.print("Real altitude = ");         //Serial port screen, prints the more precise measurement of altitude value as "meters".
  Serial.print(bmp.readAltitude(101325));     //The standard air pressure is 101.325 kPa at the sea level. We can get a more precise measurement of altitude with this value.
  Serial.println(" meters");          //"meters".
    


  
  gazdeger = analogRead(A1);                //MQ135 sensor's variable reads "Gas and Air Quality Value" from sensor.
  
 val_analogique=analogRead(capteur_A);      //Rain sensor's variable reads the analog variables from sensor.   

float chk = DHT11_sensor.read(DHT11_pin);   //DHT11 sensor's variable reads the "Temperature and Humidity Values" from sensor.





 butonDurumu = digitalRead(buton);          //Button status is reads the values from button.
 if (butonDurumu != sondurum) {             //if Button status is different from 0, (so it means we pushed to button), the counter will increase.
if (butonDurumu == HIGH) {                  //it checks the button status.
sayac++;                                    //counter increases.
  } 
} 
sondurum = butonDurumu;                     //we equates button status to 0 for avoid any errors.



switch(sayac) {                 //when we push the button, the LCD Screen will change.
    case 1:                  //when the counter equals the 1, we will see DHT11's values.
    {
      lcd.clear();       //it clears the LCD panel.
      dht();             //it runs the DHT11's function.

      digitalWrite(E,HIGH);       //7-segment display shows "1".
      digitalWrite(D,HIGH); 
      digitalWrite(C,LOW); 
      digitalWrite(B,LOW); 
      digitalWrite(A,HIGH); 
      digitalWrite(F,HIGH); 
      digitalWrite(G,HIGH);
      
    }
      break;
      
    case 2:                  //when the counter equals the 2, we will see rain sensor's values.
      {
        lcd.clear();     //it clears the LCD panel.
        rain();          //it runs the rain sensor's function.

        digitalWrite(E,LOW);        //7-segment display shows "2".
        digitalWrite(D,LOW); 
        digitalWrite(C,HIGH); 
        digitalWrite(B,LOW); 
        digitalWrite(A,LOW); 
        digitalWrite(F,HIGH); 
        digitalWrite(G,LOW); 
        
      }
      break;
    
    case 3:                  //when the counter equals the 3, we will see MQ135's values.
      {
        lcd.clear();     //it clears the LCD panel.
        gaz();           //it runs the MQ135's function.

        digitalWrite(E,HIGH);       //7-segment display shows "3".
        digitalWrite(D,LOW); 
        digitalWrite(C,LOW); 
        digitalWrite(B,LOW); 
        digitalWrite(A,LOW); 
        digitalWrite(F,HIGH); 
        digitalWrite(G,LOW); 
        
      }
      break;
    
    case 4:                  //when the counter equals the 4, we will see BMP180's values.
      {
        lcd.clear();     //it clears the LCD panel.
        bmp180();        //it runs the BMP180's function.

        digitalWrite(E,HIGH);       //7-segment display shows "4".
        digitalWrite(D,HIGH); 
        digitalWrite(C,LOW); 
        digitalWrite(B,LOW); 
        digitalWrite(A,HIGH); 
        digitalWrite(F,LOW); 
        digitalWrite(G,LOW); 
        
      }
      break;
    
    case 0:                 //when the counter equals the 0 (so when we started the system for first time), we will see "Welcome Screen".
      {
        lcd.clear();                          //it clears the LCD panel.
        lcd.setCursor(3,0);                   //it sets the cursor as the line is 0(first line)(in the computer language, 0's means is 1.), the column is 3.
        lcd.print("Welcome to");              //it prints "Welcome to" to the LCD panel.
        lcd.setCursor(0,1);                   //it sets the cursor as the line is 1(second line)(in the computer language, 1's means is 2.), the column is 0.
        lcd.print("Weather Station");         //it prints "Weather Station" to the LCD panel.

        digitalWrite(E,LOW);          //7-segment display shows "0".
        digitalWrite(D,LOW); 
        digitalWrite(C,LOW); 
        digitalWrite(B,LOW); 
        digitalWrite(A,LOW); 
        digitalWrite(F,LOW); 
        digitalWrite(G,HIGH); 
        
      }
      break;
    }



if (sayac > 4){         //if the counter will bigger than 4, (for example 5) it equates the counter to 1. So, we will see the DHT11's values on the LCD panel again.
    sayac = 1;
  }
 
 
Serial.print("Rain Sensor :");                          //Serial port screen will print the Rain sensor's variable.
Serial.println(val_analogique); 
  Serial.println("");                         //it is like as "\n".
Serial.print("Humidity :");                                  //Serial port screen will print the DHT11 sensor's humidity variable.
Serial.print((float)DHT11_sensor.humidity);
  Serial.println("");                         //it is like as "\n". 
Serial.print("Temperature :");
Serial.print((float)DHT11_sensor.temperature);          //Serial port screen will print the DHT11 sensor's temperature variable.    
  Serial.println(""); 
Serial.println("Air Quality :");                         //Serial port screen will print the MQ135 sensor's variable.
Serial.println(gazdeger);
Serial.println("Counter :");                              //Serial port screen will print the counter's variable.
Serial.println(sayac);
  Serial.println(""); 
  Serial.println(""); 

 delay(600);                //the system will wait 0,8 seconds.

}






void rain(){              //it's a function for Rain Sensor.

  val_analogique=analogRead(capteur_A);             //Rain sensor's variable reads the analog variables from sensor.
if(val_analogique > 500)                           //if the analog variable is bigger than 500; (it means the weather is dry)
  {     
     lcd.setCursor(0,0);            //it sets the cursor as the line is 0(first line)(in the computer language, 0's means is 1.), the column is 0.
     lcd.print("Rain sens:");                        //LCD will prints "Rain sens:".
    lcd.print(val_analogique);                          //LCD will prints the analog variable from the rain sensor.
     lcd.setCursor(0,2);            //it sets the cursor as the line is 2, the column is 0.
    lcd.print("Weather is dry");                        //LCD will prints "Weather is dry".
         
  }
else                                                 //if the analog variable is bigger than 500;
  {
      lcd.setCursor(0,0);          //it sets the cursor as the line is 0(first line)(in the computer language, 0's means is 1.), the column is 0.
      lcd.print("Rain sens:");                        //LCD will prints "Rain sens:".
    lcd.print(val_analogique);                           //LCD will prints the analog variable from the rain sensor.
     lcd.setCursor(0,2);            //it sets the cursor as the line is 2, the column is 0.
    lcd.print("Weather is rainy");                        //LCD will prints "Weather is rainy".
  }; 

}




void dht(){              //it's a function for DHT11 sensor.
  
lcd.clear();         //it clears the LCD screen.
  int chk = DHT11_sensor.read(DHT11_pin);           //DHT11 sensor's variable reads the "Temperature and Humidity Values" from sensor.
 
  
  lcd.setCursor(0,0);          //it sets the cursor as the line is 0(first line)(in the computer language, 0's means is 1.), the column is 0.
  lcd.print("Humidity:");                           //LCD will prints "Humidity:".
  lcd.setCursor(9,0);            //it sets the cursor as the line is 0, the column is 9.
  lcd.print((float)DHT11_sensor.humidity, 2);       //LCD will prints the digital variable from the DHT11 sensor.  
 
 
  lcd.setCursor(0,1);          //it sets the cursor as the line is 1, the column is 0.
  lcd.print("Temp:");                        //LCD will prints "Temperature:".
  lcd.print((float)DHT11_sensor.temperature, 2);    //LCD will prints the digital variable from the DHT11 sensor.   
   
  
}


void gaz(){              //it's a function for MQ135 sensor.
  
  lcd.setCursor(0,0);          //it sets the cursor as the line is 0(first line)(in the computer language, 0's means is 1.), the column is 0.
  lcd.print("Air Quality:");            //LCD will prints "Air Quality:".
  lcd.print(gazdeger);                  //LCD will prints the variable from the MQ135 sensor.
  if(gazdeger<500){           //if the sensor's value is smaller than 500, it means the air is safe but if it bigger than 500 it means the air is not safe.
   lcd.setCursor(0,1);          //it sets the cursor as the line is 1, the column is 0.
   lcd.print("Air is safe.");           //LCD will prints "Air is safe.".
  }
  else{
    lcd.setCursor(0,1);          //it sets the cursor as the line is 1, the column is 0.
    lcd.print("Air is not safe.");      //LCD will prints "Air is not safe.".
  }
  
}




void bmp180(){              //it's a function for BMP108 sensor. This function will prints the values to the LCD screen.

  lcd.setCursor(0,0);            //it sets the cursor as the line is 0(first line)(in the computer language, 0's means is 1.), the column is 0.
  lcd.print("Pressure :");                                    //LCD will prints "Pressure :".
  lcd.print(bmp.readPressure()/1000);                                        //LCD will prints the pressure value as "kilopascal".
  lcd.print("kPa");     //"kiloPascal".

  lcd.setCursor(0,1);            //it sets the cursor as the line is 1, the column is 0.
  lcd.print("Altitude :");                                      //LCD will prints "Altitude :".
  lcd.print(bmp.readAltitude(101325));       //LCD will prints the altitude value as "meters".
  lcd.print("m");     //"meters".
  
}

void ledInfo(){              //it's a function for leds. This function will turns on the leds for information.
   
  if(gazdeger>500){               //if the red led turns on, it means the air is not safe.
    digitalWrite(8,HIGH);      //turns on red led.
  }
  else{                       //turns off red led.
    digitalWrite(8,LOW);
  }

  if(DHT11_sensor.temperature > 5 && DHT11_sensor.temperature < 27){      //if the temperature is between 5 and 27 *C;
    if(val_analogique > 500){            //if the weather is dry;
    digitalWrite(9,LOW);          //RGB led lights the yellow. (Yellow RGB's mean is : The weather is not rainy and warm).
    digitalWrite(10,LOW);
    digitalWrite(11,HIGH);     
    }
    else{                               //if the weather is wet;
      digitalWrite(9,HIGH);          //RGB led lights the blue. (Blue RGB's mean is : The weather is rainy and warm or cold).
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW); 
    }
  }
  
  else if(DHT11_sensor.temperature > 27){                                 //if the temperature bigger than 27;
    digitalWrite(9,LOW);          //RGB led lights the red. (Red RGB's mean is : The weather is hot).
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
  }

  else if(DHT11_sensor.temperature < 5){                                 //if the temperature smaller than 5;
     digitalWrite(9,LOW);          //RGB led lights the white. (White RGB's mean is : The weather is cold. It can be snowy).
     digitalWrite(10,LOW);
     digitalWrite(11,LOW);
  }
  
}


void espSetup(){

Serial.begin(9600);                             //it starts serial port communication.
  Serial.println("Started");
  esp.begin(115200);                                          //start to our wi-fi module.
  esp.println("AT");                                          //checking our wi-fi module with "AT" command.
  Serial.println("AT sent");
  while(!esp.find("OK")){                                     //wait until the wi-fi module is ready.
    esp.println("AT");
    Serial.println("ESP8266 (wi-fi module) is not working.");
  }
  Serial.println("Wi-fi Module is ready");
  esp.println("AT+CWMODE=1");                                 //We make the wifi module ready for communication (we switch it to client mode).
  while(!esp.find("OK")){                                     //wait until the wifi module is ready.
    esp.println("AT+CWMODE=1");
    Serial.println("Configuring...");
  }
  Serial.println("Switched as client");
  Serial.println("Connectiong to wi-fi adress..");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");    //connecting to our wi-fi adress.
  while(!esp.find("OK"));                                     //wait until the wi-fi module is ready.
  Serial.println("Connected.");
  
}



void espLoop(){

  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           //connecting to ThingSpeak adress.
  if(esp.find("Error")){                                      //checking any errors.
    Serial.println("AT+CIPSTART Error");
  }
  String veri = "GET https://api.thingspeak.com/update?api_key=      Thingspeak_Write_API_Key    ";         //ThingSpeak data write command. (The key is our API key.)
  veri += "&field1=";
  veri += String((float)DHT11_sensor.temperature, 2);             //our temperature value as float.
  veri += "&field2=";
  veri += String((float)DHT11_sensor.humidity, 2);                //our humidity value as float.                    
  veri += "&field3=";
  veri += String(val_analogique);                                 //our rain sensor value.
  veri += "&field4=";
  veri += String(gazdeger);                                       //our air quality value.
  veri += "&field5=";
  veri += String(bmp.readPressure());                             //our pressure value.
  veri += "&field6=";
  veri += String(bmp.readAltitude(101325));                       //our altitude value.
  veri += "\r\n\r\n"; 
  esp.print("AT+CIPSEND=");                                   //We inform the module how long we will send data with the wi-fi module.
  esp.println(veri.length()+6);
  if(esp.find(">")){                                          //if the wi-fi module is ready, our command will works.
    esp.print(veri);                                          //sending sensor values to ThingSpeak.
    Serial.println(veri);
    Serial.println("Data sent.");
  }
  Serial.println("Communication has closed.");
  esp.println("AT+CIPCLOSE");                                //close our connecting with ThingSpeak.
}
