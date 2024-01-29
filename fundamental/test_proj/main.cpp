#include <stdio.h>

int main(int argc, char **argv){

	int d = 2;
	printf("this program has a bug\n");

	scanf("%d", &d);

	printf("You gave me : %d", d);
	return 0;

}
