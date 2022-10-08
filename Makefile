build-server:
	@$(MAKE) -C server build

install-server:
	@$(MAKE) -C server install

build-client:
	@$(MAKE) -C client build

install-client:
	@$(MAKE) -C client install

client:	build-client install-client

server: build-server install-server

all: server client