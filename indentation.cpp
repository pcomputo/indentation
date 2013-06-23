/*
 * indent.c
 * Indent sources using '{' and '}'
 * Displays output on screen. Use > to redirect to file.
 */

#include <stdio.h>

/* checks if the character at index i of buffer is ch, provided it is not an escape character */
int check_location(char *buffer, int i, char ch)
{
	if (buffer[i] == ch && (buffer[i-1] != '\\' || (buffer[i-1] == '\\' && buffer[i-2] == '\\')))
		return 1;
	else
		return 0;
}

int main(int argc, char **argv)
{
	FILE *infile;
	char buffer[512];
	int indent = 0, singlequote = 0, doublequote = 0, indentnext = 0;

	if (argc != 2) {
		printf("syntax: %s <file-name>\n", argv[0]);
		return -1;
	}

	infile = fopen(argv[1], "r");
	if (!infile) {
		printf("error: could not open file '%s'\n", argv[1]);
		return -2;
	}

	/* get a line */
	while (fgets(buffer, 512, infile)) {
		int skip = 0, i;

		/* remove spaces and tabs from the beginning of the line */
		while (buffer[skip] != 0 && (buffer[skip] == ' ' || buffer[skip] == '\t'))
			++skip;

		for (i = 0; buffer[i] != 0; ++i) {
			if (check_location(buffer, i, '\"'))
				doublequote = !doublequote;
			else if (check_location(buffer, i, '\''))
				singlequote = !singlequote;

			if (!singlequote && !doublequote) {
				/* unindent this line */
				if (check_location(buffer, i, '}'))
					--indent;
				/* indent next line */
				else if (check_location(buffer, i, '{'))
					indentnext = 1;
			}
		}

		/* output indented line */
		for (i = 0; i < indent; ++i)
			printf("\t");
		printf("%s", buffer + skip);

		if (indentnext) {
			++indent;
			indentnext = 0;
		}
	}

	fclose(infile);
	return 0;
}
