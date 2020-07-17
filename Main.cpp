#include "src/chain.h"
#include "src/timer.h"

using namespace std;

int rst_cnt = 0;

void power_on(Task &agr, Timer &on, int model, int task, int duration_on, int duration_off);
void power_off(Timer &off, int duration_off);

int main(int argc, char *argv[]) {
  
	// Creating an object of type Task. Starting from the first temperature task
  Task agr(0,0);
  
  int model = agr.get_origin_model();
  int task = agr.get_origin_task();

  cout << "Starting timer test" << endl;

	// Reading from waveform.txt. The Interval in which a device is on and off is stored in a vector
	ifstream file;

	file.open("waveform_data.txt");

	vector<int> data;

	string word;

	int count = 0;

	while(count < 3) {
		file >> word;
		string num = "";
		for(auto &c : word) {
			if(c != '[' && c != ']' && c != ',') {
				num += c;
			}
		}
		data.push_back(stoi(num));
		count++;
	}

	int duration_on = (data[0] / data[2]) - data[1];
	int duration_off = data[1];
	int cycles = data[2];

	// The timer starts and the tasks begin executing and information begins printing on the console. The duration 
  // that the tasks execute is based on the interval for on from the waveform. An object of type Timer is declared
	// may need to use a while loop here to execute this process n times, where n is data[2]
  while (rst_cnt < cycles) {
    Timer on = Timer();
    cout << "rst_cnt is: " << rst_cnt << endl;
    power_on(agr, on, model, task, duration_on, duration_off);
    this_thread::sleep_for (chrono::seconds(duration_on));
    
    agr.wr2file("\n***POWER FAILURE***\n");
    
    Timer off = Timer();
		power_off(off, duration_off);
		this_thread::sleep_for (chrono::seconds(duration_off));

    rst_cnt++;
  }
  cout << "\n***END OF MAIN***\n" << endl;
  agr.wr2file("\n***END OF MAIN***\n");

	file.close();

  return 0;
}

void power_on(Task &agr, Timer &on, int model, int task, int duration_on, int duration_off) {
  cout << "Powering on..." << endl;
	on.setInterval([&]() {
    cout << "Running AGR Tasks..." << endl;
		model = agr.get_origin_model();
 		task = agr.get_origin_task();
		agr(model, task);
  }, 1000); // 200 was arbitrarily chosen and depends on the technology

  on.setTimeout([&]() {
    cout << "\nStopping ON thread\n" << endl;
		on.stop();
  }, duration_on*1000);
}

void power_off(Timer &off, int duration_off) {
  cout << "\n***POWER FAILURE***\n" << endl;
	off.setInterval([&]() {
		cout << "Recharging..." << endl;
  }, 1000); // 5000 was arbitrarily chosen and depends on the environment

  off.setTimeout([&]() {
    cout << "\nStopping OFF thread\n" << endl;
		off.stop();
  }, duration_off*1000);
}
