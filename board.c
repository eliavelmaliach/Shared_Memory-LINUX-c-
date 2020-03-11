/*
 * board.c
 *
 *  Created on: Nov 27, 2018
 *      Author: parallels
 */

#include "board.h"

//data member
//-----------------------------------------------------------------------------------------------
 int shmid;//index=0;
 int index_1=0;
 board_t     *shmp;     //pointer for shared memory;
 board_ent_t *bulletin;//pointer for bolletin in shared memory.
//-----------------------------------------------------------------------------------------------
int creat_shared_mem()
{
    if((shmid = shmget(B_KEY,sizeof(board_t), 0644 | IPC_CREAT)) == -1)///* Get shared memory segment.  */
    {
        perror("shmget");
        exit(1);
    }

    // Attach to the segment to get a pointer to it.
    shmp = shmat(shmid, NULL, 0);///* Attach shared memory segment.  */
    if (shmp == (void *) -1) {
       perror("Shared memory attach");
    }
    shmp->flag=1;
    //printf("%d/n",shmid);
	return shmid;
}
//-----------------------------------------------------------------------------------------------
board_ent_t read_from_file(char* file_name)
{
    date_t date_struct;
    board_ent_t bulletin_struct;
    char line[232];
    FILE *fptr;

    if ((fptr = fopen(file_name, "r")) == NULL)//The fopen function opens a stream for I/O to the file filename, and returns a pointer to the stream.
    {//If the open fails, fopen returns a null pointer.
        printf("Error! opening file");
    	shmctl(shmid, IPC_RMID, NULL);
        exit(1);// Program exits if file pointer returns NULL.
    }

    while(getc(fptr) != EOF && shmp->flag)    // reads text until newline
    {
    fscanf(fptr,"%[^\n]", line);
    date_struct = convert_string_to_date(&line); //convert date to int
    bulletin_struct= build_bulletin(date_struct,&line);//build bulletin
    transfer_bulletin_to_sm(bulletin_struct);
    }
    fclose(fptr);//This function causes stream to be closed

    return bulletin_struct;
}
//-----------------------------------------------------------------------------------------------
void transfer_bulletin_to_sm(board_ent_t bulletin_struct)
{
	 /* Transfer blocks of data  to shared memory */
	if(index_1 < B_ENT_CNT)
		bulletin = shmp->entries+index_1;
	else
		index_1=0;
	//this copies characters from the string from (up to and including the terminating null character) into the string to
	strcpy(bulletin->category,bulletin_struct.category);//The return value is the value of to.
	strcpy(bulletin->text,bulletin_struct.text);//The return value is the value of to.
	bulletin->date=bulletin_struct.date;
	printf("--------------------------------------------------------------------------------\n");
	printf("Writing to Shared Memory entries[%d]: \n Category: %s \n Date: %d/%d/%d\n Message: %s \n",index_1,shmp->entries[index_1].category,shmp->entries[index_1].date.day,shmp->entries[index_1].date.month,shmp->entries[index_1].date.year,shmp->entries[index_1].text);//to remove
	index_1++;
	sleep(1);

}
//-----------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------
board_ent_t build_bulletin(date_t date,char *line)
{
	board_ent_t temp;
    char   category[B_CAT_LEN+1];
    char   text[B_TEXT_LEN + 1];

    strncpy(category,line+12, 18);
    strncpy(text,    line+29, 200);
    strcpy(temp.category,category);
    strcpy(temp.text,text);
    temp.date=date;

    return temp;
}
//-----------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	if(argc<2)
	{
		printf("pleas insert file name!");
		return 1;
	}

	creat_shared_mem();
	while(shmp->flag)
		read_from_file(argv[1]);//change to argv 1
	shmctl(shmid, IPC_RMID, NULL);


}
//-----------------------------------------------------------------------------------------------

