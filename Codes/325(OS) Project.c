#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h> // for true in while condition

#define MAX_STUDENT 100
#define MAX_TUTOR 10
#define MAX_CHAIR 25

	/*Initialiazations , Semaphores & Threads*/
	
int total_chair, total_student, total_tutor, help_limit;
int req_order = 0, helped_student = 0, occupied_chair = 0;

int priority[MAX_STUDENT], visited_q[MAX_STUDENT], total_visit[MAX_STUDENT];
int tutor_ID[MAX_TUTOR], student_ID[MAX_STUDENT], time_arrive[MAX_STUDENT];

sem_t student_s, coordinator_s, tutored_student[MAX_STUDENT], mutex_lock;

//pthread_mutex_t Locked_seat, Locked_q, Locked_tutor_q;

	/*Thread Function Pointers*/
	
void *student_t(void *arg){
	int student_id = *(int *) arg;
	
	while(true){
		if(total_visit[student_id - 1] == help_limit){
			sem_wait(&mutex_lock);
			helped_student++;
			sem_post(&mutex_lock);
			printf("\n!!!Student - %d terminated (Help Limit Reached))\n", student_id);
			if(helped_student == total_student)
				printf("\nCo-ordinator: All students have received Help.\n");
			sem_post(&student_s);
			pthread_exit(NULL);
		}
		
		sem_wait(&mutex_lock);
		if(occupied_chair == total_chair){
			sem_post(&mutex_lock);
			continue;
		}
		occupied_chair++;
		req_order++;
		visited_q[student_id-1] = req_order;
		printf("\n||   Student %d   || Seated. Available Chairs: %d", student_id, (total_chair-occupied_chair));
		sem_post(&mutex_lock);
		sem_post(&student_s);
		sem_wait(&tutored_student[student_id-1]);
		
		printf("\n||   Student %d   || received Help.", student_id);
		sem_wait(&mutex_lock);
		total_visit[student_id - 1]++;
		printf("\n||  Student %d  || Priority: %d", student_id, total_visit[student_id - 1]);
		sem_post(&mutex_lock);
	}
}

void *tutor_t(void *arg){
	int tutor_id = *(int *) arg;
	
	while(true){
		if(helped_student == total_student){
			sem_wait(&mutex_lock);
			printf("\n!!!Tutor - %d has finished Task.\n", tutor_id);
			sem_post(&mutex_lock);
			pthread_exit(NULL);
		}
		int max_req = total_student * help_limit + 1;
		int max_priority = help_limit - 1;
		int student_id = -1;
		sem_wait(&coordinator_s);
		sem_wait(&mutex_lock);
		for(int i = 0; i < total_student; i++){
			if(priority[i] > -1 && priority[i] <= max_priority){
				if(time_arrive[i] < max_req){
					max_priority = priority[i];
					max_req = time_arrive[i];
					student_id = student_ID[i];
				}
			}
		}
		if(student_id == -1){
			sem_post(&mutex_lock);
			continue;
		}
		priority[student_id - 1] = -1;
		time_arrive[student_id - 1] = -1;
		occupied_chair--;
		sem_post(&mutex_lock);
		sem_wait(&mutex_lock);
		printf("\n[   Tutor %d   ] tutoring Student-%d.", tutor_id,student_id);
		sem_post(&mutex_lock);
		sem_post(&tutored_student[student_id - 1]);
	}
}

void *coordinator_t(void *arg){
	while(true){
		if(helped_student == total_student){
			for (int i = 0; i < total_tutor; i++)
				sem_post(&coordinator_s);
			printf("\n!!!Co-ordinator terminated!!!\n");
			pthread_exit(NULL);
		}
		
		sem_wait(&student_s);
		sem_wait(&mutex_lock);
		for (int i = 0; i < total_student; i++){
			if(visited_q[i] > -1){
				priority[i] = total_visit[i];
				time_arrive[i] = visited_q[i];
				printf("\n<<Co-ordinator>> Student- %d  of Priority- %d is NEXT in LINE.", student_ID[i], total_visit[i]);
				visited_q[i] = -1;
				sem_post(&coordinator_s);
			}
		}
		sem_post(&mutex_lock);
	}
}

void start_simulation(){
	
	printf("\n\t\t======================================\n");
	printf("\t\t||	Welcome to Tutoring Program  ||\n");
	printf("\t\t======================================\n");	
	
	printf("\nEnter Total Number Of Students: ");
	scanf("%d", &total_student);
	
	if(total_student > MAX_STUDENT || total_student < 1){
		printf("Student Limit Exceeded (not more than 100 & non-negative).\nEnter value again: ");
		scanf("%d", &total_student);
	}
	
	printf("Enter Total Number Of Tutors: ");
	scanf("%d", &total_tutor);
	
	if(total_tutor > MAX_TUTOR || total_tutor < 1){
		printf("Tutor Limit Exceeded (not more than 10 & non-negative).\nEnter value again: ");
		scanf("%d", &total_tutor);
	}
	
	printf("Enter Total Number Of Chairs: ");
	scanf("%d", &total_chair);
	
	if(total_chair > MAX_CHAIR || total_chair < 1){
		printf("Chair Limit Exceeded (not more than 25 & less than 2).\nEnter value again: ");
		scanf("%d", &total_chair);
	}
	
	printf("Maximum Number of help a student can ask for: ");
	scanf("%d", &help_limit);
	
		/*Semaphores Initialization*/
	sem_init(&student_s, 0, 0);
	sem_init(&coordinator_s, 0, 0);
	sem_init(&mutex_lock, 0, 1);
	
	for(int i = 0; i < total_student; i++){
		visited_q[i] = -1;
		priority[i] = -1;
		time_arrive[i] = -1;
		total_visit[i] = 0;
		student_ID[i] = i + 1;
		sem_init(&tutored_student[i], 0, 0);
	}
	
		/*Thread Declaration & Creation*/
	pthread_t student[MAX_STUDENT], tutor[MAX_TUTOR], coordinator_t_id;
	
	for(int i = 0; i < total_student; i++)
		pthread_create(&student[i], NULL, student_t, (void*)&student_ID[i]);
	
	for(int i = 0; i < total_tutor; i++){
		tutor_ID[i] = i + 1;
		pthread_create(&tutor[i], NULL, tutor_t, (void*)&tutor_ID[i]);
	}
	
	pthread_create(&coordinator_t_id, NULL, coordinator_t, NULL);
	
	
	/*Thread Joining*/
	for(int i = 0; i < total_student; i++)
		pthread_join(student[i], NULL);
	
	for(int i = 0; i < total_tutor; i++)
		pthread_join(tutor[i], NULL);
		
	pthread_join(coordinator_t_id, NULL);
}

	/*MAIN FUNCTION*/
int main(){
	int menu;
	printf("\n\t::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	printf("\t\t\tProgramming Club Management System\n");
	printf("\t::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	while(1){
		printf("\t...........................Option Board...........................\n\n");
	
		printf("\t\t\t-----------------------------------\n");
		printf("\t\t\t  Program Simulation		1\n");
		printf("\t\t\t  Program Terminate		2\nYour Choice: ");
		scanf("%d", &menu);
		
		switch(menu){
			case 1:
				start_simulation();
				break;
			case 2:
				printf("\n\t\t^_^ Thank you for coming. ^_^\n");
				exit(1);
				break;
			default:
				printf("\n\t\t!!! Wrong Input. Try Again !!!!\n");
				break;
		}
	}
	return 0;
}

