using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using weather_arduino_api.Database;

namespace weather_arduino_api
{
    public class Program
    {
        public static void Main(string[] args)
        {
            Debug.Debug.Log("Starting WAPI MySQL-Test...");
            WeatherMySqlConnection weatherMySqlConnection = new WeatherMySqlConnection();
            try
            {
                if (weatherMySqlConnection.GetConnection().State != ConnectionState.Closed)
                {
                    Debug.Debug.Log("MySQL-Test successfully carried out!");
                    CreateHostBuilder(args).Build().Run();
                }else
                    Debug.Debug.Log("WAPI MySQL could not be connected. Program is stopping!");
            }
            catch (Exception e)
            {
                Debug.Debug.Log("Exception was thrown. Program is stopping! Exception: " + e.Message);
            }
        }

        private static IHostBuilder CreateHostBuilder(string[] args) => 
            Host.CreateDefaultBuilder(args).ConfigureWebHostDefaults(webBuilder =>
            {
                webBuilder.UseUrls("http://127.0.0.1:5000");
                webBuilder.UseStartup<Startup>();
            });
    }
}