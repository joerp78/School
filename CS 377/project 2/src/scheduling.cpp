#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>

using namespace std;

pqueue_arrival read_workload(string filename) {
  pqueue_arrival workload;
  string text;
  ifstream MyReadFile(filename);
  
  if (!MyReadFile.is_open()) {
    cerr << "Error opening the file!";
    return workload; 
  }

  while(getline(MyReadFile, text)){
    while (!text.empty() && isspace(text.back())){
      text.pop_back();
    }

    string word = "";
    Process process; 
    bool arrivalSet = false; 
    for (char c : text)
    {
      if (c == ' ' && !arrivalSet)
      {  
        process.arrival = stoi(word);
        word = "";
        arrivalSet = true;
      }
      else {
        word += c; 
      }
    }
    process.duration = stoi(word);
    process.first_run = -1;
    cout << "ARRIVAL: " << process.arrival << "    DURATION: " << process.duration << endl;
    workload.push(process);
  }
  //show_workload(workload);
  MyReadFile.close();
  return workload;
}

void show_workload(pqueue_arrival workload) {
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty()) {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes) {
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty()) {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion
         << endl;
    xs.pop_front();
  }
}

list<Process> fifo(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_arrival work = workload;
  int time = 0; 

while (!work.empty()){
  if (work.top().arrival <= time)
  {
    Process p = work.top();
    p.first_run = time;
    time += p.duration; 
    p.completion = time;
  
    work.pop();
    complete.push_back(p);  }
  else {
    time = work.top().arrival; 
  }
}


  return complete;
}

list<Process> sjf(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_arrival work = workload;
  pqueue_duration sorted; 
  int time = 0;

  while(!work.empty() || !sorted.empty()){
    while(!work.empty() && work.top().arrival <= time)
    {
      Process p = work.top();
      sorted.push(p);
      work.pop(); 
    }

    if (sorted.empty() && !work.empty()){
      time = work.top().arrival;
      continue; 
    }

    while(!sorted.empty()){
      Process p = sorted.top();
      p.first_run = time;
      time += p.duration; 
      p.completion = time;

      sorted.pop();
      complete.push_back(p);
    }
  }

  return complete;
}

list<Process> stcf(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_arrival work = workload;
  pqueue_duration sorted; 
  int time = 0;
  Process* running = nullptr;
  int remaining = 0;
  //cout << '1' << endl;

  while(!work.empty() || !sorted.empty() || running != nullptr){
    while(!work.empty() && work.top().arrival <= time)
    {
      //cout << '2' << endl;

      Process p = work.top();
      work.pop();
      sorted.push(p);
    }

    if (running && !sorted.empty() && sorted.top().duration < remaining) {
      //cout << '3' << endl;

      Process preempted = *running;     
      preempted.duration = remaining;
      sorted.push(preempted);
      delete running;
      running = nullptr; 
    }

    if (!sorted.empty() && !running){
      //cout << '4' << endl;

      Process next = sorted.top();
      sorted.pop();
      running = new Process(next);
      if(running->first_run == -1){
        //cout << '5' << endl;

        running->first_run = time;
      }
      remaining = next.duration; 
    }

    if (running){
      time++;
      remaining--;
      //cout << '6' << endl;

      if (remaining == 0){
        running->completion = time;
        complete.push_back(*running);
        delete running;
        running = nullptr;
        //cout << '7' << endl;

      } 
    } else {
      time++;
      //cout << '8' << endl;
    }
  }
  //cout << '9' << endl;
  return complete;
}

list<Process> rr(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_arrival work = workload;
  queue<Process> ready; 
  int time = 0;
  Process* running = nullptr;
  int remaining = 0;
  int tQuantum = 1; 
  int qRemaining = tQuantum; 


  while(!work.empty() || !ready.empty() || running != nullptr)
  {
    while(!work.empty() && work.top().arrival <= time)
    {
      //cout << '2' << endl;
      Process p = work.top();
      work.pop();
      ready.push(p);
    }

    if (running && qRemaining == 0) {
      //cout << '3' << endl;
      if (remaining > 0){
      Process preempted = *running;     
      preempted.duration = remaining;
      ready.push(preempted);
      }
      delete running;
      running = nullptr; 
      qRemaining = tQuantum;
    }

    if (!ready.empty() && !running){
      //cout << '4' << endl;
      Process next = ready.front();
      ready.pop();
      running = new Process(next);
      if(running->first_run == -1){
        //cout << '5' << endl;
        running->first_run = time;
      }
      remaining = next.duration; 
      qRemaining = tQuantum; 
    }

    if (running){
      time++;
      remaining--;
      qRemaining--; 
      //cout << '6' << endl;

      if (remaining == 0){
        running->completion = time;
        complete.push_back(*running);
        delete running;
        running = nullptr;
        //cout << '7' << endl;

      } 
    } else {
      time++;
      //cout << '8' << endl;
    }
  }
  //cout << '9' << endl;
  return complete;
}

float avg_turnaround(list<Process> processes) {
  int size = processes.size();
  int TATSum = 0;
  float avgTAT;

  for(auto it = processes.begin(); it != processes.end(); ++it){
    Process p = *it;
    int TAT = 0;
    TAT = p.completion - p.arrival;
    TATSum += TAT;
  }

  avgTAT = static_cast<float>(TATSum)/size; 

  return avgTAT;
}

float avg_response(list<Process> processes) {
  int size = processes.size();
  int ResSum = 0;
  float avgRes;

  for(auto it = processes.begin(); it != processes.end(); ++it){
    Process p = *it;
    ResSum += p.first_run;
  }

  avgRes = ResSum/size; 

  return avgRes;
}

void show_metrics(list<Process> processes) {
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_t << endl;
  cout << "Average Response Time:   " << avg_r << endl;
}
