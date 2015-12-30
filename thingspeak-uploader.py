#!/usr/bin/python2

import httplib, urllib, serial, time, datetime, os

time.sleep(60)

while 1:
	try:
		ser=serial.Serial('/dev/ttyUSB0',9600)
		break
	except OSError as e:
		print "OS error when opening serial port ({0}): {1}".format(e.errno, e.strerror)
		time.sleep(10)

params="api_key="+os.environ['THINGSPEAK-API-KEY']
headers={"Content-type": "application/x-www-form-urlencoded","Accept":"text/plain"}

while 1 :
	try:
		tempString,sensorString=ser.readline().split(" ");

		temp=float(tempString)/10
		sensor=int(sensorString)
		time_stamp=datetime.datetime.fromtimestamp(time.time()).strftime("%Y-%m-%d %H-%M-%S")

		if sensor>5:
			boiler="1"
		else:
			boiler="0"

		print "temp: %f, sensor %d, boiler is %s at %s" % (temp,sensor,boiler,time_stamp)

		conn=httplib.HTTPConnection('api.thingspeak.com:80')
		encodedParams=params+urllib.urlencode({'field1':temp,'field2':sensor,"field3":boiler})
		conn.request('POST','/update',encodedParams,headers)
		response=conn.getresponse()
		print response.status
		conn.close()
	except OSError as e:
		print "OS error when reading port ({0}): {1}".format(e.errno, e.strerror)
	time.sleep(30)

