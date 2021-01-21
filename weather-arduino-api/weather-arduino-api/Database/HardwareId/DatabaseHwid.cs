using System.Collections;
using MySqlConnector;

namespace weather_arduino_api.Database.HardwareId
{
    public class DatabaseHwid : WeatherMySqlConnection
    {
        public ArrayList GetHardwareIdList()
        {
            using MySqlConnection connection = GetConnection();
            MySqlCommand cmd1 = connection.CreateCommand();
            cmd1.CommandText = "SELECT * FROM `arduino_station_weather_hwid`";
            ArrayList arrayList = new ArrayList();
            MySqlDataReader reader1 = cmd1.ExecuteReader();
            while (reader1.Read())
            {
                arrayList.Add(new HardwareId(reader1.GetInt32("ID_PRIMARY"), reader1.GetInt32("HWID")));
            }
            if(!reader1.IsClosed)reader1.Close();
            return arrayList;
        }

        public bool ContainsHardwareId(int hwid)
        {
            using MySqlConnection connection = GetConnection();
            MySqlCommand cmd1 = connection.CreateCommand();
            cmd1.CommandText = "SELECT * FROM `arduino_station_weather_hwid` WHERE `HWID` = @hwid";
            cmd1.Prepare();
            cmd1.Parameters.AddWithValue("@hwid", hwid);
            MySqlDataReader reader1 = cmd1.ExecuteReader();
            if(reader1.Read())
            {
                if(!reader1.IsClosed)reader1.Close();
                return true;
            }

            if(!reader1.IsClosed)reader1.Close();
            return false;
        }
    }
}