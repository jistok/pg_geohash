MODULES = pg_geohash
EXTENSION = pg_geohash
DATA = pg_geohash-1.0.sql
DOCS = README.md
OBJS = pg_geohash.o geohash.o
MODULE_big = pg_geohash

PGXS := $(shell pg_config --pgxs)
include $(PGXS)

