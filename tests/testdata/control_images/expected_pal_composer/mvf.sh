#/bin/bash

cd $1

if [ -d precise ]; then
  mkdir macosx
  mv *.png macosx
fi

