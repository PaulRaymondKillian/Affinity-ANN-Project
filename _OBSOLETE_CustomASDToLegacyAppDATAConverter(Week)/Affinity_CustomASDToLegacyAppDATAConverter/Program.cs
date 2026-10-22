using System;

namespace Affinity_CustomASDToLegacyAppDATAConverter7Days
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Affinity - Custom ASD To Legacy App DATA Converter");
            Console.WriteLine("Created by: Paul Killian");
            Console.WriteLine("");
            Console.WriteLine("");
            Console.WriteLine("Please input the ASD file (Affinity Stock Data File):");

            string FilePathASD = Console.ReadLine();

            //TESTING ONLY
            //FilePathASD = "C:\\KIS\\StockDataOutput_04-09-2020 816AM.ASD";
            //FilePathASD = "C:\\_Killian Intelligence Systems\\Affinity\\_DataRetrievalUtility_OutputFiles\\StockDataOutput_04-09-2020 816AM.ASD";
            FilePathASD = "C:\\_Killian Intelligence Systems\\Affinity\\_Verify ASD File Correct\\Test_3Stocks_04-18-2020 1054AM.ASD";

            //Make sure the inputted file is a valid AXS file
            if (!FilePathASD.EndsWith(".ASD") && System.IO.File.Exists(FilePathASD))
            {
                Console.WriteLine("Invalid input file entered. What is wrong with you dude?");
                Environment.Exit(0);
            }

            var CustomASDToLegacyAppDATAUtility7Days = new Affinity_CustomASDToLegacyAppDATAUtility7Days();
            

            //Import the ASD file into the Utility to create a data structure to then use with the export functionality
            CustomASDToLegacyAppDATAUtility7Days.ImportASDFile(FilePathASD);

            Console.WriteLine("");
            Console.WriteLine("Please choose the output format you would like:");
            Affinity_CustomASDToLegacyAppDATAConverter7Days.Affinity_CustomASDToLegacyAppDATAUtility7Days.OutputFormats OutputFormatChosen = CustomASDToLegacyAppDATAUtility7Days.OutputToConsole_OutputFormatsAvailable();

            int NumWinners = 1; //default 1
            double WinnerValue = 100; //default 100
            double LoserValue = 0; //default 0


            //Add error checking to the application later. For now, just assume the user is not an idiot
            Console.WriteLine();
            Console.WriteLine("Input the number of winners in results: ");
            NumWinners = int.Parse(Console.ReadLine());

            Console.WriteLine("Input the winner value: ");
            WinnerValue = double.Parse(Console.ReadLine());

            Console.WriteLine("Input the loser value: ");
            LoserValue = double.Parse(Console.ReadLine());

            CustomASDToLegacyAppDATAUtility7Days.GenerateDataOutputFile(OutputFormatChosen, CustomASDToLegacyAppDATAUtility7Days.BuildOutputFileName(FilePathASD), true, NumWinners, WinnerValue, LoserValue);
        }
    }
}
