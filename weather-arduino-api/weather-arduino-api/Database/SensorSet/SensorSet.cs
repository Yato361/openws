namespace weather_arduino_api.Database.SensorSet
{
    public class SensorSet
    {
        public int Hwid { get; set; }
        public int UnixTimeStamp { get; set; }
        public float Temperature { get; set; }
        public float Humidity { get; set; }
        public float LPG { get; set; }
        public float CO { get; set; }
        public float Smoke { get; set; }

        public SensorSet(int hwid, int unixTimeStamp, float temperature, float humidity, float lpg, float co, float smoke)
        {
            Hwid = hwid;
            UnixTimeStamp = unixTimeStamp;
            Temperature = temperature;
            Humidity = humidity;
            LPG = lpg;
            CO = co;
            Smoke = smoke;
        }
    }
}