#include <iostream>
#include <cstdio>
using namespace std;

void DelLastRow() {
	const int maxLines = 100;
	const int maxLen = 256;
	char lines[maxLines][maxLen];
	int lineCount = 0;

	FILE* create = nullptr;
	if (fopen_s(&create, "input.txt", "w") != 0 || create == nullptr) {
		cout << "Haven`t managed creating input.txt" << endl;
		return;
	}
	fputs("It`s an example 1.\n", create);
	fputs("It`s an example 2.\n", create);
	fclose(create);

	FILE* input = nullptr;
	if (fopen_s(&input, "input.txt", "r") != 0 || input == nullptr) {
		cout << "Haven`t managed creating input.txt" << endl;
		return;
	}
	while (fgets(lines[lineCount], maxLen, input)) {
		lineCount++;
		if (lineCount >= maxLines) break;
	}
	fclose(input);

	FILE* output = nullptr;
	if (fopen_s(&output, "output.txt", "w") != 0 || output == nullptr) {
		cout << "Haven`t managed creating input.txt" << endl;
		return;
	}
	for (int i = 0; i < lineCount - 1; ++i) {
		fputs(lines[i], output);
	}
	fclose(output);
	cout << "Last row deleted successfully, result in output.txt" << endl;
}

void FindTheLenght() {
	const int MAX_LEN = 1024;
	char buffer[MAX_LEN];
	int maxLength = 0;

	FILE* file1 = nullptr;
	if (fopen_s(&file1, "input.txt", "r") != 0 || file1 == nullptr) {
		cout << "Haven`t managed opening input.txt\n";
		return;
	}
	while (fgets(buffer, MAX_LEN, file1)) {
		int len = 0;
		while (buffer[len] != '\0' && buffer[len] != '\n') {
			len++;
		}

		if (len > maxLength) {
			maxLength = len;
		}
	}
	fclose(file1);

	cout << "The lenght of the longest row: " << maxLength << endl;
}

int CountWordInFile(const char* filename, const char* word) {
	const int MAX_LEN = 1024;
	char buffer[MAX_LEN];
	int count = 0;

	FILE* file = nullptr;
	if (fopen_s(&file, filename, "r") != 0 || file == nullptr) {
		cout << "Haven`t managed opening file" << filename << endl;
		return -1;
	}

	int wordLen = strlen(word);

	while (fgets(buffer, MAX_LEN, file)) {
		const char* p = buffer;
		while ((p = strstr(p, word)) != nullptr) {
			bool leftOk = (p == buffer) || !isalnum(*(p - 1));
			bool rightOk = !isalnum(*(p + wordLen));
			if (leftOk && rightOk) {
				count++;
			}
			p += wordLen;
		}
	}

	fclose(file);
	return count;
}

int CountAndReplaceWordInFile(const char* filename, const char* wordToFind, const char* wordToReplace) {
	const int MAX_LEN = 1024;
	char buffer[MAX_LEN];
	char newLine[MAX_LEN * 2];
	int count = 0;

	FILE* file = nullptr;
	if (fopen_s(&file, filename, "r") != 0 || file == nullptr) {
		cout << "Haven`t managed opening file " << filename << endl;
		return -1;
	}

	FILE* output = nullptr;
	if (fopen_s(&output, "output.txt", "w") != 0 || output == nullptr) {
		cout << "Haven`t managed creating output.txt\n";
		fclose(file);
		return -1;
	}

	int lenFind = strlen(wordToFind);
	int lenReplace = strlen(wordToReplace);

	while (fgets(buffer, MAX_LEN, file)) {
		int lineLen = strlen(buffer);
		int pos = 0;
		int newPos = 0;

		while (pos < lineLen) {
			bool match = false;
			if (pos + lenFind <= lineLen && strncmp(buffer + pos, wordToFind, lenFind) == 0) {
				bool leftOk = (pos == 0) || !isalnum(buffer[pos - 1]);
				bool rightOk = (pos + lenFind == lineLen) || !isalnum(buffer[pos + lenFind]);

				if (leftOk && rightOk) {
					for (int i = 0; i < lenReplace; i++) {
						newLine[newPos++] = wordToReplace[i];
					}
					pos += lenFind;
					count++;
					match = true;
				}
			}
			if (!match) {
				newLine[newPos++] = buffer[pos++];
			}
		}

		newLine[newPos] = '\0';
		fputs(newLine, output);
	}

	fclose(file);
	fclose(output);

	return count;
}

void CompareFiles(const char* file1Name, const char* file2Name, const char* resultName) {
	const int MAX_LEN = 1024;
	char line1[MAX_LEN];
	char line2[MAX_LEN];

	FILE* file1 = nullptr;
	FILE* file2 = nullptr;
	FILE* result = nullptr;

	if (fopen_s(&file1, file1Name, "r") != 0 || file1 == nullptr) {
		cerr << "Haven`t managed opening " << file1Name << endl;
		return;
	}

	if (fopen_s(&file2, file2Name, "r") != 0 || file2 == nullptr) {
		cerr << "Haven`t managed opening " << file2Name << endl;
		fclose(file1);
		return;
	}

	if (fopen_s(&result, resultName, "w") != 0 || result == nullptr) {
		cerr << "Haven`t managed opening " << resultName << endl;
		fclose(file1);
		fclose(file2);
		return;
	}

	int lineNumber = 1;
	bool identical = true;

	while (fgets(line1, MAX_LEN, file1) && fgets(line2, MAX_LEN, file2)) {
		if (strcmp(line1, line2) != 0) {
			fprintf(result, "Line %d differs:\n", lineNumber);
			fprintf(result, "File1: %s", line1);
			fprintf(result, "File2: %s\n", line2);
			identical = false;
		}
		lineNumber++;
	}

	while (fgets(line1, MAX_LEN, file1)) {
		fprintf(result, "Extra line in %s at line %d:\n", file1Name, lineNumber++);
		fprintf(result, "%s", line1);
		identical = false;
	}

	while (fgets(line2, MAX_LEN, file2)) {
		fprintf(result, "Extra line in %s at line %d:\n", file2Name, lineNumber++);
		fprintf(result, "%s", line2);
		identical = false;
	}

	fclose(file1);
	fclose(file2);
	fclose(result);

	if (identical) {
		cout << "Files are identical\n";
	}
	else {
		cout << "Files are different. The difference: " << resultName << endl;
	}
}

const int CaesarKey = 3;

char caesarEncryptChar(char ch, int key) {
	if (ch >= 'A' && ch <= 'Z') {
		return 'A' + (ch - 'A' + key) % 26;
	}
	if (ch >= 'a' && ch <= 'z') {
		return 'a' + (ch - 'a' + key) % 26;
	}
	return ch;
}

void caesarEncryptFile(const char* inputFileName, const char* outputFileName) {
	const int MAX_LEN = 1024;
	char buffer[MAX_LEN];

	FILE* input = nullptr;
	FILE* output = nullptr;

	if (fopen_s(&input, inputFileName, "r") != 0 || input == nullptr) {
		cout << "Haven`t managed opening file " << inputFileName << endl;
		return;
	}

	if (fopen_s(&output, outputFileName, "w") != 0 || output == nullptr) {
		cout << "Haven`t managed creating file " << outputFileName << endl;
		fclose(input);
		return;
	}

	while (fgets(buffer, MAX_LEN, input)) {
		for (int i = 0; buffer[i] != '\0'; ++i) {
			char encrypted = caesarEncryptChar(buffer[i], CaesarKey);
			fputc(encrypted, output);
		}
	}

	fclose(input);
	fclose(output);

	cout << "Encryption finished successfully! Key = " << CaesarKey << ", result in: " << outputFileName << endl;
}

int main() {

	/*TASK1*/
	/*DelLastRow();*/

	/*TASK2*/
	/*FindTheLenght();*/

	/*TASK3*/
	/*char word[100];
	cout << "Enter word to find: ";
	cin >> word;

	int result = CountWordInFile("input.txt", word);
	if (result >= 0) {
		cout << "Word \"" << word << "\" found in file " << result << " times" << endl;
	}*/

	/*TASK4*/
	//char wordToFind[100], wordToReplace[100];
	//cout << "Enter word to find: ";
	//cin >> wordToFind;
	//cout << "Enter word to replace: ";
	//cin >> wordToReplace;

	//int count = CountAndReplaceWordInFile("input.txt", wordToFind, wordToReplace);
	//if (count >= 0) {
	//	cout << "Found and replaced " << count << " words \"" << wordToFind << "\"." << endl;
	//	cout << "Result saved in output.txt\n";
	//}

	/*TASK5*/
	/*CompareFiles("file1.txt", "file2.txt", "result.txt");*/

	/*TASK6*/
	/*caesarEncryptFile("input.txt", "output.txt");*/
}
