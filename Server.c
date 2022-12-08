/*
 * Server.c
 *      Author: Craig
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/dispatch.h>
#include "suspension.h"

int main(int argc, char **argv){
		name_attach_t *attach;
		int rcvid;
		message_s msg;
		//char *message;
		attach = name_attach(NULL,SERVER_NAME,0);
		if(attach == NULL){
					return EXIT_FAILURE;
			}

		while(1){
			rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);
			printf("msg: %f, %f \n", msg.data[0],msg.data[1]);
			MsgReply (rcvid, EOK, &msg, sizeof (msg));
		}

}
