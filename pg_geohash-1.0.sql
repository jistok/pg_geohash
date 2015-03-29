CREATE OR REPLACE FUNCTION lat_lon_to_geohash (float8, float8) RETURNS text
  AS 'pg_geohash', 'lat_lon_to_geohash'
  LANGUAGE C;

CREATE OR REPLACE FUNCTION geohash_to_lat_lon (text) RETURNS text
  AS 'pg_geohash', 'geohash_to_lat_lon'
  LANGUAGE C;

