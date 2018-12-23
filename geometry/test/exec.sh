#!/bin/bash

for file in `ls | grep dot$`
do
        echo $file
        dot -Tpng $file -o $file.png
done
