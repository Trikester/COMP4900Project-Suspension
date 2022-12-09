/*
 * Control.c
 *
 *  Created on: Dec 8, 2022
 *      Author: bluep
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#include "Control.h"

double leftOffset = 0.0; //The offset of each wheel as reported by the sensors
double rightOffset = 0.0;

int rideMode = COMFORT_MODE; //The ride rideMode we are in, determines how drastically the system compensates.



int main(void)
{
	//Set off a timer, this will keep going and everytime it goes off itll call the compensation function.
	//And just report what needs to be dont to correct the things
	//should each wheel be its own thread?
	//There will definitely be ipc

	//Status Variables
	//float speed; //Should never go below zero
	//float accel; //Positive for acceleration, negative for deceleration
	//int leftTurn; //How hard we are turning in left direction, 0 is not turning at all, 3 is the hardest turn
	//int rightTurn; //leftTurn and rightTurn cannot both be more than 0 at the same time. Should never be negative




	int readings = 0;






	name_attach_t* attach;
	int rcvid;
	struct sigevent event;
	int timercoid;
	message_t msg;
	timer_t timerid;
	struct itimerspec it;

	void calculateDampening();



	//Using name_attach for Channel creation for easier connections in other processes
	attach = name_attach(NULL,CHANNEL_NAME,0);
	if(attach == NULL){
		return EXIT_FAILURE;
	}


	//Setting up timer that will go off
	//Every time this timer goes off the system checks the sensor heights and continuously outputs what updates should be made
	timercoid = ConnectAttach(0,getpid(),attach->chid,_NTO_SIDE_CHANNEL,0);

	SIGEV_PULSE_INIT(&event,timercoid,SIGEV_PULSE_PRIO_INHERIT,CONTROL_TIMER_PULSE,0);

	timer_create(CLOCK_REALTIME,&event,&timerid);

	it.it_value.tv_sec = 1;
	it.it_value.tv_nsec = 0;
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_nsec = 5000000;

	timer_settime(timerid,0,&it,NULL);




	while (1)
	{
		rcvid = MsgReceive(attach->chid,&msg,sizeof(msg),NULL);

		if(rcvid==0)
		{
			//Pulse received
			switch (msg.pulse.code) {
			case CONTROL_TIMER_PULSE:
				calculateDampening();
				break;

			case MODE_SWITCH_PULSE:
				if (rideMode == COMFORT_MODE)
				{
					rideMode = SPORT_MODE;
					printf("/nThe Ride rideMode has been switched to Sport\n");
				}

				else
				{
					rideMode = COMFORT_MODE;
					printf("/nThe Ride rideMode has been switched to Comfort\n");
				}
				break;

			default:
				printf("Unknown pulse received, code = %d\n", msg.pulse.code);
			}
		}
		else if(rcvid>0)
		{
			//Message received
			readings++;
			printf("%d\n",readings);
			leftOffset = msg.data[0];
			rightOffset = msg.data[1];
			MsgReply (rcvid, EOK, &msg, sizeof (msg));
		}
		else
		{
			printf("Error: Invalid Receive ID\n");
		}
	}

	return 0;
}




void calculateDampening()
{
	double leftForce;
	double rightForce;

	if(leftOffset==0.0)
	{
		leftForce=0.0;
	}
	else
	{
		if(rideMode==COMFORT_MODE)
			leftForce=leftOffset*0.25;
		else
			leftForce=leftOffset*0.5;
	}

	if(rightOffset==0.0)
	{
		rightForce=0.0;
	}
	else
	{
		if(rideMode==COMFORT_MODE)
			rightForce=rightOffset*0.25;
		else
			rightForce=rightOffset*0.5;
	}

	//if(rideMode==COMFORT_MODE)
		//printf("\nRide Mode is: Comfort\n");
	//else
		//printf("\nRide Mode is: Sport\n");
	//printf("The current Left Offset is %f, The required force is %f\n",leftOffset,leftForce);
	//printf("The current Right Offset is %f, The required force is %f\n",rightOffset,rightForce);
}

