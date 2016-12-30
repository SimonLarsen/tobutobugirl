#ifndef WINSCREEN_H
#define WINSCREEN_H

void drawScore(UBYTE x, UBYTE y, UBYTE value);
void winscreenPlayNote(UBYTE note, UBYTE octave);
void winscreenJingle();
void winscreenTextJingle();
void winscreenShowRank();
void countUpScore(UBYTE x, UBYTE y, UBYTE value, UBYTE delay_time);
void initWinscreen();
void enterWinscreen();

#endif
