#include <sys/file.h>
#include <stdio.h>
#include <ctype.h>
#include <sgtty.h>
#include <signal.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <utime.h>
#include <dirent.h>
#include <sys/stat.h>






// clear.c
void clearinit ();
void clear_the_screen ();
int other_fields ();


// operration.c
int add_the_entry ();
void rolo_add ();
void rolo_peruse_mode (Ptr_Rolo_List first_rlink) ;
void display_list_of_entries (Ptr_Rolo_List rlist) ;
int entry_action (Ptr_Rolo_List rlink) ;

// rlist.c
void rolo_insert (Ptr_Rolo_List link, int (*compare)()) ;
void rolo_delete (Ptr_Rolo_List link) ;
int compare_links (Ptr_Rolo_List l1,Ptr_Rolo_List l2) ;
void rolo_reorder ();
int rlength (Ptr_Rolo_List rlist);

//search
void rolo_search_mode (int field_index,char *field_name,char *search_string);
int find_all_matches (int field_index,char *field_name,char *search_string,Ptr_Rolo_List *ptr_first_match);
int match_link (Ptr_Rolo_List rlink,int field_index,char *field_name,int fnlen,char *search_string,int sslen);
int match_by_name_or_company (char *search_string,int sslen);
int select_field_to_search_by (int *ptr_index,char **ptr_name) ;


//update
void rolo_update_mode (Ptr_Rolo_List rlink) ;


//io.c
int read_rolodex (int fd) ;
void write_rolo_list (FILE *fp) ;
void write_rolo (FILE *fp1,FILE *fp2) ;
void display_basic_field (char *name,char *value,int show,int up) ;
void display_other_field (char *fieldstring) ;
void summarize_entry_list (Ptr_Rolo_List rlist,char *ss) ;
void display_field_names ();
void display_entry (Ptr_Rolo_Entry entry) ;
void display_entry_for_update (Ptr_Rolo_Entry entry) ;
void cathelpfile (char *filepath,char *helptopic,int clear);
void any_char_to_continue ();


// menuaux.c
int rolo_menu_yes_no (char *prompt,int rtn_default,int help_allowed,char *helpfile,char *subject);
int rolo_menu_data_help_or_abort (char *prompt,char *helpfile,char *subject,char **ptr_response);
int rolo_menu_number_help_or_abort (char *prompt,int low,int high,int *ptr_ival);


// option
void print_short ();
int person_match (char *person,Ptr_Rolo_Entry entry) ;
int find_all_person_matches (char *person) ;
void look_for_person (char *person) ;
void print_people ();
void interactive_rolo ();


// rolo.c
void user_interrupt ();
void suser_eof ();
void user_eof ();
void roloexit (int rval) ;
void save_to_disk ();
void save_and_exit (int rval) ;
int rolo_only_to_read ();
void locked_action ();

extern char *optionarg[50];

extern char *nonoptionarg[50];

extern int noptionargs;
extern int nnonoptionargs;
//menu.c
