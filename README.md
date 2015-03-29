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

