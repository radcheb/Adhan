#include "gtest/gtest.h"

extern "C"
{
    #include "../src/include/prayer.h"
    #include "../src/include/prayer_times.h"
}

int getDaysSinceSolstice(int year, int month, int day, double latitude) {
    // For Northern Hemisphere start from December 21
    // (DYY=0 for December 21, and counting forward, DYY=11 for January 1 and so on).
    // For Southern Hemisphere start from June 21
    // (DYY=0 for June 21, and counting forward)
    time_t date = resolve_time_2(year, month, day);
    int dayOfYear = localtime(&date)->tm_yday + 1;
    return daysSinceSolstice(dayOfYear, year, latitude);
}

TEST(PrayerTimesTest, testDaysSinceSolstice) {

    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2016, /* month */ 1, /* day */ 1, /* latitude */ 1), 11);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2015, /* month */ 12, /* day */ 31, /* latitude */ 1), 10);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2016, /* month */ 12, /* day */ 31, /* latitude */ 1), 10);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2016, /* month */ 12, /* day */ 21, /* latitude */ 1), 0);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2016, /* month */ 12, /* day */ 22, /* latitude */ 1), 1);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2016, /* month */ 3, /* day */ 1, /* latitude */ 1), 71);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2015, /* month */ 3, /* day */ 1, /* latitude */ 1), 70);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2016, /* month */ 12, /* day */ 20, /* latitude */ 1), 365);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2015, /* month */ 12, /* day */ 20, /* latitude */ 1), 364);

    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2015, /* month */ 6, /* day */ 21, /* latitude */ -1), 0);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2016, /* month */ 6, /* day */ 21, /* latitude */ -1), 0);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2015, /* month */ 6, /* day */ 20, /* latitude */ -1), 364);
    ASSERT_EQ(getDaysSinceSolstice(/* year */ 2016, /* month */ 6, /* day */ 20, /* latitude */ -1), 365);
}

void getLocalStrTime(time_t timestamp ,char buffer[]){
    tm *local_date = localtime(&timestamp);
    strftime(buffer, 9, "%I:%M %p", local_date);
}

TEST(PrayerTimesTest, testPrayerTimes) {
    date_components_t date = new_date_components(12, 7, 2015);
    calculation_method method = NORTH_AMERICA;
    calculation_parameters_t* params = getParameters(&method);
    params->madhab = HANAFI;

    coordinates_t coordinates = {35.7750, -78.6336};
    prayer_times_t* prayerTimes = new_prayer_times(&coordinates, &date, params);

    char destString[9];

    time_t fajr_timestamp = mktime(prayerTimes->fajr);
    time_t sunrise_timestamp = mktime(prayerTimes->sunrise);
    time_t dhuhr_timestamp = mktime(prayerTimes->dhuhr);
    time_t asr_timestamp = mktime(prayerTimes->asr);
    time_t maghreb_timestamp = mktime(prayerTimes->maghrib);
    time_t isha_timestamp = mktime(prayerTimes->isha);

    putenv((char *) "TZ=America/New_York");

    getLocalStrTime(fajr_timestamp, destString);
    ASSERT_STREQ(destString, "04:42 AM");

    getLocalStrTime(sunrise_timestamp, destString);
    ASSERT_STREQ(destString, "06:08 AM");

    getLocalStrTime(dhuhr_timestamp, destString);
    ASSERT_STREQ(destString, "01:21 PM");

    getLocalStrTime(asr_timestamp, destString);
    ASSERT_STREQ(destString, "06:22 PM");

    getLocalStrTime(maghreb_timestamp, destString);
    ASSERT_STREQ(destString, "08:32 PM");

    getLocalStrTime(isha_timestamp, destString);
    ASSERT_STREQ(destString, "09:57 PM");

    putenv((char *) "TZ=UTC");
}

TEST(PrayerTimesTest, testOffsets) {
    date_components_t date = new_date_components(1, 12, 2015);
    calculation_method method = MUSLIM_WORLD_LEAGUE;
    calculation_parameters_t* params = getParameters(&method);

    coordinates_t coordinates = {35.7750, -78.6336};
    prayer_times_t* prayerTimes = new_prayer_times(&coordinates, &date, params);

    char destString[9];

    time_t fajr_timestamp = mktime(prayerTimes->fajr);
    time_t sunrise_timestamp = mktime(prayerTimes->sunrise);
    time_t dhuhr_timestamp = mktime(prayerTimes->dhuhr);
    time_t asr_timestamp = mktime(prayerTimes->asr);
    time_t maghreb_timestamp = mktime(prayerTimes->maghrib);
    time_t isha_timestamp = mktime(prayerTimes->isha);

    putenv((char *) "TZ=America/New_York");

    getLocalStrTime(fajr_timestamp, destString);
    ASSERT_STREQ(destString, "05:35 AM");

    getLocalStrTime(sunrise_timestamp, destString);
    ASSERT_STREQ(destString, "07:06 AM");

    getLocalStrTime(dhuhr_timestamp, destString);
    ASSERT_STREQ(destString, "12:05 PM");

    getLocalStrTime(asr_timestamp, destString);
    ASSERT_STREQ(destString, "02:42 PM");

    getLocalStrTime(maghreb_timestamp, destString);
    ASSERT_STREQ(destString, "05:01 PM");

    getLocalStrTime(isha_timestamp, destString);
    ASSERT_STREQ(destString, "06:26 PM");

    putenv((char *) "TZ=UTC");

}

TEST(PrayerTimesTest, testMoonsightingMethod) {
    date_components_t date = new_date_components(31, 1, 2016);
    calculation_method method = MOON_SIGHTING_COMMITTEE;
    calculation_parameters_t* params = getParameters(&method);

    coordinates_t coordinates = {35.7750, -78.6336};
    prayer_times_t* prayerTimes = new_prayer_times(&coordinates, &date, params);

    char destString[9];

    time_t fajr_timestamp = mktime(prayerTimes->fajr);
    time_t sunrise_timestamp = mktime(prayerTimes->sunrise);
    time_t dhuhr_timestamp = mktime(prayerTimes->dhuhr);
    time_t asr_timestamp = mktime(prayerTimes->asr);
    time_t maghreb_timestamp = mktime(prayerTimes->maghrib);
    time_t isha_timestamp = mktime(prayerTimes->isha);

    putenv((char *) "TZ=America/New_York");

    getLocalStrTime(fajr_timestamp, destString);
    ASSERT_STREQ(destString, "05:48 AM");

    getLocalStrTime(sunrise_timestamp, destString);
    ASSERT_STREQ(destString, "07:16 AM");

    getLocalStrTime(dhuhr_timestamp, destString);
    ASSERT_STREQ(destString, "12:33 PM");

    getLocalStrTime(asr_timestamp, destString);
    ASSERT_STREQ(destString, "03:20 PM");

    getLocalStrTime(maghreb_timestamp, destString);
    ASSERT_STREQ(destString, "05:43 PM");

    getLocalStrTime(isha_timestamp, destString);
    ASSERT_STREQ(destString, "07:05 PM");

    putenv((char *) "TZ=UTC");
}

TEST(PrayerTimesTest, testMoonsightingMethodHighLat) {
    date_components_t date = new_date_components(1, 1, 2016);
    calculation_method method = MOON_SIGHTING_COMMITTEE;
    calculation_parameters_t* params = getParameters(&method);
    params->madhab = HANAFI;

    coordinates_t coordinates = {59.9094, 10.7349};
    prayer_times_t* prayerTimes = new_prayer_times(&coordinates, &date, params);

    char destString[9];

    time_t fajr_timestamp = mktime(prayerTimes->fajr);
    time_t sunrise_timestamp = mktime(prayerTimes->sunrise);
    time_t dhuhr_timestamp = mktime(prayerTimes->dhuhr);
    time_t asr_timestamp = mktime(prayerTimes->asr);
    time_t maghreb_timestamp = mktime(prayerTimes->maghrib);
    time_t isha_timestamp = mktime(prayerTimes->isha);

    putenv((char *) "TZ=Europe/Oslo");

    getLocalStrTime(fajr_timestamp, destString);
    ASSERT_STREQ(destString, "07:34 AM");

    getLocalStrTime(sunrise_timestamp, destString);
    ASSERT_STREQ(destString, "09:19 AM");

    getLocalStrTime(dhuhr_timestamp, destString);
    ASSERT_STREQ(destString, "12:25 PM");

    getLocalStrTime(asr_timestamp, destString);
    ASSERT_STREQ(destString, "01:36 PM");

    getLocalStrTime(maghreb_timestamp, destString);
    ASSERT_STREQ(destString, "03:25 PM");

    getLocalStrTime(isha_timestamp, destString);
    ASSERT_STREQ(destString, "05:02 PM");

}

TEST(PrayerTimesTest, testTimeForPrayer) {
    date_components_t date = new_date_components(1, 7, 2016);
    calculation_method method = MUSLIM_WORLD_LEAGUE;
    calculation_parameters_t* params = getParameters(&method);
    params->madhab = HANAFI;
    params->highLatitudeRule = TWILIGHT_ANGLE;

    coordinates_t coordinates = {59.9094, 10.7349};
    prayer_times_t* prayerTimes = new_prayer_times(&coordinates, &date, params);

    putenv((char *) "TZ=Europe/Oslo");

    ASSERT_EQ(mktime(prayerTimes->fajr), timeForPrayer(prayerTimes, FAJR));
    ASSERT_EQ(mktime(prayerTimes->sunrise), timeForPrayer(prayerTimes, SUNRISE));
    ASSERT_EQ(mktime(prayerTimes->dhuhr), timeForPrayer(prayerTimes, DHUHR));
    ASSERT_EQ(mktime(prayerTimes->asr), timeForPrayer(prayerTimes, ASR));
    ASSERT_EQ(mktime(prayerTimes->maghrib), timeForPrayer(prayerTimes, MAGHRIB));
    ASSERT_EQ(mktime(prayerTimes->isha), timeForPrayer(prayerTimes, ISHA));
}

TEST(PrayerTimesTest, testCurrentPrayer) {
    date_components_t date = new_date_components(1, 9, 2015);
    calculation_method method = KARACHI;
    calculation_parameters_t* params = getParameters(&method);
    params->madhab = HANAFI;
    params->highLatitudeRule = TWILIGHT_ANGLE;

    coordinates_t coordinates = {33.720817, 73.090032};
    prayer_times_t* prayerTimes = new_prayer_times(&coordinates, &date, params);

    ASSERT_EQ(currentPrayer2(prayerTimes, add_seconds(mktime(prayerTimes->fajr) ,-1)) ,NONE);
    ASSERT_EQ(currentPrayer2(prayerTimes, mktime(prayerTimes->fajr)) ,FAJR);
    ASSERT_EQ(currentPrayer2(prayerTimes, add_seconds(mktime(prayerTimes->fajr) ,1)) ,FAJR);
    ASSERT_EQ(currentPrayer2(prayerTimes, add_seconds(mktime(prayerTimes->sunrise) ,1)) ,SUNRISE);
    ASSERT_EQ(currentPrayer2(prayerTimes, add_seconds(mktime(prayerTimes->dhuhr) ,1)) ,DHUHR);
    ASSERT_EQ(currentPrayer2(prayerTimes, add_seconds(mktime(prayerTimes->asr) ,1)) ,ASR);
    ASSERT_EQ(currentPrayer2(prayerTimes, add_seconds(mktime(prayerTimes->maghrib) ,1)) ,MAGHRIB);
    ASSERT_EQ(currentPrayer2(prayerTimes, add_seconds(mktime(prayerTimes->isha) ,1)) ,ISHA);
}

TEST(PrayerTimesTest, testNextPrayer) {
    date_components_t date = new_date_components(1, 9, 2015);
    calculation_method method = KARACHI;
    calculation_parameters_t* params = getParameters(&method);
    params->madhab = HANAFI;
    params->highLatitudeRule = TWILIGHT_ANGLE;

    coordinates_t coordinates = {33.720817, 73.090032};
    prayer_times_t* prayerTimes = new_prayer_times(&coordinates, &date, params);

    ASSERT_EQ(next_prayer2(prayerTimes, add_seconds(mktime(prayerTimes->fajr) ,-1)) ,FAJR);
    ASSERT_EQ(next_prayer2(prayerTimes, mktime(prayerTimes->fajr)) ,SUNRISE);
    ASSERT_EQ(next_prayer2(prayerTimes, add_seconds(mktime(prayerTimes->fajr) ,1)) ,SUNRISE);
    ASSERT_EQ(next_prayer2(prayerTimes, add_seconds(mktime(prayerTimes->sunrise) ,1)) ,DHUHR);
    ASSERT_EQ(next_prayer2(prayerTimes, add_seconds(mktime(prayerTimes->dhuhr) ,1)) ,ASR);
    ASSERT_EQ(next_prayer2(prayerTimes, add_seconds(mktime(prayerTimes->asr) ,1)) ,MAGHRIB);
    ASSERT_EQ(next_prayer2(prayerTimes, add_seconds(mktime(prayerTimes->maghrib) ,1)) ,ISHA);
    ASSERT_EQ(next_prayer2(prayerTimes, add_seconds(mktime(prayerTimes->isha) ,1)) ,NONE);
}