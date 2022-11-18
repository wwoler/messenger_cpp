#include"Shuffle_functions.h"

int main() {
	setlocale(LC_ALL, "");

	const char * suit[4]{"Черви", "Бубны","Трефы","Пики"};
	const char * face[13]{"Туз","Двойка","Тройка","Четверка","Пятерка","Шестерка","Семёрка","Восмерка","Девятка","Десятка","Валет","Дама","Король"};

	int deck[4][13]{0};
	srand(time(0));
	shuffle(deck);
	deal(deck, face, suit);

	return 0;
}