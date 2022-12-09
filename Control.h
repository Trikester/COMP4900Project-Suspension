/*
 * Control.h
 *
 *  Created on: Dec 8, 2022
 *      Author: bluep
 */

#ifndef CONTROL_H_
#define CONTROL_H_

//#define MAX_SPEED 120.0

//For leftTurn and rightTurn
//#define SOFT_TURN 1
//#define MED_TURN 2
//#define HARD_TURN 3

//For mode
#define COMFORT_MODE 1
#define SPORT_MODE 2

#define CHANNEL_NAME "Proj"

#define CONTROL_TIMER_PULSE (_PULSE_CODE_MINAVAIL + 7) //Timer pulse code
#define MODE_SWITCH_PULSE (_PULSE_CODE_MINAVAIL + 15)
#define SENSOR_TIMER_PULSE (_PULSE_CODE_MINAVAIL + 10)


//#define SERVER_NAME "Suspension"
#define MAX_DEPTH 15
#define MIN_DEPTH 1


typedef union
{
	struct _pulse pulse;
	int type;
	double data[2];
} message_t;



#endif /* CONTROL_H_ */
