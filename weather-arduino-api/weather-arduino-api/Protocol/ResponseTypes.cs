namespace weather_arduino_api.Protocol
{
    public enum ResponseTypes
    {
        SUCCESSFULL = 0,
        UNFORMATTED_TYPE_ERROR = 1,
        NOT_COMPLETE_ERROR = 2,
        NO_RIGHTS_ERROR = 3,
        EXCEPTION_ERROR = 4
    }
}