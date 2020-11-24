using System;
using Microsoft.Extensions.Logging;
using weather_arduino_api.Database;
using weather_arduino_api.Database.SensorSet;

namespace weather_arduino_api.Protocol.Util
{
    public class ResponseTypeSensorSetEngine
    {
        public string Hwid { get; set; }
        public string Temperature { get; set; }
        public string Humidity { get; set; }
        public string Lpg { get; set; }
        public string Co { get; set; }
        public string Smoke { get; set; }

        public ResponseTypeSensorSetEngine(string hwid, string temperature, string humidity, string lpg, string co, string smoke)
        {
            Hwid = hwid;
            Temperature = temperature;
            Humidity = humidity;
            Lpg = lpg;
            Co = co;
            Smoke = smoke;
        }

        public bool IsCompletelyNotNull()
        {
            return Hwid != null && Temperature != null && Humidity != null && Lpg != null && Co != null &&
                   Smoke != null;
        }

        public bool IsFormattedRightly()
        {
            try
            {
                Int32.Parse(Hwid);
                Single.Parse(Temperature);
                Single.Parse(Humidity);
                Single.Parse(Lpg);
                Single.Parse(Co);
                Single.Parse(Smoke);
                return true;
            }
            catch (Exception e)
            {
                return false;
            }
        }

        public SensorSet FormatSensorSet()
        {
            return new SensorSet(Int32.Parse(Hwid), WeatherMySqlConnection.GetCurrentTimeStamp(),Single.Parse(Temperature) , Single.Parse(Humidity), Single.Parse(Lpg), Single.Parse(Co), Single.Parse(Smoke));
        }
    }
}