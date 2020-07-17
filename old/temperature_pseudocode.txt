//sudo code for phase 1:
//consider three input channels

//first input for testing the sense_temp:
//every temp will test 3 time for temp from sensor
//calculate average temp and send to compare with reference
//similar progress for the other I/O
 

origin task Temp_measure(){
	if (count  < 3) 
		int temp = sensor();
		ChOut (TEMP[count] <- temp),Temp_Avg; //save 3 sets data
	else
		NextTask Temp_Avg;
}

task Temp_Avg(){
	temp_head = ChIn HEAD, Temp_measure; //data set from the measurement sensor
	ref_head = ChIn HEAD, self; //reference temperature
							//this reference temp save 2 threshold data High & Low
	
	if (count < 3)
		total_temp = ChIn temp[temp_head + count];
		count ++;
	avg_temp = total / 3;
	
	for(count < 3)
		if(avg_temp > ref[ref_head]){
			return Temp_High;
			NextTask Temp_DOWN;
		else if (avg_temp < ref[ref_head + 1])
			return Temp_Low;
			NextTask Temp_UP;
		else
			NextTask Temp_nutron;
}

task Temp_DOWN(){
	turn on fan for cool down 
	NextTask Temp_measure;
}

task Temp_UP{
	turn on light to heat up;
	NextTask Temp_measure;
}

task Temp_nutron{
	turn off light or fan;
	NextTask Temp_measure;
}

//pending similar pseudocode for water and humidity sensors and the actual implementations in C++