#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <sys/wait.h>
#include <stack>
using namespace std;

// The main process fork times is FORK_TIME
// Each child will fork a child process.
// The total num of child process is 5 x 2 = 10.
#define FORK_TIME 5
void createProcess(void);
void deleteProcess(int);

int main(){
	int cpid;
	int count = FORK_TIME;
	stack<int> spid;

	cout << "The process pid " << getpid() << " is a main process with bash parent pid " << getppid() << endl;
	// The first fork in main process
	cpid = fork();
	// Store the first child pid
	spid.push(cpid);
	count -= 1;

	while(cpid > 0 && count > 0) {
		cpid = fork();
		while(cpid < 0)
			cpid = fork();

		if(cpid == 0) {
			// In child process of main process
			break;
		}else {
			spid.push(cpid);
			count -= 1;
		}
	}

	if(cpid == 0) {
		// Child process in main process will fork a new child process.
		cpid = fork();
		while(cpid < 0 )
			cpid = fork();
		// Child process delcare the creation in own process
		createProcess();
		if(cpid > 0){
			cpid = waitpid(cpid, NULL, 0);
			// The parent process declare the dead of child process.
			deleteProcess(cpid);
		}
	}
	else {
		while(!spid.empty()){
			cpid = spid.top();
			spid.pop();
			cpid = waitpid(cpid, NULL, 0);
			deleteProcess(cpid);
		}
		// The main process declare the dead of child process.
		deleteProcess(getpid());
	}

	return 0;
}

void createProcess(void){
	cout << "The process pid " << getpid() << " with parent pid " << getppid() << " create" << endl;
	return;
}

void deleteProcess(int childPid){
	if(childPid == -1 )
		cout << "ChildProcess of Parent pid " << getpid() << "died in error" << endl;
	else
		cout << "The process pid " << childPid << " dead" << endl;

	return;
}

