#!/usr/bin/env python3
from comment_parser import comment_parser
import os, sys, argparse

def lint():
    global spaces,group

    try:
        with open(file, 'r') as f:
            lines = [list(line) for line in f]
    except FileNotFoundError:
        sys.exit('error: \'{}\' does not exist in directory.'.format(file))

    # loop all lines in file
    for i,line in enumerate(lines):
        try:
            if comment_parser.extract_comments_from_str(''.join(line), mime=mime):
                getComment(i, line)
            else:
                if com in ''.join(line):
                    getComment(i, line)
                else:
                    spaces += 1
                    if spaces == 2 and len(group) != 0:     # ignore empty groups
                        _max = max([x[1] for x in group])   # furthest most comment in group
                        for x,y in enumerate(group):
                            if y[1] != _max:
                                lines[y[0]].insert(y[1], ' ' * int(_max - y[1]))    # insert ' 's into line
                        spaces, group = 0, []
        except Exception as e:
            pass
            # print("error: line {}:".format(i), e)
            # NOTE: couldnt tokenize line, something like split up dict or parentheses. or something wrong with multiline comment in C/C++ I think. Or plain text file and comment in search is not "#".

    lines = ''.join([''.join(i) for i in lines])
    with open(output, 'w') as f:
        f.write(lines)

def getComment(i, line):
    global spaces,group

    index = ''.join(line).rindex(com)          # index of comment identifier
    before = ''.join(line).rsplit(com, 1)[0]   # all characters before comment
    if before.count(' ') == len(before):
        return
    group.append((i, index))    # group i,index (line number, comment index)
    spaces = 0

if __name__ == '__main__':
    ap = argparse.ArgumentParser(description="Comment \"Linter\"")
    opt = ap._action_groups.pop()
    req = ap.add_argument_group('required arguments')
    req.add_argument('-i','--input',
            help='path to input file', required=True)
    opt.add_argument('-s','--symbol',
            help='optional symbol to provide if no extension present')
    opt.add_argument('-o','--output',
            help='optional path to output file')
    ap._action_groups.append(opt)
    args = vars(ap.parse_args())

    types = {'.py': ['#', 'text/x-python'], '.c': ['//', 'text/x-c'], '.cpp': ['//', 'text/x-c++'], '.cc': ['//', 'text/x-c++'], '.ruby': ['#', 'text/x-ruby'], '.go': ['//', 'text/x-go'], '.js': ['//', 'application/javascript'], '.sh': ['#', 'text/x-shellscript']}
    file = output = args['input']
    name,ext = os.path.splitext(file)
    if args['output']: output = args['output']

    if ext == '' and not args['symbol']:
        sys.exit("usage: linter.py [-h] [-i INPUT] [-s SYMBOL] [-o OUTPUT]\nlinter.py: error: argument -s/--symbol: expected symbol for file without extension: '{}'".format(name))
    elif ext != '' and args['symbol']:
        sys.exit("usage: linter.py [-h] [-i INPUT] [-s SYMBOL] [-o OUTPUT]\nlinter.py: error: argument -s/--symbol: unexpected symbol for file already with extension: '{}'".format(file))
    elif ext == '' and args['symbol']:
        com = args['symbol']
        mime = "text/x-shellscript"
        # NOTE: With a plain text file, using the shellscript mimetype works to properly detect the comments, IF they are the shellscript comment "#"
        # NOTE: Definitely possible part of a line that is not a comment can be detected and moved, since in this case its not detecting proper comments in the line, just the starting character that implies a comment
    else:
        com = types[ext][0]
        mime = types[ext][1]
    
    # print(file, ext, mime, com, output)
    spaces, group = 0, []
    lint()