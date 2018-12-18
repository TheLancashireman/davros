/* demo.c - demo program to test early davroska stuff on linux host
 *
 * (c) David Haworth
*/
#include <davroska-api.h>
#include <stdio.h>

void main_Init(void);
void main_Loop(void);
void main_Ping(void);
void main_Pong(void);

dv_id_t Init, Loop, Ping, Pong;

dv_id_t Lock1;

int main(int argc, char **argv)
{
	printf("davroska starting ...\n");

	dv_startos(0);

	return 0;
}


void callout_addtasks(dv_id_t mode)
{
	Init = dv_addtask("Init", &main_Init, 3, 1);
	Loop = dv_addtask("Loop", &main_Loop, 1, 1);
	Ping = dv_addtask("Ping", &main_Ping, 2, 1);
	Pong = dv_addtask("Pong", &main_Pong, 3, 3);
}

void callout_addlocks(dv_id_t mode)
{
	Lock1 = dv_addlock("Lock1", 1);
	{
		dv_addlockuser(Lock1, Ping);
		dv_addlockuser(Lock1, Pong);
	}
}

void callout_autostart(dv_id_t mode)
{
	dv_activatetask(Init);
}

void main_Init(void)
{
	dv_statustype_t ee;

	printf("Task Init starting ...\n");

	if ( (ee = dv_activatetask(Loop)) != dv_e_ok )
		printf("Task Init: dv_activatetask(Loop) returned %d\n", ee);

	if ( (ee = dv_activatetask(Ping)) != dv_e_ok )
		printf("Task Init: dv_activatetask(Ping) returned %d\n", ee);

	printf("Task Init terminating ...\n");
	(void)dv_terminatetask();
}

void main_Loop(void)
{
	dv_statustype_t ee;

	printf("Task Loop starting ...\n");

	for ( int i = 0; i < 10; i++ )
		if ( (ee = dv_activatetask(Ping)) != dv_e_ok )
			 printf("Task Loop: dv_activatetask(Ping) returned %d\n", ee);

	printf("Task Loop terminating ...\n");
	(void)dv_terminatetask();
}

static unsigned char x;
static int pongCount;

void main_Ping(void)
{
	dv_statustype_t ee;

	printf("Task Ping starting ...\n");

	if ( (ee = dv_chaintask(Loop)) != dv_e_limit )
		 printf("Task Ping: dv_chaintask(Loop) returned %d\n", ee);

	x++;
	if ( x & 1 )
	{
		ee = dv_chaintask(Ping);
		printf("Task Ping: dv_chaintask(Ping) returned unexpectedly : %d\n", ee);
	}
	else
	{
		pongCount = 0;

		if ( (ee = dv_takelock(Lock1)) != dv_e_ok )
			printf("Task Ping: dv_takelock(Lock1) returned %d\n", ee);
		else
		{
			for ( int i = 0; i < 3; i++ )
			{
				if ( (ee = dv_activatetask(Pong)) != dv_e_ok )
				{
					printf("Task Ping: dv_activatetask(Pong) returned %d\n", ee);
				}
				else
				{
					printf("Task Ping: dv_activatetask(Pong) successful\n");
				}
			}

			if ( (ee = dv_droplock(Lock1)) != dv_e_ok )
				printf("Task Ping: dv_droplock(Lock1) returned %d\n", ee);
		}
	}

	printf("Task Ping terminating ...\n");
	(void)dv_terminatetask();
}

void main_Pong(void)
{
	printf("Task Pong ... %d\n", pongCount++);

	(void)dv_terminatetask();
}

void callout_error(dv_statustype_t e)
{
	printf("callout_error(%d) called\n", e);
}
