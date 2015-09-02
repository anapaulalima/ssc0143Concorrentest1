#! /bin/bash

# for each matrix file
for test_file in $(ls matrizes/)
do



	# ten times to have an average
	for((j=0 ;j<20;j++))
	do
		total_time_single=0
		for((i=0 ;i<2;i++))
		do
			(/usr/bin/time -f "\n%e" ./paralelo $j < $test_file) 2> tmp-$test_file 1> result-$test_file
			total_time_single=$(python -c "print $total_time_single + $(tail -n 1 tmp-$test_file)")


		done
	
		echo Quantidade de thread	$j	
		echo $test_file
		echo Average Paralel Time: $(python -c "print \"%.3f\" % ($total_time_single/10)")
		echo 
	done

	rm tmp-$test_file
done
