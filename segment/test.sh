#!/bin/bash

oname=regmap

rm ${oname}.*map.*

gdb --args ./bin/segment -t 10 -m 0.1 -n 15,15,100,2500,2500 -o $oname ../dataset/temp_byte_bip

compare=$(find ./ -regextype posix-extended -regex "./${oname}.armap.[0-9]+$")
cmp $compare ../dataset/t10-m1-n15_15_100_2500_2500/regionmap
if [ $? != 0 ]; then
    echo "Output segmentation is not the same as test case"
    exit 1
else
    echo "Program worked correctly"
fi
