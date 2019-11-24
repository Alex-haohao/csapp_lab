/*
 * Job manager for "jobber".
 */

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <string.h>
#include <errno.h>
#include "helper.h"

#define UNUSED(x) (void)(x)
int ccount = 0;
_Bool chld_signal = 0;
_Bool stop_signal = 0;
int orig_pid ;
int is_enable = 0;
int chld_flag;
int array_size(char *array);
int newjob = 0;
int sigmian_flag;



char* print_status(int status){
    if(status==0){
        return "NEW";
    }
    else if(status == 1){
        return "WAITING";
    }
    else if(status == 2){
        return "RUNNING";
    }
    else if(status == 3){
        return "PAUSED";
    }
    else if(status == 4){
        return "CANCELED";
    }
    else if(status == 5){
        return "COMPLETED";
    }
    else if(status == 6){
        return "ABORTED";
    }
    return NULL;
}

int array_size(char *array){
    int i;

    for(i = 0; *array != '\0'; ++i){
        array++;
    }
    return i;
}



void sigint_handler(int sig);
struct job_table table [MAX_JOBS];

struct job_table *get_job_by_pid( job_table *table, pid_t pid);
pid_t get_gpid(struct job_table *table);
int deletejob(struct job_table *table, pid_t pid);

pid_t get_gpid(struct job_table *jobs)
{
    for (int i = 0; i < MAX_JOBS; i++)
        if (jobs[i].status == RUNNING)
            return jobs[i].pid;
    return 0;
}

int jobid_by_pid(pid_t pid)
{


    if (pid < 1)
        return 0;
    for (int i = 0; i < MAX_JOBS; i++)
        if (table[i].pid == pid)
        {
            return table[i].jobid;
        }
    return 0;
}



 job_table *get_job_by_pid( job_table *table, pid_t pid)
{
    if (pid < 1)
        return NULL;
    for (int i = 0; i < MAX_JOBS; i++){
        if (table[i].pid == pid){
            return &table[i];
        }
    }
        return NULL;
}


void sigchld_handler(int sig)
{

    chld_flag =1;

}


void sigint_handler(int sig)
{
    exit(0);

    return;
}

void sigquit_handler(int sig){
    pid_t pid;
    int status;
      while((pid = waitpid(-1,&status,WNOHANG|WUNTRACED))>0){
        if(WEXITSTATUS(status)){
            printf("%s\n","NONONO" );
      }

}
 exit(0);
}

int chld(void);

int chld(){
    if(chld_flag ==1){

    pid_t pid;
    int status;
    while((pid = waitpid(-1,&status,WNOHANG|WUNTRACED))>0){


        if(WIFEXITED(status)){


            for (int table_number = 0; table_number < MAX_JOBS; table_number++)
     { if(table[table_number].pid ==pid){
         sf_job_status_change( table[table_number].jobid, RUNNING, COMPLETED);
        table[table_number].status = COMPLETED;
        sf_job_end( table[table_number].jobid, table[table_number].pid, status);
    }
}
        }

        if(WIFSIGNALED(status)){


        for (int table_number = 0; table_number < MAX_JOBS; table_number++)
     { if(table[table_number].pid ==pid){

        sf_job_end( table[table_number].jobid, table[table_number].pid, status);
        if(status ==134){
        sf_job_status_change( table[table_number].jobid, RUNNING, ABORTED);
        table[table_number].status = ABORTED;
        }
    }
}

        }
        if(WIFSTOPPED(status)){



        for (int table_number = 0; table_number < MAX_JOBS; table_number++)
     { if(table[table_number].pid ==pid){

        int pgid = job_get_pgid(table[table_number].jobid);
            sf_job_pause(table[table_number].jobid,  pgid);
    }
}


        }

 if(WIFCONTINUED(status)){


        for (int table_number = 0; table_number < MAX_JOBS; table_number++)
     { if(table[table_number].pid ==pid){

    }
}


        }



    }
    chld_flag =0;
    return 1;
}

return 0;

}

int jobs_init(void) {
    // TO BE IMPLEMENTED
    signal(SIGINT,  sigint_handler);
    sf_set_readline_signal_hook(chld);

   // signal(SIGSTOP, sigtstp_handler);
    for (int i = 0; i < MAX_JOBS; i++){
    table[i].pid = 0;
    table[i].jobid = -1;
    table[i].status = 10;
    table[i].task[0] = '\0';
    }


return 0;
}

void jobs_fini(void) {
    // TO BE IMPLEMENTED
     for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].status == COMPLETED ||table[table_number].status == ABORTED){
            job_expunge(table[table_number].jobid);
        }
        if(table[table_number].status == CANCELED ||table[table_number].status == PAUSED ||
            table[table_number].status == WAITING ){
            table[table_number].status = ABORTED;
            if(table[table_number].status ==WAITING){
            sf_job_status_change(table[table_number].jobid,  WAITING,  ABORTED);
        }
        else if(table[table_number].status ==CANCELED){
            sf_job_status_change(table[table_number].jobid,  CANCELED,  ABORTED);
        }
        else if(table[table_number].status ==PAUSED){
            sf_job_status_change(table[table_number].jobid,  PAUSED,  ABORTED);
        }
        job_expunge(table[table_number].jobid);

        }
}
}

int jobs_set_enabled(int val) {
    if(val ==0){
        is_enable =0;
        return is_enable;
    }

    if(val!=0){

    is_enable =1;

    for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].status == WAITING){


    TASK* new_task = table[table_number].task_save;

    int fd[2];
    pid_t pid;
   // pid_t pgid;

//-------------------------------------------------------------



     PIPELINE *first_pipe = new_task->pipelines->first;
     PIPELINE_LIST *rest_pipe = new_task->pipelines->rest;


  sigset_t mask_all, mask_child, prev_one;
 sigfillset(&mask_all);
 sigemptyset(&mask_child);
 sigaddset(&mask_child, SIGCHLD);
signal(SIGCHLD, sigchld_handler);
        sigprocmask(SIG_BLOCK, &mask_child, &prev_one);
     while(first_pipe != NULL){




        int redirect_flag = 0;
        int pipe_flag = 0;
        UNUSED(redirect_flag);
        UNUSED(pipe_flag);

        char* input_path = first_pipe->input_path;
        char* output_path = first_pipe->output_path;

        if(input_path != NULL || output_path != NULL){
            redirect_flag = 1;
        }
        if(rest_pipe != NULL){
            pipe_flag =1;
        }

        COMMAND_LIST *commands = first_pipe->commands;

        COMMAND *first_command = commands->first;
        COMMAND_LIST *rest_commands  = commands->rest;

            char *cmd[10][10];

            for (int i=0;i<10;i++){
                for (int j=0;j<10;j++){
                    cmd[i][j] = "this is magic";
                }
            }
            int cmd_counter = 0;
        while(first_command != NULL){

            char* word = first_command->words->first;
            WORD_LIST* rest_words = first_command->words->rest;



            char* one_command[50];
            char** command_pointer = one_command;
            int argv_counter =0;
            while(word != NULL){
                *command_pointer = word;
                command_pointer ++;
                argv_counter++;

                if(rest_words ==NULL){
                    break;
                 }
                word = rest_words->first;
                rest_words = rest_words->rest;
            }

            //-----------------------------------------------------------------------------

            char *argv[argv_counter];
            for(int i =0;i<argv_counter;i++){
                argv[i] = one_command[i];
                cmd[cmd_counter][i]=one_command[i];
            }

            // printf("%s\n",cmd[cmd_counter][0] );
            // printf("%d\n",cmd_counter );
            UNUSED(argv);
            cmd_counter ++;

            //-------------------------------------------------------------------------------



             if(rest_commands ==NULL){
                    break;
                 }
            first_command = rest_commands->first;
            rest_commands = rest_commands->rest;

        }


            for(int i=0;i<cmd_counter;i++){
            int command_counter =0;

            for(int j = 0;strcmp(cmd[i][j],"this is magic") != 0;j++){
                command_counter++;
            }

            cmd[i][command_counter] = '\0';
}
           *cmd[cmd_counter] = NULL;



    int fdd = 0;
    int i =0;
    while (*cmd[i] != NULL) {
        pipe(fd);
        if ((pid = fork()) == -1) {
            perror("fork ERROR");
            exit(1);
        }
        else if (pid == 0) {
            int new_pid = getpid();
            setpgid(new_pid,new_pid);

            dup2(fdd, 0);
            if (*cmd[i+1] != NULL) {
                dup2(fd[1], 1);
            }
            else{


            if(output_path !=NULL){
            int out = open(output_path,O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
            dup2(out,1);
            }
            }
            close(fd[0]);



            // if(output_path !=NULL){
            // int out = open(output_path,O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
            // dup2(out,1);
            // }

            if ( execvp(*cmd[i], cmd[i])) {

            abort();

            printf("%s\n","haha no way" );
            }



        }
        else {



            orig_pid = getpid();

            close(fd[1]);
            fdd = fd[0];
            i++;



        }
    }





        if(rest_pipe ==NULL){
            break;
        }
      first_pipe = rest_pipe->first;
      rest_pipe = rest_pipe->rest;


     }



sigprocmask(SIG_BLOCK, &mask_all, NULL);


           // int pgid = getpgid(pid);
            table[table_number].pid = pid;
            sf_job_start( table[table_number].jobid,  pid);
            sf_job_status_change(table_number,  WAITING,  RUNNING);
            table[table_number].status = RUNNING;

            sigprocmask(SIG_SETMASK, &prev_one, NULL);
     //------------------------add job-----------------------------------------------------

            // table[table_number].pid = pid;
            // sf_job_status_change(table_number,  RUNNING,  COMPLETED);

            // table[table_number].status = COMPLETED;




        }

    }




}

return 0;

}

int jobs_get_enabled() {
    // TO BE IMPLEMENTED
    if (is_enable ==0){
        return 0;
    }
    else{
        return 1;
    }
}

int job_create(char *command) {
    // TO BE IMPLEMENTED


    int current_table_place;
    int nospace =0;

    for (int i = 0; i < MAX_JOBS; i++)
    {

        if (table[i].jobid == -1)
        {
            nospace = 1;
            sf_job_create(i);
            table[i].status = NEW;
            table[i].jobid = i;
            current_table_place = i;



            strcpy(table[i].task, command);
            break;
        }
    }

    if(nospace ==0){
        return -1;
    }

    TASK* new_task =parse_task(&command);

    if(new_task != NULL){
        sf_job_status_change(current_table_place,  NEW,  WAITING);
        table[current_table_place].status = WAITING;
        table[current_table_place].task_save = new_task;
    }




     //----------------start----------------------------------


//----------------------------------------------------------------------------------------


if(is_enable ==1){
    jobs_set_enabled(1);
}
if(is_enable ==0){
    jobs_set_enabled(0);
}








return 0;
}

int job_expunge(int jobid) {
    // TO BE IMPLEMENTED
    for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].jobid == jobid){
            if(table[table_number].status == ABORTED ||table[table_number].status == COMPLETED){
                free_task(table[table_number].task_save);
                table[table_number].pid = 0;
                table[table_number].jobid = -1;
                table[table_number].status = 10;
                table[table_number].task[0] = '\0';
                return 0;
            }
        }
    }
return -1;

}

int job_cancel(int jobid) {
    // TO BE IMPLEMENTED
     for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].jobid == jobid){
            if(table[table_number].status == RUNNING ){
            int pgid = job_get_pgid(jobid);
            killpg(pgid,SIGKILL);
            sf_job_status_change(jobid,  RUNNING,  CANCELED);
            table[table_number].status = CANCELED;
            return 0;
            }
            else if(table[table_number].status == PAUSED){
            int pgid = job_get_pgid(jobid);
            killpg(pgid,SIGKILL);
            sf_job_status_change(jobid,  PAUSED,  CANCELED);
            table[table_number].status = CANCELED;
            return 0;
            }
            else if(table[table_number].status == WAITING){
                sf_job_status_change(jobid,  WAITING,  ABORTED);
                table[table_number].status = ABORTED;
                return 0;
            }
        }
    }
    return -1;
}

int job_pause(int jobid) {
    // TO BE IMPLEMENTED

    for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].jobid == jobid){
            if(table[table_number].status == RUNNING){
            int pgid = job_get_pgid(jobid);
            killpg(pgid,SIGSTOP);

            sf_job_status_change(jobid,  RUNNING,  PAUSED);
            table[table_number].status = PAUSED;
            return 0;
            }
        }
    }
    return -1;
}

int job_resume(int jobid) {
    // TO BE IMPLEMENTED

    for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].jobid == jobid){
            if(table[table_number].status == PAUSED){
            int pgid = job_get_pgid(jobid);
            killpg(pgid,SIGCONT);
            sf_job_resume(table[table_number].jobid, pgid);
            sf_job_status_change(jobid,  PAUSED,  RUNNING);
            table[table_number].status = RUNNING;
            return 0;
            }
        }
    }
    return -1;
}

int job_get_pgid(int jobid) {
    // TO BE IMPLEMENTED
    for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].jobid == jobid){
        if (table[table_number].status == RUNNING || table[table_number].status == PAUSED || table[table_number].status == CANCELED){
            int pid = table[table_number].pid;
            int pgid = getpgid(pid);
            return pgid;
        }
    }
    }
    return -1;
}

void jobs_print_all(){
    for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].jobid != -1){
            char*status_name = print_status(table[table_number].status);
        printf("print job [%d] [%s] %d %s\n", table[table_number].jobid,status_name, table[table_number].pid, table[table_number].task);

        }
    }
}

JOB_STATUS job_get_status(int jobid) {
    // TO BE IMPLEMENTED
    for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].jobid == jobid){
            printf("Status: %s\n",print_status(table[table_number].status) );
            return table[table_number].status;


        }

    }

    return -1;
}

int job_get_result(int jobid) {
    // TO BE IMPLEMENTED
for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {

        if(table[table_number].status == COMPLETED){
            int status;
            int wpid = waitpid(table[table_number].pid, &status, WNOHANG);
            return wpid;
        }

    }

    return -1;
}

int job_was_canceled(int jobid) {
    // TO BE IMPLEMENTED
    for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {

        if(table[table_number].status == CANCELED ||table[table_number].status == ABORTED){

            return 1;
        }

    }

    return 0;
}

char *job_get_taskspec(int jobid) {
    // TO BE IMPLEMENTED
    for (int table_number = 0; table_number < MAX_JOBS; table_number++)
    {
        if(table[table_number].jobid == jobid){
        return table[table_number].task;
        }
    }
    return NULL;
}

void disable(void){

    is_enable =0;
    printf("%s  %d\n","is is_enable = ",is_enable);
}


void cleanall(void){

}