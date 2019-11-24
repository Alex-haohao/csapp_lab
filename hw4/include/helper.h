#include <stdlib.h>
#include <signal.h>
#include<stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <string.h>
#include <errno.h>
#include<fcntl.h>

#include "jobber.h"
#include "task.h"



typedef struct job_table
{
    pid_t pid;
    int jobid;
    JOB_STATUS status;
    char task[1000];
    struct TASK* task_save;
} job_table;

extern struct job_table table [MAX_JOBS];
extern int is_enable;
extern int chld_flag;
extern int sigmian_flag;




void jobs_print_all();
void disable(void);

void cleanall(void);
