#include "c_fetch_data.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static CURL* curl_handle;
static bool initalized = false;

static bool idempotent_init() {
  if (initalized) {
    if (!curl_handle) {
      return false;
    }
    curl_easy_reset(curl_handle);
    return true;
  }
  initalized = true;
  curl_handle = curl_easy_init();
  if (!curl_handle) {
    return false;
  } else {
    return true;
  }
}

struct response {
  char* memory;
  size_t size;
};

static size_t mem_cb(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t realsize = size * nmemb;
  struct response* mem = (struct response*)userp;

  char* ptr = realloc(mem->memory, mem->size + realsize + 1);
  if (!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

static const char fetch_loc_url_fmt[] =
    "https://geocoding.geo.census.gov/geocoder/locations/"
    "onelineaddress?address=%s&benchmark=Public_AR_Current&format=json";

FETCH_LOCATION_RESULT
fetch_location(/* in */ char* addressOneLine,
               /* in */ size_t addressOneLine_len,
               /* out */ char** lat, /*out */ size_t* lat_len,
               /* out */ char** lon, /* out */ size_t* lon_len) {
  FETCH_LOCATION_RESULT ret = FETCH_LOC_SUCCESS;
  if (!idempotent_init()) {
    return FETCH_LOC_LIB_PROBLEM;
  }
  char* urlEncodedAddress =
      curl_easy_escape(curl_handle, addressOneLine, (int)addressOneLine_len);
  if (!urlEncodedAddress) {
    ret = FETCH_LOC_MEM_PROBLEM;
    goto cleanup_addr;
  }

  size_t url_buf_len = ((size_t)strlen(urlEncodedAddress) * sizeof(char)) +
                       sizeof(fetch_loc_url_fmt);
  char* url_buf = malloc(url_buf_len);
  if (!url_buf) {
    ret = FETCH_LOC_MEM_PROBLEM;
    goto cleanup_url_buf;
  }

  {
    int res =
        snprintf(url_buf, url_buf_len, fetch_loc_url_fmt, urlEncodedAddress);
    if (!res) {
      ret = FETCH_LOC_MEM_PROBLEM;
      printf("urlprintErr %d\n", res);
      goto cleanup_url_buf;
    }
  }
  curl_easy_setopt(curl_handle, CURLOPT_URL, url_buf);
  curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);

  struct response chunk = {.memory = (char*)malloc(0), .size = 0};
  if (!chunk.memory) {
    ret = FETCH_LOC_MEM_PROBLEM;
    goto cleanup_chunk;
  }
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, mem_cb);
  // Have curl pass chunk through to mem_cb
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

  {
    CURLcode res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) {
      printf("curl failure %d", res);
      ret = FETCH_LOC_LIB_PROBLEM;
      goto cleanup_curl;
    }
  }

  long responseCode;
  curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &responseCode);
  if (responseCode != 200) {
    ret = FETCH_LOC_NETWORK_ISSUE;
    goto cleanup_curl;
  }

  *lat_len = (size_t)chunk.size;
  *lat = malloc(*lat_len);
  memcpy(*lat, chunk.memory, chunk.size);

cleanup_curl:
  curl_easy_cleanup(curl_handle);
cleanup_chunk:
  free(chunk.memory);
cleanup_url_buf:
  free(url_buf);
cleanup_addr:
  curl_free(urlEncodedAddress);
  return ret;
}
