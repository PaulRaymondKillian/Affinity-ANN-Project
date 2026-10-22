using System;
using System.Net;
using System.Collections.Generic;

namespace Affinity_DataRetrievalUtility
{
    class Program
    {
        static public string _APIKey = "NAMJN0O7OARDDWZF";
        static public List<string> _StockSymbols;
        static public string _StockListInputFile = "C:\\_KillianIntelligentAnalytics\\ProductionFiles\\UtilityIndustry\\UtilityIndustry_StockList_Large.txt";
        static public string _StockDataOutputFolder = "C:\\_KillianIntelligentAnalytics\\ProductionFiles\\UtilityIndustry";
        static public string _StockDataOutputFileNamePrefix = "UtilityIndustryIndustry_StockList_Large";
        static public string _StockDataExemplarFileNamePrefix = "ExemplarSet_Large";
        static void Main(string[] args)
        {
            Console.WriteLine("Affinity - Data Retrieval Utility");
            

            var DataRetrievalUtility = Affinity_DataRetrievalUtility.DataRetrievalUtilityFactory.Build(StockDataSource.AlphaVantage);

            _StockSymbols = DataRetrievalUtility.LoadStockSymbolsFromFile(_StockListInputFile);
            DataRetrievalUtility.RetrieveStockData(_APIKey, _StockSymbols, DataRetrievalSize.Compact);
            DataRetrievalUtility.ExportStockDataToFile(_StockDataOutputFolder, DataRetrievalUtility.GetStockDataOutputFileName(_StockDataOutputFileNamePrefix));
            DataRetrievalUtility.ExportExemplarSetToFile(_StockDataOutputFolder, DataRetrievalUtility.GetStockDataOutputFileName(_StockDataExemplarFileNamePrefix), 1);

            Console.WriteLine("Processing Complete. Good Luck!");
            Console.WriteLine();
            Console.WriteLine("Press enter to exit the application.");
            string exit = Console.ReadLine();
        }
    }
}
