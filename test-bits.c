#include <stdio.h>
#include <string.h>

void printbits(char *wow) {
	printf("\rBits: ");
	for (int n = 0; n < 4; n++) {
		char c = *wow;
		wow++;
		for (int i = 0; i < 8; i++) {
			printf("%d", (c & (1 << (7 - i))) > 0);
		}
		if (n + 1 != 4) {
			printf(" ");
		}
	}
	fflush(stdout);
}

#include <unistd.h>

int main(void) {
	char sex[4];

	memset(&sex, 0, sizeof(sex));
	for (int i = 0; i < 8 * 4; i++) {
		sex[i / 8] |= (1 << ((7 - i) % 8));
		printbits(sex);
		usleep(100000);
	}
	printf("\n");
}
