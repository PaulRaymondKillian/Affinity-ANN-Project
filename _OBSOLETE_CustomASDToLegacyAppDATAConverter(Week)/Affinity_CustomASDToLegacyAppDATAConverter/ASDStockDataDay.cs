using System;
using System.Collections.Generic;
using System.Text;

namespace Affinity_CustomASDToLegacyAppDATAConverter7Days
{
    public class ASDStockDataDay
    {
        bool _Valid = false;
        public string _Date;
        public string _Symbol;
        public double _Open;
        public double _High;
        public double _Low;
        public double _Close;
        public int _Volume;
        public ASDStockDataDay _PrevStockDataDay;
        public ASDStockDataDay _NextStockDataDay;

        #region Properties
        public double _DaysDelta
        {
            get
            {
                return _Close - _Open;
            }
        }//Property - _DaysDelta

        public double _PrevDaysDelta
        {
            //Returns the previous day's gain or loss while the market is open
            get
            {
                if (_PrevStockDataDay != null)
                    return _PrevStockDataDay._Close - _PrevStockDataDay._Open;
                else
                    return 0; //Return a 0 for now. Maybe later will change this to return an exception or something. Probably not too important because it should only occur at the last stock data day. 
            }
        }//Property - _PrevDaysDelta

        public int _PrevDayVolumeDelta
        {
            get
            {
                if (_PrevStockDataDay != null)
                    return _Volume - _PrevStockDataDay._Volume;
                else
                    return 0; //Return a 0 for now. Maybe later change this to return exception or something else. Not sure if this property is needed, but might be useful info.
            }
        }//_PrevDayVolumeDelta

        public double _TwoDaysDelta
        {
            //Returns the delta over the past 2 days. Previous day open to current day close
            get
            {
                if (_PrevStockDataDay != null)
                    return _Close - _PrevStockDataDay._Open;
                else
                    return 0; //Return a 0 for now. Maybe later will change this to return an exception or something. Probably not too important because it should only occur at the last stock data day. 
            }
        }//Property - _PrevDaysDelta

        public double _PrevDayAfterMarketDelta
        {
            //Returns the gain or loss from the previous day's close to the current days open, after market delta
            get
            {
                if (_PrevStockDataDay != null)
                    return _Open - _PrevStockDataDay._Close;
                else
                    return 0; //Return a 0 for now. Maybe later will change this to return an exception or something. Probably not too important because it should only occur at the last stock data day. 
            }
        }//Property - _PrevDaysDelta

        #endregion //endregion for Properties

        public void SetASDStockDataDayValues(string pASDStockDataDayString, string pDate)
        {
            //ASD Stock Data Day is expected to be in the following format:
            //      Symbol1, Open1, High1, Low1, Close1, Volume1

            //split on comma to get the individual values
            string[] split = pASDStockDataDayString.Split(",");

            if (split.Length != 6)
                throw new Exception("ASD Stock Data Day Inputted is in Incorrect Format. Expected format is: Symbol, Open, High, Low, Close, Volume");

            _Date = pDate;
            _Symbol = split[0];
            _Open = double.Parse(split[1]);
            _High = double.Parse(split[2]);
            _Low = double.Parse(split[3]);
            _Close = double.Parse(split[4]);
            _Volume = int.Parse(split[5]);

            //This stock data day is not initialized with valid values. Set _Valid to true so higher scope apps know this is not a book-end of a list.
            _Valid = true;

        }//SetASDStockDataDayValues
    }
}
