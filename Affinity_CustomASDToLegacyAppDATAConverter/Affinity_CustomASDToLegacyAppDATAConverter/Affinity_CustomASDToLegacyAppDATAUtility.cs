using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;

namespace Affinity_CustomASDToLegacyAppDATAConverter
{
    class Affinity_CustomASDToLegacyAppDATAUtility
    {


        List<string> _ASDFileStrings = new List<string>();
        List<ASDStockDataRow> _ASDStockDataRows = new List<ASDStockDataRow>();

        public enum OutputFormats
        {
            Method1 = 1,
            Method2 = 2,
            Method3 = 3,
            Method4 = 4,
            Method5 = 5,
            Method6 = 6,
            Method7 = 7,
            Method8 = 8,
            Method9 = 9,
            Method10 = 10,
            Method11 = 11,
            Method12 = 12,
            Method13 = 13,
            Method14 = 14,
            Method15 = 15,
            Method16 = 16
        }//enum - Output Formats

        public void ImportASDFile(string pImportASDFileLocation)
        {
            Console.WriteLine("Importing ASD File:" + pImportASDFileLocation);

            try
            {
                System.IO.StreamReader file = new System.IO.StreamReader(pImportASDFileLocation);
                string line;

                while ((line = file.ReadLine()) != null)
                {
                    _ASDFileStrings.Add(line);
                    //System.Console.WriteLine("  " + line);
                }

                file.Close();
            }
            catch (Exception e)
            {
                var result = "Failure to load stock input file: " + e.Message;
            }

            _ASDStockDataRows = ConstructStockDataRows(_ASDFileStrings);

            Console.WriteLine("Successfully imported ASD file!");

        }//ImportASDFile()

        public string RemoveExtensionFromFile(string pFileName)
        {
            string FileName = string.Empty;

            string[] split = pFileName.Split(".");

            if (split.Length == 2)
                FileName = split[0];
            else
                for (int i = 0; i < split.Length - 2; i++)
                    FileName += split[i];

            return FileName;
        }

        public List<ASDStockDataRow> ConstructStockDataRows(List<string> pASDFileStrings)
        {
            List<ASDStockDataRow> ASDStockDataRowList = new List<ASDStockDataRow>();
            ASDStockDataRow CurrentASDStockDataRow;
            ASDStockDataRow Previous1ASDStockDataRow = new ASDStockDataRow();

            for (int i = 0; i < pASDFileStrings.Count; i++)
            {
                CurrentASDStockDataRow = new ASDStockDataRow();

                //Set the basic values for the Stock Data Row (not references to the Prev and Next StockDataRows)
                CurrentASDStockDataRow.SetASDStockDataRowValues(pASDFileStrings[i]);

                //Set the Previous Stock Data Row 
                CurrentASDStockDataRow._PrevStockDataRow = Previous1ASDStockDataRow;

                //Set the Previous Stock Data Row to this Stock Data Row
                Previous1ASDStockDataRow._NextStockDataRow = CurrentASDStockDataRow;


                //Increment the stock data day row objects for use of references to the queue structure
                Previous1ASDStockDataRow = CurrentASDStockDataRow;


                ASDStockDataRowList.Add(CurrentASDStockDataRow);

            }//for

            SetStockDataRowsPrevNextReferences(ASDStockDataRowList);

            return ASDStockDataRowList;

        }//ConstructStockDataRows

        public void SetStockDataRowsPrevNextReferences(List<ASDStockDataRow> pASDStockDataRowList)
        {
            //It is expected at this time for the ASTStockDataRowList to be fully populated and each stock in the row to have basic values.
            //This method will set the stocks to the prev and next between the stock data rows.

            for (int i = 1; i < pASDStockDataRowList.Count; i++)
            {
                for (int j = 0; j < pASDStockDataRowList[i]._ASDStockDataDays.Count; j++)
                {
                    //Set each StockDataDay in StockDataRow i to the previous StockDataRow StockDataDay
                    pASDStockDataRowList[i]._ASDStockDataDays[j]._PrevStockDataDay = pASDStockDataRowList[i - 1]._ASDStockDataDays[j];

                    //Set the previous StockDataRow's StockDataDay to the current StockDataDay of this StockDataRow
                    pASDStockDataRowList[i - 1]._ASDStockDataDays[j]._NextStockDataDay = pASDStockDataRowList[i]._ASDStockDataDays[j];

                }//for
            }//for

        }//SetStockDataRowsPrevNextReferences

        public Affinity_CustomASDToLegacyAppDATAConverter.Affinity_CustomASDToLegacyAppDATAUtility.OutputFormats OutputToConsole_OutputFormatsAvailable()
        {
            int OutputFormatChosen = 0;

            Console.WriteLine("     1 -  Open, High, Low, Close, Volume (.DATA)");
            Console.WriteLine("     2 -  Open, Close, Volume (.DATA)");
            Console.WriteLine("     3 -  Open, Close, Volume, Current Day Delta (.DATA)");
            Console.WriteLine("     4 -  Open, High, Low, Close, Volume, Current Day Delta (.DATA)");
            Console.WriteLine("     5 -  Open, High, Low, Close, Volume, Current Day Delta, Previous Day Delta (.DATA)");
            Console.WriteLine("     6 -  Volume, Current Day Delta (.DATA)");
            Console.WriteLine("     7 -  Volume, Current Day Delta, Previous Day Delta (.DATA)");
            Console.WriteLine("     8 -  Volume, Previous Day Delta (.DATA)");
            Console.WriteLine("     9 -  Open, Close, Volume, Current Day Delta (.DATA)");
            Console.WriteLine("     10 - Open, Close, Volume, Current Day Delta, Previous Day Delta (.DATA)");
            Console.WriteLine("     11 - Current Day Delta, Previous Day Delta, Two Day Delta, Previous Day After Market Delta (.DATA)");
            Console.WriteLine("     12 - Current Day Delta, Previous Day Delta, Two Day Delta, Previous Day After Market Delta, Volume, Previous Day Volume (.DATA)");
            Console.WriteLine("     13 - Volume, Previous Day Volume (.DATA)");
            Console.WriteLine("     14 - Open, High, Low, Close, Volume, Current Day Delta, Previous Day Delta, Two Day Delta, Previous Day After Market Delta (.DATA)");
            Console.WriteLine("     15 - Volume, Previous Day Volume (.DATA)");
            Console.WriteLine("     16 - Volume, Current Day %, Previous Day % (.DATA");   
            Console.WriteLine();
            Console.WriteLine("NOTE: DATA files will start or end at index 1 instead of 0 to enable Previous Stock Data in the result set.");
            Console.WriteLine();
            Console.WriteLine("Also NOTE: Values available are also Previous Day Volume Delta, Two Day Delta, and Previous Day After Market Delta. But not in any methods as of this time.");
            bool ValidInput = false;
            do
            {
                try
                {
                    OutputFormatChosen = int.Parse(Console.ReadLine());
                    ValidInput = true;
                }//try
                catch (Exception e)
                {
                    Console.WriteLine("Invalid input. Please try again.");
                }//catch
            }//do
            while (ValidInput == false);

            switch (OutputFormatChosen)
            {
                case 1:
                    return OutputFormats.Method1;
                case 2:
                    return OutputFormats.Method2;
                case 3:
                    return OutputFormats.Method3;
                case 4:
                    return OutputFormats.Method4;
                case 5:
                    return OutputFormats.Method5;
                case 6:
                    return OutputFormats.Method6;
                case 7:
                    return OutputFormats.Method7;
                case 8:
                    return OutputFormats.Method8;
                case 9:
                    return OutputFormats.Method9;
                case 10:
                    return OutputFormats.Method10;
                case 11:
                    return OutputFormats.Method11;
                case 12:
                    return OutputFormats.Method12;
                case 13:
                    return OutputFormats.Method13;
                case 14:
                    return OutputFormats.Method14;
                case 15:
                    return OutputFormats.Method15;
                case 16:
                    return OutputFormats.Method16;
                default:
                    throw new Exception("Invalid output format chosen... you knucklehead!");
            }//switch      

        }////OutputToConsole_OutputFormatsAvailable()

        public void GenerateDataOutputFile(OutputFormats pOutoutFormat, string pOutputFile, bool pIncludeMethodInFileName, int pNumWinners, double pWinnerValue, double pLoserValue, bool pTrainingFile, bool pReverseOrder = false)
        {
            string CurrentLine = string.Empty;

            if (pIncludeMethodInFileName)
                pOutputFile = AddMethodToFileName(pOutoutFormat, pOutputFile);

            Console.WriteLine("Generating output file: " + pOutputFile + " using format method: " + pOutoutFormat.ToString() + "...");

            try
            {
                using (StreamWriter writer = new StreamWriter(pOutputFile))
                {
                    if (pTrainingFile == true)
                    {
                        //CREATE THE TRAINING FILE - INCLUDES FUTURE PREDICTION
                        if (pReverseOrder == false)
                        {
                            //NOTE: Starting at index 1 and not 0 so that we can use the value Previous Stock Day in the data return
                            for (int i = 1; i < _ASDStockDataRows.Count - 1; i++)
                            {
                                CurrentLine = BuildOutputLine(pOutoutFormat, _ASDStockDataRows[i], pNumWinners, pWinnerValue, pLoserValue, pTrainingFile);

                                writer.WriteLine(CurrentLine);
                            }//for  
                        }//if
                        else //Reserve Order
                        {
                            //NOTE: Ending at index 1 and not 0 so that we can use the value Previous Stock Day in the data return
                            for (int i = _ASDStockDataRows.Count - 2; i >= 1; i--)
                            {
                                CurrentLine = BuildOutputLine(pOutoutFormat, _ASDStockDataRows[i], pNumWinners, pWinnerValue, pLoserValue, pTrainingFile);

                                writer.WriteLine(CurrentLine);
                            }//for 
                        }//else
                    }
                    else
                    {
                        //CREATE THE TESTING FILE - DOES NOT INCLUDE PREDICTION - INCLUDES MOST RECENT DAY OF DATA
                        if (pReverseOrder == false)
                        {
                            //NOTE: Starting at index 1 and not 0 so that we can use the value Previous Stock Day in the data return
                            for (int i = 1; i < _ASDStockDataRows.Count; i++)
                            {
                                CurrentLine = BuildOutputLine(pOutoutFormat, _ASDStockDataRows[i], pNumWinners, pWinnerValue, pLoserValue, pTrainingFile);

                                writer.WriteLine(CurrentLine);
                            }//for  
                        }//if
                        else //Reserve Order
                        {
                            //NOTE: Ending at index 1 and not 0 so that we can use the value Previous Stock Day in the data return
                            for (int i = _ASDStockDataRows.Count - 1; i >= 1; i--)
                            {
                                CurrentLine = BuildOutputLine(pOutoutFormat, _ASDStockDataRows[i], pNumWinners, pWinnerValue, pLoserValue, pTrainingFile);

                                writer.WriteLine(CurrentLine);
                            }//for 
                        }//else
                    }
                    writer.Close();
                }//using
            }//try
            catch (Exception e)
            {
                Console.WriteLine("Error occurred trying to generate output file: " + e.Message);
            }

            Console.WriteLine("Output file successfully generated!");

        }//GenerateOutputFile


        public string BuildOutputLine(OutputFormats pOutputFormat, ASDStockDataRow pStockDataRow, int pNumWinners, double pWinnerValue, double pLoserValue, bool pTrainingFile)
        {
            string OutputLine = string.Empty;

            //Build the data input part of the output line
            OutputLine = BuildInputDataOutputLine(pOutputFormat, pStockDataRow);

            if (pTrainingFile == true)
            {
                //Calculate and build the result part of the output line
                OutputLine += "=" + BuildResultsAddendumOutputLine(pStockDataRow, pNumWinners, pWinnerValue, pLoserValue);
            }
            return OutputLine;

        }//BuildOutputLine

        public string BuildInputDataOutputLine(OutputFormats pOutputFormat, ASDStockDataRow pStockDataRow)
        {
            string OutputLine = string.Empty;

            switch (pOutputFormat)
            {
                case OutputFormats.Method1:
                    OutputLine = BuildOutputLine_Method1(pStockDataRow);
                    break;
                case OutputFormats.Method2:
                    OutputLine = BuildOutputLine_Method2(pStockDataRow);
                    break;
                case OutputFormats.Method3:
                    OutputLine = BuildOutputLine_Method3(pStockDataRow);
                    break;
                case OutputFormats.Method4:
                    OutputLine = BuildOutputLine_Method4(pStockDataRow);
                    break;
                case OutputFormats.Method5:
                    OutputLine = BuildOutputLine_Method5(pStockDataRow);
                    break;
                case OutputFormats.Method6:
                    OutputLine = BuildOutputLine_Method6(pStockDataRow);
                    break;
                case OutputFormats.Method7:
                    OutputLine = BuildOutputLine_Method7(pStockDataRow);
                    break;
                case OutputFormats.Method8:
                    OutputLine = BuildOutputLine_Method8(pStockDataRow);
                    break;
                case OutputFormats.Method9:
                    OutputLine = BuildOutputLine_Method9(pStockDataRow);
                    break;
                case OutputFormats.Method10:
                    OutputLine = BuildOutputLine_Method10(pStockDataRow);
                    break;
                case OutputFormats.Method16:
                    OutputLine = BuildOutputLine_Method16(pStockDataRow);
                    break;
                default:
                    Console.WriteLine("Invalid input. What are you doing dude? Adios amigo!");
                    break;
            }//switch

            return OutputLine;
        }//BuildInputDataOutputLine()

        public string BuildResultsAddendumOutputLine(ASDStockDataRow pStockDataRow, int pNumWinners, double pWinnerValue, double pLoserValue)
        {
            string ResultsOutputAddendumLine = string.Empty;

            //Build the results section of the output line.
            double[] NextDayDelta = new double[pStockDataRow._ASDStockDataDays.Count];
            bool[] WinnersLosersArray = new bool[pStockDataRow._ASDStockDataDays.Count];

            for(int i=0; i<NextDayDelta.Length; i++)
            {
                NextDayDelta[i] = pStockDataRow._ASDStockDataDays[i]._NextStockDataDay._DaysDelta;
            }//for

            WinnersLosersArray = IdentifyWinnersAndLosers(NextDayDelta, pNumWinners);

            for(int i=0; i<WinnersLosersArray.Length;i++)
            {
                if (WinnersLosersArray[i])
                    ResultsOutputAddendumLine += pWinnerValue;
                else
                    ResultsOutputAddendumLine += pLoserValue;

                if (i != WinnersLosersArray.Length - 1)
                    ResultsOutputAddendumLine += ",";
            }//

            return ResultsOutputAddendumLine;

        }//BuildResultsAddendumOutputLine()

        public bool[] IdentifyWinnersAndLosers(double[] pNextDayDelta, int pNumWinners)
        {
            bool[] WinnersAndLosersArray = new bool[pNextDayDelta.Length];

            //Set all values in the array to false
            for (int i = 0; i < WinnersAndLosersArray.Length; i++)
                WinnersAndLosersArray[i] = false;

            for (int j = 0; j < pNumWinners; j++)
            {
                double CurrentHighestValue = double.MinValue;
                int CurrentHighestValueIndex = -1;
                
                for (int i = 0; i < WinnersAndLosersArray.Length; i++)
                {
                    //If the next day delta is greater than current highest, isnt already a selected winner, and is positive
                    if (pNextDayDelta[i] > CurrentHighestValue && WinnersAndLosersArray[i] == false && pNextDayDelta[i] > 0)
                    {
                        CurrentHighestValue = pNextDayDelta[i];
                        CurrentHighestValueIndex = i;
                    }

                }//for
            
                 //Only set the index to a winner value if a winner was identified. Otherwise, all stocks are losers.
                 //This scenario needs to be taken into account because we do not want to buy a stock because it went down the least...
                if(CurrentHighestValueIndex > -1)
                    WinnersAndLosersArray[CurrentHighestValueIndex] = true;

            }//for

            return WinnersAndLosersArray;

        }//IdentifyWinnersAndLosers

        public string BuildOutputLine_Method1(ASDStockDataRow pStockDataRow)
        {
            //Method1 = O,H,L,C,V
            string OutputRowString = string.Empty;
            
            for(int i=0; i<pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Open + "," + pStockDataRow._ASDStockDataDays[i]._High + "," + pStockDataRow._ASDStockDataDays[i]._Low + "," + pStockDataRow._ASDStockDataDays[i]._Close + "," + pStockDataRow._ASDStockDataDays[i]._Volume;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method1

        public string BuildOutputLine_Method2(ASDStockDataRow pStockDataRow)
        {
            //Method2 = O,C,V
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Open  + "," + pStockDataRow._ASDStockDataDays[i]._Close + "," + pStockDataRow._ASDStockDataDays[i]._Volume;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method2

        public string BuildOutputLine_Method3(ASDStockDataRow pStockDataRow)
        {
            //Method3 = O,C,V,DD
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Open + "," + pStockDataRow._ASDStockDataDays[i]._Close + "," + pStockDataRow._ASDStockDataDays[i]._Volume + pStockDataRow._ASDStockDataDays[i]._DaysDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method3

        public string BuildOutputLine_Method4(ASDStockDataRow pStockDataRow)
        {
            //Method4 = O,H,L,C,V,DD
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Open + "," + pStockDataRow._ASDStockDataDays[i]._High + "," + pStockDataRow._ASDStockDataDays[i]._Low + "," + pStockDataRow._ASDStockDataDays[i]._Close + "," + pStockDataRow._ASDStockDataDays[i]._Volume + pStockDataRow._ASDStockDataDays[i]._DaysDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method4

        public string BuildOutputLine_Method5(ASDStockDataRow pStockDataRow)
        {
            //Method5 = O,H,L,C,V,DD,PDD
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Open + "," + pStockDataRow._ASDStockDataDays[i]._High + "," + pStockDataRow._ASDStockDataDays[i]._Low + "," + pStockDataRow._ASDStockDataDays[i]._Close + "," + pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._DaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDaysDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method5

        public string BuildOutputLine_Method6(ASDStockDataRow pStockDataRow)
        {
            //Method6 = V,DD
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._DaysDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method6

        public string BuildOutputLine_Method7(ASDStockDataRow pStockDataRow)
        {
            //Method7 = V,DD,PDD
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._DaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDaysDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method7

        public string BuildOutputLine_Method8(ASDStockDataRow pStockDataRow)
        {
            //Method8 = V,PDD
            string OutputRowString = string.Empty;

            for (int i = 1; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                //NOTE: Any DATA file that contains the Previous Day's Delta must start at index 1 and not index 0
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._PrevDaysDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method8

        public string BuildOutputLine_Method9(ASDStockDataRow pStockDataRow)
        {
            //Method9 = O,C,V,DD
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Open + "," + pStockDataRow._ASDStockDataDays[i]._Close + "," + pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._DaysDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method9

        public string BuildOutputLine_Method10(ASDStockDataRow pStockDataRow)
        {
            //Method10 = O,C,V,DD,PDD
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Open + "," + pStockDataRow._ASDStockDataDays[i]._Close + "," + pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._DaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDaysDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method10

        public string BuildOutputLine_Method11(ASDStockDataRow pStockDataRow)
        {
            //Method11 = DD,PDD,2DD,PDAMD
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._DaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._TwoDaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDayAfterMarketDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method11

        public string BuildOutputLine_Method12(ASDStockDataRow pStockDataRow)
        {
            //Method12 = DD,PDD,2DD,PDAMD,V,PDV
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._DaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._TwoDaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDayAfterMarketDelta + "," + pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._PrevDayVolumeDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method12

        public string BuildOutputLine_Method13(ASDStockDataRow pStockDataRow)
        {
            //Method13 = V, PDV
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString +=  pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._PrevDayVolumeDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method13

        public string BuildOutputLine_Method14(ASDStockDataRow pStockDataRow)
        {
            //Method14 = O,H,L,C,V,DD,PDD,PDV,2DD,PDAMD
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Open + "," + pStockDataRow._ASDStockDataDays[i]._High + "," + pStockDataRow._ASDStockDataDays[i]._Low + "," + pStockDataRow._ASDStockDataDays[i]._Close + "," + pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._DaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDayVolumeDelta + "," + pStockDataRow._ASDStockDataDays[i]._TwoDaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDayAfterMarketDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method14

        public string BuildOutputLine_Method15(ASDStockDataRow pStockDataRow)
        {
            //Method15 = DD,V,PDD,PDV
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._DaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._Volume + "," + pStockDataRow._ASDStockDataDays[i]._PrevDaysDelta + "," + pStockDataRow._ASDStockDataDays[i]._PrevDayVolumeDelta;

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method15

        public string BuildOutputLine_Method16(ASDStockDataRow pStockDataRow)
        {
            //Method16 = V,DD%,PDD%
            string OutputRowString = string.Empty;

            for (int i = 0; i < pStockDataRow._ASDStockDataDays.Count; i++)
            {
                OutputRowString += pStockDataRow._ASDStockDataDays[i]._Volume + "," + (pStockDataRow._ASDStockDataDays[i]._DaysDelta / pStockDataRow._ASDStockDataDays[i]._Open) + "," + (pStockDataRow._ASDStockDataDays[i]._PrevStockDataDay._DaysDelta / pStockDataRow._ASDStockDataDays[i]._PrevStockDataDay._Open);

                if (i != pStockDataRow._ASDStockDataDays.Count - 1)
                    OutputRowString += ",";

            }//for

            return OutputRowString;

        }//BuildOutputLine_Method16
        public string AddMethodToFileName(OutputFormats pOutoutFormat, string pOutputFileName)
        {
            string ResultFileName = string.Empty;

            //separate out the extension if there is one
            string[] split1 = pOutputFileName.Split(".");

            if(split1.Length > 1)
            {
                for (int i = 0; i < split1.Length-1; i++)
                    ResultFileName += split1[i];
            }

            ResultFileName += "_" + pOutoutFormat.ToString() + "." + split1[split1.Length-1];


            return ResultFileName;

        }//AddMethodToFileName

        public string BuildOutputFileName(string pInputFileName)
        {
            string OutputFileName = string.Empty;
            string InputFileName = Path.GetFileName(pInputFileName);
            string Directory = Path.GetDirectoryName(pInputFileName);

            string[] split = InputFileName.Split(".");

            if (split.Length > 1)
            {
                for (int i = 0; i < split.Length - 1; i++)
                    OutputFileName += split[i];
            }

            OutputFileName += ".DATA";

            return Directory + "//" + OutputFileName;
        }
    }//Class - Affinity_CustomASDToLegacyAppDATAUtility

    public class ASDStockDataRow
    {
        public bool _Valid = false;
        public string _Date;
        public List<ASDStockDataDay> _ASDStockDataDays = new List<ASDStockDataDay>();
        public ASDStockDataRow _PrevStockDataRow;
        public ASDStockDataRow _NextStockDataRow;

        public void SetASDStockDataRowValues(string pASDStockDataRow)
        {
            //ASD Stock DataRow is expected to be in the following format:
            //      Date:Symbol1, Open1, High1, Low1, Close1, Volume1|Symbol2, Open2, High2, Low2, Close2, Volume2|Symbol3, Open3, High3, Low3, Close3, Volume3

            //split the Date from the rest of the stock data - Date should be 0 index, and rest will be 1 index
            string[] split1 = pASDStockDataRow.Split(":");
            if (split1.Length == 2)
                _Date = split1[0];
            else
                throw new Exception("ASD File Inputted is in Incorrect Format. Expected format is: Date:Symbol1, Open1, High1, Low1, Close1, Volume1|Symbol2, Open2, High2, Low2, Close2, Volume2|Symbol3, Open3, High3, Low3, Close3, Volume3");

            //The remainder of the stock data should be in split1[1]. Now split that into the individual stocks.
            string[] split2 = split1[1].Split("|");
            for(int i=0; i<split2.Length; i++)
            {
                //For each item in split2, create a Stock Data Day object and set the values.
                ASDStockDataDay CurrentASDStockDataDay = new ASDStockDataDay();
                CurrentASDStockDataDay.SetASDStockDataDayValues(split2[i], _Date);

                _ASDStockDataDays.Add(CurrentASDStockDataDay);
            }//for

            _Valid = true;

        }//SetASDStockDataRowValues


    }//ASDStockDataRow
}
