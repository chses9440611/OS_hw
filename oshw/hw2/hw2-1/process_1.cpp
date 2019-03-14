#include <cstdio>
#include <ctime>
#include <iostream>
#include <stack>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

void createProcess(void);
void deleteProcess(void);

int main() {
  int cpid;
  int count = 0;
  stack<int> spid;

  cout << "The process pid " << getpid()
       << " is a main process with bash parent pid " << getppid() << endl;
  cpid = fork();
  if(cpid == 0)
	  createProcess();

  count ++;

  while(cpid == 0 && count < 10){
	cpid = fork();
	if(cpid == 0)
		createProcess();
	count++;
  }
	waitpid(cpid, NULL, 0);
	deleteProcess();
  return 0;
}

void createProcess(void) {
  cout << "The process pid " << getpid() << " with parent pid " << getppid()
       << " create" << endl;
  return;
}

void deleteProcess(void) {
	cout << "The process pid " << getpid() << " dead" << endl;
	return;
}
