/*
 * MotorPDRegulator.h
 *
 *  Created on: 18 aug. 2020
 *      Author: Daniel
 */

#ifndef MOTORPIDREGULATOR_H_
#define MOTORPIDREGULATOR_H_

#include "PID.h"
#include "EncoderWrapper.h"
#include "MotorDriver.h"

class MotorPIDRegulator {
public:
	MotorPIDRegulator( MotorDriver* _driver, EncoderWrapper * _encoder, PID* _pid, int _ID );
	virtual void run(unsigned long int now);
	virtual void init();
	void setWantedPositionRev( float _setPointRev, unsigned long int now );
	boolean hasSettled( unsigned long int now );
private:
	double angleDifference( double angle1, double angle2 );
	MotorDriver * driver;
	EncoderWrapper * encoder;
	PID * pid;
	double setPointRev;
	int ID;
	unsigned long int lastChangeSetpointTime;
};

#endif /* MOTORPIDREGULATOR_H_ */
