#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>

using namespace std;

struct process{
	int pid;
	string proc_name;
	double mem_usage;
	long min_flt;
	long maj_flt;
	process(int _pid, string _name, double _mem, long _minf, long _maj) : 
	pid{_pid}, proc_name{_name}, mem_usage{_mem}, min_flt{_minf}, maj_flt{_maj}{}
};

vector<double> meminfo(){
	ifstream file("/proc/meminfo", ifstream::in);
	string input, output;

	vector<double> info;
	
	int iterator = 0;
	double total, free, buffers, cached, sreclaimable, shmem, swaptotal, swapfree;
	while(getline(file, input)){
		stringstream ss;
		if(iterator == 0){
			ss << input; 
			ss >> output;
			ss >> output;
			total = stod(output) / 1024;
		}else if(iterator == 1){
			ss << input; 
			ss >> output;
			ss >> output;
			free = stod(output) / 1024;
		}else if(iterator == 3){
			ss << input; 
			ss >> output;
			ss >> output;
			buffers = stod(output) / 1024;
		}else if(iterator == 4){
			ss << input; 
			ss >> output;
			ss >> output;
			cached = stod(output) / 1024;
		}else if(iterator == 14){
			ss << input; 
			ss >> output;
			ss >> output;
			swaptotal = stod(output) / 1024;
		}else if(iterator == 15){
			ss << input; 
			ss >> output;
			ss >> output;
			swapfree = stod(output) / 1024;
		}else if(iterator == 20){
			ss << input; 
			ss >> output;
			ss >> output;
			shmem = stod(output) / 1024;
		}else if(iterator == 22){
			ss << input; 
			ss >> output;
			ss >> output;
			sreclaimable = stod(output) / 1024;
			break;
		}
		++iterator;
	}
	
	info.push_back(total - free - (cached + sreclaimable - shmem) - buffers); // ram
	info.push_back(swaptotal - swapfree); //swap
	info.push_back(cached + sreclaimable - shmem); // cache

	info[0] = info[0] / total * 100; // ram
	info[1] = info[1] / swaptotal * 100; // swap

	return info;
}

vector<process> process_by_mem_usage(){
	system("ps axo pid,comm,%mem,min_flt,maj_flt --sort=-%mem --no-header > process_by_mem_usage.txt");
	
	ifstream file("process_by_mem_usage.txt", ifstream::in);
	string input,comm;
	int pid;
	double mem;
	long maj_flt, min_flt;

	stringstream ss;
	vector<process> proc;

	while(getline(file, input)){
		ss << input;
		ss >> pid;
		ss >> comm;
		ss >> mem;
		ss >> min_flt;
		ss >> maj_flt;
		proc.push_back( process(pid, comm, mem, min_flt, maj_flt) );
		ss.str(string());
		ss.clear();
	}

	//system("rm process_by_mem_usage.txt");
	return proc;
}

double mem_usage(int pid){
	string command = "ps --pid " + to_string(pid) + " o %mem --no-header > mem_usage.txt";
	system(command.c_str());

	ifstream file("mem_usage.txt", ifstream::in);
	string input;
	getline(file, input);

	//system("rm mem_usage.txt");
	return stod(input);

}
