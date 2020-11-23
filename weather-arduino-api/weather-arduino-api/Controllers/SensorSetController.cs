using System;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using weather_arduino_api.Database.SensorSet;

namespace weather_arduino_api.Controllers
{
    [ApiController]
    [Route("/insert")]
    public class SensorSetController : ControllerBase
    {
        private readonly ILogger<SensorSetController> _logger;

        public SensorSetController(ILogger<SensorSetController> logger)
        {
            _logger = logger;
        }
        
        [HttpGet]
        public SensorSet Get(string hwid, string temperature, string humidity, string lpg, string co, string smoke)
        {
            if (hwid == null) hwid = "0";
            if (temperature == null) temperature = "0";
            if (humidity == null) humidity = "0";
            if (lpg == null) lpg = "0";
            if (co == null) co = "0";
            if (smoke == null) smoke = "0";
            
            DatabaseSensorSet databaseSensorSet = new DatabaseSensorSet();
            SensorSet sensorSet = new SensorSet(Int32.Parse(hwid),databaseSensorSet.GetCurrentTimeStamp(), Single.Parse(temperature),
            Single.Parse(humidity), Single.Parse(lpg), Single.Parse(co), Single.Parse(smoke));
            
            databaseSensorSet.InsertSensorSetToDatabase(sensorSet);
            return sensorSet;
        }
    }
}