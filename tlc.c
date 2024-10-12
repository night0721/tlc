#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

enum esc_st {
	ST_NONE = 0,
	ST_ESC_BEGIN,
	ST_ESC_STRING,
	ST_ESC_CSI,
	ST_ESC_STRING_TERM,
	ST_ESC_CSI_TERM,
	ST_ESC_TERM,
	NUM_ST
};

static enum esc_st find_escape_sequences(int c, enum esc_st st)
{
	if (st == ST_NONE || st == ST_ESC_CSI_TERM) {
		if (c == '\033') {
			return ST_ESC_BEGIN;
		} else {
			return ST_NONE;
		}

	} else if (st == ST_ESC_BEGIN) {
		if (c == '[') {
			return ST_ESC_CSI;
		} else if (c == 'P' || c == ']' || c == 'X' || c == '^' || c == '_') {
			return ST_ESC_STRING;
		} else {
			return ST_ESC_TERM;
		}

	} else if (st == ST_ESC_CSI) {
		if (0x40 <= c && c <= 0x7e) {
			return ST_ESC_CSI_TERM;
		} else {
			return st;
		}

	} else if (st == ST_ESC_STRING) {
		if (c == '\007') {
			return ST_NONE;
		} else if (c == '\033') {
			return ST_ESC_STRING_TERM;
		} else {
			return st;
		}

	} else if (st == ST_ESC_STRING_TERM) {
		if (c == '\\') {
			return ST_NONE;
		} else {
			return ST_ESC_STRING;
		}

	} else {
		return ST_NONE;
	}
}

int main(int argc, char** argv)
{
	int i = 0, l = 0;
	int c;
	srand(time(NULL));
	double offx = rand() % 300;

	while ((c = getchar()) != EOF) {
		int escape_state = find_escape_sequences(c, escape_state);
		if (escape_state == ST_ESC_CSI_TERM) {
			putchar(c);
		}

		if (escape_state == ST_NONE || escape_state == ST_ESC_CSI_TERM) {
			if (c == '\n') {
				l++;
				i = 0;
			} else {
				if (escape_state == ST_NONE) {
					i++;
				}

				float theta = i * 0.23 / 5.0f + l * 0.1 + (offx / RAND_MAX) * M_PI;

				float offset = 0.1;
				unsigned char r = lrintf((offset + (1.0f - offset) * (0.5f + 0.5f * sin(theta))) * 255.0f);
				unsigned char g = lrintf((offset + (1.0f - offset) * (0.5f + 0.5f * sin(theta + 2 * M_PI / 3))) * 255.0f);
				unsigned char b = lrintf((offset + (1.0f - offset) * (0.5f + 0.5f * sin(theta + 4 * M_PI / 3))) * 255.0f);

				printf("\033[38;2;%d;%d;%dm", r, g, b);
			}
		}

		if (escape_state != ST_ESC_CSI_TERM) {
			putchar(c);
		}
	}

	printf("\033[0m");
	return 0;
}
