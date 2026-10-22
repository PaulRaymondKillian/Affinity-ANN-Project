		/*------------------------------------------------------------------------
    Program :           Program01.cpp
    Written by:         Paul Killian
    Created:            2006-01-25
    Description:        Elementary Multiplication Program
 -------------------------------------------------------------------------*/
#include "Population_Backprop_App_CPP.h"					//Includes header file for program
#include "Population.h"
#include <windows.h>

	vector<string> AppPreviousEntries(10);
	int countPreviousEntries;


enum { Blank, DarkBlue, DarkGreen, DarkLightBlue, DarkRed, Purple, DarkYellow, White, Grey, Blue, Green, LightBlue, Red, Pink, Yellow, BrightWhite };


//////////////////////////
// Begin main of program
int main( )  // alternatively: main(int argc, char *argv[])
{
	

	HANDLE  hConsole;	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, DarkRed);

        //ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.RedForte)
        cout << "Population Backprop App - SP (2009)" << endl;
        cout << "Created by: Paul Killian";
        cout << endl;
		SetConsoleTextAttribute(hConsole, White);

		cout << "Creating Population Object..." << endl;
		//Population* Pop = new Population();
		LoadPopulationAppDataFile();
		string filename;
		filename = InputPopDataFile();

		Population* Pop = new Population(filename);

		cout << "\nPress Enter to Exit...";
		char a;
		cin >> a;

    return 0;
}//main()



void LoadPopulationAppDataFile()
{   
	countPreviousEntries = 0;
  ifstream in("AppPreviousEntries.txt"); // input

  if(!in) {
    cout << "Cannot open test.txt file.\n";
  }
	char buffer[256];

	for(int i=0; i<10; i++)
	{
		in.getline(buffer,100);
		AppPreviousEntries[i] = buffer;
		if(AppPreviousEntries[i] != "")
			countPreviousEntries += 1;
	}
    in.close();

}//LoadPopulationAppDataFile()

string InputPopDataFile()
{
	string file;
	cout << "Previous Pop Data Files: " << endl;
	for(int i=0; i<10; i++)
	{
		if(AppPreviousEntries[i] != "")
			cout << "[" << i+1 << "] - " << AppPreviousEntries[i] << endl;
	}
InputFile:
	cout << "Please Enter Number of Previous Entry to Use or New File Location: ";
	
	string input;
	cin >> input;
	
	int nStart;
	nStart = input.find ( string ( ".POP"));

	if(atoi(input.c_str()) <= countPreviousEntries && atoi(input.c_str()) > 0)
		file = AppPreviousEntries[atoi(input.c_str())-1];
	else if( nStart != -1)
	{
		AddFileToPreviousEntries(input);
		file = input;
	}
	else
	{
		cout << "Invalid Input Entered! Please Re-enter" << endl;
		goto InputFile;
	}

	return file;
	
}//InputPopDataFile()

void AddFileToPreviousEntries( string file )
{
	for(int i=countPreviousEntries-1; i>=0; i--)
		if(i != 9 )
			AppPreviousEntries[i+1] = AppPreviousEntries[i];

	AppPreviousEntries[0] = file;

	//Update Previous Entries File
	ofstream ofile;
	ofile.open("AppPreviousEntries.txt");

	for(int i=0; i<10; i++)
	{
		if( AppPreviousEntries[i] != "" )
			ofile << AppPreviousEntries[i] << endl;
	}
	ofile.close();

}//AddFileToPreviousEntries