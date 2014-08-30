# allow user to set the toolset
TOOLSET ?= gcc
# allow user to set the --extras flag
TEST_EXTRAS ?=

all:

test: FORCE
	./bootstrap.sh --with-toolset=$(TOOLSET)
	cd test && python test_all.py $(TOOLSET)

FORCE:
