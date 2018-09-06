#!/bin/sh

if [ -e /opt/ko/load3531a_cas_sm ]; then
	cd /opt/ko
	./load3531a_cas_sm 
else
  echo "no ko to load!!!!!"
fi

if [ -e /opt/update/update.sh ]; then
	cd /opt/update
	./update.sh 
	rm *.*
else
  echo "no update to run!!!!!"
fi


if [ -e /opt/app/run.sh ]; then
	cd /opt/app
	./run.sh
else
  echo "no app to run!!!!!"
fi	
