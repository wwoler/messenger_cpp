#ifndef SHUFFLE_H
#define SHUFFLE_H

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<iomanip>

void shuffle(int wDeck[][13]) {
	int row, column;

	for (int card{1}; card < 53; card++) {
		do {
			row = rand() % 4;
			column = rand() % 13;
		} while (wDeck[row][column] != 0);
		
		wDeck[row][column] = card;
	}
}

void deal(const int wDeck[][13], const char * wFace[], const char * wSuit[]) {
	for (int card = 1; card < 53; card++)
		for (int row = 0; row < 4; row++)
			for (int column = 0; column < 13; column++)
				if (wDeck[row][column] == card)
					std::cout << std::setw(9) << std::setiosflags(std::ios::right)
					<< wFace[column] << " " << std::setw(3) << std::setiosflags(std::ios::left)
					<< wSuit[row] << (card % 2 == 0 ? '\n' : '\t');
}

#endif
