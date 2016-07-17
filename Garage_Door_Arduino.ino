
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

int getFingerprintIDez();

int output = 4;
int input = 9; //Right side of microcontroller. Quicker to wire to ESP8266

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

void loop()                     
{
  esp8266(); //Check if the button has been clicked from the website
  getFingerprintIDez(); //Check if a finger has been scanned
  delay(50);            
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

  if(finger.fingerID == 1){ //Repalce '1' with the ID for the fingerprint. I used ascending numerical values (1-6)
    openClose(); 
  }

  /*
  This is for multiple fingerprints. This can be continued for as many fingerprints as you added. I personally used six.
  
  if(finger.fingerID == 1 || finger.fingerID == 2){
    openClose(); 
  }
  */

  return finger.fingerID; 
}

void openClose(){
   digitalWrite(output, HIGH); //Turn on relay
   delay(4000); //Wait four seconds
   digitalWrite(output, LOW); //Turn off relay

   /*  For my garage door, sometimes the signal needed to be sent for a few seconds. 
    *  A four second delay ensures that the door will completly open or close.
    *  This may need to be adjusted for you depending on your garage door.
    */
}

void esp8266(){
   if(digitalRead(input) == HIGH){
    Serial.print("Website button clicked"); //For testing/debugging
    openClose();
  }
  delay(15); //Delay for stability
}


