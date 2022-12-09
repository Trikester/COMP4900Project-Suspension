#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#include "Control.h"

int main(void)
{
	char input[20];
	int coid;

	coid = name_open(CHANNEL_NAME,0);

	while(1)
	{
		printf("\nType 'Toggle' to switch ride modes\n");
		scanf("%s",input);

		if(strcmp(input,"Toggle")==0)
		{
			MsgSendPulse(coid,1,MODE_SWITCH_PULSE,0);
		}
		else
		{
			printf("Error, please enter 'Toggle'\n");
		}
	}

	name_close(coid);

	return 0;
}
