#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>

struct termios orig_termios;

void enable_raw();
void disable_raw();

int main(int argc, char *argv[]) {
	if (argc < 1) return -1;
	enable_raw();

	char c;
	char lastc;
	char buff[1024];
	int k = 0;
	
	FILE *f = fopen(argv[1], "r+");

    do {
        c = fgetc(f);
        buff[k] = c;
		write(STDIN_FILENO, &buff[k], 1);
        k++;
    } while (c != EOF); 

    k = 0;

	while (read(STDIN_FILENO, &c, 1) == 1) {
		if (lastc == ':') {
			if (c == 'q') break;
			if (c == 'w') {
				fprintf(f, "%s", buff);
				printf("\tSaved!\n");
			}
		} else {
			if (c != ':') {
				write(STDIN_FILENO, &c, 1);
				buff[k] = c;
				k++;
			}
		}

		lastc  = c;
	}

	fclose(f);
	return 0;
}

void enable_raw() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disable_raw);

	struct termios raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON | ISIG);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw()  {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
