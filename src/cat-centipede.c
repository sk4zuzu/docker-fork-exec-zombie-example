#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>


#define _fail_if(__cond__)                       \
    if (__cond__) {                              \
        printf("%s():%s:%s\n", __func__,         \
                              #__cond__,         \
                               strerror(errno)); \
        exit(errno);                             \
    }


#define _close(__pipe__) \
    close(__pipe__[0]); \
    close(__pipe__[1]);


#define _trolo(__fdwr__, __mesg__) \
    _fail_if(write(__fdwr__, __mesg__, strlen(__mesg__)+1) < 0);


static void _spawn(char *cat, int *glue,
                              int *prev,
                              int *next) { 
    pid_t pid = fork();

    _fail_if(pid < 0);

    if (pid == 0) {
        close(0);
        close(1);

        dup2(prev[0], 0);
        dup2(next[1], 1);

        if (prev != glue && next != glue) _close(glue);
        if (prev != next) _close(next);
        _close(prev);

        _fail_if(execl(cat, cat, NULL) < 0);
    }
}


static void _cat_centipede(char *cat, int N, char *M) { // xD
    int glue [2];

    if (N == 1) {
        _fail_if(pipe(glue) < 0);

        _spawn(cat, glue,  // glue
                    glue,  // prev
                    glue); // next
    } else {
        int prev [2],
            next [2];

        _fail_if(pipe(glue) < 0);
        _fail_if(pipe(next) < 0);

        _spawn(cat, glue,  // glue
                    glue,  // prev
                    next); // next

        int k;
        for (k = 1; k < N-1; k++) {
            prev[0] = next[0];
            prev[1] = next[1];

            _fail_if(pipe(next) < 0);

            _spawn(cat, glue,  // glue
                        prev,  // prev
                        next); // next

            _close(prev);
        }

        _spawn(cat, glue,  // glue
                    next,  // prev
                    glue); // next

        _close(next);
    }

    _trolo(glue[1], M);
    _close(glue);

    wait(0);
    exit(0);
}


static void _usage(char *argv[]) {
    printf("Usage: %s [-h] [-e path]"
                         " [-n number]"
                         " [-m string]\n", argv[0]);
}


int main(int argc, char *argv[]) {
    char *cat = "/bin/cat"; int N = 69; char *M = "MEOW";

    int opt;

    while ((opt = getopt(argc, argv, "e:n:m:h")) != -1) {
        switch (opt) {
            case 'e': cat = optarg;       break;
            case 'n': N   = atoi(optarg); break;
            case 'm': M   = optarg;       break;
            case 'h':
            default : _usage(argv); exit(0);
        }
    }

    errno = EINVAL;
    _fail_if(N < 1 || !M);

    _cat_centipede(cat, N, M);
}


// vim:ts=4:sw=4:et:
