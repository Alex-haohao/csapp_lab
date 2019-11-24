#include <stdlib.h>
#include <errno.h>
#include <string.h>


#include "helper.h"


#define UNUSED(x) (void)(x)

/*
 * "Jobber" job spooler.
 */

int main(int argc, char *argv[])
{
    // TO BE IMPLEMENTED


    int batchmode = 0;
    int start_flag = 0;
    jobs_init();

    //                 printf("%s\n",argv[0] );

    // if(argc>1){

    //     if(strcmp(argv[1],"<") && argc>1){
    //      int in = open(argv[2], O_RDWR);
    //      batchmode =1;
    //      dup2(in,0);



    //      exit(EXIT_SUCCESS);
    //     }
    //     else{
    //         exit(EXIT_FAILURE);
    //     }
    // }




    if(batchmode ==0){
    while(start_flag != 1){


        char delim[] = " ";

        sleep(1);
        char* c = sf_readline("jobber> ");

    if(strcmp(c,"")==0){
        continue;
    }

    char *token = strtok(c, delim);



    if(strcmp(token,"help") ==0){
        printf("Available commands:\n"
            "help (0 args) Print this help message\n"
            "quit (0 args) Quit the program\n"
            "enable (0 args) Allow jobs to start\n"
            "disable (0 args) Prevent jobs from starting\n"
            "spool (1 args) Spool a new job\n"
            "pause (1 args) Pause a running job\n"
            "resume (1 args) Resume a paused job\n"
            "cancel (1 args) Cancel an unfinished job\n"
            "expunge (1 args) Expunge a finished job\n"
            "status (1 args) Print the status of a job\n"
            "jobs (0 args) Print the status of all jobs\n");
        continue;
    }
    else if (strcmp(token,"quit") ==0){
      start_flag =1;
      jobs_fini();
      exit(EXIT_SUCCESS);
    }
    else if(strcmp(token,"enable") ==0){


        int enable =jobs_set_enabled(1);
        UNUSED(enable);
    }
    else if(strcmp(token,"disable") ==0){
        disable();
    }
    else if(strcmp(token,"spool") ==0){


        char * token = strtok(NULL, "");


        char *p = token;
        if(p[0] == '\''){
            p++;
            p[strlen(p)-1] = 0;
        }


        int jobid = job_create(p);
        UNUSED(jobid);

    }
    else if(strcmp(token,"pause") ==0){
        char * token = strtok(NULL, "");
        int jobid = atoi(token);

        int pause = job_pause(jobid);
        UNUSED(pause);

    }
    else if(strcmp(token,"resume") ==0){
        char * token = strtok(NULL, "");
        int jobid = atoi(token);

        int resume = job_resume(jobid);
        UNUSED(resume);
    }
    else if(strcmp(token,"cancel")==0){
        char * token = strtok(NULL, "");
        int jobid = atoi(token);

        int cancel = job_cancel(jobid);
        UNUSED(cancel);
    }
    else if(strcmp(token,"expunge")==0){
        char * token = strtok(NULL, "");
        int jobid = atoi(token);

        int expunge = job_expunge(jobid);
        UNUSED(expunge);
    }
    else if(strcmp(token,"status")==0){
        char * token = strtok(NULL, "");
        int jobid = atoi(token);

        JOB_STATUS status = job_get_status(jobid);
        UNUSED(status);
    }
    else if(strcmp(token,"jobs")==0){


        jobs_print_all();
    }






}




    exit(EXIT_FAILURE);
}
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */
