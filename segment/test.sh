#!/bin/bash

oname=regmap

rm ${oname}.*map.*

gdb --args ./bin/segment -t 10 -m 0.1 -n 15,15,100,2500,2500 -o $oname ../dataset/temp_byte_bip
