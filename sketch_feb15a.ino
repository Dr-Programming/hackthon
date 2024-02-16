#define BLYNK_TEMPLATE_ID "TMPL3H71BFmL7"
#define BLYNK_TEMPLATE_NAME "Hackathon"
#define BLYNK_AUTH_TOKEN "NdINob4lt77Qw2eaeDXWLAAC4KJzdUtY"

#define RF_SENDER 15
#define SIM_RX 16
#define SIM_TX 17
#define IGNITION_RELAY 23
#define FRONT_TOUCH 27
#define FRONT_TOUCH_2 14
#define BACK_TOUCH 12
#define BACK_TOUCH_2 13

const char WifiName[] = "My Hotspot";
const char wifiPass[] = "nai dena";
const char *RF_msg = "ALERT - IND BH 07 3958";

bool RF_FLAG = false;
// bool SIM_NORMAL_FLAG = false;
// bool SIM_URGENT_FLAG = false;

#include <BlynkSimpleEsp32.h>
#include <RH_ASK.h>
#include <SPI.h>
// #include <HardwareSerial.h>

BlynkTimer BlynkTimer;

RH_ASK rf_driver(2000, 4, 5, 0);
// HardwareSerial sim(2);


void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, WifiName, wifiPass, "blynk.cloud", 80);
  // sim.begin(9600, SERIAL_8N1, SIM_RX, SIM_TX);
  PinSetup();

  Serial.begin(115200);
  if(!rf_driver.init()){
    Serial.println("RF MODULE FAILED");
  }
  Serial.println("Setup Complete....");

  BlynkTimer.setInterval(1000L, Touch_Sense);
  BlynkTimer.setInterval(1000L, Rf_Send);
}

void PinSetup(){
  pinMode(FRONT_TOUCH, INPUT);
  pinMode(FRONT_TOUCH_2, INPUT);
  pinMode(BACK_TOUCH, INPUT);
  pinMode(BACK_TOUCH_2, INPUT);
  pinMode(IGNITION_RELAY, OUTPUT);
}

void Touch_Sense(){
  int val_1 = touchRead(FRONT_TOUCH);
  int val_2 = touchRead(FRONT_TOUCH_2);
  int val_3 = touchRead(BACK_TOUCH);
  int val_4 = touchRead(BACK_TOUCH_2);

  Blynk.virtualWrite(V0, val_1);
  // Blynk.virtualWrite(V1, val_2);
  Blynk.virtualWrite(V1, val_3);
  // Blynk.virtualWrite(V3, val_4);

  if(val_1 <= 20 || val_3 <= 20){
    Blynk.logEvent("touch-detected", "Someone has touched your car's number plate.");
    Serial.println("==> Touch Detected...");
    // SIM_NORMAL_FLAG = true;
  }
  if(val_2 > 90 || val_4 > 85){
    digitalWrite(IGNITION_RELAY, HIGH);
    Blynk.virtualWrite(V4, 1);
    Blynk.logEvent("number-plate-changes", "There's something wrong with your car's License Plate AND car's Ignition system has been switched OFF.");
    Serial.println("==> Number plate removed...");
    // SIM_URGENT_FLAG = true;
  }
}

void Rf_Send(){
  if(RF_FLAG){
    rf_driver.send((uint8_t *)RF_msg, strlen(RF_msg));
    
    rf_driver.waitPacketSent();
    Serial.println("==> RF Data Sent...");
  }
}

BLYNK_WRITE(V3){
  int val = param.asInt();
  if(val == 1){
    RF_FLAG = true;
    Blynk.logEvent("rf-transmitter", "RF Transmission ENABLED.");
    Serial.println("==> RF Transmission Enabled...");
  }else{
    RF_FLAG = false;
    Blynk.logEvent("rf-transmitter", "RF Transmission DISABLED.");
    Serial.println("==> RF Transmission Disabled...");
  }
}

BLYNK_WRITE(V4){
  int val = param.asInt();
  if(val == 1){
    digitalWrite(IGNITION_RELAY, HIGH);
    Blynk.logEvent("ignition-relay", "Your car's Ignition box is switched OFF");
    Serial.println("==> Ignition Relay ON");
  }else{
    digitalWrite(IGNITION_RELAY, LOW);
    Blynk.logEvent("ignition-relay", "Your car's Ignition box is switched ON");
    Serial.println("==> Ignition Relay OFF");
  }
}

BLYNK_WRITE(V2){
  // for(int i=1; i<=2; i++){
  //   sim.println("AT+CMFG=1");
  //   sim.println("AT+CMGS=\"+916351942236\"\r");
  //   sim.println("CAR THEFT!!!!\nMy name is Prince and my car is theft, I need assistance. \nMy car is Transmitting Radio Alert. \nMy car number is IND BH 07 3958.");
  //   sim.println((char)26);
  // }
  Blynk.logEvent("share-data", "Your car SIM card has sent Alert SMS to Police.");
  Blynk.virtualWrite(V2, 0);
  Serial.println("==> SMS Sent to Police...");
}

void loop() {

  // if(SIM_NORMAL_FLAG){
  //   for(int i=1; i<=2; i++){
  //     sim.println("AT+CMFG=1");
  //     sim.println("AT+CMGS=\"+916351942236\"\r");
  //     sim.println("CAR ALERT!!!");
  //     sim.println((char)26);
  //     Serial.println(i);
  //   }
  //   SIM_NORMAL_FLAG = false;
  //   Serial.println("==> Normal SMS Sent...");
  // }
  // if(SIM_URGENT_FLAG){
  //   for(int i=1; i<=5; i++){
  //     sim.println("AT+CMFG=1");
  //     sim.println("AT+CMGS=\"+916351942236\"\r");
  //     sim.println("Yout CAR is under DANGER!!!");
  //     sim.println((char)26);
  //     Serial.println(i);
  //   }
  //   SIM_URGENT_FLAG = false;
  //   Serial.println("==> Urgent SMS Sent...");
  // }

  Blynk.run();
  BlynkTimer.run();
}


