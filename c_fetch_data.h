#ifndef C_FETCH_DATA_H_
#define C_FETCH_DATA_H_

#include <curl/curl.h>

#define FETCH_PROBLEM_MEM 0
#define FETCH_PROBLEM_NETWORK 1
#define FETCH_PROBLEM_FETCH_LIB 2

typedef enum FETCH_LOCATION_RESULT {
  FETCH_LOC_MEM_PROBLEM = FETCH_PROBLEM_MEM,
  FETCH_LOC_NETWORK_ISSUE = FETCH_PROBLEM_NETWORK,
  FETCH_LOC_LIB_PROBLEM = FETCH_PROBLEM_FETCH_LIB,
  FETCH_LOC_NO_ADDRESS_FOUND,
  FETCH_LOC_SUCCESS
} FETCH_LOCATION_RESULT;

char* fetch_location_error_msg(FETCH_LOCATION_RESULT result,
                               /* out */ size_t* msg_len);

int fetch_init();

FETCH_LOCATION_RESULT
fetch_location(/* in */ char* addressOneLine,
               /* in */ size_t addressOneLine_len,
               /* out */ char** lat, /* out */ size_t* lat_len,
               /* out */ char** lon, /* out */ size_t* lon_len);

#endif // C_FETCH_DATA_H_
