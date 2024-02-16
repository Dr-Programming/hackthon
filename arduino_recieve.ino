#include <RH_ASK.h>
#include <SPI.h>
//11
RH_ASK rf_driver;

void setup() {
  Serial.begin(9600);
  if(!rf_driver.init()){
    Serial.println("RF MODULE FAILED");
  }
  Serial.println("Setup Completed....");
}

void loop() {
  uint8_t buf[18];
  uint8_t bufLen = sizeof(buf);
  if(rf_driver.recv(buf, &bufLen)){
    Serial.print("Message Recieved: ");
    Serial.println((char*)buf);
    delay(1000);
  }
}
