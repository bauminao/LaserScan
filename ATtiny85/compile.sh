#!/bin/bash 
if [ -z $1 ]; then
  echo ""
  echo "Syntax:"
  echo "  compile <PathToIno>/<Ino> [-keep]" 
  echo ""
  exit 1
fi

if test ! -f $1 ; then
  echo ""
  echo "Wrong Ino-file selected."
  echo ""
  exit 2
fi

sketchfile=$1

tmpdir=`mktemp -d -p .`

# ESP8266 Board
#board=esp8266:esp8266:nodemcuv2:CpuFrequency=80,VTable=flash,FlashSize=4M1M,LwIPVariant=v2mss536,Debug=Disabled 
#result=bin

# ATtiny85
board=attiny:avr:ATtinyX5:cpu=attiny85,clock=internal8
result=hex


arduino                     \
  --verify                  \
  --pref build.path=./$tmpdir \
  --board $board \
  $sketchfile

cp $tmpdir/*.${result} . 

if [ "$2" == "-keep" ];
then
  exit 0 
else
  rm -rf $tmpdir
fi
