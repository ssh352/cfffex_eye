#!/bin/bash

VER="$(git rev-list HEAD -n 1 | cut -c 1-7)"
if git status | grep -E "modified|ÐÞ¸Ä" ; then
  VER="${VER}M"
fi
if grep ${VER} src/main/version.h; then
  echo "do nothing"
  exit
fi

VER="$VER $(git log -n 1|grep -E "Date")"

rm -f src/main/version.h
cat version.h.template | sed "s/\$FULL_VERSION/$VER/g" > src/main/version.h

echo $VER
echo "generated version.h"
