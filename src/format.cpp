#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
    const int cseconds_in_day = 86400;
    const int cseconds_in_hour = 3600;
    const int cseconds_in_minute = 60;
    const int cseconds = 1;

    long hours = (seconds % cseconds_in_day) / cseconds_in_hour;
    long minutes = ((seconds % cseconds_in_day) % cseconds_in_hour) / cseconds_in_minute;
    seconds = (((seconds % cseconds_in_day) % cseconds_in_hour) % cseconds_in_minute) / cseconds;
    string s;
    s += "" + std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    return s;
}