#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <limits.h>

extern char **environ;

int main(int argc, char *argv[]) {
    int opt;
    int i;
    char options[argc];
    char *args[argc];
    int count = 0;

    while ((opt = getopt(argc, argv, "ispU:cC:dV:v")) != -1) {
        options[count] = opt;
        args[count] = optarg;
        count++;
    }

    for (i = count - 1; i >= 0; i--) {

        switch (options[i]) {

        case 'i':
            printf("Real UID: %d\n", getuid());
            printf("Effective UID: %d\n", geteuid());
            printf("Real GID: %d\n", getgid());
            printf("Effective GID: %d\n", getegid());
            break;

        case 's':
            setpgid(0, 0);
            break;

        case 'p':
            printf("PID: %d\n", getpid());
            printf("PPID: %d\n", getppid());
            printf("PGID: %d\n", getpgrp());
            break;

        case 'u': {
            struct rlimit limit;

            getrlimit(RLIMIT_FSIZE, &limit);
            printf("Ulimit: %ld\n", (long)limit.rlim_cur);
            break;
        }

        case 'U': {
            struct rlimit limit;

            getrlimit(RLIMIT_FSIZE, &limit);
            limit.rlim_cur = atol(args[i]);
            setrlimit(RLIMIT_FSIZE, &limit);
            break;
        }

        case 'c': {
            struct rlimit limit;

            getrlimit(RLIMIT_CORE, &limit);
            printf("Core size: %ld\n", (long)limit.rlim_cur);
            break;
        }

        case 'C': {
            struct rlimit limit2;

            limit2.rlim_cur = atol(args[i]);
            limit2.rlim_max = atol(args[i]);
            setrlimit(RLIMIT_CORE, &limit2);
            break;
        }

        case 'd': {
            char dir[4096];

            getcwd(dir, sizeof(dir));
            printf("%s\n", dir);
            break;
        }

        case 'v': {
            char **env = environ;

            while (*env != NULL) {
                printf("%s\n", *env);
                env++;
            }
            break;
        }

        case 'V':
            putenv(args[i]);
            break;

        default:
            printf("Invalid option\n");
            break;
        }
    }

    return 0;
}