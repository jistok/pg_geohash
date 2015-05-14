# pg_geohash
Geohashing functionality for HAWQ, Greenplum DB, PostgreSQL

* Motivated by the need to compute aggregates based on geographic location
* Geohash background: http://en.wikipedia.org/wiki/Geohash
* C based user defined functions
* Based on this C language geohashing library: https://github.com/lyokato/libgeohash.git
* Install (everthing shown here was run on the Pivotal HD single node VM, CentOS release 6.5):
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
demo=# SELECT SUBSTRING(LAT_LON_TO_GEOHASH(latitude, longitude) FROM 1 for 5) geohash, COUNT(*)
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

* This shows the inverse function to recover a (lat, lon) from a geohash:

```
demo=# SELECT location, GEOHASH_TO_LAT_LON(LAT_LON_TO_GEOHASH(latitude, longitude))
demo-# FROM crimes
demo-# LIMIT 20;
                 location                 |   geohash_to_lat_lon
------------------------------------------+-------------------------
 (41.88312316336702, -87.73659457163214)  | (41.883123, -87.736595)
 (41.791348021159834, -87.78192025331069) | (41.791348, -87.781920)
 (41.842387914655085, -87.71337551836481) | (41.842388, -87.713375)
 (41.755760706231364, -87.65956849676316) | (41.755761, -87.659569)
 (41.80897612485923, -87.72254872914976)  | (41.808976, -87.722549)
 (41.722449100658835, -87.58488176146959) | (41.722449, -87.584882)
 (41.79087621277985, -87.65342904803647)  | (41.790876, -87.653429)
 (41.780107932510866, -87.73487049029012) | (41.780108, -87.734871)
 (41.768369162544815, -87.68797048054597) | (41.768369, -87.687971)
 (41.847183350218096, -87.71106103128797) | (41.847183, -87.711061)
 (41.75070078042353, -87.63439725625732)  | (41.750701, -87.634397)
 (41.876025230855, -87.70949617323915)    | (41.876025, -87.709496)
 (41.86698574675321, -87.72382153400046)  | (41.866986, -87.723822)
 (41.94454673907203, -87.79009306423538)  | (41.944547, -87.790093)
 (41.86199495131283, -87.72764278275724)  | (41.861995, -87.727643)
 (41.719811440283024, -87.5636380070539)  | (41.719812, -87.563638)
 (41.7499925263042, -87.60044819246234)   | (41.749993, -87.600448)
 (41.7961127248025, -87.75907981055788)   | (41.796113, -87.759080)
 (41.742918494975775, -87.5498300179199)  | (41.742919, -87.549830)
 (41.91321195720614, -87.75993816620701)  | (41.913212, -87.759938)
(20 rows)

Time: 1243.502 ms
```

* Researching the loss of precision in the coordinates returned by geohash_to_lat_lon is a TODO,
but it doesn't seem to affect the ability to recover an address using the supplied result.


