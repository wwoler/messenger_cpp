#include"Shuffle_functions.h"

int main() {
	setlocale(LC_ALL, "");

	const char * suit[4]{"�����", "�����","�����","����"};
	const char * face[13]{"���","������","������","��������","�������","��������","������","��������","�������","�������","�����","����","������"};

	int deck[4][13]{0};
	srand(time(0));
	shuffle(deck);
	deal(deck, face, suit);

	return 0;
}