#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
//Date & Time
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String formattedDate;
String dayStamp;
String timeStamp;

// Insert your network credentials
#define WIFI_SSID "MAJU-BELL"
#define WIFI_PASSWORD "847549632548"

//#define WIFI_SSID "DevSol2"
//#define WIFI_PASSWORD "M@maju0007$"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCWqoGDAMXns6JnQcFJk3XJLFbraCZLoKs"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://maju-smartparking-default-rtdb.firebaseio.com/" 
//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte n_rows = 4;
const byte n_cols = 4;
 
char keys[n_rows][n_cols] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};
 
byte rowPins[n_rows] = {3, 1, D3, D4}; 
byte colPins[n_cols] = {D5, D6, D7, D8}; 
 
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols); 

// 
char Data[7];
byte data_count = 0;
char IDC[12];
byte id_count = 0;
char Plate[7];
byte Plate_count=0;



void setup(){
 Serial.begin(115200);
Wire.begin(D2, D1);   //Use predefined PINS consts
lcd.begin(20,4);      // The begin call takes the width and height. This
                      // Should match the number provided to the constructor.

lcd.backlight();      // Turn on the backlight.

lcd.home();

lcd.setCursor(0, 0);  // Move the cursor at origin
lcd.print("MAJU SMART Parking");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.setCursor(0,1);
  lcd.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
    lcd.setCursor(0,2);
    lcd.print(".");
    lcd.setCursor(0,3);
    lcd.print(WIFI_SSID);
    delay(300);
  }
  Serial.println();
  clearLCDLine(1);
  clearLCDLine(2);
  clearLCDLine(3);
  lcd.setCursor(0,1);
  lcd.print("Connected with IP:");
  lcd.setCursor(0,2);
  lcd.print(WiFi.localIP());
  delay(1000);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
  clearLCDLine(1);
  clearLCDLine(2);
  lcd.setCursor(0,1);
    lcd.print("Firebase Connected");
    clearLCDLine(1);
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  clearLCDLine(1);
  clearLCDLine(2);
  lcd.setCursor(0,1);
    lcd.print("Firebase ERROR");
  }
  delay(500);

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

    
  timeClient.begin();
  timeClient.setTimeOffset(18000);
    while(!timeClient.update()) {
    timeClient.forceUpdate();
  }

//


  GETF();
  
}
 
void loop(){
    formattedDate = timeClient.getFormattedDate();
  lcd.setCursor(0,3);
  lcd.print(formattedDate);
 char myKey = myKeypad.getKey();

    if (myKey != NULL && data_count !=7){
    lcd.setCursor(0,1);
    lcd.print("Enter Plate Number:");
      Data[data_count] = myKey; 
      lcd.setCursor(data_count,2); 
      lcd.print(Data[data_count]); 
      data_count++;
    }
    delay(100);
    
    if (data_count == 7 && id_count == 0){
      clearLCDLine(1);
      clearLCDLine(2);
    }  

    if (data_count == 7 && myKey != NULL && id_count !=12){
    lcd.setCursor(0,1);
    lcd.print("Enter ID Card:");
      IDC[id_count] = myKey; 
      lcd.setCursor(id_count,2); 
      lcd.print(IDC[id_count]); 
      id_count++;
    delay(100);      
    }
    if (data_count == 7 && id_count == 12){
      clearLCDLine(1);
      clearLCDLine(2);
      lcd.setCursor(4,2);
      lcd.print("Confirm [ok]?");
      delay(300);
    }
          if (data_count == 7 && id_count == 12 && myKey != NULL){
      if(myKey == '#'){
        clearLCDLine(2);
        data_count = 0;
        id_count = 0;
        POSTF();
        delay(500);
        clearLCDLine(2);
      }
      
    }
}
void clearLCDLine(int line)
{               
        lcd.setCursor(0,line);
        for(int n = 0; n < 20; n++)
        {
                lcd.print(" ");
        }
}
void POSTF ()
{
  char cstr[16];
//  itoa(Data, cstr, 10);
    char sourced[32];


strcpy(sourced,"PDB/");
strcat(sourced,Data);
strcat(sourced,"/plate");


    char sourcedID[32];
strcpy(sourcedID,"PDB/");
strcat(sourcedID,Data);
strcat(sourcedID,"/stuid");

    char sourcedtime[32];
strcpy(sourcedtime,"PDB/");
strcat(sourcedtime,Data);
strcat(sourcedtime,"/intime");
  
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    Serial.println("------------------------------------");
    Serial.println("Set JSON test...");

    if (Firebase.RTDB.setString(&fbdo, sourcedID, IDC))
    {
        lcd.setCursor(4,2);
        lcd.print("Database Post");
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
        lcd.setCursor(4,2);
        lcd.print("Database Failed");
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
    
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setString(&fbdo, sourced, Data)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    if (Firebase.RTDB.setString(&fbdo, sourcedtime, formattedDate)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
  count++; 
}

void GETF ()
{
    if (Firebase.RTDB.getString(&fbdo, "PDB/MH")) {

        lcd.setCursor(5,2);
      lcd.print(String(fbdo.stringData()).c_str());

  } else {
    lcd.setCursor(5,2);
    lcd.print("GET ERROR");
    Serial.println(fbdo.errorReason());
  }
  delay(1000);
  clearLCDLine(2);
}
