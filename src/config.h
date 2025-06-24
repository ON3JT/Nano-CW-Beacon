/************************************************************************************
* pins for keying the radio and led to blink simultaneous
************************************************************************************/
const int TX_LED = 13;
const int KEYER  = 12;

/************************************************************************************
* CW stuff
************************************************************************************/
const int SPEED     = 12;           // speed in WPM.
const float DIT     = (1200/SPEED); // length unit of a dit (or dot = "." )
const int DAH       = DIT * 3;      // length unit of a dah (or dash = "-" )
const int LSPACE    = DIT;          // length unit between dit/dah in a character
const int CSPACE    = DAH;          // length unit between the letters 
const int WORDSPACE = DIT * 7;      // length unit between the words
/*
    TX text = ON2OT JO21FE
    Timings = O = - - -  ==> dah LSPACE dah LSPACE dah
              N = - .    ==> dah LSPACE dit
              time between O and N = CSPACE
              time between ON2OT and JO21FE = WORDSPACE
*/
const int PAUSE     = 2;            // Time in seconds between transmissions
const int CARRIER   = 15;           // Time in seconds beacon carrier

/************************************************************************************
* Callsign and dummy locator to start with
************************************************************************************/
const char CALLSIGN[] = "ON2OT";
char LOCATOR[6] = "JO20mm" ;

/************************************************************************************
* GPS Serial Setup
************************************************************************************/
const int GPS_RXD = 9;
const int GPS_TXD = 10;

/************************************************************************************
* Magic
************************************************************************************/
// letters
char* letters[] = 
{
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // J-R 
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." // S-Z
};

// Numbers
char* numbers[] = 
{
  "-----", ".----", "..---", "...--", "....-", ".....","-....", "--...", "---..", "----."
};

// forward slash
char* extra[] = 
{
  "-..-."
};

/*
https://github.com/sp6q/maidenhead/tree/master
*/

