#!/usr/bin/env python
# -*- coding: utf-8 -*-


from __future__ import division
from pync import Notifier
import urllib2
import json
import datetime

result = urllib2.urlopen("https://home.micbase.com:11443/get.php").read()
data = json.loads(result)
if data['valid'] == "1":
    d = u'\N{DEGREE SIGN}'
    content = "Room Temperature is %.1f%sF, Humidity is %s%%.\nLast Updated: %s" % \
    (int(data['temperature']) * 9 / 5 + 32, d, data['humidity'], datetime.datetime.fromtimestamp(float(data['update_time'])))
    Notifier.notify(content, title='Smart Home')
else:
    Notifier.notify("No Valid Data", title='Smart Home')
