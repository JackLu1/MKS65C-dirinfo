#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

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
    struct dirent *d;

    int dir_count = 0;
    int file_count = 0;
    while (d = readdir(dir_stream))
    {
        if (d->d_type == DT_REG)
        {
            file_count++;
        }
        if (d->d_type == DT_DIR)
        {
            dir_count++;
        }
    }

    char **dirs = malloc(dir_count * sizeof(char *));
    char **files = malloc(file_count * sizeof(char *));
    
    closedir(dir_stream);
    dir_stream = opendir(dirname);
    
    int i = 0;
    int j = 0;
    while (d = readdir(dir_stream))
    {
        if (d->d_type == DT_REG)
        {
            files[i++] = d->d_name;
        }
        if (d->d_type == DT_DIR)
        {
            dirs[j++] = d->d_name;
        }
    }

    printf("[[[Directories]]]\n");
    for (i = 0; i < dir_count; i++)
    {
        printf("%s\n", dirs[i]);
    }

    printf("[[[Files]]]\n");
    for (i = 0; i < file_count; i++)
    {
        printf("%s\n", files[i]);
    }

    return 0;
}
