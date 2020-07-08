#include "chain.h"

using namespace std;

int origin_set;
int origin_model;
int origin_task;
int mtx_indx_cnt; //column counter used for power reboots 

Data_nonvol Data_Index_Table[ROW][COL];//data matrix representing nonvolatile memory

// Constructor of Data_nonvol class. Assigns NULL to head.
Data_nonvol::Data_nonvol(){
  head = NULL;
}

// Destructor of Data_nonvol class. 
Data_nonvol::~Data_nonvol(){}

// Set function of Data_nonvol class for writing data into LL. 
void Data_nonvol::set(int index,float value){
  if (head == NULL){ //start first data on list
    head = new Data();
    head->value = value;
    head->index = index;
    head->next = NULL;
  }
  else{ //insert new data at the end of list
    Data *p = head;
    while(p->next != NULL){
      p = p->next;
    }
    Data *t = new Data();
    p->next = t;
    t->value = value;
    t->index = index;
    t->next = NULL;
  }
}

// Read function of Data_nonvol class for reading data from LL. 
float Data_nonvol::read(int index){
  if (head == NULL){
    return 0;
  }
  else{
    Data *p = head;
    if (p->next != NULL){
      while(p->index != index){
        p = p ->next;
      }
      return p->value;
    }
    else{
      if (p->index == index){
        return p->value;
      }
      return 0;
    }
  }
}

// Constructor of Task class.
Task::Task(int model_idx, int task_idx){
  //init model index and task index for Task class
  model_index = model_idx;
  task_index = task_idx;
  mtx_indx_cnt = 0;
}

// Destructor of Task class.
Task::~Task() {}

//Get_origin function of Task class. Returns the origin task variable "origin"
//which is stored in nonvolatile memory. This function will read data structure
//Nonvol_data_mtx (matrix) to find origin variable, which is stored in last row
int Task::get_origin_model(){
  //add code here to read nonvolatile memory based on last row index to get 
  //origin_model and origin_task global variables
  
  
  //just for now, use origin_model = 0
  //int origin_model_idx = 0;    //delete later


  //origin_model = origin_model_idx;
  
  //return tuple origin_model and origin_task global variables that were stored in 
  //nonvolatile memory
  return origin_model;
}

int Task::get_origin_task(){
  //add code here to read nonvolatile memory based on last row index to get 
  //origin_model and origin_task global variables
  
  
  //just for now, use origin_task = 0
  // int origin_task_idx = 0;    //delete later


  // origin_task = origin_task_idx;
  
  //return tuple origin_model and origin_task global variables that were stored in 
  //nonvolatile memory
  return origin_task;
}

//Set_origin function of Task class. Sets the origin variable (i.e. the index of task 
//that will be the first task executed upon resuming after power failure).
void Task::set_origin(int model_idx, int task_idx){
  
  //Since origin variable is volatile (i.e. global), store origin variable in nonvolatile 
  //memory at last index of matrix. This stored origin variable will later be accessed 
  //via get_origin() function.


  //for now, just set like so...delete later
  origin_model = model_idx;
  origin_task = task_idx;  
  
}

// Operator() function of Task class. Determines function (Task) to execute 
// based on task index provided.
void Task::operator()(int model_idx, int task_idx) {

  //choose task type for model temperature 
  if (task_idx == 0) {
      //execute sensor_RAW() task for model type
      this->sensor_RAW(model_idx);
  } else if (task_idx == 1) {
    //execute sensor_AVG() task for model type
    this->sensor_AVG(model_idx);
  } else if (task_idx == 2) {
    //execute sensor_IO() task for model type
    this->sensor_IO(model_idx);
  }
}

void Task::sensor_RAW(int model_type) {

//add sensor task function here, randomly generate 3 float data between 100 to 50 degree
  float A,B;
  if(model_type == 0) {A = 100.0;  B = 50.0;} //temp
  else if(model_type == 1) { A = 60.0;  B = 0.0;} //Water
  else { A = 70.0;  B = 0.0;} //Humidity

  for (int i = 0; i < NUM; i++){
    float data = A + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(B-A)));
    //choose task type for model temperature 
    if (model_type == 0) {
      //execute sensor_RAW() task for model type
      Data_Index_Table[0][mtx_indx_cnt].set(i,data);
    } else if (model_type == 1) {
      //execute sensor_AVG() task for model type
      Data_Index_Table[3][mtx_indx_cnt].set(i,data);
    } else if (model_type == 2) {
      //execute sensor_IO() task for model type
      Data_Index_Table[6][mtx_indx_cnt].set(i,data);
    }
  }

#ifdef DEBUG
  cout << "executing Sensor RAW function" << endl;
#endif

  if (model_type == 0) {
    //temperature stuff here for sensor_AVG() function
    this->set_origin(model_type, 1);
  } else if (model_type == 1) {
    //water stuff here for sensor_AVG() function
    this->set_origin(model_type, 1);
  } else if (model_type == 2) {
    //humidity stuff here for sensor_AVG() function
    this->set_origin(model_type, 1);
  }

}

void Task::sensor_AVG(int model_type) {
  //add sensor task function here
  int table_index;
  if (model_type == 0) {table_index = 0;}
  else if(model_type == 1){table_index = 3;}
  else{table_index = 6;}
  float A,B,C;
  A = Data_Index_Table[table_index][mtx_indx_cnt].read(0);
  B = Data_Index_Table[table_index][mtx_indx_cnt].read(1);
  C = Data_Index_Table[table_index][mtx_indx_cnt].read(2);
  float avg = (A + B + C)/NUM;

#ifdef DEBUG
  cout<< "  raw data is: "<<A<<" | "<<B<<" | "<<C<<endl;
  cout << "executing Sensor AVG function" << endl;
#endif

  if (model_type == 0) {
    //temperature stuff here for sensor_AVG() function
    Data_Index_Table[1][mtx_indx_cnt].set(0,avg);
    this->set_origin(model_type, 2);
  } else if (model_type == 1) {
    //water stuff here for sensor_AVG() function
    Data_Index_Table[4][mtx_indx_cnt].set(0,avg);
    this->set_origin(model_type, 2);
  } else if (model_type == 2) {
    //humidity stuff here for sensor_AVG() function
    Data_Index_Table[7][mtx_indx_cnt].set(0,avg);
    this->set_origin(model_type, 2);
  }
}

void Task::sensor_IO(int model_type){
  //store 0 = temperature OK, 1 = too high, 2 = too low. 
  //read the average data inside the list
  float avg;
  if (model_type == 0) {
    avg = Data_Index_Table[1][mtx_indx_cnt].read(0);
  }else if(model_type == 1){
    avg = Data_Index_Table[4][mtx_indx_cnt].read(0);
  }else{
    avg = Data_Index_Table[7][mtx_indx_cnt].read(0);
  }

  cout<<"   average number read non-vol memory is: "<<avg<<endl;
  
  //comparation function
  if (avg > TEMP_HIGH){
    cout<<"**data too high**"<<endl;
    Data_Index_Table[2][mtx_indx_cnt].set(0,1);
  }else if(avg < TEMP_LOW){
    cout<<"**data too low**"<<endl;
    Data_Index_Table[5][mtx_indx_cnt].set(0,2);
  }else{
    cout<<"**data good**"<<endl;
    Data_Index_Table[8][mtx_indx_cnt].set(0,0);
  }

#ifdef DEBUG
  cout << "executing Sensor IO function" << endl;
  cout << "---------------------------------------"<<endl;
#endif
  
  if (model_type == 0) {
    //temperature stuff here for sensor_IO() function
    this->set_origin(1, 0);
  } else if (model_type == 1) {
    //water stuff here for sensor_IO() function
    this->set_origin(2, 0);
  } else if (model_type == 2) {
    if (mtx_indx_cnt == 9){
      mtx_indx_cnt = 0;
    }else{
      mtx_indx_cnt++;
    }
    //humidity stuff here for sensor_IO() function
    if (mtx_indx_cnt == 9){
      mtx_indx_cnt = 0;
    }else{
      mtx_indx_cnt++;
    }
    this->set_origin(0, 0);
  }
}

