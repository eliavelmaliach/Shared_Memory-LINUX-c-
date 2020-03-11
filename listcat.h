/*
 * listcat.h
 *
 *  Created on: Nov 27, 2018
 *      Author: parallels
 */

#ifndef LISTCAT_H_
#define LISTCAT_H_

#include "board.h"

#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>


#endif /* LISTCAT_H_ */


void attach_sm();
void read_from_sm(int argc,char *argv[]);
void check_argument(int num_of_argument,char *argv[]);

