#include <iostream>
#include <list>
#include "Simulation.h"

using namespace std;

/////////////////
// Constructor
/////////////////

Simulation::Simulation(int windowCount, list<Student> studentList)
{
  allStudents = studentList;
  for(int i=0 ; i<windowCount ; ++i)
  {
    Window tempWindow;
    windows.push_front(tempWindow);
  }


  cout << "allStudents contains elements: " << allStudents.size() << endl;


  int currentTime=0;
}



//////////////////////
// Timing Function
//////////////////////

// iterates simulation by 1 time unit
void Simulation::tick()
{
  // first, make moves:
  //    (out of line) -> (line)
  //    (line) -> (windows)
  cout << "Moving students from nowhere to line... ";
  insertStudents();
  cout << "Done!" << endl;
  cout << "Moving students from line to window... " << endl;
  fillWindows();
  cout << "Done!" << endl;


  cout << "Ticking for all students..." << endl;
  // then, iterate time for all students in line
  for(list<Student>::iterator s=allStudents.begin(); s != allStudents.end(); ++s)
  {
    (*s).tick();
  }
  cout << "Done!" << endl;

  cout << "Checking all windows..." << endl;
  for(list<Window>::iterator w=windows.begin(); w != windows.end(); ++w)
  {
    cout << "    Checking a window..." << endl;
    (*w).checkStudent();
    cout << "    Done!" << endl;
  }
  cout << "Done!" << endl;

  ++currentTime;
}



/////////////////////////
// Migration Functions
/////////////////////////

// check allStudents for any students ready to join the line
void Simulation::insertStudents()
{
  cout << "Attempting to insert students" << endl;
  for(list<Student>::iterator s=allStudents.begin(); s != allStudents.end(); ++s){
    if ((*s).timeActivation == currentTime && !(*s).activated){
      cout <<
      Student *studentPtr = &(*s);
      studentPtr->activated = true;
      studentLine.enqueue(studentPtr);

    }
  }
}

// check for empty windows and insert the next students from the line into them
// then, check if a student is done at a window to remove them and record their stats
void Simulation::fillWindows()
{
  cout << "    Beginning to fill windows..." << endl;
  for(list<Window>::iterator w=windows.begin(); w != windows.end(); ++w){
    if (studentLine.empty())
    {
      cout << "    The queue is empty, no windows will be filled" << endl;
      continue;
    }
    if ((*w).student == NULL){
      cout << "    Passing pointer from queue to window...";
      (*w).enterStudent(studentLine.dequeue());
      cout << "Done!" << endl;
    }
    if ((*w).student->timeRequired == 0)
    {
      (*w).removeStudent();
    }
    cout << "    Student added to window" << endl;
  }
}



//////////////////////////
// Statistics Functions
//////////////////////////

// determines if all students have been helped
bool Simulation::simulationDone()
{
  int unfinished = 0;
  bool finished = true;
  for(list<Student>::iterator s=allStudents.begin(); s != allStudents.end(); ++s){
    if ((*s).timeRequired != 0)
      finished = false;
      ++unfinished;
  }
  cout << unfinished << " students are not done yet" << endl;
  return finished;
}


// collect all statistics of the simulation and print information to console
void Simulation::printStatistics()
{
// collect data from simulation
  list<int> studentIdleTimes;
  for(list<Student>::iterator s=allStudents.begin(); s != allStudents.end(); ++s)
    studentIdleTimes.push_front((*s).timeIdle);

  list<int> windowIdleTimes;
  for(list<Window>::iterator w=windows.begin(); w != windows.end(); ++w)
    windowIdleTimes.push_front((*w).timeIdle);

// calculate values

  //students
  double sMean = 0;
  int sIdleOverTen = 0;
  int sLongestWait = 0;
  for(list<int>::iterator i=studentIdleTimes.begin(); i != studentIdleTimes.end(); ++i)
  {
    sMean += *i;
    if (*i > 10)
      ++sIdleOverTen;
    if (*i > sLongestWait)
      sLongestWait = *i;
  }
  sMean = (sMean / (double)studentIdleTimes.size());

  double median = 0;
  studentIdleTimes.sort();
  while (studentIdleTimes.size() > 2){
    studentIdleTimes.pop_back();
    studentIdleTimes.pop_front();
  }
  for(list<int>::iterator i=studentIdleTimes.begin(); i != studentIdleTimes.end(); ++i){
    median+=*i;
  }
  median = (median / (double)studentIdleTimes.size());

  //windows
  double wMean = 0;
  int wIdleOverFive = 0;
  int wLongestWait = 0;
  for(list<int>::iterator i=windowIdleTimes.begin(); i != windowIdleTimes.end(); ++i)
  {
    wMean += *i;
    if (*i > 10)
      ++wIdleOverFive;
    if (*i > wLongestWait)
      wLongestWait = *i;
  }
  wMean = (wMean / (double)windowIdleTimes.size());

  cout << "          Simulation Statistics" << endl
       << "-----------------------------------------" << endl
       << "Mean Student Wait Time: " << sMean << " minutes" << endl
       << "Median Student Wait Time: " << median << " minutes" << endl
       << "Longest Student Wait Time: " << sLongestWait << " minutes" << endl
       << "Number of Students who waited over 10 minutes: " << sIdleOverTen << endl
       << "Mean Window Idle Time: " << wMean << " minutes" << endl
       << "Longest Window Idle Time: " << wLongestWait << endl
       << "Number of Windows that waited over 5 minutes: " << wIdleOverFive << endl;
}
