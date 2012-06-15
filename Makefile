MAKE=make
PACKAGES = \
	extern/boost \
	extern/erlang \
	extern/pugixml \
	uhal/log \
	uhal/grammars \
	uhal/uhal \
	uhal/tests \
	controlhub

TARGETS=clean rpm build all


.PHONY: $(TARGETS)
default: build

$(TARGETS):
	for p in $(PACKAGES) ; do $(MAKE) -C $$p $@ ; done