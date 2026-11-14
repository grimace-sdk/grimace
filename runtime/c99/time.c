#include <stddef.h>
#include <time.h>

typedef struct {
    unsigned short year;
    unsigned short month;
    unsigned short day_of_week;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned short milliseconds;
} system_time;

typedef struct {
    unsigned long low;
    unsigned long high;
} file_time;

typedef struct {
    long bias;
    unsigned short standard_name[32];
    system_time standard_date;
    long standard_bias;
    unsigned short daylight_name[32];
    system_time daylight_date;
    long daylight_bias;
} time_zone_information;

void GetSystemTime(system_time *time);
void GetLocalTime(system_time *time);
int SystemTimeToFileTime(const system_time *time, file_time *result);
unsigned long GetTimeZoneInformation(time_zone_information *information);
unsigned long GetTickCount(void);

#define UNIX_EPOCH_IN_FILETIME 116444736000000000LL
#define TIME_ZONE_ID_DAYLIGHT 2

static const char *const day_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
static const char *const month_names[] = {"January", "February", "March",     "April",   "May",      "June",
                                          "July",    "August",   "September", "October", "November", "December"};

static long long days_from_civil(long long year, int month, int day) {
    year -= month <= 2;
    long long era = (year >= 0 ? year : year - 399) / 400;
    long long year_of_era = year - era * 400;
    long long day_of_year = (153 * (month + (month > 2 ? -3 : 9)) + 2) / 5 + day - 1;
    long long day_of_era = year_of_era * 365 + year_of_era / 4 - year_of_era / 100 + day_of_year;
    return era * 146097 + day_of_era - 719468;
}

static void civil_from_seconds(long long seconds, struct tm *result) {
    long long days = seconds / 86400;
    long long remainder = seconds % 86400;
    if (remainder < 0) {
        remainder += 86400;
        days--;
    }
    result->tm_hour = (int)(remainder / 3600);
    result->tm_min = (int)(remainder / 60 % 60);
    result->tm_sec = (int)(remainder % 60);
    result->tm_wday = (int)((days % 7 + 11) % 7);
    long long shifted = days + 719468;
    long long era = (shifted >= 0 ? shifted : shifted - 146096) / 146097;
    long long day_of_era = shifted - era * 146097;
    long long year_of_era = (day_of_era - day_of_era / 1460 + day_of_era / 36524 - day_of_era / 146096) / 365;
    long long day_of_year = day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100);
    long long month_index = (5 * day_of_year + 2) / 153;
    int day = (int)(day_of_year - (153 * month_index + 2) / 5 + 1);
    int month = (int)(month_index < 10 ? month_index + 3 : month_index - 9);
    long long year = year_of_era + era * 400 + (month <= 2);
    result->tm_mday = day;
    result->tm_mon = month - 1;
    result->tm_year = (int)(year - 1900);
    result->tm_yday = (int)(days - days_from_civil(year, 1, 1));
    result->tm_isdst = 0;
}

static long long file_time_seconds(const system_time *time) {
    file_time converted;
    if (!SystemTimeToFileTime(time, &converted)) {
        return 0;
    }
    long long ticks = ((long long)converted.high << 32) | converted.low;
    return (ticks - UNIX_EPOCH_IN_FILETIME) / 10000000;
}

static long long local_offset(int *daylight) {
    system_time universal;
    system_time local;
    GetSystemTime(&universal);
    GetLocalTime(&local);
    long long difference = file_time_seconds(&local) - file_time_seconds(&universal);
    if (daylight) {
        time_zone_information zone;
        *daylight = GetTimeZoneInformation(&zone) == TIME_ZONE_ID_DAYLIGHT;
    }
    return (difference >= 0 ? difference + 30 : difference - 30) / 60 * 60;
}

time_t time(time_t *result) {
    system_time now;
    GetSystemTime(&now);
    time_t seconds = (time_t)file_time_seconds(&now);
    if (result) {
        *result = seconds;
    }
    return seconds;
}

clock_t clock(void) {
    return (clock_t)GetTickCount();
}

struct tm *gmtime_r(const time_t *time, struct tm *result) {
    civil_from_seconds((long long)*time, result);
    return result;
}

struct tm *localtime_r(const time_t *time, struct tm *result) {
    int daylight;
    civil_from_seconds((long long)*time + local_offset(&daylight), result);
    result->tm_isdst = daylight;
    return result;
}

static struct tm shared_time;

struct tm *gmtime(const time_t *time) {
    return gmtime_r(time, &shared_time);
}

struct tm *localtime(const time_t *time) {
    return localtime_r(time, &shared_time);
}

time_t mktime(struct tm *time) {
    long long month = time->tm_mon;
    long long year = 1900LL + time->tm_year + month / 12;
    month %= 12;
    if (month < 0) {
        month += 12;
        year--;
    }
    long long seconds = days_from_civil(year, (int)month + 1, 1) * 86400 + (long long)(time->tm_mday - 1) * 86400 +
                        (long long)time->tm_hour * 3600 + (long long)time->tm_min * 60 + time->tm_sec;
    int daylight;
    seconds -= local_offset(&daylight);
    time_t result = (time_t)seconds;
    localtime_r(&result, time);
    return result;
}

typedef struct {
    char *buffer;
    size_t size;
    size_t length;
} output_text;

static int put_text(output_text *output, const char *text) {
    while (*text) {
        if (output->length + 1 >= output->size) {
            return 0;
        }
        output->buffer[output->length++] = *text++;
    }
    return 1;
}

static int put_number(output_text *output, long long value, int width, char padding) {
    char digits[24];
    int count = 0;
    int negative = value < 0;
    unsigned long long magnitude = negative ? 0 - (unsigned long long)value : (unsigned long long)value;
    do {
        digits[count++] = (char)('0' + magnitude % 10);
        magnitude /= 10;
    } while (magnitude);
    char text[32];
    int length = 0;
    if (negative) {
        text[length++] = '-';
    }
    for (int pad = count + negative; pad < width; pad++) {
        text[length++] = padding;
    }
    while (count) {
        text[length++] = digits[--count];
    }
    text[length] = 0;
    return put_text(output, text);
}

static int twelve_hour(int hour) {
    return hour % 12 == 0 ? 12 : hour % 12;
}

static int put_abbreviation(output_text *output, const char *name) {
    char text[4] = {name[0], name[1], name[2], 0};
    return put_text(output, text);
}

static int format_time(output_text *output, const char *format, const struct tm *time);

static int put_field(output_text *output, char conversion, const struct tm *time) {
    switch (conversion) {
    case 'a':
        return put_abbreviation(output, day_names[(time->tm_wday % 7 + 7) % 7]);
    case 'A':
        return put_text(output, day_names[(time->tm_wday % 7 + 7) % 7]);
    case 'b':
    case 'h':
        return put_abbreviation(output, month_names[(time->tm_mon % 12 + 12) % 12]);
    case 'B':
        return put_text(output, month_names[(time->tm_mon % 12 + 12) % 12]);
    case 'c':
        return format_time(output, "%a %b %e %H:%M:%S %Y", time);
    case 'C':
        return put_number(output, (1900LL + time->tm_year) / 100, 2, '0');
    case 'd':
        return put_number(output, time->tm_mday, 2, '0');
    case 'D':
    case 'x':
        return format_time(output, "%m/%d/%y", time);
    case 'e':
        return put_number(output, time->tm_mday, 2, ' ');
    case 'F':
        return format_time(output, "%Y-%m-%d", time);
    case 'H':
        return put_number(output, time->tm_hour, 2, '0');
    case 'I':
        return put_number(output, twelve_hour(time->tm_hour), 2, '0');
    case 'j':
        return put_number(output, time->tm_yday + 1, 3, '0');
    case 'm':
        return put_number(output, time->tm_mon + 1, 2, '0');
    case 'M':
        return put_number(output, time->tm_min, 2, '0');
    case 'n':
        return put_text(output, "\n");
    case 'p':
        return put_text(output, time->tm_hour < 12 ? "AM" : "PM");
    case 'r':
        return format_time(output, "%I:%M:%S %p", time);
    case 'R':
        return format_time(output, "%H:%M", time);
    case 'S':
        return put_number(output, time->tm_sec, 2, '0');
    case 't':
        return put_text(output, "\t");
    case 'T':
    case 'X':
        return format_time(output, "%H:%M:%S", time);
    case 'u':
        return put_number(output, time->tm_wday == 0 ? 7 : time->tm_wday, 1, '0');
    case 'U':
        return put_number(output, (time->tm_yday + 7 - time->tm_wday) / 7, 2, '0');
    case 'w':
        return put_number(output, time->tm_wday, 1, '0');
    case 'W':
        return put_number(output, (time->tm_yday + 7 - (time->tm_wday + 6) % 7) / 7, 2, '0');
    case 'y':
        return put_number(output, ((1900LL + time->tm_year) % 100 + 100) % 100, 2, '0');
    case 'Y':
        return put_number(output, 1900LL + time->tm_year, 1, '0');
    case 'z': {
        long long offset = local_offset(NULL) / 60;
        if (!put_text(output, offset < 0 ? "-" : "+")) {
            return 0;
        }
        offset = offset < 0 ? -offset : offset;
        return put_number(output, offset / 60 * 100 + offset % 60, 4, '0');
    }
    case 'Z':
        return 1;
    case '%':
        return put_text(output, "%");
    default: {
        char text[3] = {'%', conversion, 0};
        return put_text(output, text);
    }
    }
}

static int format_time(output_text *output, const char *format, const struct tm *time) {
    for (; *format; format++) {
        if (*format != '%') {
            char text[2] = {*format, 0};
            if (!put_text(output, text)) {
                return 0;
            }
            continue;
        }
        format++;
        if (*format == 'E' || *format == 'O') {
            format++;
        }
        if (!*format) {
            return put_text(output, "%");
        }
        if (!put_field(output, *format, time)) {
            return 0;
        }
    }
    return 1;
}

size_t strftime(char *buffer, size_t size, const char *format, const struct tm *time) {
    output_text output = {buffer, size, 0};
    if (size == 0 || !format_time(&output, format, time)) {
        if (size) {
            buffer[0] = 0;
        }
        return 0;
    }
    buffer[output.length] = 0;
    return output.length;
}

char *asctime(const struct tm *time) {
    static char text[32];
    strftime(text, sizeof text, "%a %b %e %H:%M:%S %Y\n", time);
    return text;
}

char *ctime(const time_t *time) {
    return asctime(localtime(time));
}
