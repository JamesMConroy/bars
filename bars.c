#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 2

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#define OUTFORMAT " %*s | %-*s | %d \n"
#define INFORMAT "%d %ms\n"
// How large can/should this value be set to?
#define MAXITEMS 10000
#define MINPAD 8

struct item
{
	char *label;
	int val;
};

void printHistogram(struct item items[], int n, int col);
void usage(char* name, int v);
int getFileInput(char* argv[], struct item items[]);
int getStdin(char *files[], struct item items[]);

int main(int argc, char *argv[])
{
	// get the terminal width
	struct winsize w;
	ioctl(1, TIOCGWINSZ, &w);
	int col = w.ws_col;
	// default to 80 columns if we can't get the terminal width
	if (col <= 0) { col = 80; }

	int opt;
	while ((opt = getopt(argc, argv, ":hc:")) != -1) {
		switch (opt) {
			case 'h':
				usage(argv[0], 0);
				break;
			case 'c':
				col = atoi(optarg);
				break;
			case ':':
				fprintf(stderr,
						"Option -%c requires an operand\n",
						optopt);
				usage(argv[0], 1);
				break;
			case '?':
				fprintf(stderr,
						"Unrecognized option: '-%c'\n",
						optopt);
				usage(argv[0], 1);
				break;
		}
	}

	struct item items[MAXITEMS];
	int nkeys = 0;

	argv += optind;
	// if there are any args not grabed by getopt treat them as files
	if (*argv) {
		nkeys = getFileInput(argv, items);
	} else { // else get input from stdin
		nkeys = getStdin(argv, items);
	}

	printHistogram(items, nkeys, col);
	return 0;
}

int getStdin(char *files[], struct item items[]) {
	int nkeys = 0;
	while(scanf(INFORMAT, &items[nkeys].val, &items[nkeys].label) != EOF)
		nkeys++;
	return nkeys;
}

int getFileInput(char *files[], struct item items[]) {
	int nkeys = 0;
	for (; *files; files++) {
		FILE *f;
		if ((f = fopen( *files, "r")) == NULL) {
			perror("fopen");
			return errno;
		}
		while(fscanf(f, INFORMAT, &items[nkeys].val,
					&items[nkeys].label) != EOF)
			nkeys++;
		fclose(f);
	}

	return nkeys;
}

int getLargestVal(struct item items[], int n)
{
	// get the largest value from the struct
	int max = 0;
	for (int i = 0; i < n ; i++) {
		if (items[i].val > max)
			max = items[i].val;
	}

	return max;
}

int getLongestLabel(struct item items[], int n)
{
	int lenLabel = 0;
	for (int i = 0; i < n ; i++) {
		if (strlen(items[i].label) > lenLabel) {
			lenLabel = strlen(items[i].label);
		}
	}
	return lenLabel;
}

void printHistogram(struct item items[], int n, int col)
{
	int max = getLargestVal(items, n);
	int lenLabel = getLongestLabel(items, n);
	// get the string len of the largest val
	// XXX This assumes the len of the longest value is also the longest
	int nDigits = floor(log10(abs(max))) + 1;
	// remove the length of the padding / label / value characters
	col -= MINPAD + lenLabel + nDigits;
	for (int i = 0; i < n ; i++) {
		char z[col];
		// get the length of the current bar to print
		float len = col * (items[i].val / (float)max);
		int n = 0;
		for (; n < len ; n++) {
			z[n] = '#';
		}
		z[n] = '\0';

		printf(OUTFORMAT, lenLabel, items[i].label, col, z, items[i].val);
	}
}

void usage(char* name, int v) {
	printf("%s prints a labeled histogram to stdout\n\n", name);
	printf("options:\n");
	printf("\t-%c\t%s\n", 'h', "Print this message");
	printf("\t-%c\t%s\n", 'c', "Set the number of columns to use");
	exit(v);
}


