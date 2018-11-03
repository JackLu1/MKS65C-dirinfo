#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * struct dir_file contains information about files in a directory.
 *
 * It contains:
 *  dir_count, number of directories in the directory
 *  dir_list, an array containing names of directories
 *  file_count, number of files in the directory
 *  file_list, an array containing names of other files
 */
struct dir_files
{
    int dir_count;
    char** dir_list;
    int file_count;
    char** file_list;
};

struct dir_files *read_files(DIR* dir_stream)
{
    /*
     * Reads files in dir_stream and returns a pointer to struct dir_file.
     */
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
    struct dir_files *dir = malloc(sizeof(struct dir_files) + sizeof(char *) * (dir_count + file_count));
    dir->dir_count = dir_count;
    dir->dir_list = malloc(sizeof(char *) * dir_count);
    dir->file_count = file_count;
    dir->file_list = malloc(sizeof(char *) * file_count);
    rewinddir(dir_stream);
    int i = 0; int j = 0; int len;
    while (f = readdir(dir_stream))
    {
        len = strlen(f->d_name);
        if (f->d_type == DT_DIR)
        {
            dir->dir_list[j] = malloc(sizeof(char *) * len);
            strcpy(dir->dir_list[j], f->d_name);
            j++;
        }
        else
        {
            dir->file_list[i] = malloc(sizeof(char *) * len);
            strcpy(dir->file_list[i], f->d_name);
            i++;
        }
    }
    return dir;
}

void print_dir(struct dir_files *d)
{
    /*
     * Print all files in struct dir_files
     */
    int i;
    printf("\x1b[34;1m"); // Print text in blue
    for (i = 0; i < d->dir_count; i++)
    {
        printf("%s\n", d->dir_list[i]);
    }
    printf("\x1b[0m"); // Reset color
    for (i = 0; i < d->file_count; i++)
    {
        printf("%s\n", d->file_list[i]);
    }
}

void free_dir_files(struct dir_files *d)
{
    int i;
    for (i = 0; i < d->dir_count; i++)
    {
        free(d->dir_list[i]);
    }
    for (i = 0; i < d->file_count; i++)
    {
        free(d->file_list[i]);
    }
    free(d->dir_list);
    free(d->file_list);
    free(d);
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
    struct dir_files *d = read_files(dir_stream);
    closedir(dir_stream);
    print_dir(d);
    free_dir_files(d);
    return 0;
}
