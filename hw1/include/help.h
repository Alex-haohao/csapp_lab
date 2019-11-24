#ifndef HELP_H
#define HELP_H

#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include <utime.h>
#include <dirent.h>
#include <sys/stat.h>

#include "transplant.h"


int string_compare(char *str1,char *str2);

int check_dir_exist(char *dir);

int array_size(char *array);

void realcopy(char *array1,char *array2);

void list_all_file(char *name);

long file_size(char* filename);

int is_file(char* pathname);

int is_dir(char* pathname);

int file_mode(char* filename);

int file_exit(char* filename);


#endif