#!/usr/bin/python
import sys
import codecs
from collections import defaultdict

input_dic_file = sys.argv[1]

input_dic = codecs.open(input_dic_file, encoding='utf-8')
phone_insts = defaultdict(int)
for ln in input_dic:
    ln = ln.rstrip('\r\n')
    pron = ln.split()[1]
    pron_phones = list(pron)
    for ph in pron_phones:
        phone_insts[ph] += 1

input_dic.close()

print "\n".join(phone_insts.keys()).encode('utf-8')
