#ifndef CELLARWORKBENCH_CALENDAR_H
#define CELLARWORKBENCH_CALENDAR_H

#include <string>

#include "../libCellarWorkbench_global.h"


namespace cellar
{
    enum class EMonth {JANUARY,     FEBRUARY,   MARCH,      APRIL,
                      MAY,         JUNE,       JULY,       AUGUST,
                      SEPTEMBER,   OCTOBER,    NOVEMBER,   DECEMBER, NB_MONTHS};

    enum class ETimeJump {YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, NB_TIME_JUMPS};

    struct CELLAR_EXPORT Date
    {
        Date();
        Date(int year, unsigned int month, unsigned int day);

        std::string toString() const;
        bool  isThisYearLeap() const;
        int   nbDaysThisMonth();
        float yearCompleteness() const;
        int   toDays() const;
        int   toDaysInYear() const;
        Date& fromDays(int nb);
        bool  operator< (const Date& d) const;
        bool  operator==(const Date& d) const;
        bool  operator!=(const Date& t) const;
        Date  operator+ (const Date& t) const;
        Date& operator+=(const Date& t);
        Date  operator- (const Date& t) const;
        Date& operator-=(const Date& t);
        Date& assureIntegrety();

        int year;
        int month;
        int day;

        static bool isLeap(int year);
        static int  nbDaysInMonth(int year, int month);
        static const int NB_MONTHS_IN_YEAR = 12;
        static const int NB_DAYS_IN_MONTH[NB_MONTHS_IN_YEAR];
        static const std::string  MONTH_NAMES[NB_MONTHS_IN_YEAR];
    };

    struct CELLAR_EXPORT Time
    {
        Time();
        Time(unsigned int hour, unsigned int minute, unsigned int second);

        std::string toString() const;
        float dayCompleteness() const;
        int toSeconds() const;
        Time& fromSeconds(int nb);
        bool  operator< (const Time& t) const;
        bool  operator==(const Time& t) const;
        bool  operator!=(const Time& t) const;
        Time  operator+ (const Time& t) const;
        Time& operator+=(const Time& t);
        Time  operator- (const Time& t) const;
        Time& operator-=(const Time& t);
        Time& assureIntegrety();

        int hour;
        int minute;
        int second;

        static const int NB_HOURS_IN_DAY = 24;
        static const int NB_MINUTES_IN_HOUR = 60;
        static const int NB_SECONDS_IN_MINUTE = 60;
    };

    struct CELLAR_EXPORT DateAndTime : public Date, public Time
    {
        DateAndTime();
        DateAndTime(int year, unsigned int month, unsigned int day,
                    unsigned int hour, unsigned int minute, unsigned int second);
        std::string toString() const;
        bool operator< (const DateAndTime& d) const;
        bool operator==(const DateAndTime& d) const;
        bool operator!=(const DateAndTime& t) const;
        DateAndTime  operator+ (const DateAndTime& t) const;
        DateAndTime& operator+=(const DateAndTime& t);
        DateAndTime  operator- (const DateAndTime& t) const;
        DateAndTime& operator-=(const DateAndTime& t);
        DateAndTime& assureIntegrety();
    };



    // IMPLEMENTATION //
    inline bool Date::isLeap(int year)
    {
        return (!(year%400) || (!(year%4) && year%100));
    }

    inline int Date::nbDaysInMonth(int year, int month)
    {
        return NB_DAYS_IN_MONTH[month] + (isLeap(year) && month == (int)EMonth::FEBRUARY ? 1 : 0);
    }

    inline bool Date::operator!=(const Date& t) const
    {
        return !(*this==t);
    }

    inline Date Date::operator+ (const Date& t) const
    {
        return Date(*this) += t;
    }

    inline Date Date::operator- (const Date& t) const
    {
        return Date(*this) -= t;
    }


    inline bool Time::operator!=(const Time& t) const
    {
        return !(*this==t);
    }

    inline Time Time::operator+ (const Time& t) const
    {
        return Time(*this) += t;
    }

    inline Time Time::operator- (const Time& t) const
    {
        return Time(*this) -= t;
    }


    inline bool DateAndTime::operator!=(const DateAndTime& t) const
    {
        return !(*this==t);
    }

    inline DateAndTime DateAndTime::operator+ (const DateAndTime& t) const
    {
        return DateAndTime(*this) += t;
    }

    inline DateAndTime DateAndTime::operator- (const DateAndTime& t) const
    {
        return DateAndTime(*this) -= t;
    }
}

#endif // CELLARWORKBENCH_CALENDAR_H
