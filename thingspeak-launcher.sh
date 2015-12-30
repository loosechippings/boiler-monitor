#!/bin/bash

PID_FILE=/tmp/thingspeak-uploader.pid

if [[ -f ${PID_FILE} ]]
then
  if kill -0 `cat ${PID_FILE}` > /dev/null 2>&1
  then
    echo "`cat ${PID_FILE}` already running"
    exit 0
  fi
  echo "`cat ${PID_FILE}` not running"
fi

/home/pi/thingspeak-uploader.py > /tmp/thingspeak-uploader.log 2>&1 &

echo $! > ${PID_FILE}
