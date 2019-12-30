
SELF := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

CC  ?= gcc
CAT ?= $(shell which cat)

OUTPUT ?= cat-centipede
OBJS   := $(patsubst src/%.c,src/%.o,$(wildcard src/*.c))

define Dockerfile
FROM alpine:3.11

RUN apk --no-cache add gcc make musl-dev tini

WORKDIR /tmp/

COPY Makefile ./
COPY src/cat-centipede.c ./src/

RUN make

ENTRYPOINT ["/sbin/tini", "--"]

CMD ["/bin/sh"]
endef

export

.PHONY: all debug build test clean

all: $(OUTPUT)

build:
	docker build -t docker-fork-exec-zombie-example $(SELF)/ -f- <<< "$$Dockerfile"

test: test1 test2

debug: $(OUTPUT)
	gdb --args $< -e $(CAT) -n 69 -m MEOW

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^

src/%.o: src/%.c
	$(CC) -g -c -o $@ $^

clean:
	-rm $(OUTPUT) $(OBJS)

.PHONY: test1 test2

test1: build
	@(docker rm -f docker-fork-exec-zombie-example-test1 || true) && \
	docker run --rm -d --name docker-fork-exec-zombie-example-test1 docker-fork-exec-zombie-example /bin/sh -c "/tmp/cat-centipede -n 8 || true; sleep 60" && \
	docker exec docker-fork-exec-zombie-example-test1 /bin/sh -c "sleep 1 && sleep 1 && pkill -9 cat-centipede && sleep 1 && pkill cat && sleep 1 && ps ax" && \
	(docker rm -f docker-fork-exec-zombie-example-test1 || true)

test2: build
	@(docker rm -f docker-fork-exec-zombie-example-test2 || true) && \
	docker run --rm -d --entrypoint="" --name docker-fork-exec-zombie-example-test2 docker-fork-exec-zombie-example /bin/sh -c "/tmp/cat-centipede -n 8 || true; sleep 60" && \
	docker exec docker-fork-exec-zombie-example-test2 /bin/sh -c "sleep 1 && sleep 1 && pkill -9 cat-centipede && sleep 1 && pkill cat && sleep 1 && ps ax" && \
	(docker rm -f docker-fork-exec-zombie-example-test2 || true)

# vim:ts=8:sw=8:noet:
