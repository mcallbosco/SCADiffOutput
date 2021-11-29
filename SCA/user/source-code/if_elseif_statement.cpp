#include <iostream>
using namespace std;

int main() {
	int x = 4;
	int y = 3;
	if (x == 4) {
		x++;
	}
	else if (x <= 2) {
		x = x + y;
	}
	else if (x != 0) {
		y = 10 - x;
	}
	else {
		x = y;
	}
	
	
	return 0;
}