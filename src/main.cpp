#include <Arduino.h>

//includes :
#include "pins.h"
#include "params.h"

//libs :
//sonars :
#include <HC_SR04.h>
//#include <SonarArray.hpp>

//CAN :
#include <cpp_header_gen/can.hpp>

#include <Ultrasonic.h>

// Sonars functions and objects
void sendSonarsValueToCan();

/*SonarArray sonarArray(NB_SONARS,
  SONAR_TRIG_FRONT_L, SONAR_ECHO_FRONT_L,
  SONAR_TRIG_FRONT_R, SONAR_ECHO_FRONT_R,
  SONAR_TRIG_L, SONAR_ECHO_L,
  SONAR_TRIG_R, SONAR_ECHO_R,
  SONAR_TRIG_BACK, SONAR_ECHO_BACK
);*/

Ultrasonic sonar0(SONAR_TRIG_FRONT_L, SONAR_ECHO_FRONT_L);
Ultrasonic sonar1(SONAR_TRIG_FRONT_R, SONAR_ECHO_FRONT_R);
Ultrasonic sonar2(SONAR_TRIG_BACK_L, SONAR_ECHO_BACK_L);
Ultrasonic sonar3(SONAR_TRIG_BACK_R, SONAR_ECHO_BACK_R);
Ultrasonic sonar4(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT); 
Ultrasonic sonar5(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT); 
unsigned long lastMillis = 0;

// Serial handler for frames coming from serial
CanHandler canHandler;

// OLED display (width, height, I2C, reset pin = -1 for sharing arduino reset pin)

void setup() {
  // Serial frames
  canHandler.setup();
  Serial.begin(115200);
  Serial.println("\n");

}

void loop() {
  //sonarArray.update();

  std::vector<int> frame = canHandler.read();

  if (millis() - lastMillis > 200) {
    sendSonarsValueToCan();
    lastMillis = millis();
  }
  delay(1);
}


void sendSonarsValueToCan()
{
  uint8_t measures[6] = {(uint8_t)sonar0.Ranging(CM), (uint8_t)sonar1.Ranging(CM), (uint8_t)sonar2.Ranging(CM), (uint8_t)sonar3.Ranging(CM),(uint8_t)sonar4.Ranging(CM),(uint8_t)sonar5.Ranging(CM) };
  for (uint8_t i = 0; i < 6; i++) {
    Serial.print(measures[i]);
    Serial.print("\t");
  }
  Serial.println("\n");

  canHandler.send(SONAR_DISTANCE, measures[0], measures[1], measures[2], measures[3], measures[4], measures[5]);

  
  //Serial.print(measures[3]);
  canHandler.flush();
}