#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

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

int main(int argc, char** argv)
{
    char *dirname;
    if (argc < 2)
    {
        dirname = ".";
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
    printf("\x1b[34;1m"); // Print text in blue
    printf("\nDirectories:\n");
    for (i = 0; i < dir_count; i++)
    {
        stat(dir_list[i], s); 
        size += s->st_size;

        printf("%s\n", dir_list[i]);
        free(dir_list[i]);
    }
    printf("\x1b[0m"); // Reset color
    printf("\nRegular Files:\n");
    for (i = 0; i < file_count; i++)
    {
        stat(file_list[i], s); 
        size += s->st_size;

        printf("%s\n", file_list[i]);
        free(file_list[i]);
    }

    printf("\nTotal Directory Size: %d Bytes\n", size);
    printf("size of stat.c is %d bytes, %f KB, %f MB, %f GB\n", 
            size, size/1000., size/1000000., size/1000000000. );

    free(dir_list);
    free(file_list);
    return 0;
}
