#include <iostream>

const int MAX = 10;

void init_array(int[MAX]);
void print_array(int[MAX]);
void apply_mirror(int[MAX]);

int main() {

	int myArr[MAX];
	init_array(myArr);
	print_array(myArr);
	apply_mirror(myArr);
	print_array(myArr);


	return 0;
}

void init_array(int arr[MAX]) {
	for (int i = 0; i < MAX; i++)
		arr[i] = i*2;
}

void print_array(int arr[MAX]) {
	for (int i = 0; i < MAX; i++)
		std::cout << arr[i] << " ";
}

void apply_mirror(int arr[MAX]) {
	for (int i = 0; i < MAX/2; i++)
		arr[MAX-1-i] = arr[i];
}