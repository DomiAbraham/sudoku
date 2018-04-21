// Pro_Sudoku.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;

extern void initBasicSudoku();
extern int basicSudoku[30][10][10];
extern int basicSudokuNum[30][8];
extern void initBasicSudokuNum();
int sudoku[10][10];
int puzzle[10][10];
bool flag = false;
FILE *outfile;
FILE *infile;
bool validInput = true;

bool checkSudoku(int a[10][10])
{
	int f[10];
	memset(f, 0, sizeof(f));
	int fsum = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (f[a[i][j]] == 0) {
				f[a[i][j]] = 1;
				fsum++;
			}
			else {
				printf("1 %d %d\n", i, j);
				return false;
			}
		}
		fsum = 0;
		memset(f, 0, sizeof(f));
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (f[a[j][i]] == 0) {
				f[a[j][i]] = 1;
				fsum++;
			}
			else {
				printf("2 %d %d\n", i, j);
				return false;
			}
		}
		fsum = 0;
		memset(f, 0, sizeof(f));
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					if (f[a[i * 3 + k][j * 3 + l]] == 0) {
						f[a[i * 3 + k][j * 3 + l]] = 1;
						fsum++;
					}
					else {
						printf("3 %d %d\n", i, j);
						return false;
					}
				}
			}
			fsum = 0;
			memset(f, 0, sizeof(f));
		}
	}
	return true;
}

void updateBasicSudokuNum(int num)
{
	int k = -1;
	for (int i = 5; i >= 0; i--) {
		if (basicSudokuNum[num][i] < basicSudokuNum[num][i + 1]) {
			k = i;
			break;
		}
	}
	if (k == -1) {
		return;
	}
	int min = 10;
	int minNum = 0;
	for (int i = k + 1; i <= 7; i++) {
		if (basicSudokuNum[num][i] < min && basicSudokuNum[num][i] > basicSudokuNum[num][k]) {
			min = basicSudokuNum[num][i];
			minNum = i;
		}
	}
	int tmp = basicSudokuNum[num][minNum];
	basicSudokuNum[num][minNum] = basicSudokuNum[num][k];
	basicSudokuNum[num][k] = tmp;
	for (int i = k + 1; i <= 7; i++) {
		int t = 7 - (i - k - 1);
		tmp = basicSudokuNum[num][i];
		basicSudokuNum[num][i] = basicSudokuNum[num][t];
		basicSudokuNum[num][t] = tmp;
		if (t - i <= 1) {
			break;
		}
	}
}
void createCompleteSudoku(int num)
{
	updateBasicSudokuNum(num);
	int key = basicSudoku[num][0][0];
	int arr[10];
	arr[key] = 6;
	int counter = 0;
	for (int i = 1; i <= 9; i++) {
		if (i != key) {
			arr[i] = basicSudokuNum[num][counter];
			counter++;
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			sudoku[i][j] = arr[basicSudoku[num][i][j]];
		}
	}
}
bool checkPuzzle(int i, int j, int k)
{
	for (int l = 0; l < 9; l++) {
		if (puzzle[i][l] == k)
			return false;
	}
	for (int l = 0; l < 9; l++) {
		if (puzzle[l][j] == k)
			return false;
	}
	for (int l = i - i % 3; l <i - i % 3 + 3; l++) {
		for (int o = j - j % 3; o < j - j % 3 + 3; o++) {
			if (puzzle[l][o] == k)
				return false;
		}
	}
	return true;
}
void solvePuzzle(int x, int y)
{
	for (int i = x; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (puzzle[i][j] != 0)
				continue;
			for (int k = 1; k <= 9; k++) {
				if (checkPuzzle(i, j, k)) {
					puzzle[i][j] = k;
					solvePuzzle(i, j);
					if (flag) return;
					puzzle[i][j] = 0;
				}
			}
			if (puzzle[i][j] == 0)
				return;
		}
	}
	if (puzzle[x][y] == 0)
		return;
	flag = true;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (puzzle[i][j] == 0)
				flag = false;
		}
	}
	if (!checkSudoku(puzzle))
		flag = false;
	if (flag) {
		for (int i = 0; i < 9; i++) {
			fprintf(outfile, "%d", puzzle[i][0]);
			for (int j = 1; j < 9; j++) {
				fprintf(outfile, " %d", puzzle[i][j]);
			}
			fprintf(outfile, "\n");
		}
	}
	fprintf(outfile, "\n");
}
int transArgv2(char *s)
{
	int num = 0;
	for (int i = 0; i < strlen(s); i++) {
		if (s[i] < '0' || s[i] > '9') {
			return -1;
		}
		num = num * 10 + (s[i] - '0');
		if (num > 1000001)
			return -1;
	}
	return num;
}

int main(int argc, char * argv[])
{
	validInput = true;
	clock_t start, finish;
	start = clock();
	initBasicSudokuNum();
	initBasicSudoku();
	char *outfilePath = "sudoku.txt";
	while ((outfile = fopen(outfilePath, "w")) == NULL) {
		printf("cannot open the file sudoku.txt\n");
		exit(0);
	}
	// -c function
	//argc = 3;
	//argv[1] = "-c";
	//argv[2] = "1000000";
	if (argc == 3 && strlen(argv[1]) == 2 && argv[1][0] == '-' && argv[1][1] == 'c') {
		int num = transArgv2(argv[2]);
		if (num == -1 || num == 0) {
			validInput = false;
			printf("error\n");
			return 0;
		}
		string str;
		for (int i = 0; i < num; i++) {
			createCompleteSudoku(i / 40000);
			char str[163] = "";
			int count = 0;
			for (int j = 0; j < 9; j++) {
				str[count++] = '0' + sudoku[j][0];
				for (int k = 1; k < 9; k++) {
					str[count++] = ' ';
					str[count++] = '0' + sudoku[j][k];
				}
				str[count++] = '\n';
			}
			fputs(str, outfile);
			if (i != num - 1) {
				fputc('\n', outfile);
			}
		}
		// -s function
	}
	else if (argc == 3 && strlen(argv[1]) == 2 && argv[1][0] == '-' && argv[1][1] == 's') {
		while ((infile = fopen(argv[2], "r")) == NULL) {
			printf("cannot open the file sudoku.txt\n");
			exit(0);
		}
		int x = 0;
		int num = -1;
		while (fscanf(infile, "%d", &x) != EOF) {
			num++;
			puzzle[num / 9][num % 9] = x;
			if (num == 80) {
				num = -1;
				flag = false;
				solvePuzzle(0, 0);
				if (!flag) {
					validInput = false;
					fclose(infile);
					fclose(outfile);
					return 0;
				}
			}
		}
		fclose(infile);
	}
	else {
		validInput = false;
		printf("error");
	}
	finish = clock();
	printf("time = %.2f", ((double)(finish - start)) / 1000);
	fclose(outfile);
	return 0;
}

