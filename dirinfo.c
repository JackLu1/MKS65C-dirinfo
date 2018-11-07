#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int cmpstr(const void *a, const void *b)
{
    /*
     * For use with qsort(), since
     * qsort requires the function to have const void * as arguments.
     *
     * Casts the void pointers to a pointer to an array of char pointers
     * (which is the original types of a and b),
     * and dereferences that pointer to get the strings and pass it into strcmp. 
     */
    return strcmp(*(char **)a, *(char **) b);
}

char * format(char * name, struct stat * s, struct passwd * pw, struct group * gr, char * p){
    char * owner = malloc(32); 
    char * group = malloc(32); 
    char * time = malloc(100);
    char * ll = malloc(60);

    owner = pw->pw_name;
    group = gr->gr_name;
    strftime(time, 20, "%m %d %H:%M", localtime(&(s->st_mtime)));
    sprintf(ll, "%s %-2ld %s %s %10ld %s %s", p, s->st_nlink, owner, group, s->st_size, time, name);
    return ll;
}

int main(int argc, char** argv)
{
    char *dirname = malloc(100);
    if (argc < 2)
    {
        printf("Enter directory path:\n");
        scanf("%s", dirname);
    }
    else
    {
        dirname = argv[1];
    }
    DIR *dir_stream = opendir(dirname);
    if (dir_stream == NULL)
    {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    struct dirent *f;
    int dir_count = 0; int file_count = 0;
    while (f = readdir(dir_stream))
    {
        if (f->d_type == DT_DIR)
        {
            dir_count++;
        }
        else
        {
            file_count++;
        }
    }
    char **dir_list = malloc(sizeof(char *) * dir_count);
    char **file_list = malloc(sizeof(char *) * file_count);
    rewinddir(dir_stream);
    int i = 0; int j = 0; int len;
    while (f = readdir(dir_stream))
    {
        len = strlen(f->d_name);
        if (f->d_type == DT_DIR)
        {
            dir_list[j] = malloc(sizeof(char *) * len);
            strcpy(dir_list[j], f->d_name);
            j++;
        }
        else
        {
            file_list[i] = malloc(sizeof(char *) * len);
            strcpy(file_list[i], f->d_name);
            i++;
        }
    }
    closedir(dir_stream);

    qsort(dir_list, dir_count, sizeof(char *), cmpstr);
    qsort(file_list, file_count, sizeof(char *), cmpstr);



    int size = 0;

    struct stat *s = malloc(sizeof(struct stat));
    struct passwd *pw = malloc(sizeof(struct passwd));
    struct group  *gr = malloc(sizeof(struct group));
    char * perm = malloc(100);
    int p;
    char * rwx[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    char * info = malloc(20);

    printf("\x1b[34;1m"); // Print text in blue
    for (i = 0; i < dir_count; i++)
    {
        stat(dir_list[i], s); 
        size += s->st_size;
        pw = getpwuid(s->st_uid);
        gr = getgrgid(s->st_gid);
        p = s->st_mode % 01000;

        sprintf(perm, "d%s%s%s", rwx[p >> 6 & 7], rwx[p >> 3 & 7], rwx[p & 7]);
        info = format(dir_list[i], s, pw, gr, perm);
        printf("%s\n", info);
    }
    printf("\x1b[0m"); // Reset color
    for (i = 0; i < file_count; i++)
    {
        stat(file_list[i], s); 
        size += s->st_size;

        pw = getpwuid(s->st_uid);
        gr = getgrgid(s->st_gid);
        p = s->st_mode % 01000;
        sprintf(perm, "-%s%s%s", rwx[p >> 6 & 7], rwx[p >> 3 & 7], rwx[p & 7]);
        info = format(file_list[i], s, pw, gr, perm);
        printf("%s\n", info);
        

        free(file_list[i]);
    }

    printf("\nTotal Directory Size: %d Bytes\n", size);
    printf("size of stat.c is %d bytes, %f KB, %f MB, %f GB\n", 
            size, size/1000., size/1000000., size/1000000000. );

    free(dir_list);
    free(file_list);
    return 0;
}
