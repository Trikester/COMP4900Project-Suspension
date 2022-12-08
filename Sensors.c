#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#include "suspension.h"

#define TIMER_PULSE_EVENT (_PULSE_CODE_MINAVAIL + 7)

double gen();
void determineOb(double *data);


int main(void) {
	printf("Timer event received! \n");
	//main controller channel
	//struct sigevent event;
	//*


	int Serverchid, Servercoid;
	message_s Servermsg, Serverply;


	if((Servercoid = name_open(SERVER_NAME, 0)) == -1){
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
	SIGEV_PULSE_INIT(&event,coid,  0 ,TIMER_PULSE_EVENT, 0);
	timer_create(CLOCK_REALTIME,&event, &timerid);

	//1 second delay
	it.it_value.tv_sec = 1;
	it.it_value.tv_nsec = 0;
	//send every 0.05
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_nsec = 50000000;

	if(timer_settime(timerid,0,&it,NULL) == -1){
		return -1;
	}
	//****


	//Send messages to Server
	while (1)
		{
			rcvid = MsgReceive(chid,&msg,sizeof(msg), NULL);

			if(rcvid == 0){
				switch (msg.pulse.code){
					case TIMER_PULSE_EVENT:
						Servermsg.type = 1;
						determineOb(Servermsg.data); //Generates random numbers for "obstacle" on road
						printf("Timer event received! \n");
						MsgSend(Servercoid,&Servermsg,sizeof(Servermsg),&Serverply, sizeof(Serverply));
						break;
					default:
						printf("Unsupported pulse received. \n");
				}
			}

		}




	return EXIT_SUCCESS;

}

//Generates a random floating point number
double gen(){
	return  (float)rand()/(float)(RAND_MAX/10.0);;
}


// determines when to generate an object for suspension
void determineOb(double *data){

	data[0] = gen();
	data[1] = gen();
}
