for i in `ls -1 samples/*.pdf` ; 
do 
	for mode in static dynamic; 
	do 
		echo "$mode $i:" 
		cat $i | time ./pdfpreview_$mode 200 300 10 > $i.$mode.out.jpg
		echo
	done
done
