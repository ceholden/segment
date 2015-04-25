#!/bin/bash

rm test.rmap.*

gdb --args ./bin/segment -t 10 -m 0.1 -o test ../dataset/test_3456.bip
