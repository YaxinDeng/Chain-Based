import os
import sys
import csv
import random
import unittest
import subprocess
import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

check_output = lambda x: subprocess.check_output(x, universal_newlines=True)

class ProjectCppTest(unittest.TestCase):

    def setUp(self):

        print("Compiling C++ program before testing...\n")

        #call Makefile command to compile C++ code
        subprocess.check_call(["make", "all"])


    def gen_waveform(self):

        print("Generating random voltage waveform intervals...\n")

        # open output files
        outfile = open('waveform_data.txt', 'w')

        # generate random parameters for voltage waveform with "charge-time" range from 70 to 95
        # (i.e. "on-time" ranges from 5 to 30 ticks); number of cycles ranges from 3 to 10
        charge_time = random.randint(70, 95)
        cycles = random.randint(3, 5)
        tot_t = cycles * 100

        #print parameters
        print("Random voltage waveform intervals are:")
        print("Total Time (s): ", tot_t)
        print("Total Off (Charge) Time per Period (s): ", charge_time)
        print("Total On (Discharge) Time per Period (s): ", int(100-charge_time))
        print("Total Number of Periods: ", cycles)
        print('\n')

        # use random parameters to create sawtooth waveform
        t = np.linspace(0, tot_t, tot_t)
        triangle = signal.sawtooth(2 * np.pi * cycles * t, charge_time/100)

        # # print waveform total time and charge time intervals/duration to output file
        parms_arr = [tot_t, charge_time, cycles]    # total time of waveform, total charge time per period, num of cycles
        print(parms_arr, file=outfile)

        # generate all waveform intervals into array using random parameters from above
        t_volt = []
        for i in range(0, cycles + 1):
            if i == 0:
                t_volt.append(i)
                vhi = int(charge_time)
                t_volt.append(vhi)
            else:
                if i < cycles:
                    vlo = i * 100
                    t_volt.append(vlo)
                    vhi = int((charge_time) + (i * 100))
                    t_volt.append(vhi)
                if i == cycles:
                    vlo = i * 100
                    t_volt.append(vlo)

        # print array of waveform voltage time intervals to output file
        print(t_volt, file=outfile)

        # print array of waveform raw data to output file
        print(triangle, file=outfile)

        # close output files
        outfile.close()

        return t, charge_time, cycles, triangle


    def calc_task(self, models, tasks):
        #determine tasks from given sets
        temp = 0
        task_set = []
        for i in range(0, len(models)):
            if models[i] == 0:
                temp = tasks[i]
            if models[i] == 1:
                temp = tasks[i] + 3
            if models[i] == 2:
                temp = tasks[i] + 6
            task_set.append(temp)

        return task_set


    def plot_data(self, t1, charge_t, cycles, wave):

        #parse through results.txt output file and determine tasks executed during each on period
        models = []
        tasks = []
        task_lst = []
        res_file = open('results.txt', "r")
        word_line = [line.strip() for line in res_file]  # separate lines within input text file
        for words in word_line:
            if 'New origin model is:' in words:
                word = words[-1]
                models.append(int(word))
            elif 'New origin task is:' in words:
                word = words[-1]
                tasks.append(int(word))
            elif '***POWER FAILURE***' in words:
                task_set = self.calc_task(models, tasks)
                task_lst.append(task_set)
                models = []
                tasks = []
            elif '***END***' in words:
                task_set = self.calc_task(models, tasks)
                task_lst.append(task_set)
                models = []
                tasks = []

        #plot waveform amplitude and tasks vs time
        t_lst = []
        for i in range(0, len(task_lst)):
            t_temp = np.linspace(charge_t + (100 * i), 100 + (100 * i), len(task_lst[i]))
            t_lst.append(t_temp)

        #configure plot for dual data lines
        fig, ax1 = plt.subplots()
        wave_color = 'blue'
        ax1.set_xlabel('Time (s)')
        ax1.set_ylabel('Amplitude', color=wave_color)
        ax1.plot(t1, wave, color=wave_color)
        ax1.tick_params(axis='y', labelcolor=wave_color)

        #setup second data line on same plot as above data line
        ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
        task_color = 'red'
        ax2.set_ylabel('Task #', color=task_color)  # we already handled the x-label with ax1

        #setup task data line based on number of cycles generated
        if cycles == 3:
            ax2.plot(t_lst[0], task_lst[0], t_lst[1], task_lst[1], t_lst[2], task_lst[2], marker='o', markerfacecolor='red', color=task_color)
        if cycles == 4:
            ax2.plot(t_lst[0], task_lst[0], t_lst[1], task_lst[1], t_lst[2], task_lst[2], t_lst[3], task_lst[3], marker='o', markerfacecolor='red', color=task_color)
        if cycles == 5:
            ax2.plot(t_lst[0], task_lst[0], t_lst[1], task_lst[1], t_lst[2], task_lst[2], t_lst[3], task_lst[3], t_lst[4], task_lst[4], marker='o', markerfacecolor='red', color=task_color)

        ax2.tick_params(axis='y', labelcolor=task_color)
        fig.tight_layout()  # otherwise the right y-label is slightly clipped
        plt.show()


    def test_chain_cpp(self):
        #test chain application
        print("Testing Chain-based application\n")

        #generate random voltage waveform
        t1, charge_t, cycles, wave = self.gen_waveform()

        #call Main executable with argument (waveform file)
        subprocess.check_call(["./Main", "waveform_data.txt"])


        #parse through results.txt output file and plot data
        self.plot_data(t1, charge_t, cycles, wave)


if __name__ == '__main__':

    suite = unittest.TestSuite()

    if len(sys.argv) == 2 and sys.argv[1] == "test-chain":
        suite.addTest(unittest.defaultTestLoader.loadTestsFromTestCase(ProjectCppTest))

    result = unittest.TextTestRunner().run(suite)

    # Set the exit code based on the test result
    sys.exit(not result.wasSuccessful())
