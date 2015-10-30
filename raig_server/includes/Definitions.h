/*
 * Definitions.h
 *
 *  Created on: 4 Oct 2015
 *      Author: david
 */

#ifndef INCLUDES_DEFINITIONS_H_
#define INCLUDES_DEFINITIONS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

#define MAX_LIVES 12
#define NUM_OF_WORDS (sizeof (word) / sizeof (word [0]))
#define MAXLEN 80 /* Maximum size in the world of Any string */
#define HANGMAN_TCP_PORT 1071

#endif /* INCLUDES_DEFINITIONS_H_ */
