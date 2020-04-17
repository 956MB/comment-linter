struct groups {
    int groups;
    int lines[100];
    int far[100];
};

int not_blank(char *arr, int size) {
    // Loops chars in string, if all blanks, return 0
    for (size_t i = 0; i < size; i++) {
        if (arr[i] != ' ') {
            return 1;
        }
    }
    return 0;
}

struct groups get_groups(char filename[]) {
    FILE *fp;
    char *line = NULL;
    char *c;
    char sub[200];
    int groups = 0, g = 0, spaces = 0, linenum = 1;
    int far;
    // static int groups_far[100];
    size_t len = 0;
    ssize_t read;
    struct groups grps;
    grps.groups = 0;

    // get groups of comments and far comment, first file read
    if ((fp = fopen(filename, "r")) != NULL) {
        while ((read = getline(&line, &len, fp)) != -1) {
            if ((c = strchr(line, '/')) != NULL) {
                int index = (int)(c - line);
                int next = index + 1;
                if (line[next] == '/') {
                    memcpy(sub, line, index);
                    if (not_blank(sub, index)) {
                        if (index > far) {
                            far = index;
                        }
                        spaces = 0;
                    } else {
                        spaces++;
                    }
                    memset(sub, 0, sizeof(sub));
                } else {
                    spaces++;
                }
            } else {
                spaces++;
            }
            if (spaces == 2) {
                if (far > 0) {
                    grps.far[g] = far;
                    grps.lines[g] = linenum;
                    grps.groups++;
                    g++;
                }
                far = 0;
                spaces = 0;
            }
            linenum++;
        }
        fclose(fp);
        return grps;
    }
}

void append_spaces(char subj[], int pos, int longest) {
    // TODO: Fix bug inserting garbage into updated line. think its something to do with the allocated space in buf
    int spaces = longest-pos;
    char insert[spaces];
    for (int i=0; i<spaces; i++) {
        insert[i] = ' ';
    }
    char buf[100] = {};
    memset(buf, 0, 100);

    strncpy(buf, subj, pos);   // copy at most first pos characters
    int len = strlen(buf);
    strcpy(buf+len, insert);   // copy all of insert[] at the end
    len += strlen(insert);     // increase the length by length of insert[]
    strcpy(buf+len, subj+pos); // copy the rest

    strcpy(subj, buf);         // copy it back to subj
}