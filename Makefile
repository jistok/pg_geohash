MODULES = pg_geohash
EXTENSION = pg_geohash
DATA_built = pg_geohash--1.0.sql
DOCS = README.md
OBJS = pg_geohash.o geohash.o
MODULE_big = pg_geohash

PG_CONFIG ?= pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)


pg_geohash--1.0.sql: pg_geohash-1.0.sql
	cp $< $@
