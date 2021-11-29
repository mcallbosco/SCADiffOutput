#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

void examCalc(double rawGrades[20][11], double weightGrades[20][4]);
void quizCalc(double rawGrades[20][11], double weightGrades[20][4]);
void hwCalc(double rawGrades[20][11], double weightGrades[20][4]);

int main()
{
	ifstream inData;
	ofstream outData;
	
	cout << fixed << showpoint << setprecision(2);

	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	double weightedSum = 0.0;
	int index = 0;

	inData.open("C:\\Users\\itsto\\Google Drive\\College\\J-term '20\\CSIT 121\\grade.txt");
	
	char letterGrade[20][2];
	char firstName[20][12];
	char lastName[20][14];
	double rawGrades[20][11];
	
	for (k = 0; k < 20; k++)
		for (l = 0; l < 11; l++)
			rawGrades[k][l] = 0.0;
	
	double weightGrades[20][4];
	
	for (k = 0; k < 20; k++)
		for (l = 0; l < 4; l++)
			weightGrades[k][l] = 0.0;

	

	for (i = 0; i < 20; i++)
	{
		inData >> firstName[i];
		inData >> lastName[i];
		for (j = 0; j < 11; j++)
		{
			inData >> rawGrades[i][j];
		}
	}

	examCalc(rawGrades, weightGrades);
	quizCalc(rawGrades, weightGrades);
	hwCalc(rawGrades, weightGrades);

	for (i = 0; i < 20; i++)
	{
		weightedSum = 0.0;
		for (j = 0; j < 3; j++)
		{
			weightedSum = weightedSum + weightGrades[i][j];
		}
		weightGrades[i][4] = weightedSum;
	}

	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 4; j++)
		{
			cout << setw(7) << weightGrades[i][j];
		}
		cout << endl;
	}



	return 0;
}


void examCalc(double inArray[20][11], double outArray[20][4])
{
	static int a = 0;
	static int b = 0;
	static double sum = 0.0;

	for (a = 0; a < 20; a++)
	{
		sum = 0.0;
		for (b = 0; b < 3; b++)
		{
			sum = sum + inArray[a][b];
		}
		outArray[a][0] = (sum / 3.0) * .6;
	}
}

void quizCalc(double inArray[20][11], double outArray[20][4])
{
	static int a = 0;
	static int b = 0;
	static double sum = 0.0;

	for (a = 0; a < 20; a++)
	{
		sum = 0.0;
		for (b = 3; b < 6; b++)
		{
			sum = sum + inArray[a][b];
		}
		outArray[a][1] = (sum / 3.0) * .15;
	}
}

void hwCalc(double inArray[20][11], double outArray[20][4])
{
	static int a = 0;
	static int b = 0;
	static double sum = 0.0;

	for (a = 0; a < 20; a++)
	{
		sum = 0.0;
		for (b = 6; b < 11; b++)
		{
			sum = sum + inArray[a][b];
		}
		outArray[a][2] = (sum / 3.0) * .25;
	}
}