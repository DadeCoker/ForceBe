/* Applying Kalman filters to GPS data. */
///<  liuwei 654188547@qq.com
#include "algo_gps.h"

#include <assert.h>
#include <math.h>

static const double PI                    = 3.14159265;
static const double EARTH_RADIUS_IN_MILES = 3963.1676;

KalmanFilter alloc_filter_velocity2d(double noise)
{
    /* The state model has four dimensions:
       x, y, x', y'
       Each time step we can only observe position, not velocity, so the
       observation vector has only two dimensions.
    */
    KalmanFilter f = alloc_filter(4, 2);

    /* Assuming the axes are rectilinear does not work well at the
       poles, but it has the bonus that we don't need to convert between
       lat/long and more rectangular coordinates. The slight inaccuracy
       of our physics model is not too important.
     */
    //  double v2p = 0.001;
    set_identity_matrix(f.state_transition);
    set_seconds_per_timestep(f, 1.0);

    /* We observe (x, y) in each time step */
    set_matrix(f.observation_model, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);

    /* Noise in the world. */
    double pos = 0.000001;
    set_matrix(f.process_noise_covariance, pos, 0.0, 0.0, 0.0, 0.0, pos, 0.0, 0.0, 0.0, 0.0, 1.0,
               0.0, 0.0, 0.0, 0.0, 1.0);

    /* Noise in our observation */
    set_matrix(f.observation_noise_covariance, pos * noise, 0.0, 0.0, pos * noise);

    /* The start position is totally unknown, so give a high variance */
    set_matrix(f.state_estimate, 0.0, 0.0, 0.0, 0.0);
    set_identity_matrix(f.estimate_covariance);
    double trillion = 1000.0 * 1000.0 * 1000.0 * 1000.0;
    scale_matrix(f.estimate_covariance, trillion);

    return f;
}

/* The position units are in thousandths of latitude and longitude.
   The velocity units are in thousandths of position units per second.

   So if there is one second per timestep, a velocity of 1 will change
   the lat or long by 1 after a million timesteps.

   Thus a typical position is hundreds of thousands of units.
   A typical velocity is maybe ten.
*/
void set_seconds_per_timestep(KalmanFilter f, double seconds_per_timestep)
{
    /* unit_scaler accounts for the relation between position and
       velocity units */
    double unit_scaler            = 0.001;
    f.state_transition.data[0][2] = unit_scaler * seconds_per_timestep;
    f.state_transition.data[1][3] = unit_scaler * seconds_per_timestep;
}

void update_velocity2d(KalmanFilter f, double lat, double lon, double seconds_since_last_timestep)
{
    set_seconds_per_timestep(f, seconds_since_last_timestep);
    set_matrix(f.observation, lat * 1000.0, lon * 1000.0);
    update(f);
}

int read_lat_long(FILE* file, double* lat, double* lon)
{
    while (1)
    {
        /* If we find a lat long pair, we're done */
        if (2 == fscanf(file, "%lf,%lf", lat, lon))
        {
            return 1;
        }

        /* Advance to the next line */
        int ch;
        while ((ch = getc(file)) != '\n')
        {
            if (EOF == ch)
            {
                return 0;
            }
        }
    }
}

void get_lat_long(KalmanFilter f, double* lat, double* lon)
{
    *lat = f.state_estimate.data[0][0] / 1000.0;
    *lon = f.state_estimate.data[1][0] / 1000.0;
}

void get_velocity(KalmanFilter f, double* delta_lat, double* delta_lon)
{
    *delta_lat = f.state_estimate.data[2][0] / (1000.0 * 1000.0);
    *delta_lon = f.state_estimate.data[3][0] / (1000.0 * 1000.0);
}

/* See
   http://www.movable-type.co.uk/scripts/latlong.html
   for formulas */
double get_bearing(KalmanFilter f)
{
    double lat, lon, delta_lat, delta_lon, x, y;
    get_lat_long(f, &lat, &lon);
    get_velocity(f, &delta_lat, &delta_lon);

    /* Convert to radians */
    double to_radians = PI / 180.0;
    lat *= to_radians;
    lon *= to_radians;
    delta_lat *= to_radians;
    delta_lon *= to_radians;

    /* Do math */
    double lat1    = lat - delta_lat;
    y              = sin(delta_lon) * cos(lat);
    x              = cos(lat1) * sin(lat) - sin(lat1) * cos(lat) * cos(delta_lon);
    double bearing = atan2(y, x);

    /* Convert to degrees */
    bearing = bearing / to_radians;
    while (bearing >= 360.0)
    {
        bearing -= 360.0;
    }
    while (bearing < 0.0)
    {
        bearing += 360.0;
    }

    return bearing;
}

double calculate_mph(double lat, double lon, double delta_lat, double delta_lon)
{
    /* First, let's calculate a unit-independent measurement - the radii
       of the earth traveled in each second. (Presumably this will be
       a very small number.) */

    /* Convert to radians */
    double to_radians = PI / 180.0;
    lat *= to_radians;
    lon *= to_radians;
    delta_lat *= to_radians;
    delta_lon *= to_radians;

    /* Haversine formula */
    double lat1          = lat - delta_lat;
    double sin_half_dlat = sin(delta_lat / 2.0);
    double sin_half_dlon = sin(delta_lon / 2.0);
    double a = sin_half_dlat * sin_half_dlat + cos(lat1) * cos(lat) * sin_half_dlon * sin_half_dlon;
    double radians_per_second = 2 * atan2(1000.0 * sqrt(a), 1000.0 * sqrt(1.0 - a));

    /* Convert units */
    double miles_per_second = radians_per_second * EARTH_RADIUS_IN_MILES;
    double miles_per_hour   = miles_per_second * 60.0 * 60.0;
    return miles_per_hour;
}

double get_mph(KalmanFilter f)
{
    double lat, lon, delta_lat, delta_lon;
    get_lat_long(f, &lat, &lon);
    get_velocity(f, &delta_lat, &delta_lon);
    return calculate_mph(lat, lon, delta_lat, delta_lon);
}
