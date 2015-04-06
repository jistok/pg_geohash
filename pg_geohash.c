/*
 * The MIT License
 *
 * Copyright (c) 2015 Michael Goddard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "postgres.h"
#include "fmgr.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "geohash.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

Datum lat_lon_to_geohash (PG_FUNCTION_ARGS);
Datum geohash_to_lat_lon (PG_FUNCTION_ARGS);

/*
 * Turn this into a PostreSQL callable PL/C function
 *
 * Ref. http://www.postgresql.org/docs/9.2/static/xfunc-c.html
 *
 * Function: TEXT geohash_lat_lon (FLOAT8 lat, FLOAT8 lon)
 *
 * CREATE FUNCTION geohash (FLOAT8, FLOAT8) RETURNS TEXT
 *   AS 'pg_geohash', 'geohash_lat_lon'
 *   LANGUAGE C;
 *
 *   psql      C           header
 * -----------------------------------------------
 *  float8   float8*     postgres.h
 *  varchar  VarChar*    postgres.h
 *  text     text*       postgres.h
 *
 */

PG_FUNCTION_INFO_V1 (lat_lon_to_geohash);
Datum
lat_lon_to_geohash (PG_FUNCTION_ARGS)
{
    /*
     * char* GEOHASH_encode(double latitude, double longitude, unsigned int hash_length);
     * double atof(const char *nptr);
     *
     * Earth radius = 6371 km
     * Distance per degree: ds = PI/180.0 * 6371.0e+03 m = 111,195 m/degree,
     * so if we have lat/lon values to 1.0e-15, we get down to ridiculous precision
     * 1 m precision is about 5 digits to the right of the decimal point
     *
     * Example input lat/lon: 41.884559527232234 -87.62610231958186
     *
     * Need to determine the size of the area per character chopped off the right hand
     * side of the geohash.
     */
    char *hash;
    double lat, lon;
    text *rv;
    /* Max. length of geohash is 12 */
    int hash_len = 12;
    size_t rv_len;

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) {
      PG_RETURN_NULL();
    }

    lat = PG_GETARG_FLOAT8(0);
    lon = PG_GETARG_FLOAT8(1);
    hash = GEOHASH_encode(lat, lon, hash_len);

    rv_len = strlen(hash) + 1;
    rv = (text *) palloc(VARHDRSZ + rv_len);
    SET_VARSIZE(rv, VARHDRSZ + rv_len);
    /*
     * VARDATA is a pointer to the data region of the struct.
     */
    memcpy(VARDATA(rv), hash, rv_len);
    free(hash);
    PG_RETURN_TEXT_P(rv);
}

PG_FUNCTION_INFO_V1 (geohash_to_lat_lon);
Datum
geohash_to_lat_lon (PG_FUNCTION_ARGS)
{
    text *hash; /* Passed in by ref. as arg0 */
    GEOHASH_area *area; /* Freed here */
    double lat, lon;
    char buf[255];
    text *rv; /* Return value */
    size_t rv_len;
    char *hash_as_char;
    size_t hash_len;

    if (PG_ARGISNULL(0)) {
      PG_RETURN_NULL();
    }

    hash = PG_GETARG_TEXT_P(0);
    if (hash == NULL) {
      elog(ERROR, "Geohash is NULL");
      PG_RETURN_NULL();
    }

    hash_len = VARSIZE(hash) - VARHDRSZ;
    hash_as_char = (char *) palloc(hash_len + 1);
    memcpy(hash_as_char, VARDATA(hash), hash_len);
    if (hash_as_char == NULL) {
      elog(ERROR, "VARDATA(geohash) returns NULL");
      PG_RETURN_NULL();
    }
    *(hash_as_char + hash_len) = '\0';
 
    area = GEOHASH_decode(hash_as_char);
    pfree(hash_as_char);
    if (area == NULL) {
      elog(ERROR, "GEOHASH_decode returns NULL");
      PG_RETURN_NULL();
    }

    lat = area->latitude.min + (area->latitude.max - area->latitude.min)/2.0;
    lon = area->longitude.min + (area->longitude.max - area->longitude.min)/2.0;
    GEOHASH_free_area(area);
    snprintf(buf, 255, "(%f, %f)", lat, lon);

    rv_len = strlen(buf) + 1;
    rv = (text *) palloc(VARHDRSZ + rv_len);
    if (rv == NULL) {
      elog(ERROR, "Failed to palloc() return value");
      PG_RETURN_NULL();
    }
    SET_VARSIZE(rv, VARHDRSZ + rv_len);
    memcpy(VARDATA(rv), buf, rv_len);
    PG_RETURN_TEXT_P(rv);
}

