#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
int main(int argc ,char* argv[]){
    int pidx=0,pidy=0,pidz=0;
    printf("create main process %d\n",getpid());
    printf("process %d create process %d\n",getpid(),pidx=fork());
    printf("process %d create process %d\n",getpid(),pidy=fork());
    printf("process %d create process %d\n",getpid(),pidz=fork());


	if(pidz == 0){
		pidx = 0;
		pidy = 0;
	}
	else if(pidy == 0){
		waitpid(pidz, NULL, 0);
		pidx = 0;
	}
	else if(pidx == 0) {
		waitpid(pidy, NULL, 0);
		waitpid(pidz, NULL, 0);
	}
	else{
		waitpid(pidx, NULL, 0);
		waitpid(pidy, NULL, 0);
		waitpid(pidz, NULL, 0);

	}


    printf("process %d deaded its child process %d %d %d\n",getpid(),pidx,pidy,pidz);
}
