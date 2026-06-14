#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
size_t count_chars(FILE* a) {
	rewind(a);
	size_t chars = 0;
	char line[4096];
	size_t n;
	while ((n = fread(line, 1, sizeof(line), a)) > 0) {
		chars += n;
	}
	return chars;
}
size_t count_line(FILE* a) {
	rewind(a);
	int lines = 0;
	char line[1000];
	while(fgets(line, 1000, a)) {
		lines++;
	}
	return lines;
}

int main(int argc, char* argv[]) {
	if(!argv[1]) return 1;
	if(strcmp(argv[1], "--help")==0) {
		printf("editor		- opens the file and edits it (ctrl d to stop)\n"
			"filter		- filter anything on argv[2] with the argument passed in argv[3]\n"
			"read		- read a file\n"
			"list		- list all files in argv[2], default its .\n"
			"count		- count things\n"
			"create_t	- create folder\n"
			"create		- create a file\n"
			"hex		- read a file in hex\n"
			"usage = %s editor archive.txt\n", argv[0]);
			return 0;
	}

	if(strcmp(argv[1], "editor") == 0) {
		if(!argv[2]) return printf("%s editor filename\n", argv[0]);
		FILE* f = fopen(argv[2], "w");
		int c;
		for(c; (c = getchar()) != EOF; fputc(c, f));
		fclose(f);
	} else if(strcmp(argv[1], "filter") == 0) {
		FILE* my_file = fopen(argv[2], "r");
		char* line = malloc(sizeof(char)*1000);
		char* line_searched = malloc(sizeof(char)*1000);
		strcpy(line_searched, argv[3]);
		while(fgets(line, 1000, my_file) != NULL){ 
			char *trying = strstr(line, line_searched);
			if(trying != NULL) printf("%s", line);
		}
		free(line);
		free(line_searched);
		fclose(my_file);
		printf("\n");
	} else if(strcmp(argv[1], "read") == 0) {
		FILE* my_file = fopen(argv[2], "r");
		char* line = malloc(sizeof(char)*1000);
		while(fgets(line, 1000, my_file) != NULL){ 
			printf("%s", line);
		}
		free(line);
		fclose(my_file);
	} else if(strcmp(argv[1], "list") == 0) {
		struct dirent *de;
		DIR *dr;
		if(argv[2]) {dr = opendir(argv[2]);} else {dr = opendir(".");}
		while ((de = readdir(dr)) != NULL)
        		printf("%s\n", de->d_name);
		closedir(dr); 
	} else if(strcmp(argv[1], "count") == 0) {
		FILE* file = fopen(argv[2], "r");
		printf("lines: %zu\n", count_line(file));
		printf("bytes: %zu\n", count_chars(file));
	} else if(strcmp(argv[1], "create_t") == 0) {
		mkdir(argv[2], 0700);
	} else if(strcmp(argv[1], "create") == 0) {
		FILE* f = fopen(argv[2], "w");
		printf("%s created\n", argv[2]);
	} else if(strcmp(argv[1], "hex") == 0) {
		FILE* my_file = fopen(argv[2], "r");
		char* line = malloc(sizeof(char)*1000);
		while(fgets(line, 1000, my_file) != NULL){ 
			unsigned char c=0;
			for(c=0; c < strlen(line); ++c) {
				printf("0x%X ", line[c]);
			}
			printf("\n");
		}
		free(line);
		fclose(my_file);
	} else {
		return 1;
	}
	return 0;
}
