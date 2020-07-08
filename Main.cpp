#include "chainSIM.h"

using namespace std;

int main(int argc, char *argv[]) {


  //MAIN CODE SECTION: keep for final
  Task t0(0,1);  //start with arbitrary orig_model and orig_task for now...

  //set orig_model using get_origin_model() functions
  //FIXME: may combine into one function later...
  int orig_model = t0.get_origin_model();
  int orig_task = t0.get_origin_task();
  

#ifdef DEBUG
  int cnt=0;
#endif 
  
  while (cnt < 18) { //change condition to '1' later

    orig_model = t0.get_origin_model();
    orig_task = t0.get_origin_task();

#ifdef DEBUG
    cout << "orig_model is: " << orig_model << " orig_task is: " << orig_task << endl;
#endif

    //use Task operator to execute next task based on index ("orig")
    t0(orig_model, orig_task);
    cnt++;
  }

  //cout<< Data_Index_Table[0][0].read(0);

/*

  //### COMPLETE ###
  //MATRIX DATA TEST: test Data_nonvol, Nonvol_data_mtx and Ch_read  
  Task t0_temp(0);
  Task t1_water(1);
  Nonvol_data_mtx t_mtx(ROW,COL);
  Data_nonvol t0_temp_data;
  Data_nonvol t1_water_data;
  
  //setup Data_nonvol with task data 
  t0_temp_data.set(0,2);
  t0_temp_data.set(1,4);
  t0_temp_data.set(2,6);
  t1_water_data.set(0,1);
  t1_water_data.set(1,3);
  t1_water_data.set(2,5);
  
  for (int i=0; i < 2; i++) {
    for (int j=0; j < 3; j++) {
      if (i == 0) {
        t_mtx.set(i, j, t0_temp_data.read(j));
      } else if (i == 1) {
        t_mtx.set(i, j, t1_water_data.read(j));
      }
    }
  }
  
  for (int i=0; i < 2; i++) {
    for (int j=0; j < 3; j++) {
      cout << t_mtx.get(i, j) << " ";
    }
    cout << endl;
  }
  
  //test Ch_read function
  Data_nonvol t0_nonvol_data;
  t0_nonvol_data = t1_water.Ch_read(t0_temp->index, t_mtx);
  
  for (int i=0; i < 3; i++) {
    cout << t0_nonvol_data.read(i) << " ";
  }
  cout << endl;


  //test Ch_write by writing new data to matrix
  
  
  
  //read task data again
  
  */


	return 0;
} 