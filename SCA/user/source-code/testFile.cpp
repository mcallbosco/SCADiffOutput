#include <iostream>

using namespace std;

int addInts(int int1, int int2)
{
    return int1 + int2;
}

int multInts(int int1, int int2)
{
    return int1 * int2;
}

double divInts(int int1, int int2)
{
    return int1 / int2;
}

int subInts(int int1, int int2)
{
    return int1 - int2;
}

int main()
{
    int userIn1, userIn2;

    cout << "Input first integer: ";
    cin >> userIn1;
    cout << endl;
    cout << "Input second integer: ";
    cin >> userIn2;
    cout << endl
         << endl;

    cout << "Sum of inputs: " << addInts(userIn1, userIn2) << endl;
    cout << "Product of inputs: " << multInts(userIn1, userIn2) << endl;
    cout << "First input divided by second input: " << divInts(userIn1, userIn2) << endl;
    cout << "Second input divided by first input: " << divInts(userIn2, userIn1) << endl;
    cout << "First input subracted by second input: " << subInts(userIn1, userIn2) << endl;
    cout << "Second input subracted by first input: " << subInts(userIn2, userIn1) << endl;

    system("pause");
    return 0;
}