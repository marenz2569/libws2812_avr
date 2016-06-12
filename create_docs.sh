#!/bin/bash

if [[ -z "$1" ]]; then
	echo "Please enter a git commit message"
	exit
fi

rm -rf html &&\
	make docs &&\
	cd html &&\
	git add * &&\
	git commit -am "$1" &&\
	git push origin gh-pages
