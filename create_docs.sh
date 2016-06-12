#!/bin/bash

if [[ -z "$1" ]]; then
	echo "Please enter a git commit message"
	exit
fi

rm -rf html &&\
	mkdir html &&\
	cd html &&\
	git init &&\
	git remote add origin git@github.com:marenz2569/libws2812_avr.git &&\
	git checkout --orphan gh-pages &&\
	git pull origin gh-pages &&\
	git rm -r * &&\
	cd .. &&\
	make docs &&\
	cd html &&\
	git add * &&\
	git commit -am "$1" &&\
	git push origin gh-pages
