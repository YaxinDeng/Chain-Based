#include <iostream>
#include <stdlib.h>
#include <stdio.h>  
#include <cstdlib>
#include <time.h>
#include "chain.h"
#include "repeat.h"

using namespace std;

TASK (1,sensor)
void sensor(){
  float data = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));
  //CHAN_OUT1(float, data, , CH(sensor,Temp_measure));
}

TASK (2,Temp_measure)
void Temp_meausre(){
  cout<< "successful define original function" <<endl;
}

struct msg_type {
    CHAN_FIELD(float, data);
};

CHANNEL(task_name_from, task_name_to, msg_type);


int main(){
  cout<<"^_^ file can be successful exiscute"<<endl;

  return 0;
}