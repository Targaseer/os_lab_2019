#!/bin/bash
port=20001
ip="127.0.0.1"
filename="/projects/os_lab_2019/lab6/src/Servers.txt"
echo "Enter number of servers: "
read num
if [ -f "$filename" ]; then
    read prev_num < "$filename"
    let num=$num-$prev_num
    if [ $num -gt 0 ]; then
        let res=$num+$prev_num
        sed -i "1s/.*/$res/" "$filename"
    fi
else
    echo "$num" > "$filename"
fi
while [[ $num -gt 0 ]]
do
if !(lsof -Pi :$port -sTCP:LISTEN -t >/dev/null) ; then
    ./server --port $port --tnum 3 &
    echo "$ip : $port" >> "$filename"
    let num=$num-1
fi
let port+=1
done
