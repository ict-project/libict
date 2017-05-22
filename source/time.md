# `ict::time` module

Provides a class for a point in time manipulation.

## Usage

When created with no parameters - contains present point in time (local time).

```cpp
ict::time::TimeInfo ti;
std::cout<<"Current local time: "<<ti<<std::endl;
```

If unix timestamp (seconds from 1970-01-01 00:00:00 +0000 (UTC)) is provided in constructor (it might be an integer and float point number) - given point in time is set.

```cpp
ict::time::TimeInfo ti(1.49545e+09);
std::cout<<"Stored local time: "<<ti<<std::endl;
```

Following class properties are availbile:

* `unix_sec` - unix seconds;
* `msec` - miliseconds after the second (0-999);
* `sec` - seconds after the minute (0-59);
* `min` - minutes after the hour (0-59);
* `hour` - hours since midnight (0-23);
* `mday` - day of the month (1-31);
* `mon` - months since January (0-11);
* `year` - years since 1900;
* `wday` - days since Sunday (0-6);
* `yday` - days since January 1st (0-365);
* `isdst` - Daylight Saving Time flag;
* `gmtoff` - Seconds east of UTC;
* `zone` - Timezone abbreviation.

Following class properties can be modified (even out of range) in order to move (forward and backward) the point in time:

* `msec` - miliseconds after the second;
* `sec` - seconds after the minute;
* `min` - minutes after the hour;
* `hour` - hours since midnight;
* `mday` - day of the month;
* `mon` - months since January;
* `year` - years since 1900.

If properties mentioned above has been modified function `correctLocalTime()` must be called in order to normalize resulting point in time. Example:

```cpp
ict::time::TimeInfo ti;
std::cout<<"Current local time: "<<ti<<std::endl;
ti.mon-=20;
ti.correctLocalTime();
std::cout<<"Local time 20 months ago: "<<ti<<std::endl;
```

Other auxiliary functions:
* `void setLocalTime()` - sets current local time;
* `void setUtcTime()` - sets current universal time;
* `void setLocalTime(time)` - sets given (unix timestamp) local time;
* `void setUtcTime(time)` - sets given (unix timestamp) universal time;
* `std::string formatTime(format)` - returns string with date and time formated by `strftime()` function.

Class `ict::time::TimeInfo` can be put to a stream - the output gives followin format:

```
2017-05-22 12:25:49.057
```