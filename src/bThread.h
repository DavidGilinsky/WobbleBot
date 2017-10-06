/*
 * bThread.h
 *
 *  Created on: 19 Oct 2016
 *      Author: pi
 */

#ifndef _RTIMULIB_H
#include "RTIMULib.h"
#endif //RTIMULib.h

#ifndef BTHREAD_H_
#define BTHREAD_H_

typedef struct {
    double windup_guard;
    double proportional_gain;
    double integral_gain;
    double derivative_gain;
    double prev_error;
    double int_error;
    double control;
} PID;

typedef struct {
	 uint8_t outputEnablePin;
	 int motor_left_pin;
	 int motor_right_pin;
	 int motor_left_in1;
	 int motor_left_in2;
	 int motor_right_in1;
	 int motor_right_in2;
	 double motor_stop;
	 double motor_forward_full;
	 double motor_backward_full;
	 double motor_1_pwm,motor_2_pwm;
	 int pwm_avg_count;
	 double G_SI;
	 double PI;
	bool MOTORS_ENABLED;
	bool EXIT_THREAD;
	float ax, ay, az;
	float gx, gy, gz;
	float mx, my, mz;
	float wheel_1_position,wheel_2_position,wheel_1_delta,wheel_2_delta;
	float wheel_1_new,wheel_2_new;
	float wheel_1_position_error,wheel_2_position_error;
	float wheel_1_speed,wheel_2_speed, body_speed, body_delta;
	float wheel_1_speedArray[10];
	float wheel_1_speedAvg;
	int wheelSpeedCount;
	float wheel_2_speedArray[10];
	float wheel_2_speedAvg;
	float motor_1_error,motor_2_error;
	float balance_error;
	double motor_speed_1_in1,motor_speed_1_in2;
	double motor_speed_2_in1,motor_speed_2_in2;
	double motor_pwm_1_prev, motor_pwm_2_prev;
	float roll, pitch, yaw;
	float heading;
	float prev_pitch;
	float prev_yaw;
	float offset[3];
	struct timeval tv;
	struct timeval mainTV;
	float dt, maxdt;
	double maxDeltaT,minDeltaT;
	float mindt;
	unsigned long startTime;
	unsigned long previoustime, currenttime;
	std::atomic<unsigned long> previousMainTime;
	std::atomic<unsigned long> currentMainTime;
	std::atomic<double> dtsumm;
	std::atomic<long double> deltaT;
	int IMUsampleCount;
	int IMUsampleRate;
	uint64_t IMUrateTimer;
	uint64_t IMUdisplayTimer;
	uint64_t IMUnow;
	int isFirst;
	float targetPitch;
	float failsafePitch;
	bool lyingDown;
	PID PID_motor_1;
	PID PID_motor_2;
	PID PID_position_1;
	PID PID_position_2;
	PID PID_bodyV;
	PID PID_wheelV;
	PID PID_yaw;
	PID PID_balance;
	RTIMU_DATA imuData;
	int isMainFirst;
	bool readReady;
	bool newData;
}bThread_struct;



#endif /* BTHREAD_H_ */
