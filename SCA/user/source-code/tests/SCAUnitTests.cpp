#include "pch.h"
#include "CppUnitTest.h"
#include "../Senior Project/SCA.h"	//FILE PATH WILL CHANGE?
#include <stdio.h>
#include <fstream>



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SCAUnitTests
{
	TEST_CLASS(SCAUnitTests)
	{
	public:
		
		//loadSourceCode
		TEST_METHOD(LoadSourceCodeTestExisting)
		{
		std:string filePath = "testMain.cpp";		//cpp file containing a user defined function and a main function

			SCA test(filePath);

			Assert::AreEqual((string)"existing", test.loadSourceCode(filePath));	//Should return that the file already exists as we want it
		}
		TEST_METHOD(LoadSourceCodeTestNew)
		{
		std:string filePath = "testNoMain.cpp";		//cpp file containing a user defined function, but no main

			SCA test(filePath);

			test.loadSourceCode(filePath);

			Assert::AreEqual(test.existsFile("newtestNoMain.cpp"), true);			//Should have made a new file called newtestNoMain.cpp

			remove("newtestNoMain.cpp");											//Clean up the file after ourselves
		}
		TEST_METHOD(LoadSourceCodeTestNotExisting)
		{
		std:string filePath = "doesntExist.cpp";

			SCA test(filePath);

			Assert::AreEqual((string)"failed", test.loadSourceCode(filePath));		//Should return "failed" as the file don't exist
		}

		//existsFile
		TEST_METHOD(FileExistsTrue)
		{
			ofstream newFile("newTestFile.txt");									//Make a file so we know it exists
			
			SCA test("newTestFile.txt");

			Assert::AreEqual(test.existsFile("newTestFile.txt"), true);

			remove("newTestFile.txt");												//Clean up
		}
		TEST_METHOD(FileExistsFalse)
		{
			SCA test("thisFileDoesntExist.txt");									//Edge case: someone has this file, for some reason

			Assert::AreEqual(test.existsFile("thisFileDoesntExist.txt"), false);
		}

		//loadTemplateFile
		TEST_METHOD(LoadTemplateFileExists)
		{
		std:string templateTableFile = "placeholder text";	//TODO: get template file that exists to use here for testing purposes

			SCA test("testNoMain.cpp");

			Assert::AreEqual(test.loadTemplateTable(templateTableFile), true);
		}
		TEST_METHOD(LoadTemplateFileNotExists)
		{
		std:string templateTableFile = "notATemplateFile.txt";

			SCA test("testNoMain.cpp");

			Assert::AreEqual(test.loadTemplateTable(templateTableFile), false);
		}

		//matchTemplateFileWithTree

		TEST_METHOD(MatchTemplateFile)
		{
			SCA test("testNoMain.cpp", "templateTableFile Placeholder"); //TODO: template table file that exists to use here

			Assert::AreEqual(test.matchTemplateWithTree(), (string)"some string");
				//function should have predictable output given a constant template table file, just need one here
		}

		//readANTLROutputTree

		TEST_METHOD(ReadANTLROutputTree)
		{
			//Not so sure for this one. should have a set text file tree and then parse the
			//node it gives? I need to talk with Matt for this one.
		}

		//serveCodeToANTLR
		TEST_METHOD(ServeCodeToANTLR)
		{
			SCA test("testNoMain.cpp");
		std:string treeFilePath;
		std:string errorFilePath;
			test.serveCodeToANTLR(treeFilePath, errorFilePath);

			Assert::AreEqual(test.existsFile(treeFilePath), true);
			Assert::AreEqual(test.existsFile(errorFilePath), true);
			
		}
	};
}
