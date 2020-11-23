namespace weather_arduino_api.Database.HardwareId
{
    public class HardwareId
    {
        private int PrimaryId { get; set; }
        private int HwId { get; set; }
        
        public HardwareId(int primaryId, int hwId)
        {
            PrimaryId = primaryId;
            HwId = hwId;
        }
    }
}