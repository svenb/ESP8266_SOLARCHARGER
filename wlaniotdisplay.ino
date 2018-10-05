#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
const char* ssid = "";
const char* password = "";

int analogpin = A0;
String usrnamelcd = "Sven";
int lcdConnectWlan = 0; // Optional usage, just for design to delay the connected msg 
float messtoleranz_temp = 3.00;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

uint8_t Celsius[8] = {B11100, B10100, B11100, B0000, B00000, B00000, B00000, B00000};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0}; // Bitmap character example
 
void setup() {
  
  Serial.begin(115200);   //Setup Serial Com
  Serial.println("Hallo"); 
  lcd.init();
  for(int i = 0; i< 3; i++) //option for-loop to blink the display at the beginning. 
  {
    lcd.backlight();
    delay(500);
    lcd.noBacklight();
    delay(500);
  }
  lcd.backlight(); // finish for-loop with backlight on  
  
  lcd.home();

  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Hello " + usrnamelcd + "!");  //Welcome Msg
  delay(1500);
  lcd.clear();  //clear the LCD
  Serial.println();

  Serial.print("Connecting to ");     //Connection MSg
  lcd.setCursor(0,0);           //set cursor to character 0 on line 0
  lcd.println("WLAN: ");
  lcd.print(ssid);
  Serial.println(ssid);
                                
  WiFi.begin(ssid, password);   //Connect to Wifi with SSID and PASSWORD

  for (int i = 0; i< 2; i++)      //Optional for-loop to blink twice the "connecting" msg. The NodeMCU / ESP8266 connects so fast that most of the time it jumped right to the connected msg without this loop. It just looks nicer....
  {
      lcd.setCursor(0,1);
      lcd.print("   connecting   ");
      delay(500);
      lcd.setCursor(0,1);
      lcd.print("                ");        
      delay(500);
  }

   while (WiFi.status() != WL_CONNECTED) {   //blink the "connecting" MSG unless the connection is established. If your SSID and PW are correct, you won`t see this most of the time.
                                              //I had sometimes connection issues for several hours, figured out that it was either my wireing or power supply    
    Serial.print(".");
    if (lcdConnectWlan < 1)
    {
      lcd.setCursor(0,1);
      lcd.print("   connecting   ");
      lcdConnectWlan = 1;
      }
      else
      {
      lcd.setCursor(0,1);
      lcd.print("                ");        
      lcdConnectWlan = 0;
      }
    delay(500);
  }
  lcd.setCursor(0,1);
  Serial.println("");
  Serial.println("WiFi connected");         //Print the connected msg after success
  lcd.print("!! Connected !!");
  delay(2000);

  Serial.print("Your IP is: ");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("My IP is:");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(4000);
  
  lcd.clear();
  Serial.print("\n\n");
  Serial.print("Checking all Sensors!\n");

  lcd.setCursor(0,0);
  lcd.println("Checking Sensors!");
  delay(2000);
  Serial.print("\n\n");
  Serial.print("Read temperature!\n");
  delay(1000);
  lcd.createChar(6, Celsius);
  Serial.println(gettemp());
  lcd.setCursor(0,1);
  lcd.print("Tmp:");
  lcd.print(gettemp());
  //Creating Chars for Celcius 
  lcd.createChar(8, Celsius);  
  lcd.write(byte(8)); 
  lcd.print("C");
  //SECOND SENSOR for Reading Voltage
  //must be implemented ;)
  delay(3000);
  Serial.println();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("Boot complete!"); 
 
  delay(3000);
  Serial.println();
  lcd.clear();

  //If an Error comes up, change boot message
  
 
}
 
void loop() {

 //Implementing http binding for openhab
  lcd.setCursor(0,0);
  lcd.print(gettemp());  
  lcd.write(byte(8)); 
  lcd.print("C");
  delay(1000); 
  
   
}



float gettemp()  {
  int analogValue = analogRead(analogpin);
  Serial.println(analogValue); 
  //Check if sensor defect / disabled
  
  float millivolts = (analogValue/1024.0) * 3300; //3300 is the voltage provided by NodeMCU
  float celsius = (millivolts/10)-messtoleranz_temp;  
  Serial.println(celsius);  
  return celsius;

}




