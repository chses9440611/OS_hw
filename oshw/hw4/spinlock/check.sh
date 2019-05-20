#!/bin/bash

for i in {1..100};
do
    ./spinlock `shuf -i 1000-1000000 -n 1`
    if [ $? -ne 0 ]; then
        echo "Wrong Answer!"
        exit 0
    fi
done

echo "Correct!!"

