#include "SZCommon.h"

#ifdef SZ_TARGET_PLATFORM_WINDOWS

void get_locale_strings(void);

/* XXX This version of the implementation is not really complete.
Some of the fields cannot add information alone.  But if seeing
some of them in the same format (such as year, week and weekday)
this is enough information for determining the date.  */

#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#define match_char(ch1, ch2) \
	if (ch1 != ch2)          \
	return NULL

#ifdef _MSC_VER

#ifndef strcasecmp
#define strcasecmp _stricmp
#endif

#ifndef strncasecmp
#define strncasecmp _strnicmp
#endif

#endif

#ifndef Macintosh
#if defined __GNUC__ && __GNUC__ >= 2
#define match_string(cs1, s2) \
	({ size_t len = strlen (cs1);                                               \
      int result = strncasecmp ((cs1), (s2), len) == 0;                        \
      if (result) (s2) += len;                                                 \
      result; })
#else
/* Oh come on.  Get a reasonable compiler.  */
#define match_string(cs1, s2) \
	(strncasecmp((cs1), (s2), strlen(cs1)) ? 0 : ((s2) += strlen(cs1), 1))
#endif
#else
#define match_string(cs1, s2) \
	(strncmp((cs1), (s2), strlen(cs1)) ? 0 : ((s2) += strlen(cs1), 1))
#endif /* mac */

/* We intentionally do not use isdigit() for testing because this will
lead to problems with the wide character version.  */
#define get_number(from, to, n)                                            \
	do                                                                     \
	{                                                                      \
		int __n = n;                                                       \
		val = 0;                                                           \
		while (*rp == ' ')                                                 \
			++rp;                                                          \
		if (*rp < '0' || *rp > '9')                                        \
			return NULL;                                                   \
		do                                                                 \
		{                                                                  \
			val *= 10;                                                     \
			val += *rp++ - '0';                                            \
		} while (--__n > 0 && val * 10 <= to && *rp >= '0' && *rp <= '9'); \
		if (val < from || val > to)                                        \
			return NULL;                                                   \
	} while (0)
#define get_alt_number(from, to, n)                    \
	/* We don't have the alternate representation.  */ \
	get_number(from, to, n)
#define recursive(new_fmt) \
	(*(new_fmt) != '\0' && (rp = strptime_internal(rp, (new_fmt), tm, decided)) != NULL)

/* This version: may overwrite these with versions for the locale */
static char weekday_name[][20] =
	{
		"Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday"};
static char ab_weekday_name[][10] =
	{
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static char month_name[][20] =
	{
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"};
static char ab_month_name[][10] =
	{
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static char am_pm[][4] = {"AM", "PM"};

#define HERE_D_T_FMT "%a %b %e %H:%M:%S %Y"
#define HERE_D_FMT "%y/%m/%d"
#define HERE_T_FMT_AMPM "%I:%M:%S %p"
#define HERE_T_FMT "%H:%M:%S"

static const unsigned short int __mon_yday[2][13] =
	{
		/* Normal years.  */
		{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
		/* Leap years.  */
		{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}};

/* Status of lookup: do we use the locale data or the raw data?  */
enum locale_status
{
	null,
	loc,
	raw
};

#define __isleap(year) \
	((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

/* Compute the day of the week.  */
void day_of_the_week(struct tm *tm)
{
	/* We know that January 1st 1970 was a Thursday (= 4).  Compute the
	the difference between this data in the one on TM and so determine
	the weekday.  */
	int corr_year = 1900 + tm->tm_year - (tm->tm_mon < 2);
	int wday = (-473 + (365 * (tm->tm_year - 70)) + (corr_year / 4) - ((corr_year / 4) / 25) + ((corr_year / 4) % 25 < 0) + (((corr_year / 4) / 25) / 4) + __mon_yday[0][tm->tm_mon] + tm->tm_mday - 1);
	tm->tm_wday = ((wday % 7) + 7) % 7;
}

/* Compute the day of the year.  */
void day_of_the_year(struct tm *tm)
{
	tm->tm_yday = (__mon_yday[__isleap(1900 + tm->tm_year)][tm->tm_mon] + (tm->tm_mday - 1));
}

char *
strptime_internal(const char *rp, const char *fmt, struct tm *tm,
				  enum locale_status *decided)
{
	// const char *rp_backup;
	int cnt;
	size_t val;
	int have_I, is_pm;
	int century, want_century;
	int have_wday, want_xday;
	int have_yday;
	int have_mon, have_mday;

	have_I = is_pm = 0;
	century = -1;
	want_century = 0;
	have_wday = want_xday = have_yday = have_mon = have_mday = 0;

	while (*fmt != '\0')
	{
		/* A white space in the format string matches 0 more or white
		space in the input string.  */
		if (isspace(*fmt))
		{
			while (isspace(*rp))
				++rp;
			++fmt;
			continue;
		}

		/* Any character but `%' must be matched by the same character
		in the iput string.  */
		if (*fmt != '%')
		{
			match_char(*fmt++, *rp++);
			continue;
		}

		++fmt;

		/* We need this for handling the `E' modifier.  */
	start_over:

		/* Make back up of current processing pointer.  */
		// rp_backup = rp;

		switch (*fmt++)
		{
		case '%':
			/* Match the `%' character itself.  */
			match_char('%', *rp++);
			break;
		case 'a':
		case 'A':
			/* Match day of week.  */
			for (cnt = 0; cnt < 7; ++cnt)
			{
				if (*decided != loc && (match_string(weekday_name[cnt], rp) || match_string(ab_weekday_name[cnt], rp)))
				{
					*decided = raw;
					break;
				}
			}
			if (cnt == 7)
				/* Does not match a weekday name.  */
				return NULL;
			tm->tm_wday = cnt;
			have_wday = 1;
			break;
		case 'b':
		case 'B':
		case 'h':
			/* Match month name.  */
			for (cnt = 0; cnt < 12; ++cnt)
			{
				if (match_string(month_name[cnt], rp) || match_string(ab_month_name[cnt], rp))
				{
					*decided = raw;
					break;
				}
			}
			if (cnt == 12)
				/* Does not match a month name.  */
				return NULL;
			tm->tm_mon = cnt;
			want_xday = 1;
			break;
		case 'c':
			/* Match locale's date and time format.  */
			if (!recursive(HERE_T_FMT_AMPM))
				return NULL;
			break;
		case 'C':
			/* Match century number.  */
			get_number(0, 99, 2);
			century = val;
			want_xday = 1;
			break;
		case 'd':
		case 'e':
			/* Match day of month.  */
			get_number(1, 31, 2);
			tm->tm_mday = val;
			have_mday = 1;
			want_xday = 1;
			break;
		case 'F':
			if (!recursive("%Y-%m-%d"))
				return NULL;
			want_xday = 1;
			break;
		case 'x':
			/* Fall through.  */
		case 'D':
			/* Match standard day format.  */
			if (!recursive(HERE_D_FMT))
				return NULL;
			want_xday = 1;
			break;
		case 'k':
		case 'H':
			/* Match hour in 24-hour clock.  */
			get_number(0, 23, 2);
			tm->tm_hour = val;
			have_I = 0;
			break;
		case 'I':
			/* Match hour in 12-hour clock.  */
			get_number(1, 12, 2);
			tm->tm_hour = val % 12;
			have_I = 1;
			break;
		case 'j':
			/* Match day number of year.  */
			get_number(1, 366, 3);
			tm->tm_yday = val - 1;
			have_yday = 1;
			break;
		case 'm':
			/* Match number of month.  */
			get_number(1, 12, 2);
			tm->tm_mon = val - 1;
			have_mon = 1;
			want_xday = 1;
			break;
		case 'M':
			/* Match minute.  */
			get_number(0, 59, 2);
			tm->tm_min = val;
			break;
		case 'n':
		case 't':
			/* Match any white space.  */
			while (isspace(*rp))
				++rp;
			break;
		case 'p':
			/* Match locale's equivalent of AM/PM.  */
			if (!match_string(am_pm[0], rp))
			{
				if (match_string(am_pm[1], rp))
					is_pm = 1;
				else
					return NULL;
			}
			break;
		case 'r':
			if (!recursive(HERE_T_FMT_AMPM))
				return NULL;
			break;
		case 'R':
			if (!recursive("%H:%M"))
				return NULL;
			break;
		case 's':
		{
			/* The number of seconds may be very high so we cannot use
			the `get_number' macro.  Instead read the number
			character for character and construct the result while
			doing this.  */
			time_t secs = 0;
			if (*rp < '0' || *rp > '9')
				/* We need at least one digit.  */
				return NULL;

			do
			{
				secs *= 10;
				secs += *rp++ - '0';
			} while (*rp >= '0' && *rp <= '9');

			if ((gmtime_s(tm, &secs)) == 0)
				/* Error in function.  */
				return NULL;
		}
		break;
		case 'S':
			get_number(0, 61, 2);
			tm->tm_sec = val;
			break;
		case 'X':
			/* Fall through.  */
		case 'T':
			if (!recursive(HERE_T_FMT))
				return NULL;
			break;
		case 'u':
			get_number(1, 7, 1);
			tm->tm_wday = val % 7;
			have_wday = 1;
			break;
		case 'g':
			get_number(0, 99, 2);
			/* XXX This cannot determine any field in TM.  */
			break;
		case 'G':
			if (*rp < '0' || *rp > '9')
				return NULL;
			/* XXX Ignore the number since we would need some more
			information to compute a real date.  */
			do
				++rp;
			while (*rp >= '0' && *rp <= '9');
			break;
		case 'U':
		case 'V':
		case 'W':
			get_number(0, 53, 2);
			/* XXX This cannot determine any field in TM without some
			information.  */
			break;
		case 'w':
			/* Match number of weekday.  */
			get_number(0, 6, 1);
			tm->tm_wday = val;
			have_wday = 1;
			break;
		case 'y':
			/* Match year within century.  */
			get_number(0, 99, 2);
			/* The "Year 2000: The Millennium Rollover" paper suggests that
			values in the range 69-99 refer to the twentieth century.  */
			tm->tm_year = val >= 69 ? val : val + 100;
			/* Indicate that we want to use the century, if specified.  */
			want_century = 1;
			want_xday = 1;
			break;
		case 'Y':
			/* Match year including century number.  */
			get_number(0, 9999, 4);
			tm->tm_year = val - 1900;
			want_century = 0;
			want_xday = 1;
			break;
		case 'Z':
			/* XXX How to handle this?  */
			break;
		case 'E':
			/* We have no information about the era format.  Just use
			the normal format.  */
			if (*fmt != 'c' && *fmt != 'C' && *fmt != 'y' && *fmt != 'Y' && *fmt != 'x' && *fmt != 'X')
				/* This is an invalid format.  */
				return NULL;

			goto start_over;
		case 'O':
			switch (*fmt++)
			{
			case 'd':
			case 'e':
				/* Match day of month using alternate numeric symbols.  */
				get_alt_number(1, 31, 2);
				tm->tm_mday = val;
				have_mday = 1;
				want_xday = 1;
				break;
			case 'H':
				/* Match hour in 24-hour clock using alternate numeric
				symbols.  */
				get_alt_number(0, 23, 2);
				tm->tm_hour = val;
				have_I = 0;
				break;
			case 'I':
				/* Match hour in 12-hour clock using alternate numeric
				symbols.  */
				get_alt_number(1, 12, 2);
				tm->tm_hour = val - 1;
				have_I = 1;
				break;
			case 'm':
				/* Match month using alternate numeric symbols.  */
				get_alt_number(1, 12, 2);
				tm->tm_mon = val - 1;
				have_mon = 1;
				want_xday = 1;
				break;
			case 'M':
				/* Match minutes using alternate numeric symbols.  */
				get_alt_number(0, 59, 2);
				tm->tm_min = val;
				break;
			case 'S':
				/* Match seconds using alternate numeric symbols.  */
				get_alt_number(0, 61, 2);
				tm->tm_sec = val;
				break;
			case 'U':
			case 'V':
			case 'W':
				get_alt_number(0, 53, 2);
				/* XXX This cannot determine any field in TM without
				further information.  */
				break;
			case 'w':
				/* Match number of weekday using alternate numeric symbols.  */
				get_alt_number(0, 6, 1);
				tm->tm_wday = val;
				have_wday = 1;
				break;
			case 'y':
				/* Match year within century using alternate numeric symbols.  */
				get_alt_number(0, 99, 2);
				tm->tm_year = val >= 69 ? val : val + 100;
				want_xday = 1;
				break;
			default:
				return NULL;
			}
			break;
		default:
			return NULL;
		}
	}

	if (have_I && is_pm)
		tm->tm_hour += 12;

	if (century != -1)
	{
		if (want_century)
			tm->tm_year = tm->tm_year % 100 + (century - 19) * 100;
		else
			/* Only the century, but not the year.  Strange, but so be it.  */
			tm->tm_year = (century - 19) * 100;
	}

	if (want_xday && !have_wday)
	{
		if (!(have_mon && have_mday) && have_yday)
		{
			/* we don't have tm_mon and/or tm_mday, compute them */
			int t_mon = 0;
			while (__mon_yday[__isleap(1900 + tm->tm_year)][t_mon] <= tm->tm_yday)
				t_mon++;
			if (!have_mon)
				tm->tm_mon = t_mon - 1;
			if (!have_mday)
				tm->tm_mday = tm->tm_yday - __mon_yday[__isleap(1900 + tm->tm_year)][t_mon - 1] + 1;
		}
		day_of_the_week(tm);
	}
	if (want_xday && !have_yday)
		day_of_the_year(tm);

	return (char *)rp;
}

char *
strptime(const char *buf, const char *format, struct tm *tm)
{
	enum locale_status decided;
#ifdef HAVE_LOCALE_H
	if (!have_used_strptime)
	{
		get_locale_strings();
		/* have_used_strptime = 1; might change locale during session */
	}
#endif
	decided = raw;
	return strptime_internal(buf, format, tm, &decided);
}

#ifdef HAVE_LOCALE_H
void get_locale_strings(void)
{
	int i;
	struct tm tm;
	char buff[4];

	tm.tm_sec = tm.tm_min = tm.tm_hour = tm.tm_mday = tm.tm_mon = tm.tm_isdst = 0;
	tm.tm_year = 30;
	for (i = 0; i < 12; i++)
	{
		tm.tm_mon = i;
		strftime(ab_month_name[i], 10, "%b", &tm);
		strftime(month_name[i], 20, "%B", &tm);
	}
	tm.tm_mon = 0;
	for (i = 0; i < 7; i++)
	{
		tm.tm_mday = tm.tm_yday = i + 1; /* 2000-1-2 was a Sunday */
		tm.tm_wday = i;
		strftime(ab_weekday_name[i], 10, "%a", &tm);
		strftime(weekday_name[i], 20, "%A", &tm);
	}
	tm.tm_hour = 1;
	/* in locales where these are unused, they may be empty: better
	not to reset them then */
	strftime(buff, 4, "%p", &tm);
	if (strlen(buff))
		strcpy(am_pm[0], buff);
	tm.tm_hour = 13;
	strftime(buff, 4, "%p", &tm);
	if (strlen(buff))
		strcpy(am_pm[1], buff);
}
#endif

#endif

class TimezoneHelper
{
public:
	TimezoneHelper();

public:
#if !SZ_TARGET_PLATFORM_WINDOWS
	static std::string timezone_local;
#endif
	static int64_t timezone_diff_secs;
};

TimezoneHelper::TimezoneHelper()
{
	struct tm timeinfo;
	time_t secs, local_secs, gmt_secs;

	time(&secs);

	SZ_Common::localtime_r(&secs, &timeinfo);
	local_secs = ::mktime(&timeinfo);
#if !SZ_TARGET_PLATFORM_WINDOWS
	timezone_local = std::string(timeinfo.tm_zone);
#endif

	SZ_Common::gmtime_r(&secs, &timeinfo);

	gmt_secs = ::mktime(&timeinfo);
	timezone_diff_secs = local_secs - gmt_secs;
}

#if !SZ_TARGET_PLATFORM_WINDOWS
std::string TimezoneHelper::timezone_local;
#endif

int64_t TimezoneHelper::timezone_diff_secs = 0;

TimezoneHelper _TimeZoneHelper;

void SZ_Common::localtime_r(const time_t *clock, struct tm *result)
{
#if SZ_TARGET_PLATFORM_WINDOWS
	::localtime_s(result, clock);
#else
	::localtime_r(clock, result);
#endif
}

void SZ_Common::gmtime_r(const time_t *clock, struct tm *result)
{
#if SZ_TARGET_PLATFORM_WINDOWS
	::gmtime_s(result, clock);
#else
	::gmtime_r(clock, result);
#endif
}

time_t SZ_Common::timegm(struct tm *timeptr)
{
#if SZ_TARGET_PLATFORM_WINDOWS
	return ::_mkgmtime(timeptr);
#else
	return ::timegm(timeptr);
#endif
}

int SZ_Common::gettimeofday(struct timeval &tv)
{
#if SZ_TARGET_PLATFORM_WINDOWS
	static const DWORDLONG FILETIME_to_timeval_skew = 116444736000000000;
	FILETIME tfile;
	::GetSystemTimeAsFileTime(&tfile);

	ULARGE_INTEGER tmp;
	tmp.LowPart = tfile.dwLowDateTime;
	tmp.HighPart = tfile.dwHighDateTime;
	tmp.QuadPart -= FILETIME_to_timeval_skew;

	tv.tv_sec = (long)(tmp.QuadPart / (10000 * 1000));
	tv.tv_usec = (long)((tmp.QuadPart % (10000 * 1000)) / 10);
	return 0;
#else
	return ::gettimeofday(&tv, 0);
#endif
}

void SZ_Common::time2tmUTC(const time_t &tt, struct tm &stm)
{
	time_t localt = tt;

	SZ_Common::gmtime_r(&localt, &stm);

#if !SZ_TARGET_PLATFORM_WINDOWS
	static std::string local_timezone = "";
	stm.tm_zone = const_cast<char *>(local_timezone.c_str());
	stm.tm_gmtoff = TimezoneHelper::timezone_diff_secs;
#endif
}

void SZ_Common::tm2timeUTC(struct tm &stm, time_t &tt)
{
	tt = SZ_Common::timegm(&stm);
}

std::string SZ_Common::time2strUTC(const time_t &tt, const std::string &sFormat)
{
	struct tm stm
	{
	};
	SZ_Common::time2tmUTC(tt, stm);
	return tm2strUTC(stm, sFormat);
}

std::string SZ_Common::tm2strUTC(const struct tm &stm, const std::string &sFormat)
{
	char sTimeString[255] = "\0";
	strftime(sTimeString, sizeof(sTimeString), sFormat.c_str(), &stm);
	return std::string(sTimeString);
}

std::string SZ_Common::now2strUTC(const std::string &sFormat)
{
	auto t = time(nullptr);
	return time2strUTC(t, sFormat);
}

time_t SZ_Common::str2timeUTC(const std::string &sString, const std::string &sFormat)
{
	struct tm stm
	{
	};
	if (0 == SZ_Common::str2tmUTC(sString, sFormat, stm))
	{
		return SZ_Common::timegm(&stm);
	}
	return 0;
}

int SZ_Common::str2tmUTC(const std::string &sString, const std::string &sFormat, struct tm &stm)
{
	char *pst = strptime(sString.c_str(), sFormat.c_str(), &stm);
	return (pst != NULL) ? 0 : -1;
}

void SZ_Common::time2tm(const time_t &tt, struct tm &stm)
{
	time_t localt = tt + TimezoneHelper::timezone_diff_secs;

	SZ_Common::gmtime_r(&localt, &stm);

#if !SZ_TARGET_PLATFORM_WINDOWS
	static std::string local_timezone = TimezoneHelper::timezone_local;
	stm.tm_zone = const_cast<char *>(local_timezone.c_str());
	stm.tm_gmtoff = TimezoneHelper::timezone_diff_secs;
#endif
}

void SZ_Common::tm2time(struct tm &stm, time_t &tt)
{
	tt = SZ_Common::timegm(&stm) - TimezoneHelper::timezone_diff_secs;
}

std::string SZ_Common::time2str(const time_t &tt, const std::string &sFormat)
{
	struct tm stm
	{
	};
	SZ_Common::time2tm(tt, stm);
	return tm2str(stm, sFormat);
}

std::string SZ_Common::tm2str(const struct tm &stm, const std::string &sFormat)
{
	char sTimeString[255] = "\0";
	strftime(sTimeString, sizeof(sTimeString), sFormat.c_str(), &stm);
	return std::string(sTimeString);
}

std::string SZ_Common::now2str(const std::string &sFormat)
{
	auto t = time(nullptr);
	return time2str(t, sFormat);
}

time_t SZ_Common::str2time(const std::string &sString, const std::string &sFormat)
{
	struct tm stm
	{
	};
	if (0 == SZ_Common::str2tm(sString, sFormat, stm))
	{
		return SZ_Common::timegm(&stm) - TimezoneHelper::timezone_diff_secs;
	}
	return 0;
}

int SZ_Common::str2tm(const std::string &sString, const std::string &sFormat, struct tm &stm)
{
	char *pst = strptime(sString.c_str(), sFormat.c_str(), &stm);
	return (pst != NULL) ? 0 : -1;
}

int64_t SZ_Common::nows()
{
	return time(nullptr);
}

int64_t SZ_Common::nowms()
{
	timeval tv{0, 0};
	SZ_Common::gettimeofday(tv);
	return int64_t(tv.tv_sec) * 1000 + int64_t(tv.tv_usec) / 1000;
}

int64_t SZ_Common::nowus()
{
	timeval tv{0, 0};
	SZ_Common::gettimeofday(tv);
	return int64_t(tv.tv_sec) * 1000000 + int64_t(tv.tv_usec);
}

int SZ_Common::nowDate()
{
	return SZ_Common::stringTo<int>(SZ_Common::now2str("%Y%m%d"));
}

int64_t SZ_Common::nowTime()
{
	return SZ_Common::stringTo<int64_t>(SZ_Common::now2str("%Y%m%d%H%M%S"));
}

int SZ_Common::nextDate(int iDate, int offset)
{
	if (offset == 0)
	{
		return iDate;
	}
	else if (offset < 0)
	{
		return prevDate(iDate, std::abs(offset));
	}

	time_t t = SZ_Common::str2time(SZ_Common::toString(iDate * 1000000LL), "%Y%m%d%H%M%S");
	t = t + SECOND_AT_ONE_DAY * offset;
	return SZ_Common::stringTo<int>(SZ_Common::time2str(t, "%Y%m%d"));
}

int SZ_Common::prevDate(int iDate, int offset)
{
	if (offset == 0)
	{
		return iDate;
	}
	else if (offset < 0)
	{
		return nextDate(iDate, std::abs(offset));
	}

	time_t t = SZ_Common::str2time(SZ_Common::toString(iDate * 1000000LL), "%Y%m%d%H%M%S");
	t = t - SECOND_AT_ONE_DAY * offset;
	return SZ_Common::stringTo<int>(SZ_Common::time2str(t, "%Y%m%d"));
}

void SZ_Common::sleep(uint32_t sec)
{
	std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void SZ_Common::mssleep(uint32_t mssec)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(mssec));
}

int SZ_Common::rand(uint32_t &seed)
{
	seed = seed * 1103515245 + 12345;
	return static_cast<uint16_t>(seed / UINT16_MAX % INT16_MAX);
}

int SZ_Common::rand()
{
	thread_local static uint32_t seed = static_cast<uint16_t>(std::hash<std::string>()(SZ_Common::toString(std::this_thread::get_id())) % UINT32_MAX);

	return SZ_Common::rand(seed);
}

bool SZ_Common::equal(float x, float y, float epsilon)
{
	return std::fabs(x - y) < epsilon;
}
bool SZ_Common::equal(double x, double y, double epsilon)
{
	return std::fabs(x - y) < epsilon;
}
bool SZ_Common::equal(long double x, long double y, long double epsilon)
{
	return std::fabs(x - y) < epsilon;
}

std::string SZ_Common::ltrim(const std::string &source, const std::string &target)
{
	if (source.empty() || target.empty())
	{
		return source;
	}

	size_t pos = 0;
	while (pos < source.length())
	{
		if (target.find_first_of(source[pos]) == std::string::npos)
		{
			break;
		}
		++pos;
	}

	return (0 == pos ? source : source.substr(pos));
}

std::string SZ_Common::rtrim(const std::string &source, const std::string &target)
{
	if (source.empty() || target.empty())
	{
		return source;
	}

	size_t pos = source.length();
	while (pos > 0)
	{
		if (target.find_first_of(source[pos - 1]) == std::string::npos)
		{
			break;
		}
		--pos;
	}

	return (source.length() == pos ? source : source.substr(0, pos));
}

std::string SZ_Common::trim(const std::string &source, const std::string &target)
{
	return rtrim(ltrim(source, target), target);
}

std::string SZ_Common::ltrimSpace(const std::string &source)
{
	if (source.empty())
	{
		return source;
	}

	std::string::size_type pos = 0;
	while (pos < source.length())
	{
		if (!std::isspace(source[pos]))
		{
			break;
		}

		pos++;
	}

	if (pos == 0)
	{
		return source;
	}

	return source.substr(pos);
}

std::string SZ_Common::rtrimSpace(const std::string &source)
{
	if (source.empty())
	{
		return source;
	}

	std::string::size_type pos = source.length();
	while (pos != 0)
	{
		if (!std::isspace(source[pos - 1]))
		{
			break;
		}

		pos--;
	}

	if (pos == source.length())
	{
		return source;
	}

	return source.substr(0, pos);
}

std::string SZ_Common::trimSpace(const std::string &source)
{
	if (source.empty())
	{
		return source;
	}

	return rtrimSpace(ltrimSpace(source));
}

std::string SZ_Common::toLower(const std::string &source)
{
	std::string str = source;
	for (auto &c : str)
	{
		c = std::tolower(c);
	}
	return str;
}

std::string SZ_Common::toUpper(const std::string &source)
{
	std::string str = source;
	for (auto &c : str)
	{
		c = std::toupper(c);
	}
	return str;
}

bool SZ_Common::isDigit(const std::string &source)
{
	if (source.empty())
	{
		return false;
	}

	for (auto &c : source)
	{
		if (!std::isdigit(c))
		{
			return false;
		}
	}
	return true;
}

bool SZ_Common::isXDigit(const std::string &source)
{
	if (source.empty())
	{
		return false;
	}

	for (auto &c : source)
	{
		if (!std::isxdigit(c))
		{
			return false;
		}
	}
	return true;
}

bool SZ_Common::isAlpha(const std::string &source)
{
	if (source.empty())
	{
		return false;
	}

	for (auto &c : source)
	{
		if (!std::isalpha(c))
		{
			return false;
		}
	}
	return true;
}

bool SZ_Common::isAlnum(const std::string &source)
{
	if (source.empty())
	{
		return false;
	}

	for (auto &c : source)
	{
		if (!std::isalnum(c))
		{
			return false;
		}
	}
	return true;
}

std::vector<std::string> SZ_Common::splitString(const std::string &str, const std::string &split, bool withEmpty)
{
	std::vector<std::string> vStr;

	std::string::size_type start = 0;
	std::string::size_type pos = 0;

	while (true)
	{
		std::string s;
		pos = str.find_first_of(split, start);
		if (pos == std::string::npos)
		{
			if (start + 1 <= str.length())
			{
				s = str.substr(start);
			}
		}
		else if (pos == start)
		{
			s = "";
		}
		else
		{
			s = str.substr(start, pos - start);
			start = pos;
		}

		if (withEmpty || !s.empty())
		{
			vStr.push_back(std::move(s));
		}

		if (pos == std::string::npos)
		{
			break;
		}

		start++;
	}

	return vStr;
}

std::string SZ_Common::joinString(const std::vector<std::string> &vstr, const std::string &join, bool withEmpty)
{
	std::stringstream sstr;
	bool isFirst = true;
	for (const auto &str : vstr)
	{
		if (!str.empty() || withEmpty)
		{
			sstr << (isFirst ? "" : join) << str;
			isFirst = false;
		}
	}

	return sstr.str();
}

std::string SZ_Common::replaceString(const std::string &str, const std::string &src, const std::string &dest)
{
	if (src.empty())
	{
		return str;
	}

	std::string sResult = str;
	std::string::size_type pos = 0;

	while ((pos = sResult.find(src, pos)) != std::string::npos)
	{
		sResult.replace(pos, src.length(), dest);
		pos += dest.length();
	}

	return sResult;
}

template <>
std::string SZ_Common::toString(const char &val)
{
	char buf[2] = "\0";
	snprintf(buf, 2, "%c", val);
	return std::string(buf);
}

template <>
std::string SZ_Common::toString(const unsigned char &val)
{
	char buf[2] = "\0";
	snprintf(buf, 2, "%c", val);
	return std::string(buf);
}

template <>
std::string SZ_Common::toString(const int &val)
{
	return std::to_string(val);
}

template <>
std::string SZ_Common::toString(const unsigned &val)
{
	return std::to_string(val);
}

template <>
std::string SZ_Common::toString(const long &val)
{
	return std::to_string(val);
}

template <>
std::string SZ_Common::toString(const unsigned long &val)
{
	return std::to_string(val);
}

template <>
std::string SZ_Common::toString(const long long &val)
{
	return std::to_string(val);
}

template <>
std::string SZ_Common::toString(const unsigned long long &val)
{
	return std::to_string(val);
}

template <>
std::string SZ_Common::toString(const float &val)
{
	return std::to_string(val);
}

template <>
std::string SZ_Common::toString(const double &val)
{
	return std::to_string(val);
}

template <>
std::string SZ_Common::toString(const long double &val)
{
	return std::to_string(val);
}

template <>
std::string SZ_Common::toString(const std::string &val)
{
	return val;
}
