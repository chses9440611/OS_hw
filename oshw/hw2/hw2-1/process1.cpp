#include <cstdio>
#include <ctime>
#include <iostream>
#include <stack>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

// The main process fork times is FORK_TIME
// Each child will fork a child process.
// The total num of child process is 5 x 2 = 10.
#define FORK_TIME 5
void createProcess(void);
void deleteProcess(void);

int main() {
  int cpid;
  int count = FORK_TIME;
  stack<int> spid;

  cout << "The process pid " << getpid()
       << " is a main process with bash parent pid " << getppid() << endl;
  // The first fork in main process
  cpid = fork();
  // Store the first child pid
  spid.push(cpid);
  count -= 1;

  while (cpid > 0 && count > 0) {
    cpid = fork();
    while (cpid < 0)
      cpid = fork();

    if (cpid == 0) {
      // In child process of main process
      break;
    } else {
      spid.push(cpid);
      count -= 1;
    }
  }

  if (cpid == 0) {
    // Child process in main process will fork a new child process.
    cpid = fork();
    while (cpid < 0)
      cpid = fork();
    // Child process delcare the creation in own process
    createProcess();
    if (cpid > 0) {
    	if( waitpid(cpid, NULL, 0) == -1)
		  cout<< "Error occurs in child process " << cpid << endl;
    }
  } else {

    while (!spid.empty()) {
      cpid = spid.top();
      spid.pop();
      if( waitpid(cpid, NULL, 0) == -1)
		  cout << "Error occurs in child process " << cpid << endl;
    }
  }
  	// Process declare delete then return 0
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
