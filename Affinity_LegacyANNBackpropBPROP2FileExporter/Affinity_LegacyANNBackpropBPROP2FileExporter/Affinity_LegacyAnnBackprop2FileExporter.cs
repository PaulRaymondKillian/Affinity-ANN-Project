using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;

namespace Affinity_LegacyANNBackpropBPROP2FileExporter
{
    public class Affinity_LegacyAnnBackprop2FileExporter
    {
        List<string> _AXSFileStrings = new List<string>();
        string _FileExension = "BPROP2";

        public void ImportAXSFile(string pImportAXSFileLocation)
        {
            Console.WriteLine("Importing AXS File:" + pImportAXSFileLocation);

            try
            {
                System.IO.StreamReader file = new System.IO.StreamReader(pImportAXSFileLocation);
                string line;

                while ((line = file.ReadLine()) != null)
                {
                    _AXSFileStrings.Add(line);
                    System.Console.WriteLine("  " + line);
                }

                file.Close();
            }
            catch (Exception e)
            {
                var result = "Failure to load stock input file: " + e.Message;
            }

            Console.WriteLine("Successfully imported AXS file!");

        }//ImportAXSFile()

        public void ExportToBPROP2File(string pImportAXSFileLocation, bool pReverseOrder = false)
        {
            try
            {
                string FilePath = Path.GetDirectoryName(pImportAXSFileLocation);
                string FileName = RemoveExtensionFromFile(Path.GetFileName(pImportAXSFileLocation)); //Does this include the extension? If so, need to remove it.

                using (StreamWriter writer = new StreamWriter(FilePath + "\\" + FileName + "." + _FileExension))
                {
                    if (!pReverseOrder)
                    {
                        for (int i = 0; i < _AXSFileStrings.Count; i++)
                        {
                            writer.WriteLine(BuildBPROP2StringFromAXSString(_AXSFileStrings[i]));
                        }//for
                    }
                    else
                    {
                        for (int i = _AXSFileStrings.Count - 1; i >= 0; i--)
                        {
                            writer.WriteLine(BuildBPROP2StringFromAXSString(_AXSFileStrings[i]));
                        }//for
                    }//else

                    writer.Close();

                }//using
            }//try
            catch(Exception e)
            {
                Console.WriteLine("Error writing BPROP2 File - " + e.Message);
            }//catch

        }//ExportToBPROP2File()

        public string BuildBPROP2StringFromAXSString(string pAXSString)
        {
            string BPROP2String = string.Empty;
            string inputs = string.Empty;
            string results = string.Empty;

            //Split the Date section from the rest of the string, then discard the Date section.
            string[] split1 = pAXSString.Split(":");

            //Split the non-Date section by = to segregate the inputs from the results
            string[] split2 = split1[1].Split("=");
            results = split2[1];

            //Split the inputs section into individual strings for each stock
            string[] split3 = split2[0].Split("|");

            //For each stock item split in split3, split it further by comma to get individual stock datum and then add it to the inputs string
            for(int i=0; i<split3.Length;i++)
            {
                string[] split4 = split3[i].Split(",");

                inputs += split4[1] + "," + split4[2] + "," + split4[3] + "," + split4[4] + "," + split4[5];

                //If this is not the last stock, add another comma for the next set of stocks.
                if (i < split3.Length - 1)
                    inputs += ",";
            }

            if(inputs.Length == 0 || results.Length == 0)
            {
                //An uhoh must have happened. Throw an error and cry a little.
                Console.WriteLine("Error occurred building BRPOP2 line from AXS line. Please check to make sure inputted AXS file is in a valid format.");
                throw new Exception("Error occurred building BRPOP2 line from AXS line. Please check to make sure inputted AXS file is in a valid format.");
            }

            BPROP2String = inputs + "=" + results;

            return BPROP2String;
        }//BuildBPROP2StringFromAXSString

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
    }//Class Affinity_LegacyAnnBackprop2FileExporter
}
