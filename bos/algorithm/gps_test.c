#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "gps.h"


int main(int argc, char *argv[]) {
  FILE* file = fopen("testdata/out_main.csv", "r");
  assert(file);
  KalmanFilter f = alloc_filter_velocity2d(1.0);

  double lat, lon;

  while(read_lat_long(file, &lat, &lon))
  {

	  update_velocity2d(f, lat, lon, 2.0);

	  double lat1 = 0,lon1 = 0;
	  get_lat_long(f, &lat1, &lon1);

	  printf("%f %f %f %f %f %f\n", lat, lat1, (lat-lat1), lon, lon1, (lon-lon1));

  }

  double lat1 = 0,lon1 = 0;
  get_lat_long(f, &lat1, &lon1);

  printf("%f %f %f %f %f %f\n", lat, lat1, (lat-lat1), lon, lon1, (lon-lon1));

  fclose(file);
}

