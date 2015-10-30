/*
 * game.cpp
 *
 *  Created on: 4 Oct 2015
 *      Author: david
 */

#include "../includes/AIManager.h"

#include "../includes/Definitions.h"
#include <syslog.h> // syslog()
#include <string.h> // strlen()
#include <unistd.h> // gethostname(), write()
#include <stdlib.h>

char *word[] = {
# include "../words"
		};

extern time_t time();

void processRequest(int in, int out) {

	char * whole_word, part_word[MAXLEN], guess[MAXLEN], outbuf[MAXLEN];

	int lives = MAX_LIVES;
	int game_state = 'I'; //I = Incomplete
	int i, good_guess, word_length;
	char hostname[MAXLEN];

	gethostname(hostname, MAXLEN);
	sprintf(outbuf, "Playing hangman on host %s: \n \n", hostname);
	write(out, outbuf, strlen(outbuf));


	/* Pick a word at random from the list */
	srand((int) time((long *) 0)); /* randomize the seed */

	whole_word = word[rand() % NUM_OF_WORDS];
	word_length = strlen(whole_word);
	syslog(LOG_USER | LOG_INFO, "server chose hangman word %s", whole_word);

	/* No letters are guessed Initially */
	for (i = 0; i < word_length; i++)
		part_word[i] = '-';

	part_word[i] = '\0';

	sprintf(outbuf, "%s %d \n", part_word, lives);
	write(out, outbuf, strlen(outbuf));

	while (game_state == 'I')
	/* Get a letter from player guess */
	{
		while (read(in, guess, MAXLEN) < 0) {
			if (errno != EINTR)
				exit(4);
			printf("re-read the startin \n");
		} /* Re-start read () if interrupted by signal */
		good_guess = 0;
		for (i = 0; i < word_length; i++) {
			if (guess[0] == whole_word[i]) {
				good_guess = 1;
				part_word[i] = whole_word[i];
			}
		}
		if (!good_guess)
			lives--;
		if (strcmp(whole_word, part_word) == 0)
			game_state = 'W'; /* W ==> User Won */
		else if (lives == 0) {
			game_state = 'L'; /* L ==> User Lost */
			strcpy(part_word, whole_word); /* User Show the word */
		}
		sprintf(outbuf, "%s %d \n", part_word, lives);
		write(out, outbuf, strlen(outbuf));
	}
}



