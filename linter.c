#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

int main(int argc, char *argv[]) {
    FILE *fp;
    FILE *fpOut;
    char *line = NULL;
    char *c;
    char sub[200];
    char linecpy[200];
    int comments = 0, linenum = 1, counter = 0;
    size_t len = 0;
    ssize_t read;

    char filename[64];
    char outname[64] = "linted-";
    strncpy(filename, argv[1], 64);
    strcat(outname, filename);
    struct groups group_indexes = get_groups(filename);
    int skip, ret;

    fpOut = fopen(outname, "w");
    
    if ((fp = fopen(argv[1], "r")) != NULL) {
        while ((read = getline(&line, &len, fp)) != -1) {
            strcpy(linecpy, line);
            skip = 0;
            if ((c = strchr(line, '/')) != NULL) {
                int index = (int)(c - line);
                int next = index + 1;
                if (line[next] == '/') {
                    memcpy(sub, line, index);
                    if (not_blank(sub, index)) {
                        int spaces = group_indexes.far[counter] - index;
                        if (spaces > 0) {
                            insert_spaces(linecpy, index, spaces);
                        }
                        fputs(linecpy, fpOut);
                        skip = 1;
                    }
                }
            }
            if (skip == 0) {
                fputs(linecpy, fpOut);
            }
            memset(sub, 0, sizeof(sub));
            memset(linecpy, 0, sizeof(linecpy));
            linenum++;
            if (linenum >= group_indexes.lines[counter]) {
                counter++;
            }
        }
        fclose(fp);
        fclose(fpOut);
        if ((ret = rename(outname, filename)) != 0) {
            printf("err: unable to rename the file\n");
        }
    } else {
        exit(1);
    }
}