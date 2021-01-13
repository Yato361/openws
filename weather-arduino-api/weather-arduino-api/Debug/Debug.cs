using System;

namespace weather_arduino_api.Debug
{
    public static class Debug
    {
        private static bool _debug = true;

        public static void Log(string s)
        {
            if (_debug)
            {
                Console.WriteLine(s);
            }
        }
    }
}