#include <iostream>

#define MAX 10

struct coupleType{
	int value;
	int ind;
};

int find_largest(int[MAX], int&);
coupleType find_largest(int [MAX]);

int main() {
	int arr[MAX] = { 5,6,3,2,4,9,76,2,3,5 };
	int large = 0;

	std::cout << large << " found at " << find_largest(arr, large) << "\n";

	std::cout << find_largest(arr).value << " found at " << find_largest(arr).ind;

	return 0;
}

int find_largest(int arr[MAX], int &l) {

	int largest = arr[0]; ///or -99999
	int ind = 0;

	for (int i = 0; i < MAX; i++) {
		if (arr[i] > largest) {
			largest = arr[i];
			ind = i;
		}
	}
	l = largest;		//returned passively byRef variables
	return ind;			//returned by the fn
}


coupleType find_largest(int arr[MAX]) {
	coupleType largest;
	largest.value = arr[0];
	largest.ind = 0;

	for (int i = 0; i < MAX; i++) {
		if (arr[i] > largest.value) {
			largest.value = arr[i];
			largest.ind = i;
		}
	}
	return largest;			//returned by the fn
}
