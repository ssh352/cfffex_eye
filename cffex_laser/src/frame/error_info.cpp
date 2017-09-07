#include "error_info.h"
#include <stdio.h>

const char *get_error_message(int code)
{
    switch (code)
    {
        case STATUS_OK:             return "ok";
        case ERROR_BAD_REQUEST:     return "bad request";
        case ERROR_BAD_PARAM:       return "bad request param";
        case ERROR_NO_REQ_SERVICE:  return "request service schema not found";
        case ERROR_NO_REQ_MESSAGE:  return "request message schema not found";
        case ERROR_REQ_SCHEMA:      return "request schema is error";
        case ERROR_NO_RSP_SERVICE:  return "response service schema not found";
        case ERROR_NO_RSP_MESSAGE:  return "response message schema not found";
        case ERROR_RSP_SCHEMA:      return "response schema is error";
        case ERROR_NO_ALIVE_SERVICE: return "no alive service";
        case ERROR_SEND_REQUEST:    return "send service request failed";

        default:
        {
            static char error_buff[256]; // not thread safe but ignored
            snprintf(error_buff, sizeof(error_buff), "unknown error[%d]", code);
            return error_buff;
        }
    };
    return "";
}
