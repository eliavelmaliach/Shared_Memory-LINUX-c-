/*
 * listdate.h
 *
 *  Created on: Nov 27, 2018
 *      Author: parallels
 */

#ifndef LISTDATE_H_
#define LISTDATE_H_

#include "board.h"


#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#endif /* LISTDATE_H_ */


void attach_sm();
void read_from_sm(int argc,char *argv[]);
void check_argument(int num_of_argument,char *argv[]);
date_t convert_string_to_date(char* line);
