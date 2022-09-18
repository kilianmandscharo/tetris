#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
	srand(time(NULL));
	for (int i = 0; i < 100; i++) {
		int random_number = rand() % 8;	
		printf("%i ", random_number);
	}
}
