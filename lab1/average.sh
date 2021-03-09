#!/bin/bash
numbers=0
sum=0
while read j
do
    ((numbers+=1)) && ((sum+=j))
done
echo "Sum of inputs = $sum"
echo "Number of inputs = $numbers"
avg=$((sum / numbers))
printf "Average input = %.5f\n" $avg