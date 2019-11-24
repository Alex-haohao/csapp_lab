#include "const.h"
#include "transplant.h"
#include "debug.h"
#include "help.h"
#include <dirent.h>



int string_compare(char *str1,char *str2){
    while(*str1==*str2)
   {
      if ( *str1 == '\0' || *str2 == '\0' )
         break;

      str1++;
      str2++;
   }
   if( *str1 == '\0' && *str2 == '\0' )
      return 0;
   else
      return -1;
}

int check_dir_exist(char *dir){

DIR *dirs = opendir(dir);
if (dirs) {
    /* Directory exists. */
    closedir(dirs);
    return 0;
}
else {
    return -1;
}

}

int array_size(char *array){
    int i;

    for(i = 0; *array != '\0'; ++i){
        array++;
    }
    return i;
}

void realcopy(char *array1,char *array2){
    while (*array1) {
        *(array2++) = *(array1++);
    }
    *array2 = '\0';
}

void list_all_file(char *name){

    DIR *d;
    struct dirent *dir;
    d = opendir(name);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    return ;
}

long file_size(char* filename){
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

int file_mode(char* filename){
    struct stat st;
    stat(filename, &st);
    return st.st_mode;
}


int is_file(char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int is_dir(char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}



int file_exit(char* filename){
    struct stat buffer;
    return(stat (filename, &buffer) ==0);
}




