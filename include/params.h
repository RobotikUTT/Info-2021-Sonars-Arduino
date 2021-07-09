#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "Arduino.h"

#define NB_SONARS	6
#define AVG_SAMPLE	4
#define DISTANCE_THRESHOLD	30
#define SUM_THRESHOLD (AVG_SAMPLE * DISTANCE_THRESHOLD)

#define SONAR_ECHO_FRONT_L  SONAR_1_ECHO
#define SONAR_TRIG_FRONT_L  SONAR_1_TRIG
#define SONAR_ECHO_FRONT_R  SONAR_0_ECHO
#define SONAR_TRIG_FRONT_R  SONAR_0_TRIG
#define SONAR_ECHO_BACK_L   SONAR_2_ECHO
#define SONAR_TRIG_BACK_L   SONAR_2_TRIG
#define SONAR_ECHO_BACK_R   SONAR_5_ECHO
#define SONAR_TRIG_BACK_R   SONAR_5_TRIG
#define SONAR_ECHO_LEFT     SONAR_3_ECHO 
#define SONAR_TRIG_LEFT     SONAR_3_TRIG
#define SONAR_ECHO_RIGHT    SONAR_4_ECHO
#define SONAR_TRIG_RIGHT    SONAR_4_TRIG

#endif