/* To use these functions:

   1. Start with a KalmanFilter created by alloc_filter_velocity2d.
   2. At fixed intervals, call update_velocity2d with the lat/long.
   3. At any time, to get an estimate for the current position,
   bearing, or speed, use the functions:
     get_lat_long
     get_bearing
     get_mph
 */

#ifndef __ALGO_GPS_H__
#define __ALGO_GPS_H__

#include <stdio.h>

#include "algo_kalman.h"

/* Create a GPS filter that only tracks two dimensions of position and
   velocity.
   The inherent assumption is that changes in velocity are randomly
   distributed around 0.
   Noise is a parameter you can use to alter the expected noise.
   1.0 is the original, and the higher it is, the more a path will be
   "smoothed".
   Free with free_filter after using. */
KalmanFilter alloc_filter_velocity2d(double noise);

/* Set the seconds per timestep in the velocity2d model. */
void set_seconds_per_timestep(KalmanFilter f, double seconds_per_timestep);

/* Update the velocity2d model with new gps data. */
void update_velocity2d(KalmanFilter f, double lat, double lon, double seconds_since_last_update);

/* Read a lat,long pair from a file.
   Format is lat,long<ignored>
   Return whether there was a lat,long to be read */
int read_lat_long(FILE* file, double* lat, double* lon);

/* Extract a lat long from a velocity2d Kalman filter. */
void get_lat_long(KalmanFilter f, double* lat, double* lon);

/* Extract velocity with lat-long-per-second units from a velocity2d
   Kalman filter. */
void get_velocity(KalmanFilter f, double* delta_lat, double* delta_lon);

/* Extract a bearing from a velocity2d Kalman filter.
   0 = north, 90 = east, 180 = south, 270 = west */
double get_bearing(KalmanFilter f);

/* Convert a lat, long, delta lat, and delta long into mph.*/
double calculate_mph(double lat, double lon, double delta_lat, double delta_lon);

/* Extract speed in miles per hour from a velocity2d Kalman filter. */
double get_mph(KalmanFilter f);

#endif
