#include "c_application.h"
#include "c_fetch_data.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  if (argc < 2) {
    printf("Must provide command line arg\n");
    exit(1);
  }
  char* lat;
  size_t lat_len;
  FETCH_LOCATION_RESULT res =
      fetch_location(argv[1], strlen(argv[1]), &lat, &lat_len, 0, 0);
  if (res != FETCH_LOC_SUCCESS) {
    printf("err, %d", res);
    exit(1);
  }
  fwrite(lat, sizeof(char), lat_len, stdout);
  free(lat);
  curl_global_cleanup();
}
