#!/bin/bash
for ((i=1;i<26;i++))
do
   num=$i
if (( num < 10 ))
then
   num="0$num"
fi
if ([ -e "pos_${num}_in_m1.txt" ] || [ -e "pos_${num}_in_m2.txt" ]) && [ -e "pos_${num}_out.txt" ] && [ -e "pos_${num}_args.txt" ]
then
    sh pos_test.sh $num
else
    break
fi
    done

echo

for ((i=1;i<36;i++))
do
    num=$i
    if (( num < 10 ))
    then
        num="0$num"
    fi
    if ([ -e "neg_${num}_in_m1.txt" ] || [ -e "neg_${num}_in_m2.txt" ]) && [ -e "neg_00_out.txt" ] && [ -e "neg_${num}_args.txt" ]
    then
        sh neg_test.sh $num $i
    else
        break
    fi
        done
