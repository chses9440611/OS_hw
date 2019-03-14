#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
using namespace std;

int main(){
	int key_value;
	int times = 100;
	int* value;
	int shmid;
	int s=0, t=0, p=0;
	string str, ta;
	cin >> key_value;

	shmid = shmget(key_value, 0, 0);
	value = (int*)shmat(shmid, NULL, 0);
	while(times--){
		cin >> str;
		if(str != "OK"){
			cout << "Cannt catch OK signal!" << endl;
			break;
		}
		if(*value == 0){
			cout << "Scissor" << endl;
			s++;
		}
		else if (*value == 1){
			cout << "Stone" << endl;
			t++;
		}
		else if (*value == 2){
			cout << "Paper" << endl;
			p++;
		}
		else
			cout << "You cheat me!!" << endl;

		cout.flush();
		cin >> ta;
		
	}
	return 0;
}

