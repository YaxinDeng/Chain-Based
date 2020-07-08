#include <iostream>
#include <map>
#include <vector>
#include <time.h>
#include <random>
#include <cstdlib>
#include <ctime>

#define ROW 5   //total number of tasks + 1 
#define COL 10  //total number of data members that we will record (per vector)

#define NUM 3  //how many time to measure temperature
#define TEMP_HIGH 90.0  //high temperature range
#define TEMP_LOW 65.0 //low temperature range

#define WATER_HIGH 40.0  //high water range 40%
#define WATER_LOW 20.0 //low water range 20%

#define HUMID_HIGH 50.0  //high humidity range 40%
#define HUMID_LOW 30.0 //low humidity range 20%

#define DEBUG   //comment out to not include DEBUG code

// Data struct: Data data collect from sensor to Channel,the blue color section in flow chart
struct Data{ 
  int index;
  float value;

  Data* next;
};

// Data_nonvol class that is a vector linked list of nonvolatile data. Data_nonvol class 
// is a singly linked list of data that is accessible to Channel class. Contains set()
// and read() functions to update linked list of Data class.
class Data_nonvol{
  private:
    int orig_index;   //index of origin stored for 
    Data* head;
  public:
    Data_nonvol();
    ~Data_nonvol();
    //function to get head of Data
    Data *getHead(){
      return this->head;
    }
    void set(int index, float value);   //add value of Data type to list at index 
    float read(int index);              //read value of Data type from list at index
};

/*

//Matrix class that will use the index of the Task (t0) to determine which data 
//is accessible to the task (i.e. Task Sensor (with index 0) has access to Matrix data (index 0))
class Nonvol_data_mtx {

  private:
    Data_nonvol *data;

  public:
    //Matrix dimensions. Useful for printing.
    std::vector<std::vector<int> > matrix;

    //Constructor. Allocates nonvolatile data array size.
    Nonvol_data_mtx(int m, int n);

    //class method to retrieve object's matrix contents attribute (via A(i,j) format)
    int& operator()(const int &row_num, const int &col_num);

    Data_nonvol *getData(){
      return this->data;
    }

    void putData(Data_nonvol *newData){
      data = newData;
    }

    //Destructor. Deletes the Data_nonvol array.
    ~Nonvol_data_mtx();

    //Inserts or updates an element of the Matrix.
    void set(int row_num, int col_num, float value);

    //Retrieves the element of the Matrix.
    float get(int row_num, int col_num);

};

*/

// Task class that represents linked list of tasks. Task class is a doubly linked list
// that includes set_origin functions, which have access to Class Data_nonvol
class Task{
  public:
    Task(int model_idx, int task_idx);
    ~Task();

    //model_index used to determine model type: index=0 (temperature), index=1 (water), 
    //index=2 (humidity)
    int model_index;
    //task_index used for task type: index=0 (sensor raw), index=1 (sensor avg), 
    //index=2 (sensor io)
    int task_index;
    //returns variables origin_model (stored in nonvolatile memory)
    int get_origin_model();
    //returns variables origin_task (stored in nonvolatile memory)
    int get_origin_task();
    //sets variables origin_model and origin_task and stores into nonvolatile memory
    void set_origin(int model_idx, int task_idx);
    // Operator() function of Task class. Determines function (Task) to execute 
    // based off of task_idx provided.
    void operator()(int model_idx, int task_idx);

    //tasks
    void sensor_RAW(int model_type); //task 1
    void sensor_AVG(int model_type); //task 2
    void sensor_IO(int model_type);   //taks 3
  
    //read nonvolatile data structure (matrix) and allow access to data of matrix based 
    //on model_idx (i.e. set model_idx to index of PREVIOUS task)
 		Data_nonvol Ch_read(int model_idx, int task_idx, float Data_Index_Table[][100]);
    
    //write to nonvolatile data structure (matrix) and allow access to data of 
    //matrix based on model_idx (i.e. set model_idx to index of NEXT task)
    void Ch_write(int model_idx, int task_idx, float Data_Index_Table[][100]); 

};
