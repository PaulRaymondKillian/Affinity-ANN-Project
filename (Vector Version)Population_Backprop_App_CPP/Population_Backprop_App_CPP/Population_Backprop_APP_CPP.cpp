		/*------------------------------------------------------------------------
    Program :           Program01.cpp
    Written by:         Paul Killian
    Created:            2006-01-25
    Description:        Elementary Multiplication Program
 -------------------------------------------------------------------------*/
#include "Population_Backprop_App_CPP.h"					//Includes header file for program
#include "Population.h"
#include <windows.h>

	vector<string> AppPreviousEntries(3);


enum { Blank, DarkBlue, DarkGreen, DarkLightBlue, DarkRed, Purple, DarkYellow, White, Grey, Blue, Green, LightBlue, Red, Pink, Yellow, BrightWhite };


//////////////////////////
// Begin main of program
int main( )  // alternatively: main(int argc, char *argv[])
{

	HANDLE  hConsole;	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, BrightWhite);

        //ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.RedForte)
        cout << "Population Backprop App - SP (2009)" << endl;
        cout << "Created by: Paul Killian";
        cout << endl;

		cout << "Creating Population Object..." << endl;
		Population* A = new Population("C:\\_KillianIntelligentAnalytics\\ProductionFiles\\UtilityIndustry\\UtilityIndustry_StockList_Large.POP");
        //ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.WhiteForte)

        //Try
        //    LoadPopulationAppDataFile()
        //    Dim Population As New Population.Population(PopFileLocation)

        //Catch ex As Exception
        //    ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.RedForte)
        //    Console.WriteLine(ex.Message)
        //Finally
        //    ConsoleLibrary.SetColor(Console_Library.Console_Library.ConsoleColor.White)
        //    Console.Write("Please Press Enter to Exit...")
        //    Dim z As String = Console.ReadLine()
        //End Try
		//LoadPopulationAppDataFile();
		cout << "\nPress Enter to Exit...";
		char a;
		cin >> a;

    return 0;
}//main()



void LoadPopulationAppDataFile()
{   
  ifstream in("AppPreviousEntries.txt"); // input

  if(!in) {
    cout << "Cannot open test.txt file.\n";
  }
	char buffer[256];
	in.getline(buffer,100);
	AppPreviousEntries[0] = buffer;
	in.getline(buffer,100);
	AppPreviousEntries[1] = buffer;
	in.getline(buffer,100);
	AppPreviousEntries[2] = buffer;

	cout << AppPreviousEntries[0] <<endl<< AppPreviousEntries[1] <<endl<< AppPreviousEntries[2];

    //while (inFile >> x) {
    //    sum = sum + x;
    //}
    //
    in.close();

}//LoadPopulationAppDataFile()