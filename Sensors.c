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


double gen();
void determineOb(double *data);


int main(void) {

	//main controller channel
	int Servercoid;
	message_t Servermsg, Serverply;

	//Open channel to Control
	if((Servercoid = name_open(CHANNEL_NAME, 0)) == -1){
		return EXIT_FAILURE;
	}
	 //*/

	//random number
	srand(time(NULL));

	//timer channel
	int rcvid;
	struct sigevent event;
	int chid, coid;
	message_t msg;
	timer_t timerid;
	struct itimerspec it;

	// create a channel
	chid = ChannelCreate(0);

	//establish a connection to the channel
	coid = ConnectAttach(0,0,chid, _NTO_SIDE_CHANNEL ,0);
	printf("Channel created! \n");

	/*  Timer setup
	 *
	 *
	 */
	SIGEV_PULSE_INIT(&event,coid,  0 ,SENSOR_TIMER_PULSE, 0);
	timer_create(CLOCK_REALTIME,&event, &timerid);

	//1 second delay
	it.it_value.tv_sec = 1;
	it.it_value.tv_nsec = 0;
	//send every 0.05
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_nsec = 5000000;

	if(timer_settime(timerid,0,&it,NULL) == -1){
		return -1;
	}
	//****


	//Send messages to Server
	while (1)
		{

			rcvid = MsgReceive(chid,&msg,sizeof(msg), NULL);
			if(rcvid == 0){ //receive pulse
				switch (msg.pulse.code){

					case SENSOR_TIMER_PULSE: //timer pulse
						Servermsg.type = 1;
						determineOb(Servermsg.data); //Generates random numbers for "obstacle" on road
						//printf("Timer event received! \n");
						MsgSend(Servercoid,&Servermsg,sizeof(Servermsg),&Serverply, sizeof(Serverply));

						break;
					default:
						printf("Unsupported pulse received. \n");
				}
			}

		}




	return EXIT_SUCCESS;

}

//Generates a random floating point number from -max to max
//Returns a double
double gen(double max){
	return  ((float)rand()/(float)(RAND_MAX/(max * 2))) -max;
}


// determines when to generate an object for suspension
// Takes an array of 2 double and sets the values for each index
void determineOb(double *data){
	//srand(time(NULL));
	int mode;

	mode = (int)rand()%(100);

	//printf("mode: %d \n", mode);
	if(mode == 1){
		//large object
		data[0] = gen(MAX_DEPTH);
		data[1] = gen(MAX_DEPTH);
	} else if (mode == 2){
		//medium object
		data[0] = gen((MAX_DEPTH *3)/4);
		data[1] = gen((MAX_DEPTH *3)/4);
	} else if (mode == 3){
		//small object
		data[0] = gen(MAX_DEPTH/2);
		data[1] = gen(MAX_DEPTH/2);
	}else{
		//noise
		data[0] = gen(MIN_DEPTH);
		data[1] = gen(MIN_DEPTH);
	}
}

