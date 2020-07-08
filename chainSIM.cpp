#include "chainSIM.h"

// C++ implementation of the Chain task-channel architecture
// To be used temporarily for simulation purposes
// Helps with understanding the 

const int threshold = 2000; //in mV. 

int main_task; //if 1, then temp task, if 2 then water task, if 3 then humidity task
int sub_task; //if 1 then sense task, if 2 then computing average task, if 3 then send signal to external device task

int capacity; //in mV

bool next_sensor; //if 1, then it is time to move to a different sensor 

// menu not necessary for this program and may be deleted later on

void menu() {

	std::cout << "Select an option to . Enter the option number followed by enter" << std::endl;

	std::cout << "1 Harvest Energy" << std::endl << "2 Create Task" << std::endl << "3 Run Simulation" << std::endl;

	int option;

	std::cin >> option;

	std::cout << option << std::endl;

}
/*
//when device needs to harvest energy. the previously attempted task is saved globally(a simulation for being stored in non-volatile memory)
void on() {
	next_sensor = false;
	if(main_task == 1) {
		temp(sub_task); 
	}
	else if(main_task == 2) {
		water(sub_task);
	}
	else if(main_task == 3) {
		humidity(sub_task);
	}
}

//some time is taken to harvest energy
void harvesting() {
	capacity = 0;

	while(cap < threshold) {
		cap+= (rand()%50);
	}
}
*/

/*

//tasks include reading temperature value from sensor, computing average, and turning fan on or off (accounting for recent water and humidity values if available)
void temp(sub_task) {
	while(~next_sensor && cap >= 20){
		sub_task = next_temp_task();
		cap-=20;
	}
	if(cap < 20 && ~next_sensor) {
		harvesting();
		temp(sub_task);
	}
	if(next_sensor) {
		sub_task = 1;
		water(subtask);
	}
}


//tasks include reading water level in soil from sensor, computing average, and turning irigation system on or off (accounting for recent temperature and humidity values if available)
void water(sub_task) {
	while(~next_sensor && cap >= 20){
		sub_task = next_water_task();
		cap-=20;
	}
	if(cap < 20 && ~next_sensor) {
		harvesting();
		water(sub_task);
	}
	if(next_sensor) {
		sub_task = 1;
		humidity(subtask);
	}
}

//tasks include reading humidity from sensor, computing average, and turning ventilation system on or off (accounting for recent temperature and water levels if available)
void humidity(sub_task) {
	while(~next_sensor && cap >= 20){
		sub_task = next_temp_task();
		cap-=20;
	}
	if(cap < 20 && ~next_sensor) {
		harvesting();
		humidity(sub_task);
	}
	if(next_sensor) {
		sub_task = 1;
		temp(subtask);
	}
}

// function that calls other tasks depending on which task it is currently on

int next_temp_task() {
	if(sub_task == 1) {
		int next_task = Temp_measure();
		return next_task;
	}
	else if(sub_task == 2) {
		int next_task = Temp_AVG();
		return next_task;
	}
	else if(sub_task == 3) {
		int next_task = Temp_DOWN();
		return next_task;
	}
	else if(sub_task == 4) {
		int next_task = Temp_UP();
		return next_task;
	}
	else {
		int next_task = Temp_nutron();
		return next_task;
	}
}

// function that calls other tasks depending on which task it is currently on

int next_water_task() {
	if(sub_task == 1) {
		int next_task = Water_measure();
		return next_task;
	}
	else if(sub_task == 2) {
		int next_task = Water_AVG();
		return next_task;
	}
	else if(sub_task == 3) {
		int next_task = Water_DOWN();
		return next_task;
	}
	else if(sub_task == 4) {
		int next_task = Water_UP();
		return next_task;
	}
	else {
		int next_task = Water_nutron();
		return next_task;
	}
}

// function that calls other tasks depending on which task it is currently on
int next_humidity_task() {
	if(sub_task == 1) {
		int next_task = Humidity_measure();
		return next_task;
	}
	else if(sub_task == 2) {
		int next_task = Humidity_AVG();
		return next_task;
	}
	else if(sub_task == 3) {
		int next_task = Humidity_DOWN();
		return next_task;
	}
	else if(sub_task == 4) {
		int next_task = Humidity_UP();
		return next_task;
	}
	else {
		int next_task = Humidity_nutron();
		return next_task;
	}
}

*/

// need to implement functions for each sensor that measure, find avg, and turn on or off external devices. also to change the global variable 'next_sensor'

