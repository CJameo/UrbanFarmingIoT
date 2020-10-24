/* Code Written by Rishi Tiwari
 *  Website:- https://tricksumo.com
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12

// assign the SPI bus to pins
#define BMP_SCK D1
#define BMP_MISO D5
#define BMP_MOSI D2
#define BMP1_CS D3
#define BMP2_CS D4
#define BMP3_CS D6
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP280 bmp1(BMP1_CS, BMP_MOSI, BMP_MISO, BMP_SCK); // software SPI
Adafruit_BMP280 bmp2(BMP2_CS, BMP_MOSI, BMP_MISO, BMP_SCK); // software SPI
Adafruit_BMP280 bmp3(BMP3_CS, BMP_MOSI, BMP_MISO, BMP_SCK); // software SPI

// Update HOST URL here

#define HOST "example.com"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID "<ENTER_SSID_HERE>"            // WIFI SSID here                                   
#define WIFI_PASSWORD "<ENTER_WIFI_PASSWORD_HERE>"        // WIFI password here

// Declare global variables which will be uploaded to server

const char* serverName = "http://example.000webhostapp.com/<write_to_database_file>.php";

String sTemp, sPress, sAlt, sDID, postData;


void setup() {
  // Begin Serial Clock
  Serial.begin(115200); 
  Serial.println("Communication Started \n\n");  
  delay(1000);
  
  // Begin BMP sensor array
  bool status;
  int count = 0;
  int total = 3;

  // default settings
  status = bmp1.begin();
  if (!status) {
    Serial.println("Could not find a valid BMP280 sensor 1 , check wiring!");
  }
  else { count += 1; }

  // default settings
  status = bmp2.begin();
  if (!status) {
    Serial.println("Could not find a valid BMP280 sensor 2 , check wiring!");
  }
  else { count += 1; }
  
  // default settings
  status = bmp3.begin();
  if (!status) {
    Serial.println("Could not find a valid BMP280 sensor 3 , check wiring!");
  }
  else { count += 1; }

  String s1 = "BMP Init Complete ... " + String(count) + "/" + String(total) + " Devices Initialized Successfully";
  Serial.println(s1);

  pinMode(LED_BUILTIN, OUTPUT);     // initialize built in led on the board
 
  // WiFi Setup
  WiFi.mode(WIFI_STA);           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  { Serial.print(".");
      delay(500); }
  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());    //print local IP address
  
  delay(30);
}



void loop() { 

  String DIDs[3] = { "1", "2", "3" };
  float measures[][3] = {
                          {bmp1.readTemperature(), bmp1.readPressure(), bmp1.readAltitude(SEALEVELPRESSURE_HPA)},
                          {bmp2.readTemperature(), bmp2.readPressure(), bmp2.readAltitude(SEALEVELPRESSURE_HPA)},
                          {bmp3.readTemperature(), bmp3.readPressure(), bmp3.readAltitude(SEALEVELPRESSURE_HPA)},
                        };
                        
  for(int i = 0; i < 3; i++) {
    String t = String(measures[i][0]);
    String p = String(measures[i][1]);
    String a = String(measures[i][2]);
    sendToServerHTTP(t, p, a, DIDs[i]);
    String sent = "Sent Data: DID=" + DIDs[i] + "Temp=" + t + "Press=" + p + "Alt=" + a;
    Serial.println(sent);
  }

  /*
  HTTPClient http;    // http object of clas HTTPClient
  
  
  // Convert integer variables to string
  //sendval = val;  
  //sendval2 = val2;   
  
   
  postData = "sTemp=" + temp + "&sPress=" + pres + "&sAlt=" + alt +"&sDID=1"; // Single device with hardcoded DID
  
  // We can post values to PHP files as  example.com/dbwrite.php?name1=val1&name2=val2&name3=val3
  // Hence created variable postDAta and stored our variables in it in desired format
  // For more detials, refer:- https://www.tutorialspoint.com/php/php_get_post.htm
  
  // Update Host URL here:-  
    
  http.begin(serverName);              // Connect to host where MySQL databse is hosted
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header
  
    
   
  int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
  //Serial.println("Values are, "sTemp=" + temp + "&sPress=" + pres + "&sAlt=" + alt +"&sDID=1" );
  
  
  // if connection eatablished then do this
  if (httpCode == 200) { Serial.println("Values uploaded successfully."); Serial.println(httpCode); 
  String webpage = http.getString();    // Get html webpage output and store it in a string
  Serial.println(webpage + "\n"); 
  }
  
  // if failed to connect then return and restart
  
  else { 
    Serial.println(httpCode); 
    Serial.println("Failed to upload values. \n"); 
    http.end(); 
    return; }
  
  
  delay(3000); 
  digitalWrite(LED_BUILTIN, LOW);
  delay(3000);
  digitalWrite(LED_BUILTIN, HIGH);
  */

}

void sendToServerHTTP(String temp, String pres, String alt, String DID) {
  HTTPClient http;    // http object of clas HTTPClient
  
  
  // Convert integer variables to string
  //sendval = val;  
  //sendval2 = val2;   
  
   
  postData = "sTemp=" + temp + "&sPress=" + pres + "&sAlt=" + alt +"&sDID=" + DID; 
  
  // We can post values to PHP files as  example.com/dbwrite.php?name1=val1&name2=val2&name3=val3
  // Hence created variable postDAta and stored our variables in it in desired format
  // For more detials, refer:- https://www.tutorialspoint.com/php/php_get_post.htm
  
  // Update Host URL here:-  
    
  http.begin(serverName);              // Connect to host where MySQL databse is hosted
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header
  
    
   
  int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
  //Serial.println("Values are, "sTemp=" + temp + "&sPress=" + pres + "&sAlt=" + alt +"&sDID=1" );
  
  
  // if connection eatablished then do this
  if (httpCode == 200) { Serial.println("Values uploaded successfully."); Serial.println(httpCode); 
  String webpage = http.getString();    // Get html webpage output and store it in a string
  Serial.println(webpage + "\n"); 
  }
  
  // if failed to connect then return and restart
  
  else { 
    Serial.println(httpCode); 
    Serial.println("Failed to upload values. \n"); 
    http.end(); 
    return; }
  
  
  delay(3000); 
  digitalWrite(LED_BUILTIN, LOW);
  delay(3000);
  digitalWrite(LED_BUILTIN, HIGH);
  
}
