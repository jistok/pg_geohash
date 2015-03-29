# pg_geohash
Geohashing functionality for HAWQ, Greenplum DB, PostgreSQL

* Motivated by the need to compute aggregates based on geographic location
* Geohash background: http://en.wikipedia.org/wiki/Geohash
* C based user defined function
* Based on this C language geohashing library: https://github.com/lyokato/libgeohash.git
* Install (shown here on Pivotal HD single node VM, CentOS release 6.5):
NOTE: the user running the `make install` requires write permission into the installation
directory of the target database.

```
[gpadmin@pivotalvm pg_geohash]$ make
... output deleted
[gpadmin@pivotalvm pg_geohash]$ make install
... output deleted
[gpadmin@pivotalvm pg_geohash]$ psql demo -f ./pg_geohash-1.0.sql
Timing is on.
CREATE FUNCTION
Time: 12.904 ms
```

* Usage example: compute the geohash based on latitude and longitude values, taking just
the first 5 characters of the hash to divide the region up into approximately 70 divisions.

```
demo=# SELECT SUBSTRING(GEOHASH_LAT_LON(latitude, longitude) FROM 1 for 5) geohash, COUNT(*)
demo-# FROM crimes
demo-# GROUP BY 1
demo-# ORDER BY 2 DESC
demo-# LIMIT 20;
 geohash | count
---------+-------
 dp3w7   | 72404
 dp3tt   | 70713
 dp3tw   | 63642
 dp3wm   | 62332
 dp3wk   | 56467
 dp3wh   | 46930
 dp3w5   | 45164
 dp3ts   | 43671
 dp3ty   | 42278
 dp3wj   | 37619
 dp3wt   | 37395
 dp3tm   | 35282
 dp3tv   | 34631
 dp3we   | 33593
 dp3tx   | 32438
 dp3w6   | 31644
 dp3tq   | 30284
 dp3ws   | 30049
 dp3tj   | 26494
 dp3tu   | 26061
(20 rows)

Time: 7556.363 ms
```
* Here is a count of the rows in this table:

```
demo=# select count(*) from crimes;
  count
---------
 1104437
(1 row)

Time: 1516.002 ms
```


