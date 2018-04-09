#!/bin/bash
git pull --rebase
./waf clean build $1
