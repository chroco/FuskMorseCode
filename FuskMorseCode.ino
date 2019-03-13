#include <stdio.h>
#include <ctype.h>

#define LED RED_LED
//#define LED GREEN_LED
//#define TX_PIN P5_0 
//#define FSK_PIN P5_2

#define OOK_PIN P5_0 
#define VCO_PIN P5_2

const int kMessageLength = (1<<10);
const int kAsciiOffset = 49;
const int kDotLengthMS = 150;
//const int kDotLengthMS = 1000;
const int kDeadSpaceMS = kDotLengthMS;
const int kDashLengthMS = 3 * kDotLengthMS;
//bool gUseFSK = false;
bool gUseFSK = true;

char message[kMessageLength] = "KJ7DME this is Portland State engineering EE517 boat crashing the Willamette for fun and credit  Thank you KK7B  Onward through the fog\0";

const char *pMorseCodeHash[] = 
{
  ".----\0",  // 0 
  "..---\0",  // 1
  "...--\0",  // 2 
  "....-\0",  // 3 
  ".....\0",  // 4
  "-....\0",  // 5
  "--...\0",  // 6
  "---..\0",  // 7
  "----.\0",  // 8
  "-----\0",  // 9
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  ".-\0",     // A
  "-...\0",   // B
  "-.-.\0",   // C
  "-..\0",    // D
  ".\0",      // E
  "..-.\0",   // F
  "--.\0",    // G
  "....\0",   // H
  "..\0",     // I
  ".---\0",   // J
  "-.-\0",    // K
  ".-..\0",   // L
  "--\0",     // M
  "-.\0",     // N
  "---\0",    // O
  ".--.\0",   // P
  "--.-\0",   // Q
  ".-.\0",    // R
  "...\0",    // S
  "-\0",      // T
  "..-\0",    // U
  "...-\0",   // V
  ".--\0",    // W
  "-..-\0",   // X
  "-.--\0",   // Y
  "--..\0",   // Z
  NULL
};

#define MORSE_CODE_HASH_LENGTH (sizeof(pMorseCodeHash)/sizeof(char *))

int i = 0;
int j = 0;
const char *pMorseCode = NULL;

const char *getMorseCode(char character)
{
  character = toupper(character);
  int index = character - kAsciiOffset;
  if(index < 0 || index > MORSE_CODE_HASH_LENGTH)
  {
    return NULL;
  }

  return pMorseCodeHash[index];
}

void setup() 
{
  pinMode(LED, OUTPUT); 
  pinMode(OOK_PIN, OUTPUT); 
  pinMode(VCO_PIN, OUTPUT); 
  Serial.begin(9600);
  delay(500);
  Serial.print("\n\n\r");
  digitalWrite(OOK_PIN, LOW);   
  //digitalWrite(OOK_PIN, HIGH);   
  digitalWrite(VCO_PIN, LOW);   
  //digitalWrite(VCO_PIN, HIGH);   
}

void txSignal(int pin, int length)
{
  digitalWrite(pin, HIGH);   
  digitalWrite(LED, HIGH);   
  delay(length);               
  digitalWrite(pin, LOW);    
  digitalWrite(LED, LOW);    
  delay(kDeadSpaceMS);
}

int txPin;

void loop() 
{
//*
  if(gUseFSK)
  {
    digitalWrite(OOK_PIN, HIGH);
    txPin = VCO_PIN; 
    Serial.print("FSK TX\n");
  }
  else
  {
    digitalWrite(VCO_PIN, LOW);   
    txPin = OOK_PIN;
    Serial.print("OOK TX\n");
  }
  gUseFSK = !gUseFSK;

  for(i = 0; i < kMessageLength && message[i] != '\0';++i)
  {
    pMorseCode = getMorseCode(message[i]);
    if(pMorseCode)
    {
      Serial.print(message[i]);
      Serial.print(": ");
      
      for(j = 0; pMorseCode[j] && pMorseCode[j] != '\0';++j)
      {
        Serial.print(pMorseCode[j]); 
        if(pMorseCode[j] == '.')
        {
          txSignal(txPin, kDotLengthMS);
        }
        else if(pMorseCode[j] == '-')
        {
          txSignal(txPin, kDashLengthMS);
        }
      }
    }
    
    Serial.print('\n');
    delay(kDashLengthMS);
  }

  Serial.print("\n\r");
 // Serial.flush();
   delay(5 * 1000);
//*/
}
