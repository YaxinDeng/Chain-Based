#include "chainSIM.h"

using namespace std;

int main(int argc, char *argv[]) {


  //MAIN CODE SECTION: keep for final
  Task t0(0,1);  //start with arbitrary orig_model and orig_task for now...

  //set orig_model using get_origin_model() functions
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

	return 0;
} 