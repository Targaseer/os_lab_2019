#!/bin/bash
for i in {1..150}
do
    od -An -N2 -tu2 < /dev/urandom | cat >>numbers.txt
done
