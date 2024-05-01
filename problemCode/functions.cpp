
#include <iostream>

int add(int, int);		//fn prototype
int add(int);			//this called an overloaded fn

/*function add() is overloaded
two definitions with same name but different inputs
which one is called will depend on inputs
i.e., add(2,3) will call the first one
add(2) will call the second one*/

int mul(int, int);
float divide(int, int);

int main() {

	int firstNum = 10, secondNum = 20;

	std::cout << "a+b=" << add(firstNum, secondNum);		//fn call
	std::cout << "\na*b=" << mul(firstNum, secondNum);
	std::cout << "\na/b=" << divide(firstNum, secondNum);
	std::cout << "\nIncrement a=" << add(firstNum);

	return 0;
}

int add(int a, int b) {	//fn definition
	return a + b;
}

int add(int a) {
	return ++a;
}

int mul(int a, int b) {
	return a * b;
}

float divide(int a, int b) {
	if (b != 0)
		return a / float(b);
	
	return 0;
}
