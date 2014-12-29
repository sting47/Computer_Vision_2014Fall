for ((x=20000;x<=40000;x+=1000));
do
	#./a.out n-babu $x
	rm Netatia\&Babu_$x.bmp
done

# for((x=20000;x<=40000;x+=4000));
# do
# 	open Netatia\&Babu_$x.bmp
# done
echo "Finish"
