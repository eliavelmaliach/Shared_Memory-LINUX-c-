/*
 * listcat.c
 *
 *  Created on: Nov 27, 2018
 *      Author: parallels
 */
#include "listcat.h"

//data member
//----------------------------------------------------------------------------------------
 char *category[]={"Appliances","Arts Crafts","Cameras","Computers","Electronics","Home Audio Video","Jewelry","Motorcycles","Music","Televisions","Tools"};
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
	int i,j,y,result=0;
	char temp1[B_CAT_LEN + 1];
	char temp2[B_CAT_LEN + 1];
	for(i=0 ;i<B_ENT_CNT;i++)//loop for bulletin board.
	{
		for(j=1;j<argc;j++)//loop for ctegory
		{
			//strncpy(temp1,shmp->entries[i].category,strlen(argv[j]));
			for(y=0;y<(strlen(argv[j]));y++)// This FOR compares the string argv[j] with temp1(shmp->entries[i].category) without strcmp.
			{								//if result==-1 not equal.
				if(argv[j][y] != shmp->entries[i].category[y])
					result=-1;
					//printf("%c%c\n",argv[j][y],shmp->entries[i].category[y]);//result=0;
			}
			if (result!=-1)
			{
				sleep(1);
				printf("\033[2J\033[;H");
				printf("--------------------------------------------------------------------------------\n");
				printf("Category: %s\n Message: %s \n", shmp->entries[i].category,shmp->entries[i].text);
				printf("--------------------------------------------------------------------------------\n");
			}
			result=0;
		}

	}
}
//----------------------------------------------------------------------------------------
void check_argument(int num_of_argument,char *argv[])
{
	int i=0,j=0,result=-1;
	int bool=0;
	for(i=1;i<num_of_argument;i++)
	{
		for(j=0;j<11;j++)
		{
			result=strcmp(argv[i],category[j]);
			if(result==0)
				bool=1;
		}
	}
	if(bool==0)
	{
		printf("pleas cheack your argument!");
		exit(1);
	}
}
//----------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	char c;
	if(argc<2)//check number of argument if<2 exit with message for user.
		{
			printf("pleas insert category name in argument !");
			return 1;
		}
	check_argument(argc,argv);
	attach_sm();
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
	/*while(shmp->flag)
	{
		read_from_sm(argc,argv);
		sleep(5);
	}
	shmctl(shmid, IPC_RMID, NULL);*/
}
