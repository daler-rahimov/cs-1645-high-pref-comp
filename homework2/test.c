#include <stdin.h>
#include <string.h>

int main() {
	int i, j;
	char *ptrFound; // pointer to the beginning of the found word
	char *wordToRemove;
	char blancks[] =
			"                                                                "; // replace instead of copying entire string
	char allFileData[] = "this is is i sisis isisi sis sis is "
	ptrFound = allFileData;
	wordToRemove = "is";
	char needle[strlen(wordToRemove) + 2];
	// add spaces like this " word "
	needle[0] = ' ';
	needle[strlen(wordToRemove) + 1] = ' ';
	needle[strlen(wordToRemove) + 2] = '\0';

	strncpy((needle + 1), wordToRemove, strlen(wordToRemove));
//			printf("%s\n", needle);

	//remove all appearance of the word
	while (ptrFound != NULL) {
		ptrFound = strstr(ptrFound, needle);
		if (ptrFound != NULL) {
			strncpy(ptrFound, blancks, strlen(wordToRemove) + 1);
			printf("%s\n", ptrFound);

		}
	} // while (ptrFound !=NULL
	printf("%s\n", allFileData);

	return 0;
}
