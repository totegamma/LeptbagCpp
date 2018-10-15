#!/bin/bash

TARGET=$1

mv "lib"$TARGET".so" $TARGET".friends"

if [ $# = 1 ]; then
	exit
fi

cp $TARGET".friends" "../../source/friends/"
cd ../../source
./japari_park
cd "../friends/"$TARGET
rm "../../source/friends/"$TARGET".friends"
