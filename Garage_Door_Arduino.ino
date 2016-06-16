//Code provided by Adafruit
//Modified by Areya Joya
//Working as of June 16, 2016

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

int getFingerprintIDez();

int output = 4; //Output to turn on relay
int input = 5; //Input for ESP8266

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  Serial.begin(9600);
  Serial.println("fingertest");

  pinMode(input, INPUT);
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  digitalWrite(input, LOW);
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{
  esp8266();
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  
  
  uint8_t p = finger.getImage();
  
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
      
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  /*
   * 1: Areya
   * 2: Mother
   * 3: Touraj
   * 4: Father
   * 5: Amir
   * 6: Mejgon
   */

  if(finger.fingerID == 1 || finger.fingerID == 2 || finger.fingerID == 3 || finger.fingerID == 4 || finger.fingerID == 5 || finger.fingerID == 6){
    openClose(); 
  }

   

//  if(finger.fingerID == 1){
//    Serial.print("Areya");
//    digitalWrite(output, HIGH);
//    delay(4000);
//    digitalWrite(output, LOW);
//  }
//  if(finger.fingerID == 2){
//    Serial.print("Mother");
//    digitalWrite(output, HIGH);
//    delay(4000);
//    digitalWrite(output, LOW);
//  }
//  if(finger.fingerID == 3){
//    Serial.print("Touraj");
//    digitalWrite(output, HIGH);
//    delay(4000);
//    digitalWrite(output, LOW);
//  }
//  if(finger.fingerID == 4){
//    Serial.print("Father");
//    digitalWrite(output, HIGH);
//    delay(4000);
//    digitalWrite(output, LOW);
//  }
//   if(finger.fingerID == 5){
//    Serial.print("Amir");
//    digitalWrite(output, HIGH);
//    delay(4000);
//    digitalWrite(output, LOW);
//  }
//  if(finger.fingerID == 6){
//    Serial.print("Mejgon");
//    digitalWrite(output, HIGH);
//    delay(4000);
//    digitalWrite(output, LOW);
//  }
  return finger.fingerID; 
}

void openClose(){
   digitalWrite(output, HIGH);
   delay(4000);
   digitalWrite(output, LOW);
}

void esp8266(){
  
   if(digitalRead(input) == HIGH){
    Serial.print(1);
    openClose();
  }
  delay(15); //Delay for stability
}


