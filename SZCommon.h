#pragma once

#include <algorithm>
#include <array>
#include <initializer_list>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <sstream>
#include <string>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <chrono>
#include <memory>
#include <cstdint>
#include <sys/types.h>
#include <ctime>
#include <sys/timeb.h>
#include <cstring>
#include <thread>
#include <cmath>

#if defined WIN32 || defined _WIN32 || defined _WIN64
#define SZ_TARGET_PLATFORM_WINDOWS 1
#elif __APPLE__
#define SZ_TARGET_PLATFORM_IOS 1
#elif defined ANDROID
#define SZ_TARGET_PLATFORM_ANDROID 1
#define SZ_TARGET_PLATFORM_LINUX 1
#elif __linux__
#define SZ_TARGET_PLATFORM_LINUX 1
#else
#error Unsupported platform.
#endif

#if defined SZ_TARGET_PLATFORM_WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#if defined SZ_TARGET_PLATFORM_WINDOWS
#define SZ_FILE_NAME(x) (strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x)
#else
#define SZ_FILE_NAME(x) (strrchr(x, '/') ? strrchr(x, '/') + 1 : x)
#endif
#define SZ_FILE_FUNC_LINE "[" << SZ_FILE_NAME(__FILE__) << "::" << __FUNCTION__ << "::" << __LINE__ << "]"

#define NormalPropertyBuilder(access_permission, type, name, value) \
	access_permission : type m_##name{value};                       \
                                                                    \
public:                                                             \
	inline void set_##name(type value_##name)                       \
	{                                                               \
		m_##name = value_##name;                                    \
	}                                                               \
	inline type get_##name() const                                  \
	{                                                               \
		return m_##name;                                            \
	}

#define PointerPropertyBuilder(access_permission, type, name, value) \
	access_permission : type *m_##name{value};                       \
                                                                     \
public:                                                              \
	inline void set_##name(type *const &value_##name)                \
	{                                                                \
		m_##name = value_##name;                                     \
	}                                                                \
	inline type *get_##name()                                        \
	{                                                                \
		return m_##name;                                             \
	}

#define ReferencePropertyBuilder(access_permission, type, name, value) \
	access_permission : type m_##name{value};                          \
                                                                       \
public:                                                                \
	inline void set_##name(const type &value_##name)                   \
	{                                                                  \
		m_##name = value_##name;                                       \
	}                                                                  \
	inline const type &get_##name() const                              \
	{                                                                  \
		return m_##name;                                               \
	}

#define NormalPropertyBuilderRead(access_permission, type, name, value) \
	access_permission : type m_##name{value};                           \
                                                                        \
public:                                                                 \
	inline type get_##name() const                                      \
	{                                                                   \
		return m_##name;                                                \
	}

#define PointerPropertyBuilderRead(access_permission, type, name, value) \
	access_permission : type *m_##name{value};                           \
                                                                         \
public:                                                                  \
	inline type *get_##name()                                            \
	{                                                                    \
		return m_##name;                                                 \
	}

#define ReferencePropertyBuilderRead(access_permission, type, name, value) \
	access_permission : type m_##name{value};                              \
                                                                           \
public:                                                                    \
	inline const type &get_##name() const                                  \
	{                                                                      \
		return m_##name;                                                   \
	}

namespace SZ_Common
{
	const float EPSILON_FLOAT = 1.0E-6F;
	const double EPSILON_DOUBLE = 1.0E-6F;
	const long double EPSILON_LONG_DOUBLE = 1.0E-10F;

	constexpr time_t SECOND_AT_ONE_MINUTE = 60;
	constexpr time_t MINUTE_AT_ONE_HOUR = 60;
	constexpr time_t HOUR_AT_ONE_DAY = 24;
	constexpr time_t SECOND_AT_ONE_HOUR = SECOND_AT_ONE_MINUTE * MINUTE_AT_ONE_HOUR;
	constexpr time_t MINUTE_AT_ONE_DAY = MINUTE_AT_ONE_HOUR * HOUR_AT_ONE_DAY;
	constexpr time_t SECOND_AT_ONE_DAY = SECOND_AT_ONE_MINUTE * MINUTE_AT_ONE_HOUR * HOUR_AT_ONE_DAY;

	constexpr int64_t TEN_MULTIPLE = 10;
	constexpr int64_t HUNDRED_MULTIPLE = 100;
	constexpr int64_t THOUSAND_MULTIPLE = 1000;
	constexpr int64_t MYRIAD_MULTIPLE = 10000;
	constexpr int64_t MILLION_MULTIPLE = THOUSAND_MULTIPLE * THOUSAND_MULTIPLE;
	constexpr int64_t BILLION_MULTIPLE = MILLION_MULTIPLE * THOUSAND_MULTIPLE;
	constexpr int64_t TRILLION_MULTIPLE = BILLION_MULTIPLE * THOUSAND_MULTIPLE;
	constexpr int64_t QUADRILLION_MULTIPLE = TRILLION_MULTIPLE * THOUSAND_MULTIPLE;

	constexpr int64_t KB = 1024;
	constexpr int64_t MB = KB * KB;
	constexpr int64_t GB = MB * KB;
	constexpr int64_t TB = GB * KB;
	constexpr int64_t PB = TB * KB;

	/**
	 * @brief
	 *
	 * @param clock
	 * @param result
	 */
	void localtime_r(const time_t *clock, struct tm *result);

	/**
	 * @brief
	 *
	 * @param clock
	 * @param result
	 */
	void gmtime_r(const time_t *clock, struct tm *result);

	/**
	 * @brief
	 *
	 * @param clock
	 * @param result
	 */
	time_t timegm(struct tm *timeptr);

	/**
	 * @brief
	 *
	 * @param tv
	 * @return int
	 */
	int gettimeofday(struct timeval &tv);

	/**
	 * @brief time_t -> tm
	 *
	 * @param tt
	 * @param stm
	 */
	void time2tmUTC(const time_t &tt, struct tm &stm);

	/**
	 * @brief tm -> time_t
	 *
	 * @param stm
	 * @param tt
	 */
	void tm2timeUTC(struct tm &stm, time_t &tt);

	/**
	 * @brief time_t -> string
	 *
	 * @param t
	 * @param sFormat
	 * @return std::string
	 */
	std::string time2strUTC(const time_t &tt, const std::string &sFormat = "%Y%m%d%H%M%S");

	/**
	 * @brief tm -> string
	 *
	 * @param stm
	 * @param sFormat
	 * @return std::string
	 */
	std::string tm2strUTC(const struct tm &stm, const std::string &sFormat = "%Y%m%d%H%M%S");

	/**
	 * @brief time -> string
	 *
	 * @param sFormat
	 * @return std::string
	 */
	std::string now2strUTC(const std::string &sFormat = "%Y%m%d%H%M%S");

	/**
	 * @brief string -> time_t
	 * @param sString
	 * @param sFormat
	 * @return time_t
	 */
	time_t str2timeUTC(const std::string &sString, const std::string &sFormat);

	/**
	 * @brief string -> time_t
	 * @param sString
	 * @param sFormat
	 * @param stm
	 * @return time_t
	 */
	int str2tmUTC(const std::string &sString, const std::string &sFormat, struct tm &stm);

	/**
	 * @brief time_t -> tm
	 *
	 * @param tt
	 * @param stm
	 */
	void time2tm(const time_t &tt, struct tm &stm);

	/**
	 * @brief tm -> time_t
	 *
	 * @param stm
	 * @param tt
	 */
	void tm2time(struct tm &stm, time_t &tt);

	/**
	 * @brief time_t -> string
	 *
	 * @param t
	 * @param sFormat
	 * @return std::string
	 */
	std::string time2str(const time_t &tt, const std::string &sFormat = "%Y%m%d%H%M%S");

	/**
	 * @brief tm -> string
	 *
	 * @param stm
	 * @param sFormat
	 * @return std::string
	 */
	std::string tm2str(const struct tm &stm, const std::string &sFormat = "%Y%m%d%H%M%S");

	/**
	 * @brief time -> string
	 *
	 * @param sFormat
	 * @return std::string
	 */
	std::string now2str(const std::string &sFormat = "%Y%m%d%H%M%S");

	/**
	 * @brief string -> time_t
	 * @param sString
	 * @param sFormat
	 * @return time_t
	 */
	time_t str2time(const std::string &sString, const std::string &sFormat);

	/**
	 * @brief string -> time_t
	 * @param sString
	 * @param sFormat
	 * @param stm
	 * @return time_t
	 */
	int str2tm(const std::string &sString, const std::string &sFormat, struct tm &stm);

	/**
	 * @brief
	 *
	 * @return int64_t
	 */
	int64_t nows();

	/**
	 * @brief
	 *
	 * @return int64_t
	 */
	int64_t nowms();

	/**
	 * @brief
	 *
	 * @return int64_t
	 */
	int64_t nowus();

	/**
	 * @brief
	 *
	 * @return int64_t
	 */
	int nowDate();

	/**
	 * @brief
	 *
	 * @return int64_t
	 */
	int64_t nowTime();

	/**
	 * @brief:
	 *
	 * @return: int
	 * @param:  iDate
	 * @param:  offset
	 */
	int nextDate(int iDate, int offset = 1);

	/**
	 * @brief:
	 *
	 * @return: int
	 * @param:  iDate
	 * @param:  offset
	 */
	int prevDate(int iDate, int offset = 1);

	/**
	 * @brief:
	 *
	 * @param:  sec
	 */
	void sleep(uint32_t sec);

	/**
	 * @brief:
	 *
	 * @param:
	 */
	void mssleep(uint32_t mssec);

	/**
	 * @brief
	 */
	int rand(uint32_t &seed);

	/**
	 * @brief
	 */
	int rand();

	/**
	 * @brief 比较浮点数是否相等
	 *
	 * @param x
	 * @param y
	 * @param epsilon
	 * @return bool
	 */
	bool equal(float x, float y, float epsilon = EPSILON_FLOAT);
	bool equal(double x, double y, double epsilon = EPSILON_DOUBLE);
	bool equal(long double x, long double y, long double epsilon = EPSILON_LONG_DOUBLE);

	/**
	 * @brief
	 *
	 * @param source
	 * @param str
	 * @return std::string
	 */
	std::string ltrim(const std::string &source, const std::string &target = " \r\n\t\v\f");

	/**
	 * @brief
	 *
	 * @param source
	 * @param str
	 * @return std::string
	 */
	std::string rtrim(const std::string &source, const std::string &target = " \r\n\t\v\f");

	/**
	 * @brief
	 *
	 * @param source
	 * @param str
	 * @return std::string
	 */
	std::string trim(const std::string &source, const std::string &target = " \r\n\t\v\f");

	/**
	 * @brief
	 *
	 * @param source
	 * @return std::string
	 */
	std::string ltrimSpace(const std::string &source);

	/**
	 * @brief
	 *
	 * @param source
	 * @return std::string
	 */
	std::string rtrimSpace(const std::string &source);

	/**
	 * @brief
	 *
	 * @param source
	 * @return std::string
	 */
	std::string trimSpace(const std::string &source);

	/**
	 * @brief
	 *
	 * @param source
	 * @return std::string
	 */
	std::string toLower(const std::string &source);

	/**
	 * @brief
	 *
	 * @param source
	 * @return std::string
	 */
	std::string toUpper(const std::string &source);

	/**
	 * @brief
	 *
	 * @param source
	 * @return bool
	 */
	bool isDigit(const std::string &source);

	/**
	 * @brief
	 *
	 * @param source
	 * @return bool
	 */
	bool isXDigit(const std::string &source);

	/**
	 * @brief
	 *
	 * @param source
	 * @return bool
	 */
	bool isAlpha(const std::string &source);

	/**
	 * @brief
	 *
	 * @param source
	 * @return bool
	 */
	bool isAlnum(const std::string &source);

	/**
	 * @brief
	 *
	 * @param str
	 * @param split
	 * @param withEmpty
	 * @return std::string
	 */
	std::vector<std::string> splitString(const std::string &str, const std::string &split, bool withEmpty = false);

	/**
	 * @brief
	 *
	 * @param vstr
	 * @param join
	 * @param withEmpty
	 * @return std::string
	 */
	std::string joinString(const std::vector<std::string> &vstr, const std::string &join, bool withEmpty = false);

	/**
	 * @brief
	 *
	 * @param str
	 * @param src
	 * @param dest
	 * @return std::string
	 */
	std::string replaceString(const std::string &str, const std::string &src, const std::string &dest);

	/**
	 * @brief
	 *
	 * @tparam T
	 * @param val
	 * @return std::string
	 */
	template <typename T>
	std::string toString(const T &val);

	template <typename X, typename Y>
	std::string toString(const std::pair<X, Y> &p);

	template <typename Iter>
	std::string toString(const Iter &beg, const Iter &end);

	template <typename T, size_t U>
	std::string toString(const std::array<T, U> &a);

	template <typename T>
	std::string toString(const std::initializer_list<T> &l);

	template <typename T>
	std::string toString(const std::vector<T> &v);

	template <typename T>
	std::string toString(const std::list<T> &l);

	template <typename T>
	std::string toString(const std::deque<T> &d);

	template <typename T>
	std::string toString(const std::set<T> &s);

	template <typename T>
	std::string toString(const std::unordered_set<T> &s);

	template <typename K, typename V>
	std::string toString(const std::map<K, V> &m);

	template <typename K, typename V>
	std::string toString(const std::unordered_map<K, V> &m);

	/**
	 * @brief
	 *
	 * @tparam T
	 * @param str
	 * @return T
	 */
	template <typename T>
	T stringTo(const std::string &str);

	/**
	 * @brief
	 *
	 * @tparam Container
	 * @param con
	 * @param str
	 */
	template <typename Container>
	void toString(const Container &con, std::string &str);

	/**
	 * @brief
	 *
	 * @tparam Iter
	 * @param beg
	 * @param end
	 * @param str
	 */
	template <typename Iter>
	void toString(const Iter &beg, const Iter &end, std::string &str);
};

template <>
std::string SZ_Common::toString(const char &val);

template <>
std::string SZ_Common::toString(const unsigned char &val);

template <>
std::string SZ_Common::toString(const int &val);

template <>
std::string SZ_Common::toString(const unsigned &val);

template <>
std::string SZ_Common::toString(const long &val);

template <>
std::string SZ_Common::toString(const unsigned long &val);

template <>
std::string SZ_Common::toString(const long long &val);

template <>
std::string SZ_Common::toString(const unsigned long long &val);

template <>
std::string SZ_Common::toString(const float &val);

template <>
std::string SZ_Common::toString(const double &val);

template <>
std::string SZ_Common::toString(const long double &val);

template <>
std::string SZ_Common::toString(const std::string &val);

template <typename T>
std::string SZ_Common::toString(const T &val)
{
	std::ostringstream ss;
	ss << val;
	return ss.str();
}

template <typename X, typename Y>
std::string SZ_Common::toString(const std::pair<X, Y> &p)
{
	std::string str;

	str += "(";
	str += SZ_Common::toString(p.first);
	str += ", ";
	str += SZ_Common::toString(p.second);
	str += ")";

	return str;
}

template <typename Iter>
std::string SZ_Common::toString(const Iter &beg, const Iter &end)
{
	std::string str;
	SZ_Common::toString(beg, end, str);
	return str;
}

template <typename T, size_t U>
std::string SZ_Common::toString(const std::array<T, U> &a)
{
	std::string str;
	SZ_Common::toString(a, str);
	return str;
}

template <typename T>
std::string SZ_Common::toString(const std::initializer_list<T> &l)
{
	std::string str;
	SZ_Common::toString(l, str);
	return str;
}

template <typename T>
std::string SZ_Common::toString(const std::vector<T> &v)
{
	std::string str;
	SZ_Common::toString(v, str);
	return str;
}

template <typename T>
std::string SZ_Common::toString(const std::list<T> &l)
{
	std::string str;
	SZ_Common::toString(l, str);
	return str;
}

template <typename T>
std::string SZ_Common::toString(const std::deque<T> &d)
{
	std::string str;
	SZ_Common::toString(d, str);
	return str;
}

template <typename T>
std::string SZ_Common::toString(const std::set<T> &s)
{
	std::string str;
	SZ_Common::toString(s, str);
	return str;
}

template <typename T>
std::string SZ_Common::toString(const std::unordered_set<T> &s)
{
	std::string str;
	SZ_Common::toString(s, str);
	return str;
}

template <typename K, typename V>
std::string SZ_Common::toString(const std::map<K, V> &m)
{
	std::string str;
	SZ_Common::toString(m, str);
	return str;
}

template <typename K, typename V>
std::string SZ_Common::toString(const std::unordered_map<K, V> &m)
{
	std::string str;
	SZ_Common::toString(m, str);
	return str;
}

template <typename Container>
void SZ_Common::toString(const Container &con, std::string &str)
{
	bool isFirst = true;
	str += "[";
	for (auto &element : con)
	{
		if (isFirst)
		{
			isFirst = false;
		}
		else
		{
			str += ", ";
		}
		str += SZ_Common::toString(element);
	}
	str += "]";
}

template <typename Iter>
void SZ_Common::toString(const Iter &beg, const Iter &end, std::string &str)
{
	str += "[";
	for (auto it = beg; it != end; ++it)
	{
		if (it != beg)
		{
			str += ", ";
		}
		str += SZ_Common::toString(*it);
	}
	str += "]";
}

namespace CM
{

	template <typename T>
	struct stringToX
	{
		T operator()(const std::string &str)
		{
			std::string s = "0";
			if (!str.empty())
			{
				s = str;
			}

			std::istringstream ss(s);
			T val;
			ss >> val;

			return val;
		}
	};

	template <>
	struct stringToX<int>
	{
		int operator()(const std::string &str)
		{
			if (!str.empty())
			{
				int radix = (str.find("0x") == 0 ? 16 : 10);
				return static_cast<int>(std::strtol(str.c_str(), nullptr, radix));
			}
			return 0;
		}
	};

	template <>
	struct stringToX<unsigned>
	{
		unsigned operator()(const std::string &str)
		{
			if (!str.empty())
			{
				int radix = (str.find("0x") == 0 ? 16 : 10);
				return static_cast<unsigned>(std::strtoul(str.c_str(), nullptr, radix));
			}
			return 0;
		}
	};

	template <>
	struct stringToX<long>
	{
		long operator()(const std::string &str)
		{
			if (!str.empty())
			{
				int radix = (str.find("0x") == 0 ? 16 : 10);
				return std::strtol(str.c_str(), nullptr, radix);
			}
			return 0;
		}
	};

	template <>
	struct stringToX<unsigned long>
	{
		unsigned long operator()(const std::string &str)
		{
			if (!str.empty())
			{
				int radix = (str.find("0x") == 0 ? 16 : 10);
				return std::strtoul(str.c_str(), nullptr, radix);
			}
			return 0;
		}
	};

	template <>
	struct stringToX<long long>
	{
		long long operator()(const std::string &str)
		{
			if (!str.empty())
			{
				int radix = (str.find("0x") == 0 ? 16 : 10);
				return std::strtoll(str.c_str(), nullptr, radix);
			}
			return 0;
		}
	};

	template <>
	struct stringToX<unsigned long long>
	{
		unsigned long long operator()(const std::string &str)
		{
			if (!str.empty())
			{
				int radix = (str.find("0x") == 0 ? 16 : 10);
				return std::strtoull(str.c_str(), nullptr, radix);
			}
			return 0;
		}
	};

	template <>
	struct stringToX<float>
	{
		float operator()(const std::string &str)
		{
			if (!str.empty())
			{
				return std::strtof(str.c_str(), nullptr);
			}
			return 0;
		}
	};

	template <>
	struct stringToX<double>
	{
		double operator()(const std::string &str)
		{
			if (!str.empty())
			{
				return std::strtod(str.c_str(), nullptr);
			}
			return 0;
		}
	};

	template <>
	struct stringToX<long double>
	{
		long double operator()(const std::string &str)
		{
			if (!str.empty())
			{
				return std::strtold(str.c_str(), nullptr);
			}
			return 0;
		}
	};

	template <>
	struct stringToX<std::string>
	{
		std::string operator()(const std::string &str)
		{
			return str;
		}
	};

	template <typename T>
	struct stringToY
	{
		T operator()(const std::string &str)
		{
			std::istringstream ss(str);
			T val;
			ss >> val;

			return val;
		}
	};

} // namespace CM

template <typename T>
T SZ_Common::stringTo(const std::string &str)
{
	using stringto_type = typename std::conditional<std::is_arithmetic<T>::value, CM::stringToX<T>, CM::stringToY<T>>::type;
	return stringto_type()(str);
}
