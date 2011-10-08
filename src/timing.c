/**
 * @file timing.c
 * @author Joe Wingbermuehle
 * @date 2004-2006
 *
 * @brief Timing functions.
 *
 */

#include "jwm.h"
#include "timing.h"

static const unsigned long MAX_TIME_SECONDS = 60;

/** Get the current time in milliseconds since midnight 1970-01-01 UTC. */
void GetCurrentTime(TimeType *t) {
   struct timeval val;
   gettimeofday(&val, NULL);
   t->seconds = val.tv_sec;
   t->ms = val.tv_usec / 1000;
}

/** Get the absolute difference between two times in milliseconds.
 * If the difference is larger than a MAX_TIME_SECONDS, then
 * MAX_TIME_SECONDS will be returned.
 * Note that the times must be normalized.
 */
unsigned long GetTimeDifference(const TimeType *t1, const TimeType *t2) {
   unsigned long deltaSeconds;
   int deltaMs;

   if(t1->seconds > t2->seconds) {
      deltaSeconds = t1->seconds - t2->seconds;
      deltaMs = t1->ms - t2->ms;
   } else if(t1->seconds < t2->seconds) {
      deltaSeconds = t2->seconds - t1->seconds;
      deltaMs = t2->ms - t1->ms;
   } else if(t1->ms > t2->ms) {
      deltaSeconds = 0;
      deltaMs = t1->ms - t2->ms;
   } else {
      deltaSeconds = 0;
      deltaMs = t2->ms - t1->ms;
   }

   if(deltaSeconds > MAX_TIME_SECONDS) {
      return MAX_TIME_SECONDS * 1000;
   } else {
      return deltaSeconds * 1000 + deltaMs;
   }

}

/** Get the current time. */
const char *GetTimeString(const char *format, const char *zone) {

   static char str[80];
   time_t t;

   Assert(format);

   time(&t);

   if(zone) {
      char saveTZ[256] = "";
#ifndef HAVE_SETENV
      static char newTZ[256];
#endif
      char *oldTZ = getenv("TZ");
      if(oldTZ) {
         strncpy(saveTZ, oldTZ, sizeof(saveTZ));
      }
#ifdef HAVE_SETENV
      setenv("TZ", zone, 1);
#else
      snprintf(newTZ, sizeof(newTZ), "TZ=%s", zone);
      putenv(newTZ);
#endif
      tzset();
      strftime(str, sizeof(str), format, localtime(&t));
      if(oldTZ) {
#ifdef HAVE_SETENV
         setenv("TZ", saveTZ, 1);
#else
         snprintf(newTZ, sizeof(newTZ), "TZ=%s", zone);
         putenv(newTZ);
#endif
      } else {
#ifdef HAVE_UNSETENV
         unsetenv("TZ");
#endif
      }
   } else {
      strftime(str, sizeof(str), format, localtime(&t));
   }

   return str;

}


