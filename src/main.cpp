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

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

// Sonars functions and objects
void sendSonarsValueToCan();

/*SonarArray sonarArray(NB_SONARS,
  SONAR_TRIG_FRONT_L, SONAR_ECHO_FRONT_L,
  SONAR_TRIG_FRONT_R, SONAR_ECHO_FRONT_R,
  SONAR_TRIG_L, SONAR_ECHO_L,
  SONAR_TRIG_R, SONAR_ECHO_R,
  SONAR_TRIG_BACK, SONAR_ECHO_BACK
);*/

Ultrasonic sonars[NB_SONARS] = {
  Ultrasonic(SONAR_TRIG_FRONT_L, SONAR_ECHO_FRONT_L),
  Ultrasonic(SONAR_TRIG_FRONT_R, SONAR_ECHO_FRONT_R),
  Ultrasonic(SONAR_TRIG_BACK_L, SONAR_ECHO_BACK_L),
  Ultrasonic(SONAR_TRIG_BACK_R, SONAR_ECHO_BACK_R),
  Ultrasonic(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT),
  Ultrasonic(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT)
};

std::queue<uint8_t> sonar_raw_data[NB_SONARS];
uint16_t sonar_sum[NB_SONARS];
uint16_t last_sum_send[NB_SONARS];



unsigned long last_send_millis = 0;


CanHandler canHandler;


void setup() {
  canHandler.setup();
  // Serial.begin(115200);

  // Prefill for better code efficiency later
  for (uint8_t i = 0; i < NB_SONARS; i++) {
    for (uint8_t j = 0; j < AVG_SAMPLE; j++) {
      sonar_raw_data[i].push(100);
    }
    sonar_sum[i] = 100 * AVG_SAMPLE;
  }
}

void loop() {

  std::vector<int> frame = canHandler.read();
  if (!frame.empty()) {
    switch(frame[0]) {
      case 0:   // PING
        canHandler.send(PONG, 3, 0);
        canHandler.flush();
        break;
    }
  }

  bool high_priority = false, medium_priority = false;
  for (uint8_t i = 0; i < NB_SONARS; i++) {
    sonar_raw_data[i].push((uint8_t) min(sonars[i].read(), 0xffU));
    sonar_sum[i] += sonar_raw_data[i].back();
    sonar_sum[i] -= sonar_raw_data[i].front();
    sonar_raw_data[i].pop();
    if ((sonar_sum[i] < 30 * AVG_SAMPLE && abs(last_sum_send[i] - sonar_sum[i]) > 2 * AVG_SAMPLE) || (abs(last_sum_send[i] - sonar_sum[i]) > 20 * AVG_SAMPLE)) {
      high_priority = true;
    }
    else if ((sonar_sum[i] < 50 * AVG_SAMPLE && abs(last_sum_send[i] - sonar_sum[i]) > 4 * AVG_SAMPLE) || (abs(last_sum_send[i] - sonar_sum[i]) > 10 * AVG_SAMPLE)) {
      medium_priority = true;
    }
  }

  unsigned long now = millis();
  if ((high_priority && now - last_send_millis > 10) || (medium_priority && now - last_send_millis > 40) || (now - last_send_millis > 200)) {
    // Serial.println(now - last_send_millis);

    sendSonarsValueToCan();
    
    last_send_millis = now;
  }
}


void sendSonarsValueToCan()
{
  canHandler.send(
    SONAR_DISTANCE,
    sonar_sum[0] / AVG_SAMPLE,
    sonar_sum[1] / AVG_SAMPLE,
    sonar_sum[2] / AVG_SAMPLE,
    sonar_sum[3] / AVG_SAMPLE,
    sonar_sum[4] / AVG_SAMPLE,
    sonar_sum[5] / AVG_SAMPLE
  );

  canHandler.flush();

  for(uint8_t i = 0; i < 6; i++) {
    last_sum_send[i] = sonar_sum[i];
  }
}