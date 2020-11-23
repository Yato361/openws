using System;
using System.Data;
using MySqlConnector;

namespace weather_arduino_api.Database
{
    public class WeatherMySqlConnection
    {

        private MySqlConnection _connection;

        public MySqlConnection GetConnection()
        {
            if (_connection == null || _connection.State == ConnectionState.Closed)
            {
                _connection = new MySqlConnection(_connectionString);
                _connection.Open();
            }
            return _connection;
        }
        
        public int GetCurrentTimeStamp()
        {
            return (Int32) DateTime.Now.Subtract(new DateTime(1970, 1, 1, 0, 0, 0)).TotalSeconds - 3600;
        }
    }
}