#include "Calendar.h"

#include <sstream>
using namespace std;

#include "../Misc/CellarUtils.h"


namespace cellar
{

    const string  Date::MONTH_NAMES[Date::NB_MONTHS_IN_YEAR] = {
        "January",  "February", "March",    "April",
        "May",      "June",     "July",     "August",
        "September","October",  "November", "December"};

    const int Date::NB_DAYS_IN_MONTH[Date::NB_MONTHS_IN_YEAR] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Date
    Date::Date() :
        year(2000),
        month(0),
        day(0)
    {
    }

    Date::Date(int year, unsigned int month, unsigned int day) :
        year(year),
        month(month),
        day(day)
    {
        assureIntegrety();
    }

    std::string Date::toString() const
    {
        stringstream stream;
        stream << year << '-'
               << MONTH_NAMES[month] << '-'
               << day+1;
        return stream.str();
    }

    bool Date::isThisYearLeap() const
    {
        return isLeap(year);
    }

    int Date::nbDaysThisMonth()
    {
        return nbDaysInMonth(year, month);
    }

    float Date::yearCompleteness() const
    {
        return toDaysInYear() / (isThisYearLeap()? 366.0f : 355.0f);
    }

    int Date::toDays() const
    {
        // TODO : Pour l'instant
        return toDaysInYear();
    }

    int Date::toDaysInYear() const
    {
        unsigned int totalDays = day;
        for(int m=0; m<month; ++m)
            totalDays += nbDaysInMonth(year, m);

        return totalDays;
    }

    Date& Date::fromDays(int)
    {
        // TODO : Pour l'instant
        return *this;
    }

    bool Date::operator< (const Date& d) const
    {
        return year < d.year     ? true : year > d.year     ? false :
               month < d.month   ? true : month > d.month   ? false :
               day < d.day       ? true : false;
    }

    bool Date::operator==(const Date& d) const
    {
        return year == d.year  &&  month == d.month  &&  day == d.day;
    }

    Date& Date::operator+=(const Date& t)
    {
        day   += t.day;
        month += t.month;
        year  += t.year;
        return assureIntegrety();
    }

    Date& Date::operator-=(const Date& t)
    {
        day   -= t.day;
        month -= t.month;
        year  -= t.year;
        return assureIntegrety();
    }

    Date& Date::assureIntegrety()
    {
        while(!inRange(day, 0, nbDaysThisMonth()-1))
        {
            if(day < 0)
            {
                year += modulateInt(--month, 0, NB_MONTHS_IN_YEAR);
                day  += nbDaysThisMonth();
            }
            else if(day >= nbDaysThisMonth())
            {
                day  -= nbDaysThisMonth();
                year += modulateInt(++month, 0, NB_MONTHS_IN_YEAR);
            }
        }

        year += modulateInt(month, 0, NB_MONTHS_IN_YEAR);
        return *this;
    }


    // Time
    Time::Time() :
        hour(0),
        minute(0),
        second(0)
    {
    }

    Time::Time(unsigned int hour, unsigned int minute, unsigned int second) :
        hour(hour),
        minute(minute),
        second(second)
    {
    }

    string Time::toString() const
    {
        stringstream stream;
        stream << hour << "h"
               << ((minute<10) ? "0" : "") << minute << "m"
               << ((second<10) ? "0" : "") << second << "s";
        return stream.str();
    }

    float Time::dayCompleteness() const
    {
        return (hour + (minute + second/
                        (float)NB_SECONDS_IN_MINUTE)/
                    (float)NB_MINUTES_IN_HOUR)/
                (float)NB_HOURS_IN_DAY;
    }

    int Time::toSeconds() const
    {
        return second + ((minute + (hour * NB_MINUTES_IN_HOUR)) * NB_SECONDS_IN_MINUTE);
    }

    Time& Time::fromSeconds(int nb)
    {
        second = nb % NB_SECONDS_IN_MINUTE;
        minute = (nb / NB_SECONDS_IN_MINUTE) % NB_MINUTES_IN_HOUR;
        hour = nb / (NB_SECONDS_IN_MINUTE * NB_MINUTES_IN_HOUR);
        return *this;
    }

    bool Time::operator< (const Time& t) const
    {
        return hour < t.hour     ? true : hour > t.hour     ? false :
               minute < t.minute ? true : minute > t.minute ? false :
               second < t.second ? true : false;
    }

    bool Time::operator==(const Time& t) const
    {
        return hour == t.hour  &&  minute == t.minute  &&  second == t.second;
    }

    Time& Time::operator+=(const Time& t)
    {
        second += t.second;
        minute += t.minute;
        hour   += t.hour;
        return assureIntegrety();
    }

    Time& Time::operator-=(const Time& t)
    {
        second -= t.second;
        minute -= t.minute;
        hour   -= t.hour;
        return assureIntegrety();
    }

    Time &Time::assureIntegrety()
    {
        minute += modulateInt(second, 0, (int)NB_SECONDS_IN_MINUTE);
        hour += modulateInt(minute, 0, (int)NB_MINUTES_IN_HOUR);
        modulateInt(hour, 0, (int)NB_HOURS_IN_DAY);
        return *this;
    }


    // DateAndTime
    DateAndTime::DateAndTime() :
        Date(),
        Time()
    {
    }

    DateAndTime::DateAndTime(int year, unsigned int month, unsigned int day,
                         unsigned int hour, unsigned int minute, unsigned int second) :
        Date(year, month, day),
        Time(hour, minute, second)
    {
    }

    string DateAndTime::toString() const
    {
        return Date::toString() + " at " + Time::toString();
    }

    bool DateAndTime::operator< (const DateAndTime& d) const
    {
        if(Date::operator ==(d))
            return Time::operator <(d);
        return Date::operator <(d);
    }

    bool DateAndTime::operator==(const DateAndTime& d) const
    {
        return Date::operator ==(d) && Time::operator ==(d);
    }

    DateAndTime& DateAndTime::operator+=(const DateAndTime& t)
    {
        second += t.second;
        minute += t.minute;
        hour   += t.hour;
        day    += t.day;
        month  += t.month;
        year   += t.year;
        return assureIntegrety();
    }

    DateAndTime& DateAndTime::operator-=(const DateAndTime& t)
    {
        second -= t.second;
        minute -= t.minute;
        hour   -= t.hour;
        day    -= t.day;
        month  -= t.month;
        year   -= t.year;
        return assureIntegrety();
    }

    DateAndTime& DateAndTime::assureIntegrety()
    {
        minute += modulateInt(second, 0, (int)NB_SECONDS_IN_MINUTE);
        hour += modulateInt(minute, 0, (int)NB_MINUTES_IN_HOUR);
        day += modulateInt(hour, 0, (int)NB_HOURS_IN_DAY);
        Date::assureIntegrety();
        return *this;
    }
}
