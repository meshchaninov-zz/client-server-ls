/*
 * This file is for the data structure that is sent over TCP
 */

#define PATH_SIZE 1000
#define EX_SIZE 100

struct form {
    char path[PATH_SIZE];
    char e[EX_SIZE];
};
