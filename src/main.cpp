#include <Arduino.h>
#include <math.h>
#include <TaskScheduler.h>
#include <SerialEchoBeacon.h>

#include <Encoder.h>
#include <PID.h>

#include "MotorDriver.h"

#include "OptoBreaker.h"
#include "MotorPositionInitiator.h"
#include "MotorStateHandler.h"
#include "MotorSinWaver.h"
#include "MotorPIDRegulator.h"

#include <LogStorage.h>
#define LOG Log << "Main: "

#define ENCODER_LEFT_PIN_1 2
#define ENCODER_LEFT_PIN_2 3
#define BREAKER_LEFT_PIN A0

#define ENCODER_RIGHT_PIN_1 10
#define ENCODER_RIGHT_PIN_2 11
#define BREAKER_RIGHT_PIN A1

#define DRIVER_LEFT_PIN_1 4
#define DRIVER_LEFT_PIN_2 5
#define DRIVER_LEFT_PIN_PWM 6

#define DRIVER_RIGHT_PIN_1 8
#define DRIVER_RIGHT_PIN_2 7
#define DRIVER_RIGHT_PIN_PWM 9

SerialEchoBeacon beacon_01(1000, 1);

TaskScheduler sched = TaskScheduler();

OptoBreaker breakerLeft( BREAKER_LEFT_PIN,  );
Encoder encoderLeft( ENCODER_LEFT_PIN_1, ENCODER_LEFT_PIN_2, BREAKER_LEFT_PIN );

OptoBreaker breakerRight( BREAKER_RIGHT_PIN );
Encoder encoderRight( ENCODER_RIGHT_PIN_1, ENCODER_RIGHT_PIN_2, BREAKER_RIGHT_PIN );

double Kp = 1000;
double Ki = 0;
double Kd = 0;
int sampleTime = 200 ;
PID pidLeft(Kp, Ki, Kd, sampleTime, P_ON_E, REVERSE );
PID pidRight(Kp, Ki, Kd, sampleTime, P_ON_E, REVERSE );

MotorStateHandler stateHandlerLeft ( sampleTime, 1 );
MotorStateHandler stateHandlerRight ( sampleTime, 2 );

MotorDriver driverLeft(DRIVER_LEFT_PIN_1, DRIVER_LEFT_PIN_2, DRIVER_LEFT_PIN_PWM );
MotorDriver driverRight(DRIVER_RIGHT_PIN_1, DRIVER_RIGHT_PIN_2, DRIVER_RIGHT_PIN_PWM );

MotorPositionInitiator initiatorLeft(  &stateHandlerLeft, &driverLeft, &breakerLeft, 1 );
MotorPositionInitiator initiatorRight(  &stateHandlerRight, &driverRight, &breakerRight, 2 );

MotorPIDRegulator regulatorLeft( &stateHandlerLeft, &driverLeft, &encoderLeft, &pidLeft, 1 );
MotorPIDRegulator regulatorRight( &stateHandlerRight, &driverRight, &encoderRight, &pidRight, 2 );

void setup() {
  	
	//Initilaize the communication.
	Serial.begin(9600);
	Log << "\n\n\n\n" << "Hello World again!" << endl;

  
	stateHandlerLeft.setInitiator(&initiatorLeft);
	stateHandlerLeft.setMainLoop(&regulatorLeft);
	stateHandlerLeft.startInitiator();
	stateHandlerLeft.init(millis());
	
	stateHandlerRight.setInitiator(&initiatorRight);
	stateHandlerRight.setMainLoop(&regulatorRight);
	stateHandlerRight.startInitiator();
	stateHandlerRight.init(millis());

	//Add the objects to the scheduler
	
  	beacon_01.init(millis());
	//sched.add( &beacon_01 );

	sched.add( &stateHandlerLeft );
	sched.add( &stateHandlerRight );
	
	driverLeft.init();
	driverRight.init();
	

	//Make sure the motors are not running
  	driverLeft.setMotorPWM(0);
	driverRight.setMotorPWM(0);

	regulatorLeft.setWantedPositionRev(-1.4);  
	regulatorRight.setWantedPositionRev(-1.4);  
}

unsigned long int loops = 0;
bool done = false;
void loop() {
	while ( done == false ) {
		if ( loops++ < 1e6 ) {
			sched.run();
		} else {
			done = true;
			unsigned long int totalTime = millis();			
			LOG << "Done with " << loops << " loops in " << totalTime << " ms for a rate of " << (int)floor(loops / (float)totalTime) << " loops/ms" << endl;			
			Log.sendToSerial();

			driverLeft.setMotorPWM(0);
			driverRight.setMotorPWM(0);
		}
	}
}
