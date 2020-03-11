/*
 * listdate.c
 *
 *  Created on: Nov 27, 2018
 *      Author: parallels
 */


#include "listdate.h"

//data member
//----------------------------------------------------------------------------------------
 int shmid;
 board_t     *shmp;//pointer for shared memory;
 board_ent_t *bulletin;//pointer for bolletin in shared memory.
//----------------------------------------------------------------------------------------
 void attach_sm()
{
	    if((shmid = shmget(B_KEY,sizeof(board_t), 0644 | IPC_CREAT)) == -1)///* Get shared memory segment.  */
	    {
	        perror("shmget");
	        exit(1);
	    }

	    // Attach to the segment to get a pointer to it.
	    shmp = shmat(shmid, NULL, 0);//* Attach shared memory segment.  */
	    if (shmp == (void *) -1) {
	       perror("Shared memory attach");
	    }
}
//----------------------------------------------------------------------------------------
void read_from_sm(int argc,char *argv[])
{
	int i;
	date_t start_date,end_date;
	for(i=0 ;i<B_ENT_CNT;i++)//loop for bulletin board.
	{
		if(argc==2)//only start date
		{
			start_date= convert_string_to_date(argv[1]);
			if((shmp->entries[i].date.day==start_date.day)&&(shmp->entries[i].date.month==start_date.month)&&(shmp->entries[i].date.year==start_date.year))
			{
				sleep(1);
				printf("\033[2J\033[;H");
				printf("--------------------------------------------------------------------------------\n");
				printf("Date: %d/%d/%d \n Message: %s \n", shmp->entries[i].date.day,shmp->entries[i].date.month,shmp->entries[i].date.year,shmp->entries[i].text);
				printf("--------------------------------------------------------------------------------\n");
			}
		}
		else//start date and end date
		{
			start_date= convert_string_to_date(argv[1]);
			end_date= convert_string_to_date(argv[2]);
			if(((shmp->entries[i].date.day>=start_date.day)&&(shmp->entries[i].date.month>=start_date.month)&&(shmp->entries[i].date.year>=start_date.year)) && (shmp->entries[i].date.day<=end_date.day)&&(shmp->entries[i].date.month<=end_date.month)&&(shmp->entries[i].date.year<=end_date.year))
			{
				sleep(1);
				printf("\033[2J\033[;H");
				printf("--------------------------------------------------------------------------------\n");
				printf("Date: %d/%d/%d \n Message: %s \n", shmp->entries[i].date.day,shmp->entries[i].date.month,shmp->entries[i].date.year,shmp->entries[i].text);
				printf("--------------------------------------------------------------------------------\n");
			}
		}
	}
}
//----------------------------------------------------------------------------------------
void check_argument(int num_of_argument,char *argv[])
{
	int i,day,month,year;
	char dayy[2],monthh[2],yearr[4],date[10];
	for(i=1;i<num_of_argument;i++)
	{
		day=0;month=0;year=0;
		strncpy(date,argv[i], 10);
		strncpy(dayy,date, 2);
		strncpy(monthh,date+3, 2);
		strncpy(yearr,date+6, 4);

		day = atoi(dayy);
		month = atoi(monthh);
		year = atoi(yearr);

		if(day<1 || day>31 || month<1 || month>12 || year<1900 || year >2019)
		{
			printf("%d/%d/%d is not legal format! pleas insert legal date (1-31/1-12/1990-2018)", day,month,year);
			exit(1);
		}
	}

	date_t temp;
	temp.day=day;
	temp.month=month;
	temp.year=year;
}
//----------------------------------------------------------------------------------------
date_t convert_string_to_date(char* line)
{
	int day=0,month=0,year=0;
	char dayy[2],monthh[2],yearr[4],date[10];
	strncpy(date,line, 10);
	strncpy(dayy,date, 2);
    strncpy(monthh,date+3, 2);
    strncpy(yearr,date+6, 4);

    day = atoi(dayy);
	month = atoi(monthh);
	year = atoi(yearr);

	date_t temp;
	temp.day=day;
	temp.month=month;
	temp.year=year;
	return temp;
}
//----------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	char c;

	if(argc<2 || argc>3)//check number of argument if<2 exit with message for user.
		{
			printf("pleas insert legale arguments !");
			return 1;
		}
	check_argument(argc,argv);
	attach_sm();
    // child process because return value zero
    if (fork() != 0)
    {
    	while(shmp->flag)
    		{
    			read_from_sm(argc,argv);
    			sleep(5);
    		}
    		shmctl(shmid, IPC_RMID, NULL);
    }
    // parent process because return value non-zero.
    else
    {
    	read(0,c,1);
        shmp->flag=0;
    }



}
