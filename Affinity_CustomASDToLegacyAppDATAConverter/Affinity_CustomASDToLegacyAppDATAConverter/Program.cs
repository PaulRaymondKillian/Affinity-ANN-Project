using System;

namespace Affinity_CustomASDToLegacyAppDATAConverter
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
            FilePathASD = "C:\\_KillianIntelligentAnalytics\\ProductionFiles\\UtilityIndustry\\UtilityIndustryIndustry_StockList_Large_05-19-2020 1138AM.ASD";

            //Make sure the inputted file is a valid AXS file
            if (!FilePathASD.EndsWith(".ASD") && System.IO.File.Exists(FilePathASD))
            {
                Console.WriteLine("Invalid input file entered. What is wrong with you dude?");
                Environment.Exit(0);
            }

            var CustomASDToLegacyAppDATAUtility = new Affinity_CustomASDToLegacyAppDATAUtility();

            //Import the ASD file into the Utility to create a data structure to then use with the export functionality
            CustomASDToLegacyAppDATAUtility.ImportASDFile(FilePathASD);

            Console.WriteLine("");
            Console.WriteLine("Please choose the output format you would like:");
            Affinity_CustomASDToLegacyAppDATAConverter.Affinity_CustomASDToLegacyAppDATAUtility.OutputFormats OutputFormatChosen = CustomASDToLegacyAppDATAUtility.OutputToConsole_OutputFormatsAvailable();

            int NumWinners = 1; //default 1
            double WinnerValue = 100; //default 100
            double LoserValue = 0; //default 0
            bool TrainingFile = true; //default is that it is a training file


            //Add error checking to the application later. For now, just assume the user is not an idiot
            Console.WriteLine();
            Console.WriteLine("Input the number of winners in results: ");
            NumWinners = int.Parse(Console.ReadLine());

            Console.WriteLine("Input the winner value: ");
            WinnerValue = double.Parse(Console.ReadLine());

            Console.WriteLine("Input the loser value: ");
            LoserValue = double.Parse(Console.ReadLine());

            Console.WriteLine("Is this a training file? (Y/N)");
            //string TrainingFileInput = Console.ReadLine();
            if (Console.ReadLine().ToUpper() == "Y")
                TrainingFile = true;
            else
                TrainingFile = false;

            CustomASDToLegacyAppDATAUtility.GenerateDataOutputFile(OutputFormatChosen, CustomASDToLegacyAppDATAUtility.BuildOutputFileName(FilePathASD), true, NumWinners, WinnerValue, LoserValue, TrainingFile);
        }
    }
}
