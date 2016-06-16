# Garage-Door-Opener
Created by Areya Joya
Working as of June 16, 2016
Opens and closes a garage door with a finger print and/or any WiFi connected device.
Uses Adafruit fingerprint sensor and Adafruit HUZZAH ESP8266 (any ESP8266 will work).
Can easily be modified to accomodate other input methods (keypad, RFID, IR, radio transmitter, etc.)
Programmed on the Arduino platform and uses an ATMEGA328P-PU for the "Arduino", but a normal Arduino will work.
Output from ESP8266 is read by Arduino as a digital input. This then makes another pin a digital output to turn on a relay. The relay is connected to the garage door motor.
ESP8266 outputs 3.3V. This is not enough to activate the relay so the Arduino does it instead.
