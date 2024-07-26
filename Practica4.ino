#include <SPI.h>
#include <MFRC522.h>
 
#define RST_pin 9
#define SDA_pin 10
#define LED_V 4
#define LED_R 5
#define Buz_pin 2
 
MFRC522 ident (SDA_pin, RST_pin);
MFRC522::MIFARE_Key key;
 
byte tag1[4] = {0xE3, 0xF7, 0xDD, 0x24};
byte tag2[4] = {};
 
byte tagAc[4];
 
void setup() {
  Serial.begin (9600);
  while (!Serial);
  SPI.begin();
  ident.PCD_Init();
 
  pinMode (LED_V, OUTPUT);
  pinMode (LED_R, OUTPUT);
  pinMode (Buz_pin, OUTPUT);
}
 
void loop() {
  if (ident.PICC_IsNewCardPresent()){
    if (ident.PICC_ReadCardSerial()){
      //Serial.print(F("Card UID: "));
      dump_byte_array(ident.uid.uidByte, ident.uid.size);
 
      digitalWrite (LED_V, LOW);
      digitalWrite (LED_R, LOW);
      noTone (Buz_pin);
    }
  }   
}
 
bool compararArray(byte Array1[], byte Array2[]){
  if (Array1[0] != Array2[0]) return(false);
  if (Array1[1] != Array2[1]) return(false);
  if (Array1[2] != Array2[2]) return(false);
  if (Array1[3] != Array2[3]) return(false);
  return (true);
}

void dump_byte_array(byte *buffer, byte bufferSize){
  for (byte i = 0; i < bufferSize; i++){
    Serial.print (buffer [i] < 0x10 ? "0" : " ");
    Serial.println (buffer[i], HEX);
    tagAc[i] = buffer[i];
  }
  if (compararArray(tagAc, tag1)){
    Serial.println ("Acceso Permitido");
    digitalWrite (LED_V, HIGH);
    tone (Buz_pin, 300);
    delay (4000);
  } 
  else {
    Serial.println ("Acceso Denegado");
    digitalWrite (LED_R, HIGH);
    tone (Buz_pin, 600);
    delay (2000);
  }
}
