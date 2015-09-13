#!/usr/bin/python
import sys
import codecs

orig_dico_file = sys.argv[1]
phonemap_file = sys.argv[2]
out_dico = sys.stdout

phonemap = codecs.open(phonemap_file, encoding='utf-8')
phonedict = {}
for ln in phonemap:
    ln = ln.rstrip('\r\n')
    ln_phones = ln.split()

    if len(ln_phones)==2:
        phonedict[ln_phones[0]] = ln_phones[1]
    else:
        phonedict[ln_phones[0]] = 'null'

phonemap.close()

orig_dico = codecs.open(orig_dico_file, encoding='utf-8')
for ln in orig_dico:
    ln = ln.rstrip('\r\n')
    ln_info = ln.split()
    ln_word = ln_info[0]
    print >> out_dico, ln_word.encode("utf-8"),
    for ph in list(ln_info[1]):
        if phonedict[ph]!='null':
            print >> out_dico, " "+phonedict[ph].encode("utf-8"),

    print >> out_dico, " "

orig_dico.close()
