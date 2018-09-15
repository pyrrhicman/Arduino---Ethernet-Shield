#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>


#define LED1 7
#define SAMPLE_DELAY 800
const int rs = 9, en = 8, d4 = 5, d5 = 6, d6 = 3, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int analogPin = 9;
unsigned int readedValue = 0; 
unsigned int savedValue=0;
int lightPresent = 0.0;
const byte interruptPin = 2;
volatile byte state = LOW;





byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xCD
};
IPAddress ip(192, 168, 110, 50);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);




String readString;


void setup() {
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1,HIGH);
  lcd.begin(16, 2);
  lcd.setCursor(3,0);
  lcd.print("LCD TEST");
  for(int x = 0 ; x < 5 ; x++){
      digitalWrite(LED1,LOW);
      delay(200);
      digitalWrite(LED1,HIGH);
      delay(200);
    }

  lcd.clear();
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), button, CHANGE);
  savedValue = analogRead(analogPin); 

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  Ethernet.begin(mac, ip);
  server.begin();
  lcd.print("server is at: ");
  lcd.setCursor(0,1);
  lcd.println(Ethernet.localIP());
  delay(3000);
  lcd.clear();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("TESTTTTTINGGGGG");
  lcd.println(Ethernet.localIP());
}


void loop() 
{
  //Serial.println("TESTTTTTINGGGGG");
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      
      if (client.available()) {
        
        char c = client.read();
        readString += c;
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("Saved Value is : ");
          client.print(savedValue);
          client.println("<br />");
          client.println("<button formaction=\"/?buttonOn\"\" type=""button"">Turn On !</button>");
          client.println("<a href=\"/?buttonOn\"\">Turn On Led</a>");
          client.println("<a href=\"/?buttonOff\"\">Turn Off Led</a>");
          client.println("<br />");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 15; analogChannel++) {
            int sensorReading = analogRead(analogPin);
            lightPresent = sensorReading ; //MAX = 1024 MIN = 0
            lightPresent = lightPresent / 10.24 ;
            client.print("analog input ");
            client.print(analogPin);
            client.print(" is ");
            client.print(sensorReading);
            client.print("   ");
            client.print(lightPresent);
            client.print("%");
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    
    if(readString.indexOf("?buttonOn")>0){
      digitalWrite(LED1,LOW);
    }
    if(readString.indexOf("?buttonOff")>0){
      digitalWrite(LED1,HIGH);
    }
    readString = " ";
  }
}

void button() {
  savedValue = analogRead(analogPin);  
}

/*
 * 
#include <LiquidCrystal.h>

#define LED1 0
#define SAMPLE_DELAY 800
const int rs = 9, en = 8, d4 = 5, d5 = 6, d6 = 3, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int analogPin = 9;
unsigned int readedValue = 0; 
unsigned int savedValue=0;
int lightPresent = 0.0;
const byte interruptPin = 2;
volatile byte state = LOW;

void setup() {
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1,HIGH);
  lcd.begin(16, 2);
  lcd.setCursor(3,0);
  lcd.print("LCD TEST");
  digitalWrite(LED1,LOW);
  delay(1000);
  digitalWrite(LED1,HIGH);
  lcd.clear();
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), button, CHANGE);
  savedValue = analogRead(analogPin); 
}


void loop() 
{
  
  readedValue = analogRead(analogPin);  
  lightPresent = readedValue ; //MAX = 1024 MIN = 0
  lightPresent = lightPresent / 10.24 ;
  lcd.print("Luettu: ");
  lcd.print(readedValue);
  lcd.print(" ");
  lcd.print(lightPresent);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Saved: ");
  lcd.print(savedValue);
  
    if(readedValue<=savedValue)
    {
       digitalWrite(LED1,LOW);
       lcd.print(" Dark");
    }
    else if((readedValue -50)>savedValue)
    {
       digitalWrite(LED1,HIGH); 
       lcd.print(" Light");
    }
    
  delay(SAMPLE_DELAY);
  lcd.clear();
}

void button() {
  savedValue = readedValue;  
}

*/
 
