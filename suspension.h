/*
 * suspension.h
 *
 *      Author: Craig
 */

#define SERVER_NAME "Suspension"

#ifndef SUSPENSION_H_

typedef union
{
	struct _pulse pulse;
} message_t;

typedef union
{
	int type;
	double data[2];
	struct _pulse pulse;
} message_s;


#endif /* SUSPENSION_H_ */


