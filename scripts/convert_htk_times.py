#!/usr/bin/python
import sys
lab_file = sys.stdin
lab_out = sys.stdout

htk_const = 10000000

for ln in lab_file:
    ln = ln.rstrip('\r\n')
    ln_info = ln.split(' ')
    start_time = float(ln_info[0])/htk_const
    end_time = float(ln_info[1])/htk_const
    if len(ln_info)>3:
        print >> lab_out, "{0} {1} {2}".format(start_time, end_time, " ".join(ln_info[2]))
    else:
        print >> lab_out, "{0} {1} {2}".format(start_time, end_time, ln_info[2])
