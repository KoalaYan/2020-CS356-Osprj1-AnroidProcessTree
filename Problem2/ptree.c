#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/unistd.h>

#define __NR_pstreecall 356

struct prinfo{
	pid_t parent_pid;	//process id of parent
	pid_t pid;		//process id
	pid_t first_child_pid;	//pid of youngest child
	
	pid_t next_sibling_pid;	//pid of older sibling
	long state;		//current state of process
	long uid;		//user id of process owner
	char comm[64];		//name of program executed
};


void printTree(struct prinfo *buf,int *nr)
{
	//record the generation of each process
	int gene[500] = {0};
	int tmp_pid;
	for(int i = 1;i < *nr;i++){
		tmp_pid = buf[i].parent_pid;
		gene[i] = gene[tmp_pid] + 1;
	}

	//print the tree
	for(int i = 0;i < *nr;i++){
		for(j =0 ;j < i;j++)
			printf('\t');
		printf("%s,%d,%ld,%d,%d,%d,%d\n",buf[i].comm,buf[i].pid,buf[i].state,buf[i].parent_pid,
		buf[i].first_child_pid,buf[i].next_sibling_pid,buf[i].uid);
	}
}

int main(){
	//Memory allocation
	struct print *buf = malloc(500*sizeof(struct prinfo));
	int *nr = malloc(sizeof(int));
	if(buf == NULL || nr == NULL){
		printf("Fail to allocate memory.\n");
		exit(-1);
	}

	//system call
	syscall(__NR_pstreecall,buf,nr);

	//print the process tree with DFS
	printTree(buf,nr);

	//Reclaim allocated memory
	free(buf);
	free(nr);
	
	return 0;
}