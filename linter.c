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
    int comments = 0, linenum = 1;
    size_t len = 0;
    ssize_t read;

    struct groups group_indexes = get_groups(argv[1]);
    // int groups = sizeof(group_indexes.groups)/sizeof(group_indexes.far[0])+1;
    int counter = 0;
    int skip;
    char outname[] = "fmt-ross.c";
    // printf("%d", group_indexes.groups);
    // for (int i = 0; i < groups+1; i++) {
    //     printf("%d ", group_indexes.lines[i]);
    // }
    // printf("\n");
    fpOut = fopen(outname, "w");
    
    if ((fp = fopen(argv[1], "r")) != NULL) {
        while ((read = getline(&line, &len, fp)) != -1) {
            // printf("Retrieved line of length %zu:\n", read);
            strcpy(linecpy, line);
            skip = 0;
            if ((c = strchr(line, '/')) != NULL) {
                int index = (int)(c - line);
                int next = index + 1;
                if (line[next] == '/') {
                    memcpy(sub, line, index);
                    if (not_blank(sub, index)) {
                        // printf("current LINE: %d current COUNTER: %d\n", linenum, group_indexes.lines[counter]);
                        int dif = group_indexes.far[counter] - index;
                        append_spaces(linecpy, index, index+dif);
                        fputs(linecpy, fpOut);
                        skip = 1;
                        // printf("Index of comment: %d on line %d, Before comment: \"%s\"\n", index, linenum, sub);
                        // comments++;
                    }
                    // memset(linecpy, 0, sizeof(linecpy));
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
        // printf("%d comments in %s", comments, argv[1]);
    } else {
        exit(1);
    }
}