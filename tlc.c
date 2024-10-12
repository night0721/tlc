#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

void rgb(int i, int *r, int *g, int *b)
{
    float f = 0.1;
    *r = (int)(sin(f * i + 0) * 127 + 128);
    *g = (int)(sin(f * i + 2 * M_PI / 3) * 127 + 128);
    *b = (int)(sin(f * i + 4 * M_PI / 3) * 127 + 128);
}

int main()
{
    if (isatty(fileno(stdin))) {
        fprintf(stderr, "The command is intended to work with pipes.\n");
		exit(1);
    }

    int j = 0;
    int c;
    while ((c = getchar()) != EOF) {
        int r, g, b;
        rgb(j, &r, &g, &b);
        printf("\033[38;2;%d;%d;%dm%c\033[0m", r, g, b, c);
        j++;
    }

    return 0;
}
