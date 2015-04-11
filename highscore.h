#ifndef HIGHSCORE_H
#define HIGHSCORE_H

void initHighscore();
void highscoreUpdateScreen();
void _highscoreUpdateScreen();
void highscoreScrollCircles();
void addScore(UBYTE elapsed_seconds, UBYTE score);
void enterHighscore();

#endif
