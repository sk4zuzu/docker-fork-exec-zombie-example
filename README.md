
DOCKER-FORK-EXEC-ZOMBIE-EXAMPLE
===============================

## 1. PURPOSE

This is just a devops exercise - just to show when you can get zombie processes in docker.

## 2. RUN

```
$ make test
docker build -t docker-fork-exec-zombie-example /home/gublyn/_kartonauta/docker-fork-exec-zombie-example/ -f- <<< "$Dockerfile"
Sending build context to Docker daemon  160.8kB
Step 1/8 : FROM alpine:3.11
 ---> cc0abc535e36
Step 2/8 : RUN apk --no-cache add gcc make musl-dev tini
 ---> Using cache
 ---> cf8cd6559aad
Step 3/8 : WORKDIR /tmp/
 ---> Using cache
 ---> 1f8303032ffd
Step 4/8 : COPY Makefile ./
 ---> 18314486db0b
Step 5/8 : COPY src/cat-centipede.c ./src/
 ---> df174a6d0e5a
Step 6/8 : RUN make
 ---> Running in 011e33964df7
cc -g -c -o src/cat-centipede.o src/cat-centipede.c
cc -o cat-centipede src/cat-centipede.o
Removing intermediate container 011e33964df7
 ---> 12442f879d34
Step 7/8 : ENTRYPOINT ["/sbin/tini", "--"]
 ---> Running in 3bfd837f0e16
Removing intermediate container 3bfd837f0e16
 ---> 81c836c98b1c
Step 8/8 : CMD ["/bin/sh"]
 ---> Running in 9aa4704f5bd1
Removing intermediate container 9aa4704f5bd1
 ---> ffee6e435690
Successfully built ffee6e435690
Successfully tagged docker-fork-exec-zombie-example:latest
Error: No such container: docker-fork-exec-zombie-example-test1
eeb95b4bfd8f44382a6b7d7b9290f6b8d10d7c3ca2f3396ffa194f05e26818d8
PID   USER     TIME  COMMAND
    1 root      0:00 /sbin/tini -- /bin/sh -c /tmp/cat-centipede -n 8 || true; sleep 60
    7 root      0:00 sleep 60
   17 root      0:00 ps ax
docker-fork-exec-zombie-example-test1
Error: No such container: docker-fork-exec-zombie-example-test2
93e67bd0f9f48b2ade0cc19566abc7aea9bbc869e7046e0dce7ec991da79252c
PID   USER     TIME  COMMAND
    1 root      0:00 sleep 60
    8 root      0:00 [cat]
    9 root      0:00 [cat]
   10 root      0:00 [cat]
   11 root      0:00 [cat]
   12 root      0:00 [cat]
   13 root      0:00 [cat]
   14 root      0:00 [cat]
   15 root      0:00 [cat]
   16 root      0:00 ps ax
docker-fork-exec-zombie-example-test2
```

[//]: # ( vim:set ts=2 sw=2 et syn=markdown: )
