using System;

namespace Affinity_LegacyANNBackpropBPROP2FileExporter
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Affinity - LegacyANNBackpropBPROP2FileExporter");
            Console.WriteLine("Created by: Paul Killian");
            Console.WriteLine("");
            Console.WriteLine("");
            Console.WriteLine("Please input the AXS file (Affinity Exemplar Set File:");

            string FilePathAXS = Console.ReadLine();

            //TESTING ONLY
            FilePathAXS = "C:\\_Killian Intelligence Systems\\Affinity\\_DataRetrievalUtility_OutputFiles\\ExemplarSet_04-05-2020 2121PM.AXS";

            //Make sure the inputted file is a valid AXS file
            if (!FilePathAXS.EndsWith(".AXS") && System.IO.File.Exists(FilePathAXS))
            {
                Console.WriteLine("Invalid input file entered. What is wrong with you dude?");
                Environment.Exit(0);
            }

            var LegacyANNBackprop2FileExporter = new Affinity_LegacyAnnBackprop2FileExporter();

            LegacyANNBackprop2FileExporter.ImportAXSFile(FilePathAXS);

            LegacyANNBackprop2FileExporter.ExportToBPROP2File(FilePathAXS);

        }
    }
}
