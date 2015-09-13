#!/usr/bin/python
import sys
import codecs
from suds.client import Client

input_file = sys.argv[1]
conversionString = codecs.open(input_file,encoding='utf-8')

# path to the web service wsdl
wsdl = "http://dl.ilsp.gr/UP/WS/service.php?class=grTOph&wsdl"
my_options = dict(actor='http://schema.jpapg.gr', trace=True )

# initialize soap client
client = Client(wsdl)
client.set_options(soapheaders=my_options)

for ln in conversionString:
    ln = ln.rstrip('\r\n')

    # call public method
    res = client.service.Convert(ln)
    res.rstrip('\r\n')
    print "{} {}".format(ln.encode("utf-8"),res.encode("utf-8")),

conversionString.close()
