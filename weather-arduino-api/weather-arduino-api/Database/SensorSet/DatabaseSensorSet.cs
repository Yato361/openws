using MySqlConnector;
using weather_arduino_api.Database;
using weather_arduino_api.Database.SensorSet;

namespace weather_arduino_api
{
    public class DatabaseSensorSet : WeatherMySqlConnection
    {
        public void InsertSensorSetToDatabase(SensorSet sensorSet)
        {
            MySqlCommand cmd1 = GetConnection().CreateCommand();
            cmd1.CommandText = "INSERT INTO `arduino_station_weather_information`(`HWID`, `UNIX_TIMESTAMP`, `TEMP`, `HUMIDITY`, `LPG`, `CO`, `SMOKE`) VALUES (@hw,@ut,@te,@hu,@lp,@co,@sm)";
            cmd1.Prepare();
            cmd1.Parameters.AddWithValue("@hw", sensorSet.Hwid);
            cmd1.Parameters.AddWithValue("@ut", sensorSet.UnixTimeStamp);
            cmd1.Parameters.AddWithValue("@te", sensorSet.Temperature);
            cmd1.Parameters.AddWithValue("@hu", sensorSet.Humidity);
            cmd1.Parameters.AddWithValue("@lp", sensorSet.LPG);
            cmd1.Parameters.AddWithValue("@co", sensorSet.CO);
            cmd1.Parameters.AddWithValue("@sm", sensorSet.Smoke);
            cmd1.ExecuteNonQuery();
        }
    }
}