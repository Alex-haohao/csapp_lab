#include "const.h"
#include "transplant.h"
#include "debug.h"
#include "help.h"

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/*
 * A function that returns printable names for the record types, for use in
 * generating debugging printout.
 */
static char *record_type_name(int i) {
    switch(i) {
    case START_OF_TRANSMISSION:
	return "START_OF_TRANSMISSION";
    case END_OF_TRANSMISSION:
	return "END_OF_TRANSMISSION";
    case START_OF_DIRECTORY:
	return "START_OF_DIRECTORY";
    case END_OF_DIRECTORY:
	return "END_OF_DIRECTORY";
    case DIRECTORY_ENTRY:
	return "DIRECTORY_ENTRY";
    case FILE_DATA:
	return "FILE_DATA";
    default:
	return "UNKNOWN";
    }
}

/*
 * @brief  Initialize path_buf to a specified base path.
 * @details  This function copies its null-terminated argument string into
 * path_buf, including its terminating null byte.
 * The function fails if the argument string, including the terminating
 * null byte, is longer than the size of path_buf.  The path_length variable
 * is set to the length of the string in path_buf, not including the terminating
 * null byte.
 *
 * @param  Pathname to be copied into path_buf.
 * @return 0 on success, -1 in case of error
 */
int path_init(char *name) {
    // To be implemented.
    if(array_size(name)+1>sizeof(path_buf)){
        return -1;
    }
    else{
        realcopy(name,path_buf);
        path_length = array_size(path_buf);
        return 0;
    }
}

/*
 * @brief  Append an additional component to the end of the pathname in path_buf.
 * @details  This function assumes that path_buf has been initialized to a valid
 * string.  It appends to the existing string the path separator character '/',
 * followed by the string given as argument, including its terminating null byte.
 * The length of the new string, including the terminating null byte, must be
 * no more than the size of path_buf.  The variable path_length is updated to
 * remain consistent with the length of the string in path_buf.
 *
 * @param  The string to be appended to the path in path_buf.  The string must
 * not contain any occurrences of the path separator character '/'.
 * @return 0 in case of success, -1 otherwise.
 */
int path_push(char *name) {
    if(array_size(name)+path_length+1>sizeof(path_buf)){
        return -1;
    }
    else{
    *(path_buf+path_length)= '/';
    realcopy(name,path_buf+path_length+1);
    path_length = path_length+array_size(name)+1;
    return 0;
    }
}

/*
 * @brief  Remove the last component from the end of the pathname.
 * @details  This function assumes that path_buf contains a non-empty string.
 * It removes the suffix of this string that starts at the last occurrence
 * of the path separator character '/'.  If there is no such occurrence,
 * then the entire string is removed, leaving an empty string in path_buf.
 * The variable path_length is updated to remain consistent with the length
 * of the string in path_buf.  The function fails if path_buf is originally
 * empty, so that there is no path component to be removed.
 *
 * @return 0 in case of success, -1 otherwise.
 */
int path_pop() {
    // To be implemented.
    if(*path_buf == '\0'){
        return -1;
    }
    else{
    int counter = 0;
    char *last_element = path_buf+path_length;
    while(*last_element !='/'){
        *last_element = '\0';
        last_element--;
        counter ++;
    }
        *last_element = '\0';

    path_length = path_length - counter ;
    return 0;
}
}

/*
 * @brief Deserialize directory contents into an existing directory.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory.  It reads (from the standard input) a sequence of DIRECTORY_ENTRY
 * records bracketed by a START_OF_DIRECTORY and END_OF_DIRECTORY record at the
 * same depth and it recreates the entries, leaving the deserialized files and
 * directories within the directory named by path_buf.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * each of the records processed.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including depth fields in the records read that do not match the
 * expected value, the records to be processed to not being with START_OF_DIRECTORY
 * or end with END_OF_DIRECTORY, or an I/O error occurs either while reading
 * the records from the standard input or in creating deserialized files and
 * directories.
 */
int deserialize_directory(int depth) {

        int character;

        while ((character = getchar()) != EOF ) {


        unsigned char magic1 = 0x0c;
        unsigned char magic2 = 0x0d;
        unsigned char magic3 = 0xED;


        if (character == magic1){
            continue;
        }
        else if (character == magic2){
            continue;
        }
        else if (character == magic3){
            continue;
        }

        else if(character == 0x00){
        // start of transmission
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();
        }

        else if(character == 0x01){
        // end of transmission
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();

        }
        else if(character == 0x02){
           // start of directory
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();
            deserialize_directory(depth+1);


        }
        else if(character == 0x03){
            if(depth>1){
            path_pop();
        }
           // end of directory
            character = getchar();character = getchar();
            character = getchar();character = getchar();

            character = getchar();character = getchar();
            character = getchar();character = getchar();


            character = getchar();character = getchar();
            character = getchar();character = getchar();
            character = getchar();

            deserialize_directory(depth-1);



        }

        else if(character == 0x04){
           // directory  endtry

            unsigned char deep_one = getchar();
            unsigned char deep_two = getchar();
            unsigned char deep_three = getchar();
            unsigned char deep_four = getchar();
            unsigned int deep = deep_four | deep_three << 8| deep_two << 16| deep_one << 24;
            if(deep!=depth){
                return -1;
            }


            unsigned char size_one = getchar();
            unsigned char size_two = getchar();
            unsigned char size_three = getchar();
            unsigned char size_four = getchar();
            unsigned char size_five = getchar();
            unsigned char size_six = getchar();
            unsigned char size_seven = getchar();
            unsigned char size_eight = getchar();

            unsigned long size = size_eight | size_seven << 8| size_six << 16| size_five << 24| (unsigned long)size_four << 32| (unsigned long)size_three << 40|
            (unsigned long)size_two << 48| (unsigned long)size_one  << 56;

            unsigned char type_one = getchar();
            unsigned char type_two = getchar();
            unsigned char type_three = getchar();
            unsigned char type_four = getchar();
            unsigned int type = type_four | type_three << 8| type_two << 16| type_one << 24;



            unsigned char sizeinfor_one = getchar();
            unsigned char sizeinfor_two = getchar();
            unsigned char sizeinfor_three = getchar();
            unsigned char sizeinfor_four = getchar();
            unsigned char sizeinfor_five = getchar();
            unsigned char sizeinfor_six = getchar();
            unsigned char sizeinfor_seven = getchar();
            unsigned char sizeinfor_eight = getchar();

            unsigned long size_infor = sizeinfor_eight | sizeinfor_seven << 8| sizeinfor_six << 16| sizeinfor_five << 24| (unsigned long)sizeinfor_four << 32| (unsigned long)sizeinfor_three << 40|
            (unsigned long)sizeinfor_two << 48| (unsigned long)sizeinfor_one  << 56;

            long input_number = size-16-12;
            char *name_buf_pointer = name_buf;
            int counter = 0;
            for(int i =0;i<input_number;i++){
                *name_buf_pointer = getchar();
                name_buf_pointer++;
                counter++;
            }


            path_push(name_buf);


            if (S_ISDIR(type) == 1){

                mkdir(path_buf,0700);
                chmod(path_buf, type & 0777);

            }
            else{

                int make_file = deserialize_file(depth);
                if (make_file ==-1){
                    return -1;
                }
                chmod(path_buf, type & 0777);
                path_pop();
            }
            // ;




            for(int i =0;i<counter;i++){
                *name_buf_pointer = '\0';
                name_buf_pointer--;
            }
            *name_buf_pointer = '\0';



        }

        }



    // To be implemented.
    return 0;
}

/*
 * @brief Deserialize the contents of a single file.
 * @details  This function assumes that path_buf contains the name of a file
 * to be deserialized.  The file must not already exist, unless the ``clobber''
 * bit is set in the global_options variable.  It reads (from the standard input)
 * a single FILE_DATA record containing the file content and it recreates the file
 * from the content.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * the FILE_DATA record.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including a depth field in the FILE_DATA record that does not match
 * the expected value, the record read is not a FILE_DATA record, the file to
 * be created already exists, or an I/O error occurs either while reading
 * the FILE_DATA record from the standard input or while re-creating the
 * deserialized file.
 */
int deserialize_file(int depth){
    unsigned char magic1 = getchar();
    unsigned char magic2 = getchar();
    unsigned char magic3 = getchar();
    unsigned char record = getchar();
    if(record!=0x05){
        return -1;
    }

    unsigned char deep_one = getchar();
    unsigned char deep_two = getchar();
    unsigned char deep_three = getchar();
    unsigned char deep_four = getchar();

    unsigned int deep = deep_four | deep_three << 8| deep_two << 16| deep_one << 24;
    if (deep != depth){
        return -1;
    }


    unsigned char size_one = getchar();
    unsigned char size_two = getchar();
    unsigned char size_three = getchar();
    unsigned char size_four = getchar();
    unsigned char size_five = getchar();
    unsigned char size_six = getchar();
    unsigned char size_seven = getchar();
    unsigned char size_eight = getchar();

    unsigned long size = size_eight | size_seven << 8| size_six << 16| size_five << 24| (unsigned long)size_four << 32| (unsigned long)size_three << 40|
    (unsigned long)size_two << 48| (unsigned long)size_one  << 56;

    long input_number = size-16;


    // unsigned char one_header = (unsigned char)(size >> 56);
    //             unsigned char two_header = (unsigned char)(size >> 48);
    //             unsigned char three_header = (unsigned char)(size >>  40);
    //             unsigned char four_header = (unsigned char)(size >>  32);
    //             unsigned char five_header = (unsigned char)(size >> 24);
    //             unsigned char six_header = (unsigned char)(size >> 16);
    //             unsigned char seven_header = (unsigned char)(size >>  8);
    //             unsigned char eight_header = (unsigned char)(size >>  0);

    //             printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", one_header,two_header,three_header,four_header,five_header,six_header ,seven_header,eight_header);

    if(global_options == 12 || file_exit(path_buf)!=1){
        FILE *f = fopen(path_buf,"w");
    for(int i =0;i<input_number;i++){
        fputc(getchar(),f);
    }

    fclose(f);
    return 0;

    }
    else{
        for(int i =0;i<input_number;i++){
        getchar();
    }
    return -1;

    }






}

/*
 * @brief  Serialize the contents of a directory as a sequence of records written
 * to the standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory to be serialized.  It serializes the contents of that directory as a
 * sequence of records that begins with a START_OF_DIRECTORY record, ends with an
 * END_OF_DIRECTORY record, and with the intervening records all of type DIRECTORY_ENTRY.
 *
 * @param depth  The value of the depth field that is expected to occur in the
 * START_OF_DIRECTORY, DIRECTORY_ENTRY, and END_OF_DIRECTORY records processed.
 * Note that this depth pertains only to the "top-level" records in the sequence:
 * DIRECTORY_ENTRY records may be recursively followed by similar sequence of
 * records describing sub-directories at a greater depth.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open files, failure to traverse directories, and I/O errors
 * that occur while reading file content and writing to standard output.
 */
int serialize_directory(int depth) {

    DIR *d;
    struct dirent *dir;
    d = opendir(path_buf);

    if(d == NULL){
     return -1;}

    int start_depth = depth +1;


    unsigned char first = (unsigned char)(start_depth >> 24);
    unsigned char second = (unsigned char)(start_depth >> 16);
    unsigned char thrid = (unsigned char)(start_depth >>  8);
    unsigned char fourth = (unsigned char)(start_depth >>  0);


    putchar(0x0C); putchar(0x0D); putchar(0xED); putchar(0x02);
    putchar(first); putchar(second); putchar(thrid); putchar(fourth);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x00);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x10);

///////////////////////////////////////////////////



while ((dir = readdir(d)) != NULL)
        {
            path_push(dir->d_name);
            if(string_compare(dir->d_name,".")!=0 && string_compare(dir->d_name,"..")!=0){

                if(is_dir(path_buf) && string_compare(dir->d_name,".")!=0 && string_compare(dir->d_name,"..")!=0){

                if (string_compare(dir->d_name, ".") == 0 || string_compare(dir->d_name, "..") == 0)
                continue;


                putchar(0x0C);
                putchar(0x0D);
                putchar(0xED);
                putchar(0x04);

                int entry_depth =  depth +1;
                unsigned char entry_first = (unsigned char)(entry_depth >> 24);
                unsigned char entry_second = (unsigned char)(entry_depth >> 16);
                unsigned char entry_thrid = (unsigned char)(entry_depth >>  8);
                unsigned char entry_fourth = (unsigned char)(entry_depth >>  0);

                putchar(entry_first);
                putchar(entry_second);
                putchar(entry_thrid);
                putchar(entry_fourth);



                long header_size;
                header_size = 12+16+array_size(dir->d_name);
                unsigned char one_header = (unsigned char)(header_size >> 56);
                unsigned char two_header = (unsigned char)(header_size >> 48);
                unsigned char three_header = (unsigned char)(header_size >>  40);
                unsigned char four_header = (unsigned char)(header_size >>  32);
                unsigned char five_header = (unsigned char)(header_size >> 24);
                unsigned char six_header = (unsigned char)(header_size >> 16);
                unsigned char seven_header = (unsigned char)(header_size >>  8);
                unsigned char eight_header = (unsigned char)(header_size >>  0);

                putchar(one_header);
                putchar(two_header);
                putchar(three_header);
                putchar(four_header);
                putchar(five_header);
                putchar(six_header);
                putchar(seven_header);
                putchar(eight_header);



                long size;
                int type;
                size = file_size(path_buf);
                type = file_mode(path_buf);

                unsigned char one_type = (unsigned char)(type >> 24);
                unsigned char two_type = (unsigned char)(type >> 16);
                unsigned char three_type = (unsigned char)(type >>  8);
                unsigned char four_type = (unsigned char)(type >>  0);

                putchar(one_type);
                putchar(two_type);
                putchar(three_type);
                putchar(four_type);


                unsigned char one = (unsigned char)(size >> 56);
                unsigned char two = (unsigned char)(size >> 48);
                unsigned char three = (unsigned char)(size >>  40);
                unsigned char four = (unsigned char)(size >>  32);
                unsigned char five = (unsigned char)(size >> 24);
                unsigned char six = (unsigned char)(size >> 16);
                unsigned char seven = (unsigned char)(size >>  8);
                unsigned char eight = (unsigned char)(size >>  0);


                putchar(one);
                putchar(two);
                putchar(three);
                putchar(four);
                putchar(five);
                putchar(six);
                putchar(seven);
                putchar(eight);



                int counter;
                char *name_pointer;
                name_pointer = dir->d_name;
                for(counter = 0;counter<array_size(dir->d_name);counter++){
                    putchar(*name_pointer);
                    name_pointer++;
                }



             if(is_file(path_buf) && string_compare(dir->d_name,".")!=0 && string_compare(dir->d_name,"..")!=0){
               int result = serialize_file( start_depth, file_size(path_buf));
               if (result == -1){
                return -1;
               }
            }

            serialize_directory(start_depth);
            }
            else {


                putchar(0x0C);
                putchar(0x0D);
                putchar(0xED);
                putchar(0x04);

                int entry_depth =  depth +1;
                unsigned char entry_first = (unsigned char)(entry_depth >> 24);
                unsigned char entry_second = (unsigned char)(entry_depth >> 16);
                unsigned char entry_thrid = (unsigned char)(entry_depth >>  8);
                unsigned char entry_fourth = (unsigned char)(entry_depth >>  0);

                putchar(entry_first);
                putchar(entry_second);
                putchar(entry_thrid);
                putchar(entry_fourth);



                long header_size;
                header_size = 12+16+array_size(dir->d_name);
                unsigned char one_header = (unsigned char)(header_size >> 56);
                unsigned char two_header = (unsigned char)(header_size >> 48);
                unsigned char three_header = (unsigned char)(header_size >>  40);
                unsigned char four_header = (unsigned char)(header_size >>  32);
                unsigned char five_header = (unsigned char)(header_size >> 24);
                unsigned char six_header = (unsigned char)(header_size >> 16);
                unsigned char seven_header = (unsigned char)(header_size >>  8);
                unsigned char eight_header = (unsigned char)(header_size >>  0);

                putchar(one_header);
                putchar(two_header);
                putchar(three_header);
                putchar(four_header);
                putchar(five_header);
                putchar(six_header);
                putchar(seven_header);
                putchar(eight_header);



                long size;
                int type;
                size = file_size(path_buf);
                type = file_mode(path_buf);

                unsigned char one_type = (unsigned char)(type >> 24);
                unsigned char two_type = (unsigned char)(type >> 16);
                unsigned char three_type = (unsigned char)(type >>  8);
                unsigned char four_type = (unsigned char)(type >>  0);

                putchar(one_type);
                putchar(two_type);
                putchar(three_type);
                putchar(four_type);


                unsigned char one = (unsigned char)(size >> 56);
                unsigned char two = (unsigned char)(size >> 48);
                unsigned char three = (unsigned char)(size >>  40);
                unsigned char four = (unsigned char)(size >>  32);
                unsigned char five = (unsigned char)(size >> 24);
                unsigned char six = (unsigned char)(size >> 16);
                unsigned char seven = (unsigned char)(size >>  8);
                unsigned char eight = (unsigned char)(size >>  0);


                putchar(one);
                putchar(two);
                putchar(three);
                putchar(four);
                putchar(five);
                putchar(six);
                putchar(seven);
                putchar(eight);



                int counter;
                char *name_pointer;
                name_pointer = dir->d_name;
                for(counter = 0;counter<array_size(dir->d_name);counter++){
                    putchar(*name_pointer);
                    name_pointer++;
                }



             if(is_file(path_buf) && string_compare(dir->d_name,".")!=0 && string_compare(dir->d_name,"..")!=0){
               int result = serialize_file( start_depth, file_size(path_buf));
               if (result == -1){
                return -1;
               }
            }
            }


            }

            path_pop();

        }
        closedir(d);




//////   end


    putchar(0x0C); putchar(0x0D); putchar(0xED); putchar(0x03);
    putchar(first); putchar(second); putchar(thrid); putchar(fourth);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x00);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x10);


    return 0;
}

/*
 * @brief  Serialize the contents of a file as a single record written to the
 * standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * file to be serialized.  It serializes the contents of that file as a single
 * FILE_DATA record emitted to the standard output.
 *
 * @param depth  The value to be used in the depth field of the FILE_DATA record.
 * @param size  The number of bytes of data in the file to be serialized.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open the file, too many or not enough data bytes read
 * from the file, and I/O errors reading the file data or writing to standard output.
 */
int serialize_file(int depth, off_t size) {




    FILE *f;
    long long max;
    long long i;

    char type = 5;

    putchar(0x0C);
    putchar(0x0D);
    putchar(0xED);
    putchar(type);

    long real_size = 16+size;
    unsigned char first = (unsigned char)(depth >> 24);
    unsigned char second = (unsigned char)(depth >> 16);
    unsigned char thrid = (unsigned char)(depth >>  8);
    unsigned char fourth = (unsigned char)(depth >>  0);
    putchar(first);
    putchar(second);
    putchar(thrid);
    putchar(fourth);

    unsigned char one = (unsigned char)(real_size >> 56);
    unsigned char two = (unsigned char)(real_size >> 48);
    unsigned char three = (unsigned char)(real_size >>  40);
    unsigned char four = (unsigned char)(real_size >>  32);
    unsigned char five = (unsigned char)(real_size >> 24);
    unsigned char six = (unsigned char)(real_size >> 16);
    unsigned char seven = (unsigned char)(real_size >>  8);
    unsigned char eight = (unsigned char)(real_size >>  0);

    putchar(one);
    putchar(two);
    putchar(three);
    putchar(four);
    putchar(five);
    putchar(six);
    putchar(seven);
    putchar(eight);



    f = fopen(path_buf, "r");

    if(f==NULL){
        return -1;
    }
    else{
    for (i = 0, max = size; i < max;  i++) {
        putchar(fgetc(f));

    }

    fclose(f);
    return 0;
    }



}

/**
 * @brief Serializes a tree of files and directories, writes
 * serialized data to standard output.
 * @details This function assumes path_buf has been initialized with the pathname
 * of a directory whose contents are to be serialized.  It traverses the tree of
 * files and directories contained in this directory (not including the directory
 * itself) and it emits on the standard output a sequence of bytes from which the
 * tree can be reconstructed.  Options that modify the behavior are obtained from
 * the global_options variable.
 *
 * @return 0 if serialization completes without error, -1 if an error occurs.
 */
int serialize() {

    putchar(0x0C); putchar(0x0D); putchar(0xED); putchar(0x00);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x00);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x00);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x10);

    int result =  serialize_directory(0);

    putchar(0x0C); putchar(0x0D); putchar(0xED); putchar(0x01);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x00);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x00);
    putchar(0x00); putchar(0x00); putchar(0x00); putchar(0x10);

    if (result==0){
        return 0;
    }
    else{
    return -1;
    }
}

/**
 * @brief Reads serialized data from the standard input and reconstructs from it
 * a tree of files and directories.
 * @details  This function assumes path_buf has been initialized with the pathname
 * of a directory into which a tree of files and directories is to be placed.
 * If the directory does not already exist, it is created.  The function then reads
 * from from the standard input a sequence of bytes that represent a serialized tree
 * of files and directories in the format written by serialize() and it reconstructs
 * the tree within the specified directory.  Options that modify the behavior are
 * obtained from the global_options variable.
 *
 * @return 0 if deserialization completes without error, -1 if an error occurs.
 */
int deserialize() {
    // To be implemented.
           int result =  deserialize_directory(0);
           if(result ==0){
            return 0;
           }
           else{

    return -1;
}
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{





    if(argc == 1){
        return -1;
    }
    else if(string_compare(*(argv+1),"-h") == 0) {

        global_options = global_options | (1<<0);
        return 0;
    }
    else if(string_compare(*(argv+1),"-s") == 0){
        if(argc == 2){
        global_options = global_options | (1<<1);
        path_init(".");

            //////////////////////////


////////////////////////////////
            return 0;
        }
        else if (argc ==4){
            if(string_compare(*(argv+2),"-p") == 0){
                if(check_dir_exist(*(argv+3))==0){
                    global_options = global_options | (1<<1);
                    path_init(*(argv+3));
                    return 0;
                }
                else{
                    return -1;
                }
            }
            else{
                return -1;
            }
        }
        else{
            return -1;
        }
    }
    else if(string_compare(*(argv+1),"-d") == 0){
        if(argc == 2){
            global_options = global_options | (1<<2);

            /////////////////////////////////////////

            path_init(".");

            //////////////////////////////////////////
            return 0;
        }
        else if (argc ==3 && string_compare(*(argv+2),"-c") == 0) // ./ -d -c
        {
            global_options = global_options | (1<<2);
            global_options = global_options | (1<<3);
            path_init(".");
            return 0;
        }
        else if (argc ==4){
            if(string_compare(*(argv+2),"-p") == 0 && check_dir_exist(*(argv+3))==0) // ./ -d -p dir
            {
                global_options = global_options | (1<<2);
                path_init(*(argv+3));
                return 0;
            }
            else{
                return -1;
            }
        }
        else if (argc == 5 && string_compare(*(argv+2),"-c") == 0 && string_compare(*(argv+3),"-p") == 0 && check_dir_exist(*(argv+4))==0){
            global_options = global_options | (1<<2);
            global_options = global_options | (1<<3);
            path_init(*(argv+4));
            return 0;
        }
        else if (argc == 5 && string_compare(*(argv+4),"-c") == 0 && string_compare(*(argv+2),"-p") == 0 && check_dir_exist(*(argv+3))==0){
            global_options = global_options | (1<<2);
            global_options = global_options | (1<<3);
            path_init(*(argv+3));
            return 0;
        }
        else{
            return -1;
        }

    }

    // To be implemented.
    return -1;
}
