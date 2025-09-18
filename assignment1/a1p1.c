#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h> //USED for 
#include <stdlib.h>


int main()
{

	pid_t pid, pid_child_search;
	pid_t pid_list[100];
	int fromChild;
	char delimiter_value ;
	int exit_value;
	int input_matrix[100][1000];
	int matrix_row = 0;
	int matrix_col = 0;
	int treasure_check = 0, treasure_col, treasure_row; 

	

	for(int row =0; row<100; row++){
		for(int col =0; col<1000; col++){
			if(scanf("%d", &input_matrix[row][col]) != 1) {
				fprintf(stderr, "Invalid Input file\n");
				exit(-1);
			}

		 }
		
	}
	
	


	
	while(matrix_row != 100){
		pid = fork();
		if (pid < 0) {
			fprintf(stderr, "Fork for Child %d failed", matrix_row);
			exit(-1);
		}
		else if (pid == 0){
			matrix_col = 0;
			exit_value = 0; 
			printf("Child %d (PID %d): Searching row %d\n", matrix_row, getpid(), matrix_row);
			
			// sleep(1); // allow parent to wait
			// printf("Enter an integer between 0 and 255: ");
			while(matrix_col != 1000){
				//printf("%c%c", matrix_value, delimiter_value);
				if(input_matrix[matrix_row][matrix_col]) {
					printf("value= %d,", input_matrix[matrix_row][matrix_col]);
					printf(" COLUMN = %d\n", matrix_col);
					printf(" row = %d\n", matrix_row);
					exit_value = 1;	
					
				}
				
				matrix_col++;
				
			}
			
			exit(exit_value);

		}
		else {
			pid_list[matrix_row] = pid;
		}
		
		matrix_row++;
		
	}
	
	for(int row =0 ; row < 100; row++) {
		waitpid(pid_list[row], &fromChild, 0);
		if(WEXITSTATUS(fromChild)){
			for(int col =0 ; col < 1000; col++) {
				if(input_matrix[row][col]) {
					treasure_col = col;
					treasure_row = row;
					treasure_check = 1;
					break;
				}
			}
		}
	
	}
	if(!treasure_check) fprintf(stderr, "Parent: No treasure found within the input matrix\n");
	else printf("Parent: The treasure was found by child with PID %d at row %d and column %d\n", pid_list[treasure_row], treasure_row,treasure_col);
	
	
	
	printf("The parent is done\n");
	return 1;
}
