#include <iostream>

using namespace std;

void printThisString(string output)
{
	cout << output << endl;
}

int main()
{
	string output = "Hello world!";

	printThisString(output);
	
	system("pause");
	return 0;
}