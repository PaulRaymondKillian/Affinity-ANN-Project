using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;

namespace Affinity_DataRetrievalUtility
{
    public enum StockDataSource
    {
        AlphaVantage    //So far our only data source is from AlphaVantage
    }//Enum StockDataSource

    public enum DataRetrievalSize
    {
        Compact,
        Full
    }//enum DataRetrievalSize

    public struct StockDataDay
    {
        public string Symbol;
        public string Date;
        public string Open;
        public string High;
        public string Low;
        public string Close;
        public string Volume;
    }//Struct - StockDay

    public interface IDataRetrievalUtility
    {
        Boolean RetrieveStockData(string pAPIKey, List<string> pStockSymbols, DataRetrievalSize pDataRetrievalSize);
        void ExportStockDataToFile(string pFilePath, string pFileName, string pFileExtension = "ASD");
        string GetStockDataOutputFileName(string pFileNamePrefix);

        List<string> LoadStockSymbolsFromFile(string pStockListInputFile);

        void ExportExemplarSetToFile(string pFilePath, string pFileName, int pNumWinners, string pFileExtension = "AXS");
    }//IDataRetrievalUtility





    public class DataRetrievalUtility_AlphaVantage : IDataRetrievalUtility
    {
        List<Stock> _Stocks;
        Dictionary<string, Stock> _StockDictionary;
        string[] _StockSymbolListOrder;
        DateTime _StartDate;
        DateTime _EndDate;
        int _SleepTimer = 60000;
        List<string> _StockDataOutputStrings;

        #region Retrieve and Export Stock Data
        public List<string> LoadStockSymbolsFromFile(string pStockListInputFile)
        {
            Console.WriteLine("Reading Stock Symbol File: " + pStockListInputFile);

            List<string> StockSymbols = StockSymbols = new List<string>();

            try
            {
                System.IO.StreamReader file = new System.IO.StreamReader(pStockListInputFile);
                string line;

                while ((line = file.ReadLine()) != null)
                {
                    StockSymbols.Add(line);
                    System.Console.WriteLine("  " + line);
                }

                file.Close();
            }
            catch(Exception e)
            {
                var result = "Failure to load stock input file: " + e.Message;
            }

            return StockSymbols;
        }

        public string GetStockDataOutputFileName(string pFileNamePrefix)
        {
            string FileName;

            FileName = pFileNamePrefix + "_" + DateTime.Now.ToString("MM-dd-yyyy Hmmtt");

            return FileName;
        }

        public Boolean RetrieveStockData(string pAPIKey, List<string> pStockSymbols, DataRetrievalSize pDataRetrievalSize)
        {
            _Stocks = DownloadStockData(pAPIKey, pStockSymbols, pDataRetrievalSize, _SleepTimer);

            foreach(Stock currentStock in _Stocks)
            {
                currentStock.ParseRawStockData();
            }

            return true;
        }//RetrieveStockData

        public List<Stock> DownloadStockData(string pAPIKey, List<string> pStockSymbols, DataRetrievalSize pDataRetrievalSize, int pSleepTimer)
        {
            List<Stock> Stocks = new List<Stock>();
            string results = "";
            Stock currentStock;
            int counter = 0;

            Console.WriteLine("Downloading Stock Data");

            //retrieve the stock data from the data source
            foreach (string currentStockSymbol in pStockSymbols)
            {
                Console.WriteLine(" " + currentStockSymbol);
                currentStock = new Stock();
                using (WebClient client = new WebClient())
                {
                    currentStock._Symbol = currentStockSymbol;
                    results = client.DownloadString($"https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol={currentStockSymbol}&outputsize=compact&apikey={pAPIKey}");
                    currentStock._RawStockData = results;
                    
                    //Add additional wait time
                    System.Threading.Thread.Sleep(1000);
                    
                }//using
                Stocks.Add(currentStock);
                counter += 1;

                if (counter == 5)
                {
                    System.Threading.Thread.Sleep(pSleepTimer);
                    counter = 0;
                }
            }
            return Stocks;
        }//DownloadStockData

        public void ExportStockDataToFile(string pFilePath, string pFileName, string pFileExtension = "AXS")
        {
            SetStockStartEndDates();
            SetStockDictionaryAndSymbolArray();
            StockDataExporter.ToFile(_Stocks, _StockSymbolListOrder, _StockDictionary, pFilePath, pFileName, pFileExtension, _StartDate, _EndDate,ref _StockDataOutputStrings);
        }

        public void ExportExemplarSetToFile(string pFilePath, string pFileName, int pNumWinners, string pFileExtension = "AXS")
        {
            StockExemplarSetExporter.ToFile(pFilePath, pFileName, ref _StockDataOutputStrings, pNumWinners, pFileExtension);
        }

        public void SetStockDictionaryAndSymbolArray()
        {
            //Dictionary<string, Stock> _StockDictionary;
            //string[] _StockSymbolList;

            _StockSymbolListOrder = new string[_Stocks.Count];
            _StockDictionary = new Dictionary<string, Stock>();
            int currentIndex = 0;

            foreach(Stock currentStock in _Stocks)
            {
                _StockSymbolListOrder[currentIndex] = currentStock._Symbol;
                currentIndex++;

                _StockDictionary.Add(currentStock._Symbol, currentStock);
            }//foreach


        }//SetStockDictionaryAndSymbolArray

        //public Boolean IsStockAlreadyInSymbolArray(string pStockSymbol)
        //{
        //    Boolean IsInArray = false;

        //    for(int i=0; i<_StockSymbolList.Length-1; i++)
        //    {
        //        if (_StockSymbolList[i] == pStockSymbol)
        //            IsInArray = true;
        //    }//for

        //    return IsInArray;
        //}//IsStockAlreadyInSymbolArray

        public void SetStockStartEndDates()
        {
            //The Stock list should already be initialized and populated before calling this method.
            DateTime EarliestDate = DateTime.Now;
            DateTime LatestDate = new DateTime(0);

            foreach(Stock currentStock in _Stocks)
            {
                foreach(StockDataDay DataDay in currentStock._StockDataDays)
                {
                    if (DateTime.Parse(DataDay.Date) < EarliestDate)
                    {
                        //if (DataDay.Date == "2019-11-21")
                        //    Console.WriteLine("!!!");

                        EarliestDate = DateTime.Parse(DataDay.Date);
                    }

                    if (DateTime.Parse(DataDay.Date) > LatestDate)
                        LatestDate = DateTime.Parse(DataDay.Date);

                }
            }//foreach

            _StartDate = EarliestDate;
            _EndDate = LatestDate;

        }//SetStockSTartEndDates
        #endregion

        
    }//DataRetrievalUtility_AlphaVantage

    public static class DataRetrievalUtilityFactory
    {
        public static IDataRetrievalUtility Build(StockDataSource pStockDataSource)
        {
            switch (pStockDataSource)
            {
                //For now all this utility supports is AlphaVantage but in the future ideally we will add more sources
                case StockDataSource.AlphaVantage:
                    return new DataRetrievalUtility_AlphaVantage();
                default:
                    return new DataRetrievalUtility_AlphaVantage();
            }

        }//Build

    }//DataRetrievalUtiityFactory

    public class Stock
    {
        public string _Symbol;
        public string _RawStockData;
        public List<string[]> _ParsedStockData;
        public List<StockDataDay> _StockDataDays;
        public Dictionary<string, StockDataDay> _StockDataDaysDictionary;
        public Stock()
        {

        }//class Stock
       
        public void ParseRawStockData()
        {
            _ParsedStockData = ParseRawStockDataByDay(_RawStockData);
            _StockDataDays = RefineRawStockDataByDay(_ParsedStockData, _Symbol);
            _StockDataDaysDictionary = BuildStockDataDaysDictionary(_StockDataDays);
        }

        public string StockDataExport(string pDate)
        {
            string results;

            try
            {
                results = _Symbol + "," + _StockDataDaysDictionary[pDate].Open + "," + _StockDataDaysDictionary[pDate].High + "," + _StockDataDaysDictionary[pDate].Low + "," + _StockDataDaysDictionary[pDate].Close + "," + _StockDataDaysDictionary[pDate].Volume;
            }
            catch(Exception e)
            {
                results = _Symbol + "###" + e.Message.Substring(0, 15);
            }

            return results;
        }//ExportStockData

        static List<string[]> ParseRawStockDataByDay(string pRawStockData)
        {
            List<string[]> results = new List<string[]>();

            string[] RawStockDataSplit = pRawStockData.Split("\n");

            for (int i = 9; i < RawStockDataSplit.Length - 3; i = i + 7)
            {
                string[] currentRawStockDataDay = new string[6];

                for (int j = 0; j < 6; j++)
                {
                    currentRawStockDataDay[j] = RawStockDataSplit[i + j];
                }//for

                results.Add(currentRawStockDataDay);

            }//for

            return results;
        }//RawStockDataByDay


        #region Refine Raw Stock Data from Alpha Vantage

        static List<StockDataDay> RefineRawStockDataByDay(List<string[]> pParsedRawStockDataByDay, string pStockSymbol)
        {
            List<StockDataDay> results = new List<StockDataDay>();

            for (int i = 0; i < pParsedRawStockDataByDay.Count; i++)
            {
                StockDataDay currentStockDataDay = new StockDataDay();
                string[] currentRawStockDataByDay = pParsedRawStockDataByDay[i];

                //Set the Stock symbol
                currentStockDataDay.Symbol = pStockSymbol;

                //Parse the Date string
                currentStockDataDay.Date = ParseOutDate(currentRawStockDataByDay[0]);

                //Parse the Open string
                currentStockDataDay.Open = ParseOutOpen(currentRawStockDataByDay[1]);

                //Parse the High string
                currentStockDataDay.High = ParseOutHigh(currentRawStockDataByDay[2]);

                //Parse the Low string
                currentStockDataDay.Low = ParseOutLow(currentRawStockDataByDay[3]);

                //Parse the Close string
                currentStockDataDay.Close = ParseOutClose(currentRawStockDataByDay[4]);

                //Parse the Volume string
                currentStockDataDay.Volume = ParseOutVolume(currentRawStockDataByDay[5]);

                results.Add(currentStockDataDay);



                ////Parse the Date string
                //currentStockDataDay.Date = ParseOutDate(pParsedRawStockDataByDay[i][0]);

                ////Parse the Open string
                //currentStockDataDay.Open = ParseOutOpen(pParsedRawStockDataByDay[i][1]);

                ////Parse the High string
                //currentStockDataDay.High = ParseOutHigh(pParsedRawStockDataByDay[i][2]);

                ////Parse the Low string
                //currentStockDataDay.Low = ParseOutLow(pParsedRawStockDataByDay[i][3]);

                ////Parse the Close string
                //currentStockDataDay.Close = ParseOutClose(pParsedRawStockDataByDay[i][4]);

                ////Parse the Volume string
                //currentStockDataDay.Volume = ParseOutVolume(pParsedRawStockDataByDay[i][5]);

            }//for

            return results;
        }
        #endregion

        #region ParseOutRawStockDataIndividualDatum

        static string RemoveSpecialCharacters(string pRawString)
        {
            string result = pRawString;

            result = result.Replace("\\", "");
            result = result.Replace(@"\", "");
            result = result.Replace(@":", "");
            result = result.Replace(@"{", "");
            result = result.Replace(@" ", "");
            result = result.Replace(@":", "");
            result = result.Replace(@"""", "");
            result = result.Replace(@",", "");

            return result;
        }//RemoveSpecialCharacters

        static string ParseOutDate(string pRawDate)
        {
            string Date = "N/A";

            Date = RemoveSpecialCharacters(pRawDate);

            return Date;
        }//ParseOutDate

        static string ParseOutOpen(string pRawOpen)
        {
            string Open = "N/A";

            Open = RemoveSpecialCharacters(pRawOpen);
            Open = Open.Replace("1.open", "");

            return Open;
        }//ParseOutOpen

        static string ParseOutHigh(string pRawHigh)
        {
            string High = "N/A";

            High = RemoveSpecialCharacters(pRawHigh);
            High = High.Replace("2.high", "");

            return High;
        }//ParseOutHigh

        static string ParseOutLow(string pRawLow)
        {
            string Low = "N/A";

            Low = RemoveSpecialCharacters(pRawLow);
            Low = Low.Replace("3.low", "");

            return Low;
        }//ParseOutLow

        static string ParseOutClose(string pRawClose)
        {
            string Close = "N/A";

            Close = RemoveSpecialCharacters(pRawClose);
            Close = Close.Replace("4.close", "");

            return Close;
        }//ParseOutClose

        static string ParseOutVolume(string pRawVolume)
        {
            string Volume = "N/A";

            Volume = RemoveSpecialCharacters(pRawVolume);
            Volume = Volume.Replace("5.volume", "");

            return Volume;
        }//ParseOutVolume


        #endregion

        #region Misc Methods
        static string StockToString(StockDataDay pStockDataDay)
        {
            string result = "";

            result = pStockDataDay.Symbol + " - " + pStockDataDay.Date + ": Open=" + pStockDataDay.Open + "; High=" + pStockDataDay.High + "; Low=" + pStockDataDay.Low + "; Close=" + pStockDataDay.Close + "; Volume=" + pStockDataDay.Volume;

            return result;

        }//StockToString

        #endregion

        public Dictionary<string, StockDataDay> BuildStockDataDaysDictionary(List<StockDataDay> pStockDataDays)
        {
            Dictionary<string, StockDataDay> resultDictionary = new Dictionary<string, StockDataDay>();

            foreach(StockDataDay Day in pStockDataDays)
            {
                resultDictionary.Add(Day.Date, Day);
            }//foreach

            return resultDictionary;

        }//CreateStockDataDaysDictionary
    }//class Stock

    public static class StockDataExporter
    {
        public static void ToFile(List<Stock> pStocks, string[] pStockSymbolListOrder, Dictionary<string, Stock> pStockDictionary, string pFilePath, string pFileName, string pFileExtension, DateTime pStartDate, DateTime pEndDate, ref List<string> pStockDataOutputStrings)
        {
            Console.WriteLine("ToFile");
            string CurrentLine = string.Empty;
            pStockDataOutputStrings = new List<string>();

            using (StreamWriter writer = new StreamWriter(pFilePath + "\\" + pFileName + "." + pFileExtension))
            {
                DateTime currentDate = pStartDate;

                Console.WriteLine(currentDate.ToShortDateString());
                do
                {
                    if (VerifyAllStocksHaveDataForThisDate(currentDate.ToString("yyyy-MM-dd"), pStockDictionary, pStockSymbolListOrder))
                    {
                        CurrentLine = BuildStockDataOutputString(pStocks, pStockSymbolListOrder, pStockDictionary, currentDate.ToString("yyyy-MM-dd"));
                        pStockDataOutputStrings.Add(CurrentLine);

                        writer.WriteLine(CurrentLine);

                        Console.WriteLine(currentDate.ToShortDateString());
                    }//VerifyAllStocksHaveDataForThisDate()
                    ///////////////////////////////////////////////////////
                    /////
                    ///
                    //// TO DO!!!! --- create string for stock data output line to be written to file for each date.
                     ///                MUST MAKE SURE THE STOCKS ARE ALWAYS IN THE EXACT SAME ORDER, EVEN IF A STOCK DOESNT EXIST FOR A DATE
                     ///                /////////////////////////////////////////////////////////////////////////

                    currentDate = NextBusinessDate(currentDate);

                } while (currentDate <= pEndDate);

            }//using

        }//Static ToFile

        public static bool VerifyAllStocksHaveDataForThisDate(string pDate, Dictionary<string, Stock> pStockDictionary, string[] pStockSymbolListOrder)
        {
            bool Verified = true;

            for(int i=0; i< pStockSymbolListOrder.Length;i++)
            {
                //Check to see if StockDay object really exists for this date. Some holidays or other events not accounted for could cause
                //a date to expect to have data, but not really have data.
                StockDataDay testStockDay;
                pStockDictionary[pStockSymbolListOrder[i]]._StockDataDaysDictionary.TryGetValue(pDate, out testStockDay);

                if (testStockDay.Date == null)
                {
                    //It is verified that at least one stock does not have stock data for this day. Skip the entire day.
                    //We need all stocks to have data for a day to include it.
                    Verified = false;
                    Console.WriteLine("No stock data for date: " + pDate + ", stock: " + pStockDictionary[pStockSymbolListOrder[i]]._Symbol);
                }

            }


            return Verified;

        }//VerifyAllStocksHaveDataForThisDate()

        public static string BuildStockDataOutputString(List<Stock> pStocks, string[] pStockSymbolListOrder, Dictionary<string, Stock> pStockDictionary, string pDate)
        {

            string result = string.Empty;

            result = pDate + ":";

            for(int i=0; i<pStockSymbolListOrder.Length; i++)
            {
                result += pStockDictionary[pStockSymbolListOrder[i]].StockDataExport(pDate);

                if (i < pStockSymbolListOrder.Length-1)
                    result += "|";
            }//for

            return result;
        }//BuildStockDataOutputString

        public static DateTime NextBusinessDate(DateTime pCurrentDate)
        {
            //if (pCurrentDate.DayOfWeek == DayOfWeek.Friday)
            //    return pCurrentDate.AddDays(3);
            //else
            //    return pCurrentDate.AddDays(1);
            do
            {
                pCurrentDate = pCurrentDate.AddDays(1);

            } while (IsWeekend(pCurrentDate) || IsFederalHoliday(pCurrentDate));

            return pCurrentDate;

        }//NextBusinessDate

        public static bool IsWeekend(DateTime pDate)
        {
            if (pDate.DayOfWeek == DayOfWeek.Saturday || pDate.DayOfWeek == DayOfWeek.Sunday)
                return true;
            else
                return false;
        }//IsWeekend

        /// <summary>
        /// Determines if this date is a federal holiday.
        /// </summary>
        /// <param name="date">This date</param>
        /// <returns>True if this date is a federal holiday</returns>
        public static bool IsFederalHoliday(this DateTime date)
        {
            // to ease typing
            int nthWeekDay = (int)(Math.Ceiling((double)date.Day / 7.0d));
            DayOfWeek dayName = date.DayOfWeek;
            bool isThursday = dayName == DayOfWeek.Thursday;
            bool isFriday = dayName == DayOfWeek.Friday;
            bool isMonday = dayName == DayOfWeek.Monday;
            bool isWeekend = dayName == DayOfWeek.Saturday || dayName == DayOfWeek.Sunday;

            // New Years Day (Jan 1, or preceding Friday/following Monday if weekend)
            if ((date.Month == 12 && date.Day == 31 && isFriday) ||
                (date.Month == 1 && date.Day == 1 && !isWeekend) ||
                (date.Month == 1 && date.Day == 2 && isMonday)) return true;

            // MLK day (3rd monday in January)
            if (date.Month == 1 && isMonday && nthWeekDay == 3) return true;

            // President’s Day (3rd Monday in February)
            if (date.Month == 2 && isMonday && nthWeekDay == 3) return true;

            // Memorial Day (Last Monday in May)
            if (date.Month == 5 && isMonday && date.AddDays(7).Month == 6) return true;

            // Independence Day (July 4, or preceding Friday/following Monday if weekend)
            if ((date.Month == 7 && date.Day == 3 && isFriday) ||
                (date.Month == 7 && date.Day == 4 && !isWeekend) ||
                (date.Month == 7 && date.Day == 5 && isMonday)) return true;

            // Labor Day (1st Monday in September)
            if (date.Month == 9 && isMonday && nthWeekDay == 1) return true;

            // Columbus Day (2nd Monday in October)
            if (date.Month == 10 && isMonday && nthWeekDay == 2) return true;

            // Veteran’s Day (November 11, or preceding Friday/following Monday if weekend))
            if ((date.Month == 11 && date.Day == 10 && isFriday) ||
                (date.Month == 11 && date.Day == 11 && !isWeekend) ||
                (date.Month == 11 && date.Day == 12 && isMonday)) return true;

            // Thanksgiving Day (4th Thursday in November)
            if (date.Month == 11 && isThursday && nthWeekDay == 4) return true;

            // Christmas Day (December 25, or preceding Friday/following Monday if weekend))
            if ((date.Month == 12 && date.Day == 24 && isFriday) ||
                (date.Month == 12 && date.Day == 25 && !isWeekend) ||
                (date.Month == 12 && date.Day == 26 && isMonday)) return true;

            return false;
        }//IsFederalHoliday

    }//Static Class StockDataExporter

    public static class StockExemplarSetExporter
    {
        public static List<ExemplarRow> _ExemplarSet = new List<ExemplarRow>();

        public struct ExemplarRow
        {
            public string Date;
            public List<ExemplarRowStockDayResult> StockDayResults;

            public void SetStockDayResults(List<ExemplarRowStockDayResult> pStockDayResults)
            {
                StockDayResults = pStockDayResults;
            }
        }

        public enum StockDayFitnessCalculationMethod
        {
            IntraDayOpenClose = 0,
            DayToDayCloseClose = 1
        }
        public struct ExemplarRowStockDayResult
        {
            public string Date;
            public string Symbol;
            public double Open;
            public double High;
            public double Low;
            public double Close;
            public int Volume;
            public double FitnessDelta;
            public int Result;

            public void SetStockValues(string pSymbol, double pOpen, double pHigh, double pLow, double pClose, int pVolume)
            {
                Symbol = pSymbol;
                Open = pOpen;
                High = pHigh;
                Low = pLow;
                Close = pClose;
                Volume = pVolume;
            }

            public void SetFitnessDelta(double pFitnessDelta)
            {
                FitnessDelta = pFitnessDelta;
            }

        }
        #region ExemplarSet Export Functions
        public static void ToFile(string pOutputFilePath, string pOutputFileName, ref List<string> pStockDataOutputStrings, int pNumWinners, string pFileExtension = "AXS")
        {
            _ExemplarSet = BuildExemplarSet(ref pStockDataOutputStrings);
            double[] DeltaArray = new double[_ExemplarSet[0].StockDayResults.Count];
            int[] ResultArray = new int[DeltaArray.Length];
            string[] ExemplarSetOutputStringsFull = new string[_ExemplarSet.Count];

            //NEXT TO DO: USE THE EXEMPLAR SET TO CALCULATE THE "RESULT" VALUE FOR EACH STOCK DAY. THEN OUTPUT ALL OF THIS TO A FILE IN AXE FORMAT
            for (int i=0; i<_ExemplarSet.Count; i++)
            {
                //Populate the DeltaArray for this ExemplarRow (Day)
                for(int j=0; j<_ExemplarSet[i].StockDayResults.Count;j++)
                {
                    DeltaArray[j] = _ExemplarSet[i].StockDayResults[j].FitnessDelta;
                }//for

                //The DeltaArray should now be populated with all Delta values in the order they are in the Exemplar Set.
                //Now need to identify the winners and losers
                ResultArray = IdentifyResults(DeltaArray, pNumWinners);

                //Now build the full output string including stock data string and the results into a single string.
                string ExemplarRowOutputStringFull = BuildExemplarRowOutputStringFull(pStockDataOutputStrings[i], ResultArray);

                //Add the full output string to the string exemplar set array to be used for writing to file
                ExemplarSetOutputStringsFull[i] = ExemplarRowOutputStringFull;
            }//for

            //Now the Exemplar Row Output String Full array is fully populated, write it out to a file
            Console.WriteLine("ToFile - Writing File");
            string CurrentLine = string.Empty;
            pStockDataOutputStrings = new List<string>();

            using (StreamWriter writer = new StreamWriter(pOutputFilePath + "\\" + pOutputFileName + "." + pFileExtension))
            {
                for (int i = 0; i < ExemplarSetOutputStringsFull.Length; i++)
                {
                    writer.WriteLine(ExemplarSetOutputStringsFull[i]);
                }//for
            }
        }//ToFile

        public static string BuildExemplarRowOutputStringFull(string pStockDataOutputString, int[] ResultArray)
        {
            string result;

            result = pStockDataOutputString + "=";

            for(int i=0; i<ResultArray.Length; i++)
            {
                result += ResultArray[i];

                if (i < ResultArray.Length - 1)
                    result += ",";
            }//for

            return result;
        }

        public static int[] IdentifyResults(double[] pDeltaArray, int WinnersNumTotal = 1)
        {
            int[] ResultsArray = new int[pDeltaArray.Length];
            int WinnerCounter = 0;
            double CurrentWinnerValue = double.MinValue;
            int CurrentWinnerIndex = -1;
            int[] WinnerIndices = new int[WinnersNumTotal];

            Array.Fill<int>(WinnerIndices, -1);
            for (int x = 0; x < WinnersNumTotal; x++)
            {
                CurrentWinnerIndex = -1;
                CurrentWinnerValue = double.MinValue;
                
                for (int i = 0; i < pDeltaArray.Length; i++)
                {
                    if (pDeltaArray[i] > CurrentWinnerValue && !InWinnerList(i, WinnerIndices))
                    {
                        CurrentWinnerValue = pDeltaArray[i];
                        CurrentWinnerIndex = i;
                    }//if

                }//for
                WinnerIndices[x] = CurrentWinnerIndex;
            }//for


            for (int i = 0; i < WinnerIndices.Length; i++)
                ResultsArray[WinnerIndices[i]] = 1;

            return ResultsArray;
        }//IdentifyResults

        public static bool InWinnerList(int pPotentialWinner, int[] CurrentWinnerIndices)
        {
            bool InWinnerList = false;

            for(int i=0; i<CurrentWinnerIndices.Length; i++)
            {
                if (CurrentWinnerIndices[i] == pPotentialWinner) //NOT NEEDED??? CurrentWinnerIndices[i] != null && 
                    InWinnerList = true;
            }

            return InWinnerList;
        }

        public static List<ExemplarRow> BuildExemplarSet(ref List<string> pStockDataOutputStrings)
        {
            List<ExemplarRow> ExemplarSet = new List<ExemplarRow>();

            foreach (string s in pStockDataOutputStrings)
            {
                //Console.WriteLine(s);

                //Take Stock Data Output line and structure it to create the desired results from the data set to create 
                //the full exemplar set for the ANN network to train upon.

                ExemplarRow newExemplarRow = BuildExemplarRowFromRawStockDataOutputString(s);

                ExemplarSet.Add(newExemplarRow);
            }

            //THIS LOGIC WASNT WORKING. INTRADAY FITNESS CALCS WORK, BUT INTERDAY CALCULATION IS NOT AVAILABLE UNTIL THIS MAIN ISSUE IS FIXED.
            //CalculateExemplarSetFitnessDeltas(ExemplarSet, StockDayFitnessCalculationMethod.IntraDayOpenClose);

            return ExemplarSet;
        }

        public static ExemplarRow BuildExemplarRowFromRawStockDataOutputString(string pRawStockDataOutputString)
        {
            string Date;
            ExemplarRow NewExemplarRow = new ExemplarRow();
            List<ExemplarRowStockDayResult> StockDayResults = new List<ExemplarRowStockDayResult>();

            //First split the Date section from the rest of the Stock Data
            string[] split1 = pRawStockDataOutputString.Split(":");
            Date = split1[0];
            NewExemplarRow.Date = Date;

            //Second, split the Stock Data section by pipes "|" to differentiate the individual stocks of that row
            string[] split2 = split1[1].Split("|");

            
            for(int i=0; i<split2.Length; i++)
            {
                ExemplarRowStockDayResult CurrentStockDayResult = new ExemplarRowStockDayResult();

                string[] split3 = split2[i].Split(",");

                //split3 should be formatted as: 0=Symbol, 1=Open, 2=High, 3=Low, 4=Close, 5=Volume
                CurrentStockDayResult.SetStockValues(split3[0], double.Parse(split3[1]), double.Parse(split3[2]), double.Parse(split3[3]), double.Parse(split3[4]), int.Parse(split3[5]));
                CurrentStockDayResult.Date = Date;
                CurrentStockDayResult.FitnessDelta = CurrentStockDayResult.Close - CurrentStockDayResult.Open;
                StockDayResults.Add(CurrentStockDayResult);
            }

            NewExemplarRow.SetStockDayResults(StockDayResults);

            return NewExemplarRow;
        }

        //THIS METHOD WAS NOT WORKING. IT WAS RECIEVING THE EXEMPLARSET BY REFERENCE, AND UPDATING THE FITNESSDELTA VALUE,
        //BUT THEN THE FITNESSDELTA VALUE NEVER ACTUALLY CHANGED. IDEALLY FIX THIS ISSUE EVENTUALLY, BUT FOR NOT MODIFIED APP
        //TO SET FITNESS VALUE WHILE SETTING ALL THE OTHER VALUES. THIS ALLOWS ONLY INTRA-DAY FITNESS CALCULATIONS AND NOT INTER-DAY CALCS.
        //public static void CalculateExemplarSetFitnessDeltas(List<ExemplarRow> pExemplarSet, StockDayFitnessCalculationMethod pMethod)
        //{
        //    List<ExemplarRow> ExemplarSet = pExemplarSet;
        //    //Take the entire Exemplar Set and calculate the Fitness Deltas on the referenced ExemplarSetObject based on the method chosen

        //    double FitnessDelta = -int.MaxValue;

        //    if (pMethod == StockDayFitnessCalculationMethod.IntraDayOpenClose)
        //    {
        //        //Look at each Stock's values and calculate the Fitness Delta based on its Open price compared to its Close price and set that as its Fitness Delta
        //        for (int i = 0; i < pExemplarSet.Count; i++)
        //        {
        //            for (int j = 0; j < pExemplarSet[i].StockDayResults.Count; j++)
        //            {
        //                FitnessDelta = pExemplarSet[i].StockDayResults[j].Close - pExemplarSet[i].StockDayResults[j].Open;
        //                ExemplarSet[i].StockDayResults[j].SetFitnessDelta(FitnessDelta);

        //                //ExemplarSet[i].StockDayResults[j].FitnessDelta = FitnessDelta;


        //            }//for
        //        }//for

        //    }
        //    else
        //        Console.WriteLine("Error: Only IntraDay Fitness Calculation currently enabled. Please choose intraday fitness calculation.");
        //}//CalculateExemplarSetFitnessDeltas
        #endregion

    }//Static Class StockExemplarSetExporter
}//namespace Affinity_DataRetrievalUtility

