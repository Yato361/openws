using System;
using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Logging;
using weather_arduino_api.Database.HardwareId;
using weather_arduino_api.Database.SensorSet;
using weather_arduino_api.Protocol;
using weather_arduino_api.Protocol.Util;

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
        public ResponseTypes Get(string hwid, string temperature, string humidity, string lpg, string co, string smoke)
        {
            try
            {
                Debug.Debug.Log("API INSERT request starting");
                ResponseTypeSensorSetEngine engine = new ResponseTypeSensorSetEngine(hwid,temperature,humidity,lpg,co,smoke);
                if (!engine.IsCompletelyNotNull())
                    return ResponseTypes.NOT_COMPLETE_ERROR;
                if (!engine.IsFormattedRightly())
                    return ResponseTypes.UNFORMATTED_TYPE_ERROR;
                SensorSet sensorSet = engine.FormatSensorSet();
                DatabaseHwid dbHwid = new DatabaseHwid();
                if (!dbHwid.ContainsHardwareId(sensorSet.Hwid))
                    return ResponseTypes.NO_RIGHTS_ERROR;
                DatabaseSensorSet databaseSensorSet = new DatabaseSensorSet();
                databaseSensorSet.InsertSensorSetToDatabase(sensorSet);
                Debug.Debug.Log("API INSERT request successfully completed.");
                return ResponseTypes.SUCCESSFULL;
            }
            catch (Exception e)
            {
                Debug.Debug.Log(e.Message + " " + e.StackTrace);
                return ResponseTypes.EXCEPTION_ERROR;
            }
        }
    }
}