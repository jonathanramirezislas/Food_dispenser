#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
//#include <HTTPClient.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


#include <SPI.h>
#include <MFRC522.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#define SS_PIN D2  //--> SDA / SS is connected to pinout D2
#define RST_PIN D1  //--> RST is connected to pinout D1
MFRC522 mfrc522(SS_PIN, RST_PIN);  //--> Create MFRC522 instance.
Servo servo;
 

#define ON_Board_LED 2  

//----------------------------------------SSID and Password of your WiFi router-------------------------------------------------------------------------------------------------------------//
//const char* ssid = "HERE_NAME_OF_WIFI";
//const char* password = "HERE_PASSWORD";
const char* ssid = "WIFI";
const char* password = "PASSWORD";
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

ESP8266WebServer server(80);  //--> Server on port 80

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

//-----------------------------------------------------------------------------------------------SETUP--------------------------------------------------------------------------------------//
void setup() {
  Serial.begin(115200); //--> Initialize serial communications with the PC
  SPI.begin();      //--> Init SPI bus
  mfrc522.PCD_Init(); //--> Init MFRC522 card

  delay(500);

  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");

  pinMode(ON_Board_LED, OUTPUT);
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  //----------------------------------------------------servo
 servo.attach(2);  //D4
  servo.write(0);
  delay (200); 

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Please tag a card or keychain to see the UID !");
  Serial.println("");
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------LOOP---------------------------------------------------------------------------------------//
void loop() {

int readsuccess = getid();
  

  if (readsuccess) {
    Serial.println("get Id");
    digitalWrite(ON_Board_LED, LOW);
    HTTPClient http;    //Declare object of class HTTPClient
    //ESP8266HTTPClient http;

    String UIDresultSend, postData;
    UIDresultSend = StrUID;

    //Post Data
    postData = "UIDresult=" + UIDresultSend;

    http.begin("http://proyectosita.com/dispenser/rfid.php");  //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header

    int httpCode = http.POST(postData);   //Send the request
    String payload = http.getString();    //Get the response payload

    if(payload.equals("ok")){
      Serial.println("open"); 
      servo.write(180);
      delay(500);
      servo.write(0);
    }
    else if(payload.equals("okk")){
      Serial.println("open"); 
      servo.write(180);
      delay(600);
      servo.write(0);
    }
    else if(payload.equals("okkk")){
      Serial.println("open");
      servo.write(180);
      delay(800);
      servo.write(0);
    }
    else if(payload.equals("okkkk")){
      Serial.println("open"); 
      servo.write(180);
      delay(1000);
      servo.write(0);
    }
    else if(payload.equals("not")){
      Serial.println("close");
      servo.write(0);
    }
    else if(payload.equals("register")){
      Serial.println("register");
    }else {
      Serial.println("error:");
      Serial.println(payload);
    }

    Serial.println("UIDresultSend");
    Serial.println(UIDresultSend);
    Serial.println("httpCode");
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println("payload");
    Serial.println(payload);    //Print request response payload
    Serial.println("end");

    //http.end();  //Close connection
    delay(1000);
    digitalWrite(ON_Board_LED, HIGH);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------------------------//
int getid() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial())
 
  {
    
    return 0;
  }


  Serial.print("THE UID OF THE SCANNED CARD IS : ");

  for (int i = 0; i < 4; i++) {
    readcard[i] = mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
    
    Serial.println(str);
    Serial.println(StrUID);
  }
  Serial.println(StrUID);
  mfrc522.PICC_HaltA();
  return 1;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------//
void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}
//------------------------------------------------------------------------------------
