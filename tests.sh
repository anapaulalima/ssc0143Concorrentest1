#! /bin/bash

# for each matrix file
for test_file in $(ls -p | grep matriz | grep -v /)
do

	total_time_single=0
	total_time_multi=0

	# ten times to have an average
	for((i=0 ;i<10;i++))
	do
		(/usr/bin/time -f "\n%e" ./paralelo < $test_file) 2> tmp-$test_file 1> result-$test_file
		total_time_single=$(python -c "print $total_time_single + $(tail -n 1 tmp-$test_file)")


	done

	echo $test_file
	echo Average Paralel Time: $(python -c "print \"%.3f\" % ($total_time_single/10)")
	echo 

	rm tmp-$test_file
done
