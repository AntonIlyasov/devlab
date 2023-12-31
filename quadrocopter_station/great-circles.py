# -*- coding: utf-8 -*-
#!/usr/bin/env python

# ---------------------------------------------------------------------------
# great-circles.py
# Расчет длины большого круга и начального азимута используя полное уравнение.
# More info: http://gis-lab.info/qa/great-circles.html
# Date created: 04.07.2010
# Last updated: 11.08.2010
# ---------------------------------------------------------------------------

import math

# #pi - число pi, rad - радиус сферы (Земли)
rad = 6372795

# #координаты двух точек
# llat1 = 60.028729
# llong1 = 30.257502

# llat2 = 86.2
# llong2 = 170.4

# #в радианах
# lat1 = llat1*math.pi/180.
# lat2 = llat2*math.pi/180.
# long1 = llong1*math.pi/180.
# long2 = llong2*math.pi/180.

# #косинусы и синусы широт и разницы долгот
# cl1 = math.cos(lat1)
# cl2 = math.cos(lat2)
# sl1 = math.sin(lat1)
# sl2 = math.sin(lat2)
# delta = long2 - long1
# cdelta = math.cos(delta)
# sdelta = math.sin(delta)

# #вычисления длины большого круга
# y = math.sqrt(math.pow(cl2*sdelta,2)+math.pow(cl1*sl2-sl1*cl2*cdelta,2))
# x = sl1*sl2+cl1*cl2*cdelta
# ad = math.atan2(y,x)
# dist = ad*rad

# #вычисление начального азимута
# x = (cl1*sl2) - (sl1*cl2*cdelta)
# y = sdelta*cl2
# z = math.degrees(math.atan(-y/x))

# if (x < 0):
#     z = z+180.

# z2 = (z+180.) % 360. - 180.
# z2 = - math.radians(z2)
# anglerad2 = z2 - ((2*math.pi)*math.floor((z2/(2*math.pi))) )
# angledeg = (anglerad2*180.)/math.pi

# print ('Distance >> %.0f' % dist, ' [meters]')
# print ('Initial bearing >> ', angledeg, '[degrees]')
# print ('Alfa >> %.6f' % ad, ' [rad]')

# a = 1.570796
# b = 0.261799
# c = 1.570796

a = 449444  / rad
b = 2941427 / rad
c = 3384778 / rad

chislitel   = math.cos(a) - math.cos(b)*math.cos(c)
znamenatel  = math.sin(b)*math.sin(c)

print('chislitel ', chislitel)
print('znamenatel ', znamenatel)
print('chislitel/znamenatel ', chislitel/znamenatel)

A = math.degrees(math.acos(chislitel/znamenatel))
print('A ', A)