#!/usr/bin/env python3
from comment_parser import comment_parser
import os, sys, argparse

def lint():
    spaces, group = 0, []
    try:
        with open(file, 'r') as f:
            lines = [list(line) for line in f]
            com = types[os.path.splitext(file)[1]][0]
            mime = types[os.path.splitext(file)[1]][1]
    except FileNotFoundError:
        sys.exit('error: \'{}\' does not exist in directory.'.format(file))

    # loop all lines in file
    for i,line in enumerate(lines):
        try:
            if comment_parser.extract_comments_from_str(''.join(line), mime=mime):
                index = ''.join(line).rindex(com)          # index of comment identifier
                before = ''.join(line).rsplit(com, 1)[0]   # all characters before comment
                if before.count(' ') == len(before):
                    continue
                group.append((i, index))    # group i,index (line number, comment index)
                spaces = 0
            else:
                spaces += 1
                if spaces == 2 and len(group) != 0:     # ignore empty groups
                    _max = max([x[1] for x in group])   # furthest most comment in group
                    for x,y in enumerate(group):
                        if y[1] != _max:
                            lines[y[0]].insert(y[1], ' ' * int(_max - y[1]))    # insert ' 's into line
                    spaces, group = 0, []
        except Exception as e:
            # couldnt tokenize line, something like split up dict or parentheses. or something wrong with multiline comment in C/C++ I think.
            pass

    lines = ''.join([''.join(i) for i in lines])
    with open(output, 'w') as f:
        f.write(lines)
        
if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    opt = ap._action_groups.pop()
    req = ap.add_argument_group('required arguments')
    req.add_argument('-i','--input',
            help='path to input file')
    opt.add_argument('-o','--output',
            help='path to optional output file')
    ap._action_groups.append(opt)
    args = vars(ap.parse_args())

    file = output = args['input']
    if args['output']: output = args['output']
    types = {'.py': ['#', 'text/x-python'], '.c': ['//', 'text/x-c'], '.cpp': ['//', 'text/x-c++'], '.cc': ['//', 'text/x-c++'], '.ruby': ['#', 'text/x-ruby'], '.go': ['//', 'text/x-go'], '.js': ['//', 'application/javascript']}
    lint()