#!/bin/bash
file1=$1
file2=$2
file3=$3
file4=$4
file5=$5

join $file1 $file2 > temp.txt
join temp.txt $file3 > temp2.txt
join temp2.txt $file4 > temp3.txt
join temp3.txt <(cut -f1,3 $file5) 

rm temp.txt
rm temp2.txt
rm temp3.txt
