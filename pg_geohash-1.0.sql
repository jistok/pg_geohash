CREATE FUNCTION geohash_lat_lon (float8, float8) RETURNS text
  AS 'pg_geohash', 'geohash_lat_lon'
  LANGUAGE C;

