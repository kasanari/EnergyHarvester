/*
* Transmitter file is designed to transmit a packet of 3 bytes.
* -------------------------------------------------------------
* The Packet which is to be sent is hard-coded.
*
* Pattern for the Packet is: 
* 1st Byte = Length of Packet
* 2nd Byte = Your Data
* 3rd Byte = More Data
* .
* .
* Nth Byte = Last Byte of Data
*
* -------------------------------------------------------------
*
* Hard-coded Packet for this sample program is:
* 1st Byte = 3;    // Length of Packet
* 2nd Byte = 0x09;
* 3rd Byte = 0x01;
*
* Receiver will get the Packet in the above Pattern.
*
* -------------------------------------------------------------
*
* To run this program properly, do the following steps:
*
* 1. Connect the GDO0-Pin of CC2500 With Arduino's Pin-4
* 2. Connect a Push Button(Active-High) with Arduino's Pin-2
* 3. Transmitter will continously trransmit the Hard-coded Packet Until You Release the Push-Button to Low
*
* -------------------------------------------------------------
*
* Precautions:
* - Do appropriate current and voltage conversion between your microcontroller and CC2500 module.
* - High voltage or High current may damage your CC2500 Module.
*/


#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include "cc2500.h"

#include <SPI.h>

#define No_of_Bytes    3

const int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

void setup()
{
  Serial.begin(9600);
  pinMode(SS,OUTPUT);
  SPI.begin();
  digitalWrite(SS,HIGH);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);     
  pinMode(GDO0_PIN, INPUT);     

  Serial.println("Starting..");
  init_CC2500();

  /* This function is to make sure that cc2500 is successfully configured.
   * This function read values of some registers from CC2500. 
   * To use this function, you must read the register values from 
   * -> Arduino-CC2500-Library / CC2500_Library / cc2500_VAL.h
   * Then compare the values from result of this function..
   */
  Read_Config_Regs();
}

void loop()
{
      Serial.println("Register values:");
      Read_Config_Regs();
      delay(1000);
    
    //buttonState = digitalRead(buttonPin);
    //Serial.println(buttonState);
    
//  To start transmission
//    while (buttonState)
//      {
//        // read the state of the pushbutton value:
//        buttonState = digitalRead(buttonPin);
//        Serial.println("Transmission to start");
//        delay(10);
//        TxData_RF(No_of_Bytes);    //  Transmit No_of_Bytes-1
//        Serial.println("Transmission is over");
//        delay(10);
//      }
          
     /* 
    while (buttonState)
      {
      // read the state of the pushbutton value:
      buttonState = digitalRead(buttonPin);
      Serial.println("PB = 1");
      }
      */
}