#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>


typedef enum
{
    NOT_EXIST=1,
    DIRECTORY,
    FILES
}FILE_ENUM;

FILE_ENUM check_file(char*var,struct stat* st);
int cp_file(char*src_var,char*dest_var,struct stat *st);
int cp_dir(char* src,char*dest);
int main(int argc, char *argv[])
{
    if( 3 != argc)
    {
        printf("input <source path> <destnation path>!");
        exit(-1);
    }

    struct stat src_stat;
	struct stat dest_stat;
    if( FILES == check_file(argv[1], &src_stat) )
    {
        FILE_ENUM dest_enum;
        dest_enum = check_file( argv[2], &dest_stat );

        char cmd[100] = "";
        char ch;

        switch(dest_enum)
        {
            case NOT_EXIST:
                sprintf(cmd, "touch %s", argv[2]);
                system( cmd );

                check_file( argv[2], &dest_stat );
                cp_file(argv[1], argv[2], &dest_stat);

                break;

            case DIRECTORY:
                cp_file(argv[1], argv[2], &dest_stat);
                break;

            case FILES:
                fprintf(stdout, "Overwrite the dest file %s, [y/n]\n", argv[2]);
                ch = getchar();

                if( ch == 'Y' || ch == 'y' )
                {
                    cp_file(argv[1], argv[2], &dest_stat);
                }
                else
                    exit(0);

                break;
            default:
                fprintf(stderr, "%s(%d): file type error\n", __FILE__, __LINE__);
        }
    }

    else if( DIRECTORY == check_file(argv[1], &dest_stat) )
    {
        FILE_ENUM dest_enum;
        dest_enum = check_file( argv[2], &dest_stat );

        char cmd[100] = "";

        switch(dest_enum)
        {
            case NOT_EXIST:
                sprintf(cmd, "mkdir -p %s", argv[2]);
                system( cmd );
                cp_dir(argv[1], argv[2] );
                break;

            case DIRECTORY:
                cp_dir(argv[1], argv[2]);
                break;

            case FILES:
                fprintf(stderr, "Can't copy a directory to a file\n");
                exit(EXIT_FAILURE);
                break;

            default:
                fprintf(stderr, "%s(%d): file type error\n", __FILE__, __LINE__);
                break;
        }
    }

    return 0;
}

FILE_ENUM check_file(char *var, struct stat *st)
{
    if( stat(var, st) )    
    {
        if( ENOENT == errno)    
        {
            return NOT_EXIST;
        }
        else
        {
            perror("stat");
            exit(EXIT_FAILURE);
        }
    }

    else    
    {
        
        if( S_ISDIR(st->st_mode ))  
            return DIRECTORY;
        else if( S_ISREG(st->st_mode) )
            return FILES;
        else
        {
            fprintf(stderr, "%s(%d):file type error", __FILE__ , __LINE__);
            exit(EXIT_FAILURE);
        }
    }
}

int cp_file(char *src_var, char *dest_var, struct stat *st)
{
    FILE *src = NULL;
    FILE *dest = NULL;

    if( S_ISREG(st->st_mode) )  
    {
        if( NULL == (src = fopen(src_var, "r")) )
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        if( NULL == (dest = fopen(dest_var, "w+")) )
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        char buf[1024];
        int num;

        while(1)
        {
        
            if( 1024 != (num = fread(buf, 1,1024, src)))
            {
                if( !feof(src))
                {
                    perror("fread");
                    exit(EXIT_FAILURE);
                }

                else
                {
                    fwrite(buf, 1, num, dest);
                    fclose(dest);   
                    break;
                }
            }
            fwrite(buf, 1, 1024, dest);

        }

        fclose(src);
        return 0;
    }

    if( S_ISDIR(st->st_mode) )
    {
        char buf[100]="";
        strncpy(buf, dest_var, sizeof(dest_var));
        strcat(buf, src_var);
        char cmd[100]="";
        sprintf(cmd, "touch %s",buf);
        system(cmd);

        struct stat new_dest_stat;

        if( stat(buf, &new_dest_stat))
        {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        cp_file(src_var, buf, &new_dest_stat);
    }

    return 0;
}

int cp_dir(char *src, char *dest)
{
    DIR *dirp = NULL;
    if( NULL == (dirp = opendir(src)) )
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entp = NULL;
    while( NULL != (entp = readdir(dirp)))      
    {
        if( 0 == (strcmp(entp->d_name,"..")) || 0 == (strcmp(entp->d_name, ".")))
        {
            continue;
        }

        char src_buf[100] = "";
        char dest_buf[100] = "";

        sprintf(src_buf, "%s/%s\0", src, entp->d_name);
        sprintf(dest_buf, "%s/%s\0", dest, entp->d_name); 


        struct stat src_stat;

        if( stat(src_buf,&src_stat) )
        {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if( S_ISREG(src_stat.st_mode) )
        {
            cp_file(src_buf, dest_buf, &src_stat);
        }

        else if( S_ISDIR(src_stat.st_mode) )
        {
            if( -1 == mkdir(dest_buf, src_stat.st_mode) )
            {
                perror("mkdir");
                exit(EXIT_FAILURE);
            }

            cp_dir(src_buf, dest_buf);  
        }
    }

    return 0;
}


