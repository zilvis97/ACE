#include <stdio.h>

int input(){
	int x = 0;
	printf("Enter a number to be placed in the Accumulator: ");
	scanf("%d", &x);

	while((x < -32768) || (x > 32767)){
		printf("Your entry is not the correct length.");

		printf("Enter a number to be places in the Accumulator: ");
		scanf("%d", &x);
		}
	}

	return x;
}
