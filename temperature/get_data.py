#!/usr/bin/python

from __future__ import division
import subprocess
import urllib2


def cel2fah(temp):
    return temp * 9 / 5 + 32


def fah2cel(temp):
    return (temp - 32) * 5 / 9


def cal_heat_index(temp, hum):
    if (temp > 26 and hum > 40):
        temp = cel2fah(temp)
        hum = hum
        c1 = -42.379
        c2 = 2.04901523
        c3 = 10.14333127
        c4 = -0.22475541
        c5 = -6.83783 * pow(10, -3)
        c6 = -5.481717 * pow(10, -2)
        c7 = 1.22874 * pow(10, -3)
        c8 = 8.5282 * pow(10, -4)
        c9 = -1.99 * pow(10, -6)
        index = c1 + c2 * temp + c3 * hum + c4 * temp * hum + c5 * pow(temp, 2) +\
                c6 * pow(hum, 2) + c7 * pow(temp, 2) * hum + c8 * temp * pow(hum, 2) +\
                c9 * pow(temp, 2) * pow(hum, 2)
        return fah2cel(index)
    else:
        return 0


def main():
    for i in range(0, 5):
        try:
            output = subprocess.check_output("/home/pi/smarthome/Adafruit_DHT")
            data = output.split(',')
            if len(data) == 2:
                if int(data[0]) < 0 or int(data[0]) > 50 or int(data[1]) < 20 or int(data[1]) > 90:
                    continue
                else:
                    heat = cal_heat_index(int(data[0]), int(data[1]))
                    url = "https://home.micbase.com:11443/store.php?valid=1&tmp=" +\
                            data[0] + "&hum=" + data[1] + "&heat=" + str(heat)
                    urllib2.urlopen(url).read()

                    url_open = 'http://localhost/control.php?cmd=103'
                    url_close = 'http://localhost/control.php?cmd=104'

                    if data[0] >= 27:
                        urllib2.urlopen(url_open).read()
                    else:
                        urllib2.urlopen(url_close).read()

                    return

        except Exception:
            pass

    url = "https://home.micbase.com:11443/store.php?valid=0&tmp=0&hum=0&heat=0"
    urllib2.urlopen(url).read()


if __name__ == '__main__':
    main()
