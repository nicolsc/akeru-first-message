#include <SoftwareSerial.h>
#include <Akeru.h>

#define SERIAL_BAUD 9600
#define SIGFOX_LED_PIN 13
#define SIGFOX_POWER 3

#define BOOT_DELAY 500 // ms before initializing modem
#define LOOP_DELAY 600000 //milliseconds between each loop

int count; //2bytes

void setup() {
  Serial.begin (SERIAL_BAUD);
  pinMode(SIGFOX_LED_PIN, OUTPUT);
  
  //let the akeru board wake up gently
  delay(BOOT_DELAY);

  count = 0;  

  Akeru.begin();
  Akeru.setPower(SIGFOX_POWER);
  
  digitalWrite(SIGFOX_LED_PIN, LOW);
}
void loop() {

  count++;
  
  sendMessage(count);
  delay(LOOP_DELAY);
}
boolean sendMessage(int data){
  Serial.print("Counter value = ");
  Serial.println(count);
  if (!Akeru.isReady()){
      Serial.println("Cannot send Sigfox message right now"); 
      Serial.println("Probably due to the 1 msg per 10' limit enforced by the lib");
      return false;
  }
  digitalWrite(SIGFOX_LED_PIN, HIGH);
  if (!Akeru.send(&data, sizeof(data))){
    Serial.println("An error occured while sending message");
    digitalWrite(SIGFOX_LED_PIN, LOW);
    return false;
  }
  Serial.println("Message sent");
  digitalWrite(SIGFOX_LED_PIN, LOW);
  
  return true;

}
