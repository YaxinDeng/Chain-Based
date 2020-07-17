#include "chain.h"

using namespace std;

int task_cnt = 0;   //task count
int origin_model;   //origin model index
int origin_task;    //origin tasks index 
int mtx_indx_cnt;   //column counter used for power reboots 

Data_nonvol Data_Index_Table[ROW][COL]; //data matrix representing nonvolatile memory

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
    length = 1;
  }
  else{ //insert new data at the end of list
    Data *p = head;
    if (p->index == index){
      p->value = value;
      return;
    }
    while(p->next != NULL){
      if (p->index == index){
        p->value = value;
        return;
      }
      p = p->next;
    }
    Data *t = new Data();
    p->next = t;
    t->value = value;
    t->index = index;
    t->next = NULL;
    length ++;
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

// Get_origin function of Task class. Returns the model of the current origin task.
int Task::get_origin_model(){
  return origin_model;
}

int Task::get_origin_task(){
  return origin_task;
}

void Task::wr2file(string words) {
  ofstream results;
  results.open("results.txt");
  
  data_str = data_str + words;
  results << data_str;
}


// Set_origin function of Task class. Sets the origin variable (i.e. the index of task 
// that will be the first task executed upon resuming after power failure).
void Task::set_origin(int model_idx, int task_idx){
  origin_model = model_idx;
  origin_task = task_idx;
  // cout << "New Origin Task Set!" << endl;
  wr2file("New origin model is: " + to_string(origin_model) + '\n');
  wr2file("New origin task is: " + to_string(origin_task) + '\n');
  wr2file("New Origin Task Set!\n");
}

// Operator() function of Task class. Determines function (Task) to execute 
// based on task index provided.
void Task::operator()(int model_idx, int task_idx) {
  //cout<<"++current row index: "<<mtx_indx_cnt<<endl;
  //choose next task based on model and task types
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

#ifdef DEBUG
  // cout << "Executing Sensor RAW Task for  ";
  wr2file("Executing Sensor RAW Task for ");
#endif

  float A,B;
  if(model_type == 0) {A = 100.0;  B = 50.0;} //temp
  else if(model_type == 1) { A = 60.0;  B = 0.0;} //Water
  else { A = 70.0;  B = 0.0;} //Humidity

  for (int i = 0; i < NUM; i++){
    float data = A + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(B-A)));
    if (model_type == 0) {
      //sensor_RAW() function for temperature model
      // cout << "Temperature Model" << endl;
      if (i==0) {
        wr2file("Temperature Model\n");
      }
      Data_Index_Table[0][mtx_indx_cnt].set(i,data);
    } else if (model_type == 1) {
      //sensor_RAW() function for water model
      // cout << "Water Model" << endl;
      if (i==0) {
        wr2file("Water Model\n");
      }
      Data_Index_Table[3][mtx_indx_cnt].set(i,data);
    } else if (model_type == 2) {
      //sensor_RAW() function for humidity model
      // cout << "Humidity Model" << endl;
      if (i==0) {
        wr2file("Humidity Model\n");
      }
      Data_Index_Table[6][mtx_indx_cnt].set(i,data);
    }
  }

  if (model_type == 0) {
    //set next origin task to sensor_AVG() for temperature model
    // cout << "Setting Origin Task to sensor_AVG() for Temperature Model" << endl;
    wr2file("Setting Origin Task to sensor_AVG() for Temperature Model\n");
    this->set_origin(model_type, 1);
  } else if (model_type == 1) {
    //set next origin task to sensor_AVG() for water model
    // cout << "Setting Origin Task to sensor_AVG() for Water Model" << endl;
    wr2file("Setting Origin Task to sensor_AVG() for Water Model\n");
    this->set_origin(model_type, 1);
  } else if (model_type == 2) {
    //set next origin task to sensor_AVG() for humidity model
    // cout << "Setting Origin Task to sensor_AVG() for Humidity Model" << endl;
    wr2file("Setting Origin Task to sensor_AVG() for Humidity Model\n");
    this->set_origin(model_type, 1);
  }
  task_cnt++;

#ifdef DEBUG
  // cout << "---------------------------------------"<<endl;
  wr2file("---------------------------------------\n");
#endif

}

void Task::sensor_AVG(int model_type) {

#ifdef DEBUG
  // cout << "Executing Sensor AVG Task for ";
  wr2file("Executing Sensor AVG Task for ");
#endif

  int table_index;
  if (model_type == 0) {table_index = 0;}
  else if(model_type == 1){table_index = 3;}
  else{table_index = 6;}
  float A,B,C;
  A = Data_Index_Table[table_index][mtx_indx_cnt].read(0);
  B = Data_Index_Table[table_index][mtx_indx_cnt].read(1);
  C = Data_Index_Table[table_index][mtx_indx_cnt].read(2);
  float avg = (A + B + C)/NUM;

  if (model_type == 0) {
    //sensor_AVG() function for temperature model
    // cout << "Temperature Model" << endl;
    wr2file("Temperature Model\n");
    Data_Index_Table[1][mtx_indx_cnt].set(0,avg);
    // cout << "Setting Origin Task to sensor_IO() for Temperature Model" << endl;
    wr2file("Setting Origin Task to sensor_IO() for Temperature Model\n");
    this->set_origin(model_type, 2);
  } else if (model_type == 1) {
    //sensor_AVG() function for water model
    // cout << "Water Model" << endl;
    wr2file("Water Model\n");
    Data_Index_Table[4][mtx_indx_cnt].set(0,avg);
    // cout << "Setting Origin Task to sensor_IO() for Water Model" << endl;
    wr2file("Setting Origin Task to sensor_IO() for Water Model\n");
    this->set_origin(model_type, 2);
  } else if (model_type == 2) {
    //sensor_AVG() function for humidity model
    // cout << "Humidity Model" << endl;
    wr2file("Humidity Model\n");
    Data_Index_Table[7][mtx_indx_cnt].set(0,avg);
    // cout << "Setting Origin Task to sensor_IO() for Humidity Model" << endl;
    wr2file("Setting Origin Task to sensor_IO() for Humidity Model\n");
    this->set_origin(model_type, 2);
  }
  task_cnt++;

#ifdef DEBUG
  // cout<< "Raw data is: " << A << " | " << B << " | " << C << endl;
  wr2file("Raw data is: " + to_string(A) + " | " + to_string(B) + " | " + to_string(C) + '\n');
  // cout << "---------------------------------------"<<endl;
  wr2file("---------------------------------------\n");
#endif

}

void Task::sensor_IO(int model_type){

#ifdef DEBUG
  // cout << "Executing Sensor IO Task for ";
  wr2file("Executing Sensor IO Task for ");
#endif

  //store 0 = temperature OK, 1 = too high, 2 = too low. 
  //read the average data inside the list
  float avg;
  float high_range;
  float low_range;
  int index;
  if (model_type == 0) {
    //sensor_IO() function for temperature model
    // cout << "Temperature Model" << endl;
    wr2file("Temperature Model\n");
    avg = Data_Index_Table[1][mtx_indx_cnt].read(0);
    high_range = TEMP_HIGH;
    low_range = TEMP_HIGH;
    index = 2;
  }else if(model_type == 1){
    //sensor_IO() function for water model
    // cout << "Water Model" << endl;
    wr2file("Water Model\n");
    avg = Data_Index_Table[4][mtx_indx_cnt].read(0);
    high_range = WATER_HIGH;
    low_range = WATER_LOW;
    index = 5;
  }else{
    //sensor_IO() function for humidity model
    // cout << "Humidity Model" << endl;
    wr2file("Humidity Model\n");
    avg = Data_Index_Table[7][mtx_indx_cnt].read(0);
    high_range = HUMID_HIGH;
    low_range = HUMID_LOW;
    index = 8;
  }

  // cout << "Average data read from non-volatile memory is: " << avg << endl;
  wr2file("Average data read from non-volatile memory is: " + to_string(avg) + '\n');
  
  //comparison function
  if (avg > high_range){
    // cout<<"**data too high**"<<endl;
    wr2file("**data too high**\n");
    Data_Index_Table[index][mtx_indx_cnt].set(0,1);
  }else if(avg < low_range){
    // cout<<"**data too low**"<<endl;
    wr2file("**data too low**\n");
    Data_Index_Table[index][mtx_indx_cnt].set(0,2);
  }else{
    // cout<<"**data good**"<<endl;
    wr2file("**data good**\n");
    Data_Index_Table[index][mtx_indx_cnt].set(0,0);
  }

  if (model_type == 0) {
    //set next origin task to sensor_RAW() for water model
    // cout << "Setting Origin Task to sensor_RAW() for Water Model" << endl;
    wr2file("Setting Origin Task to sensor_RAW() for Water Model\n");
    this->set_origin(1, 0);
  } else if (model_type == 1) {
    //set next origin task to sensor_RAW() for humidity model
    // cout << "Setting Origin Task to sensor_RAW() for Humidity Model" << endl;
    wr2file("Setting Origin Task to sensor_RAW() for Humidity Model\n");
    this->set_origin(2, 0);
  } else if (model_type == 2) {
    if (mtx_indx_cnt == 9){
      mtx_indx_cnt = 0;
    }else{
      mtx_indx_cnt++;
    }
    //set next origin task to sensor_RAW() for temperature model
    // cout << "Setting Origin Task to sensor_RAW() for Temperature Model" << endl;
    wr2file("Setting Origin Task to sensor_RAW() for Temperature Model\n");
    this->set_origin(0, 0);
  }
  task_cnt++;

#ifdef DEBUG
  // cout << "---------------------------------------"<<endl;
  wr2file("---------------------------------------\n");
#endif

}

//this function to measure a paticular set of three model data in non-volatile memory
//input the col number want to read
//call data_read(3) means read the COL 3 data inside index table
void data_read(int x_row){ 
  for (int i = 0; i < ROW; i++){
    cout << "data inside:[" << i << "][" << x_row << "]" << endl;
    for(int j = 0; j < Data_Index_Table[i][x_row].length; j++){
      cout << Data_Index_Table[i][x_row].read(j) << " ";
    }
    cout << endl;
  }
}

//this function is to print out the most recent many group of data storage inside the non-volatile memory
//for call this function will be Chsync(2);2 will represent the most recent 2 groups of data want to read out.
void Chsync(int most_recent_many){
  int data_row = mtx_indx_cnt;
  int count = 0;
  while(count < most_recent_many){
     data_read(data_row);
     data_row--;
     if (data_row <0){
       data_row = data_row +10;
     }
     count++;
  }
}

//MultiOut is to change data to the sychronize channel all during the same time, it can choose from the most recent channels and rewrite them during the same time
//order for call this function: MultiOut(#of_sync_channels, value, model a, type a, index a, model b, type b, index b ......)
//important, this function need to pass argument as double like:1.0
//e.g.:  MultiOut(2,1.0,1.0,0.0,0.0,0.0,0.0,0.0);
//this will wirte the position [0][x][0] && [3][x][0] to 1
void MultiOut(double count, ...){
  int size = (int)count; 
  count = count*3 + 2;
  va_list varList;
  va_start(varList, count);
  float v = (float)va_arg(varList,double);
  int model,type,index,row;
  cout<<count<<endl;
  cout<<size<<endl;
  cout<<v<<endl;
  for(int i = 0; i < size; i++){
    model = (int)va_arg(varList,double);
    type = (int)va_arg(varList,double);
    index = (int)va_arg(varList,double);
    row = model * 3 + type;
    cout<<model<<endl;
    cout<<type<<endl;
    cout<<index<<endl;
    cout<<row<<endl;
    for (int j = 0; j < 10; j++){
      if (Data_Index_Table[row][j].getHead() != NULL){
        Data_Index_Table[row][j].set(index,v);
        cout<<"enter this loop"<<endl;
      }
    }
  }
}
