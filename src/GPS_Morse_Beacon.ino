#include <maidenhead.h>
#include <TinyGPS++.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include "config.h"


/************************************************************************************
* Create the needed objects
************************************************************************************/
TinyGPSPlus gpsData;  
SoftwareSerial GPSSerial(GPS_RXD, GPS_TXD);


/************************************************************************************
* Typical Arduino Setup
* Getting things ready, unkeys the transmitter, send some text to the terminal.
************************************************************************************/
void setup() 
{
  Serial.begin(115200);
  Serial.println();
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.println("-=< GPS Beacon for ON7WP/mm >=-");
  Serial.println();

  pinMode(TX_LED, OUTPUT);
  pinMode(KEYER, OUTPUT);
  KeyUp();
  Setup_GPS();
}


/************************************************************************************
* Arduino Loop
* Waiting for meaningfull GPS data and calculate the QTH from the location.
* After sending the beacon text, wait a moment (WORDPSPACE) and activate the longer carrier.
* Send also what we are doing to the terminal.
* If GPS signal is lost, the beacon stops transmitting.
************************************************************************************/
void loop() 
{
  bool newData = false;
  bool GPSLocation = false;

  while (GPSSerial.available()) 
  {
    char c = GPSSerial.read();
    //Serial.write(c);        // uncomment this line if you want to see the GPS data flowing
    if (gpsData.encode(c))  // Did a new valid sentence come in?
    {
      newData = true;
    }
  }

  if (newData) 
  {
    if (gpsData.satellites.value() >= 4 && gpsData.location.lat() > 0 && gpsData.location.lng() > 0 && gpsData.location.isValid() && gpsData.location.age() < 1500)
    {
      GPSLocation = true;
    }
  }

  if (newData && GPSLocation) 
  {
    Serial.println("/");
    Serial.print("| You are at ");
    Serial.print(gpsData.location.lat(), 6);
    Serial.print(F(" , "));
    Serial.println(gpsData.location.lng(), 6);
    
    strcpy(LOCATOR, get_mh(gpsData.location.lat(), gpsData.location.lng(), 6));
    SendBeaconText(CALLSIGN, LOCATOR);
    delay(WORDSPACE);
    SendCarrier();
    Serial.println("\\");
    Serial.println("");
    delay(PAUSE * 1000);
  }
}


/************************************************************************************
* Function SendBeaconText
* Inputs are CallSign and Locator
* This function takes both parameters, displays them to the terminal.
* Furthermore it loops thru the characters, does a search in the tables in config.h
* Hands the data over to TxSequence(), for example the letter P in Morse code = .--.
* Every transmitted character will be sent to the terminal.
************************************************************************************/
void SendBeaconText(char CallSign[], char Locator[]) 
{
  int Len = strlen(CallSign) + " " + strlen(Locator);
  char bText[Len];
  sprintf(bText, "%s %s", CallSign, Locator);
  Serial.print("| Beacon text = ");
  Serial.println(bText);
  // Serial.println(strlen(bText));
  Serial.print("| TXing = ");

  for (int i = 0; i < strlen(bText); i++) 
  {
    Serial.print(bText[i]);

    if (bText[i] >= 'a' && bText[i] <= 'z') 
    {
      TxSequence(letters[bText[i] - 'a']);
    } 
    else if (bText[i] >= 'A' && bText[i] <= 'Z') 
    {
      TxSequence(letters[bText[i] - 'A']);
    } 
    else if (bText[i] >= '0' && bText[i] <= '9') 
    {
      TxSequence(numbers[bText[i] - '0']);
    } 
    else if (bText[i] == ' ') 
    {
      delay(WORDSPACE);
    } 
    else if (bText[i] == '/') 
    {
      TxSequence(extra[bText[i] - '/']);
    }
  }
  Serial.println("");
}

/************************************************************************************
* Function TxSequence
* Input is a sequence of characters, for example .--. (the letter P in Morse code).
* Loops thru the sequence and hand it over to the function TxDitOrDah().
* There is a delay between each letter (CSPACE).
************************************************************************************/
void TxSequence(char* Sequence) 
{
  int i = 0;
  while (Sequence[i] != NULL) 
  {
    TxDitOrDah(Sequence[i]);
    i++;
  }
  delay(CSPACE);
}


/************************************************************************************
* Function TxDitOrDah
* Input is a . (dot) or - (dash)
* This function key the transmitter KeyDown() for the length (time) of the dot or dash.
************************************************************************************/
void TxDitOrDah(char dotOrDash) 
{
  KeyDown();
  if (dotOrDash == '.') 
  {
    delay(DIT);
  } 
  else  // must be a -
  {
    delay(DAH);
  }
  KeyUp();
  delay(LSPACE);
}


/************************************************************************************
* Function KeyUp
* This function UNkeys the transmitter and the built in led. 
* You may need to swap the LOW/HIGH values according to your transceiver keyer
************************************************************************************/
void KeyUp() 
{
  digitalWrite(TX_LED, LOW);
  digitalWrite(KEYER, LOW);
}


/************************************************************************************
* Function KeyDown
* This function keys the transmitter, like a straight key pressing down 
* and the built in led .
* You may need to swap the LOW/HIGH values according to your transceiver keyer
************************************************************************************/
void KeyDown() 
{
  digitalWrite(TX_LED, HIGH);
  digitalWrite(KEYER, HIGH);
}


/************************************************************************************
* Function SendCarrier
* Keys the transceiver via the KeyDown() function for the time set. And unkeys after that.
************************************************************************************/
void SendCarrier() 
{
  Serial.println("| SendCarrier");
  KeyDown();
  delay(CARRIER * 1000);
  KeyUp();
}


/************************************************************************************
* Function Setup_GPS
* Start the GPS serial at 9600 and print it back to the terminal
************************************************************************************/
void Setup_GPS() 
{
  GPSSerial.begin(9600);  // connect gps sensor
  Serial.println(" GPS started");
}



