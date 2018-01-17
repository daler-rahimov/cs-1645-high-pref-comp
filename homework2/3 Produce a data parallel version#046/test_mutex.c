/*
 * Daler Rahimov
 * This program counts the freeq of words from the text files in the directory "./2013"
 * The output is in to 	OUTPUT.TXT
 * Student :	 	Daler Rahimiv
 Observation:	Observation.txt
 Compile : 		gcc serial_word_count.c  (gives warning in strtok_r but its fine, some bug in strtok_r :warning: assignment makes pointer from integer without a cast)
 Run 	: 		./a.out
 Run 10 times: 	./run10.sh (Compile first, chmod +x run10.sh). This gives total time for 10 runs
 Output 	:		output.txt
 Noise	: 		romove.txt holds all the words that need to be removed before counting. This could be edited
 Substitude:		sub.txt holds all substitution word format "word-wordToSubstitude". This file could be edited
 */
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define MAXWORDS	10000
#define MAXSTRING	100

int NUM_FILES;
int MAX_WORD_SIZE = 50;
char *removeData;
char *subData;
char **removeDataArr, **subDataArr;
int numRemoveWord, numSubWord;
char **allFileData;

pthread_t tid_1; // this is main thread
pthread_t tid_2;
pthread_t tid_3;
pthread_t tid_4;
/**
 *  structure holding word frequency information
 */

typedef struct _word {
	char s[MAXSTRING]; /* the word */
	int count; /* number of times word occurs */
} word;

word words[MAXWORDS];
int words_count;
/**
 * This function returns number of files in "./2013" directory
 * In data parallel ONLY MAIN THREAD calls it
 */
int num_files() {
	DIR *d;
	struct dirent *dir;
	d = opendir("./2013");
	int i = 0;
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			i++;
		}
		closedir(d);
	}
	return i;
}

/**
 * This function returns char** pointing to all files names in  "./2013" directory
 * In data parallel ONLY MAIN THREAD calls it
 */
char ** dir_names() {
	int MAX_STRING_LENGTH = 50;
	NUM_FILES = num_files();
	char** sep_foo = calloc(NUM_FILES, sizeof(char*));
	int i = 0;
	for (i = 0; i < NUM_FILES; ++i)
		sep_foo[i] = calloc(MAX_STRING_LENGTH, sizeof(char));
	DIR *d;
	struct dirent *dir;
	d = opendir("./2013");
	if (d) {
		i = 0;
		int j = 0;
		while ((dir = readdir(d)) != NULL) {
			if (j != 2) { // skip . and .. that are considered the file names in the directory
				dir->d_name;
				j++;
				continue;
			}
			strcpy(sep_foo[i], dir->d_name);
			i++;
		}
		closedir(d);
	}
	return sep_foo;
}

/* comparison function for quicksort.  this lets quicksort sort words
 * by descending order of count, i.e., from most to least frequent
 */
int wordcmp(word *a, word *b) {
	if (a->count < b->count)
		return +1;
	if (a->count > b->count)
		return -1;
	return 0;
}

/*
 * This function allocates memory and read a files to that memory and returns pointer
 * to the begging of the data in the files
 * In data parallel ONLY MAIN THREAD calls it
 */
char* read_file(const char *fileName) {
	/* declare a file pointer */
	FILE *infile;
	char *buffer;
	long numbytes;

	/* open an existing file for reading */
	infile = fopen(fileName, "r");
	/* quit if the file does not exist */
	if (infile == NULL) {
		fprintf(stderr, "read_files: error reading file %s\n", fileName);
		exit(1);
	}
	/* Get the number of bytes */
	fseek(infile, 0L, SEEK_END);
	numbytes = ftell(infile);
	/* reset the file position indicator to
	 the beginning of the file */
	fseek(infile, 0L, SEEK_SET);
	/* grab sufficient memory for the
	 buffer to hold the text */
	buffer = (char*) calloc(numbytes + 2, sizeof(char));
	buffer[numbytes + 1] = ' '; // for later easy read
	buffer[numbytes + 2] = '\0'; // for later easy read

	/* memory error */
	if (buffer == NULL) {
		fprintf(stderr, "read_files: memory error");
		exit(1);
	}
	/* copy all the text into the buffer */
	fread(buffer, sizeof(char), numbytes, infile);
	fclose(infile);
	/* confirm we have read the file by
	 outputing it to the console */
//	printf("The file called test.dat contains this text\n\n%s", buffer);
	/* free the memory we used for the buffer */
	return buffer;
}

/**
 * This function put every word in removeData string to an array
 * In data parallel ONLY MAIN THREAD calls it
 */
put_in_arr_remove(char *removeData) {
	int numWords = 1;
	int i = 0;
	for (i = 0; removeData[i] != '\0'; i++) {
		if (removeData[i] == ' ')
			numWords++;
	}
	const char s[2] = " ";
	char** sep_foo = calloc(numWords, sizeof(char*));
	for (i = 0; i < NUM_FILES; ++i)
		sep_foo[i] = calloc(MAXSTRING, sizeof(char));
	char *token;
	/* get the first token */
	token = strtok(removeData, s); // removeData global
	/* walk through other tokens */
	i = 0;
	while (token != NULL) {
		strcpy(sep_foo[i], token);
		i++;
		token = strtok(NULL, s);
	}
	removeDataArr = sep_foo; // asign to global
	numRemoveWord = numWords; // asign to global
	return 0;
}

/**
 * In data parallel ONLY MAIN THREAD calls it
 *
 */
pub_in_arr_sub(char *subData) {
	int numWords = 1;
	int i = 0;
	for (i = 0; subData[i] != '\0'; i++) {
		if (subData[i] == ' ')
			numWords++;
	}
	const char s[2] = " ";
	char** sep_foo = calloc(numWords, sizeof(char*));
	for (i = 0; i < NUM_FILES; ++i)
		sep_foo[i] = calloc(MAXSTRING, sizeof(char));
	char *token;
	/* get the first token */
	token = strtok(subData, s); // removeData global
	/* walk through other tokens */
	i = 0;
	while (token != NULL) {
		strcpy(sep_foo[i], token);
		i++;
		token = strtok(NULL, s);
	}
	subDataArr = sep_foo; // asign to global
	numSubWord = numWords; // asign to global
	return 0;
}

/**
 * make all the letters in s lowercase
 * This function is copied 4 times for better parallel performance
 */
void make_lowercase_t1(char *s) {
	int i;

	for (i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}

/**
 * make all the letters in s lowercase
 */
void make_lowercase_t2(char *s) {
	int i;

	for (i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}

/**
 * make all the letters in s lowercase
 */
void make_lowercase_t3(char *s) {
	int i;

	for (i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}

/**
 * make all the letters in s lowercase
 */
void make_lowercase_t4(char *s) {
	int i;

	for (i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}

/**
 * This fuction compares a given word with words in remove.txt
 * Returns 1 if the word is in remove.txt 0 if not.
 * This function is copied 4 times for better parallel performance
 */
int is_in_remove_t1(const char *word) {
	int i;
	for (i = 0; i < numRemoveWord; i++) {
		if (strcmp(removeDataArr[i], word) == 0)
			return 1;
	}
	return 0;
}

/**
 * This fuction compares a given word with words in remove.txt
 * Returns 1 if the word is in remove.txt 0 if not.
 * This function is copied 4 times for better parallel performance
 */
int is_in_remove_t2(const char *word) {
	int i;
	for (i = 0; i < numRemoveWord; i++) {
		if (strcmp(removeDataArr[i], word) == 0)
			return 1;
	}
	return 0;
}

/**
 * This fuction compares a given word with words in remove.txt
 * Returns 1 if the word is in remove.txt 0 if not.
 * This function is copied 4 times for better parallel performance
 */
int is_in_remove_t3(const char *word) {
	int i;
	for (i = 0; i < numRemoveWord; i++) {
		if (strcmp(removeDataArr[i], word) == 0)
			return 1;
	}
	return 0;
}

/**
 * This fuction compares a given word with words in remove.txt
 * Returns 1 if the word is in remove.txt 0 if not.
 * This function is copied 4 times for better parallel performance
 */
int is_in_remove_t4(const char *word) {
	int i;
	for (i = 0; i < numRemoveWord; i++) {
		if (strcmp(removeDataArr[i], word) == 0)
			return 1;
	}
	return 0;
}

/**
 * This function compares a given word in sub.txt
 * Returns:
 * 		if found a word to be substituted
 * 		if not found NULL pointer
 * 		returned value need to be freed
 * This function is copied 4 times for better parallel performance
 */
char * is_in_sub_t1(const char *word) {
	char *token;
	char *sub_with = calloc(MAX_WORD_SIZE, sizeof(char));
	char sub[MAX_WORD_SIZE];
	strcpy(sub, ""); // to NULL
	int i;
	for (i = 0; i < numSubWord; i++) {
		char *e;
		int index;
		e = strchr(subDataArr[i], '-');
		index = (int) (e - subDataArr[i]);
		strncpy(sub, subDataArr[i] + 0, index - 0);
		strncpy(sub_with, subDataArr[i] + index + 1,
				strlen(subDataArr[i]) - index);
//		printf("%s\n", sub_with);
		if (strcmp(word, sub) == 0) {
			return sub_with;
		}
		memset(sub, '\0', sizeof(sub));
		memset(sub_with, '\0', sizeof(sub_with));
	}
	return NULL;
}

char * is_in_sub_t2(const char *word) {
	char *token;
	char *sub_with = calloc(MAX_WORD_SIZE, sizeof(char));
	char sub[MAX_WORD_SIZE];
	strcpy(sub, ""); // to NULL
	int i;
	for (i = 0; i < numSubWord; i++) {
		char *e;
		int index;
		e = strchr(subDataArr[i], '-');
		index = (int) (e - subDataArr[i]);
		strncpy(sub, subDataArr[i] + 0, index - 0);
		strncpy(sub_with, subDataArr[i] + index + 1,
				strlen(subDataArr[i]) - index);
//		printf("%s\n", sub_with);
		if (strcmp(word, sub) == 0) {
			return sub_with;
		}
		memset(sub, '\0', sizeof(sub));
		memset(sub_with, '\0', sizeof(sub_with));
	}
	return NULL;
}

char * is_in_sub_t3(const char *word) {
	char *token;
	char *sub_with = calloc(MAX_WORD_SIZE, sizeof(char));
	char sub[MAX_WORD_SIZE];
	strcpy(sub, ""); // to NULL
	int i;
	for (i = 0; i < numSubWord; i++) {
		char *e;
		int index;
		e = strchr(subDataArr[i], '-');
		index = (int) (e - subDataArr[i]);
		strncpy(sub, subDataArr[i] + 0, index - 0);
		strncpy(sub_with, subDataArr[i] + index + 1,
				strlen(subDataArr[i]) - index);
//		printf("%s\n", sub_with);
		if (strcmp(word, sub) == 0) {
			return sub_with;
		}
		memset(sub, '\0', sizeof(sub));
		memset(sub_with, '\0', sizeof(sub_with));
	}
	return NULL;
}

char * is_in_sub_t4(const char *word) {
	char *token;
	char *sub_with = calloc(MAX_WORD_SIZE, sizeof(char));
	char sub[MAX_WORD_SIZE];
	strcpy(sub, ""); // to NULL
	int i;
	for (i = 0; i < numSubWord; i++) {
		char *e;
		int index;
		e = strchr(subDataArr[i], '-');
		index = (int) (e - subDataArr[i]);
		strncpy(sub, subDataArr[i] + 0, index - 0);
		strncpy(sub_with, subDataArr[i] + index + 1,
				strlen(subDataArr[i]) - index);
//		printf("%s\n", sub_with);
		if (strcmp(word, sub) == 0) {
			return sub_with;
		}
		memset(sub, '\0', sizeof(sub));
		memset(sub_with, '\0', sizeof(sub_with));
	}
	return NULL;
}

/*
 * This function inserts a word into the list of words.  If the word is
 * not yet in the list, a new slot is used with the count set to 1.
 * Otherwise, the count is simply incremented.
 * This function is THREAD SAFE
 */
void insert_word(word *words, int *n, char *s) {
	pthread_mutex_t lock;
	pthread_mutex_lock(&lock);
	/* linear search for the word */
	int i;
	for (i = 0; i < *n; i++)
		if (strcmp(s, words[i].s) == 0) {

			/* found it?  increment and return. */

			words[i].count++;
			return;
		}

	/* error conditions... */

	if (strlen(s) >= MAXSTRING) {
		printf("%s", s);
		fprintf(stderr, "insert_word: word too long!\n");
		exit(1);
	}
	if (*n >= MAXWORDS) {
		fprintf(stderr, "too many words!\n");
		exit(1);
	}

	/* copy the word into the structure at the first available slot,
	 * i.e., *n
	 */

	strcpy(words[*n].s, s);

	/* this word has occured once up to now, so count = 1 */

	words[*n].count = 1;

	/* one more word */

	(*n)++;
	pthread_mutex_unlock(&lock);
	pthread_mutex_destroy(&lock);
}

/**
 *  return 1 if c is alphabetic (a..z or A..Z), 0 otherwise
 *  This function is copied 4 times for batter performance
 */
int is_alpha_t1(char c) {
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
		return 1;
	return 0;
}
int is_alpha_t2(char c) {
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
		return 1;
	return 0;
}

int is_alpha_t3(char c) {
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
		return 1;
	return 0;
}

int is_alpha_t4(char c) {
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
		return 1;
	return 0;
}

/**
 *  This function is copied 4 times for batter performance
 *  remove the i'th character from the string s */
void remove_char_t1(char *s, int i) {
	while (s[i]) {
		i++;
		s[i - 1] = s[i];
	}
	s[i] = 0;
}

void remove_char_t2(char *s, int i) {
	while (s[i]) {
		i++;
		s[i - 1] = s[i];
	}
	s[i] = 0;
}

void remove_char_t3(char *s, int i) {
	while (s[i]) {
		i++;
		s[i - 1] = s[i];
	}
	s[i] = 0;
}

void remove_char_t4(char *s, int i) {
	while (s[i]) {
		i++;
		s[i - 1] = s[i];
	}
	s[i] = 0;
}

/**
 *  remove non-alphabetic characters from the string s
 *  This function is copied 4 times for batter performance
 */
void remove_non_alpha_t1(char *s) {
	int i;

	for (i = 0; s[i]; i++)
		if (!is_alpha_t1(s[i]))
			remove_char_t1(s, i);
}

void remove_non_alpha_t2(char *s) {
	int i;

	for (i = 0; s[i]; i++)
		if (!is_alpha_t2(s[i]))
			remove_char_t2(s, i);
}
void remove_non_alpha_t3(char *s) {
	int i;

	for (i = 0; s[i]; i++)
		if (!is_alpha_t3(s[i]))
			remove_char_t3(s, i);
}
void remove_non_alpha_t4(char *s) {
	int i;

	for (i = 0; s[i]; i++)
		if (!is_alpha_t4(s[i]))
			remove_char_t4(s, i);
}

void *thread_2_func() {
//	printf("This is 2nd thread\n");
	char *token;
	const char sym[] = " \t\r\n\v\f\\;:/-+=[]()$.,_<>?\"";
	int j;
	for (j = 0; j < (NUM_FILES / 4); j++) { //count first 1/4 of the files
		/* get the first token */
		token = strtok_r(allFileData[j], sym, &allFileData[j]); // removeData global
		while (token != NULL) {
//		printf("%s\n", token);
			/* only insert the word if it's not punctuation */
			if (is_alpha_t2(token[0])) {

				/* get rid of non-letters */

				remove_non_alpha_t2(token);

				/* put this word in the list */
				if (is_in_remove_t2(token) == 0) {
					char *subWith = is_in_sub_t2(token);
					if (subWith != NULL) {
						token = subWith;
					}
					insert_word(words, &words_count, token);
				}
			}
			token = strtok_r(NULL, sym, &allFileData[j]);
		}
	}
}

void *thread_3_func() {
//	printf("This is 3rd thread\n");
	char *token;
	const char sym[] = " \t\r\n\v\f\\;:/-+=[]()$.,_<>?\"";
	int j;
	for (j = (NUM_FILES / 4); j < 2 * (NUM_FILES / 4); j++) { //count from 1/4 to 2*1/4 which is second 1/4
		/* get the first token */
		token = strtok_r(allFileData[j], sym, &allFileData[j]); // removeData global
		while (token != NULL) {
//		printf("%s\n", token);
			/* only insert the word if it's not punctuation */
			if (is_alpha_t3(token[0])) {

				/* get rid of non-letters */
				remove_non_alpha_t3(token);
				/* put this word in the list */
				if (is_in_remove_t3(token) == 0) {
					char *subWith = is_in_sub_t3(token);
					if (subWith != NULL) {
						token = subWith;
					}
					insert_word(words, &words_count, token);
				}
			}
			token = strtok_r(NULL, sym, &allFileData[j]);
		}
	}
}

void *thread_4_func() {
//	printf("This is 4th thread\n");
	char *token;
	const char sym[] = " \t\r\n\v\f\\;:/-+=[]()$.,_<>?\"";
	int j;
	for (j = 2 * (NUM_FILES / 4); j < 3 * (NUM_FILES / 4); j++) { //count from 1/4 to 2*1/4 which is second 1/4
		/* get the first token */
		token = strtok_r(allFileData[j], sym, &allFileData[j]); // removeData global
		while (token != NULL) {
//		printf("%s\n", token);
			/* only insert the word if it's not punctuation */
			if (is_alpha_t4(token[0])) {

				/* get rid of non-letters */
				remove_non_alpha_t4(token);
				/* put this word in the list */
				if (is_in_remove_t4(token) == 0) {
					char *subWith = is_in_sub_t4(token);
					if (subWith != NULL) {
						token = subWith;
					}
					insert_word(words, &words_count, token);
				}
			}
			token = strtok_r(NULL, sym, &allFileData[j]);
		}
	}
}

void thread_1_func() {
	char *token;
	const char sym[] = " \t\r\n\v\f\\;:/-+=[]()$.,_<>?\"";
	int j;
	for (j = 3 * (NUM_FILES / 4); j < NUM_FILES; j++) { // last 1/4
		/* get the first token */
		token = strtok_r(allFileData[j], sym, &allFileData[j]); // removeData global
		while (token != NULL) {
//		printf("%s\n", token);
			/* only insert the word if it's not punctuation */

			if (is_alpha_t1(token[0])) {

				/* get rid of non-letters */
				remove_non_alpha_t1(token);
				/* put this word in the list */
				if (is_in_remove_t1(token) == 0) {
					char *subWith = is_in_sub_t1(token);
					if (subWith != NULL) {
						token = subWith;
					}
					insert_word(words, &words_count, token);
				}
			}
			token = strtok_r(NULL, sym, &allFileData[j]);
		}
	}
}

int main() {
	words_count=0;
	int i;
	// Get files names in the "./2013" directory
	char **filesNames;
	filesNames = dir_names(); // this need to be freed a every pointer it points to as well

	//Read all files to the memory - each files data has a pointer to it for later parallel version
	allFileData = calloc(NUM_FILES, sizeof(char*));
	char *fileName = calloc(60, sizeof(char));
	strcpy(fileName, "./2013/"); // path to files
	for (i = 0; i < NUM_FILES; i++) {
		strcpy(&fileName[7], filesNames[i]);
		allFileData[i] = read_file(fileName);
	}

	//Make all letters lowercase
	for (i = 0; i < NUM_FILES; i++) {
		make_lowercase_t1(allFileData[i]);
	}

	// Read word to be removed and word that needs to be substituted
	removeData = read_file("remove.txt");
	subData = read_file("sub.txt");
	put_in_arr_remove(removeData);
	free(removeData);
	pub_in_arr_sub(subData);
	free(subData);

	//********Start main - word count ect*********//
	tid_1 = pthread_self(); // get the id of the main thread
	int err;
	// create 2nd thread
	err = pthread_create(&(tid_2), NULL, &thread_2_func, NULL);
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));
	// create 3rd thread
	err = pthread_create(&(tid_3), NULL, &thread_3_func, NULL);
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));
	// create 4th thread
	err = pthread_create(&(tid_4), NULL, &thread_4_func, NULL);
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));

	//**** sort 1st thread/main thread 1/4 share *****//
	thread_1_func();
	//********************************************************//
	// waint till all other threads are done
	pthread_join(tid_2, NULL);
	pthread_join(tid_3, NULL);
	pthread_join(tid_4, NULL);

/* sort the list of words by descending frequency */
	int m;
	qsort((void *) words, words_count, sizeof(word),
			(int (*)(const void *, const void *)) wordcmp);

	/*
	 * 10% of all words
	 */
	m = (words_count * 10) / 100;

	/* print the words with their frequencies in a file*/
	FILE *ptr_file;
	ptr_file = fopen("output.txt", "w");
	if (!ptr_file) {
		fprintf(stderr, "main: error opening a file %s\n", "output.txt");
		exit(1);
	}
	for (i = 0; i < m; i++)
//		fprintf(ptr_file, "%-20s%d\n", words[i].s, words[i].count);
		printf("%-20s%d\n", words[i].s, words[i].count);
	fclose(ptr_file);
return 0;
}
