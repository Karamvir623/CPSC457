#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int arg_c, char **pp_arg_v)
{
	pid_t pid, pid_child_search;
	int fromChild;
	char delimiter_value ;
	int exit_value;
	char input_matrix[100][1000];
	for (int i = 0; i < 3; i++) {
        printf("argv[%d] = %s\n", i, pp_arg_v[i]);
    }
	

	for(int row =0; row<100; row++){
		printf("ROW: %d\n", row);
		for(int col =0; col<1000; col++){
			if(scanf("%c%c", &input_matrix[row][col], &delimiter_value) == 1) printf("ERROR");
			printf("%c%c", input_matrix[row][col], delimiter_value);
			
		 }
		
	}
	
	


	int matrix_row = 0;
	int matrix_col = 0;
	while(matrix_row != 100){
		pid = fork();
		if (pid < 0) {
			fprintf(stderr, "Fork Failed");
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
				if(input_matrix[matrix_row][matrix_col] == '1') {
					printf("value= %c,", input_matrix[matrix_row][matrix_col]);
					printf(" COLUMN = %d\n", matrix_col);
					printf(" row = %d\n", matrix_row);
					exit_value = matrix_col;	
				}
				
				matrix_col++;
				
			}
			
			exit(exit_value);

		}
		
		matrix_row++;
		
	}
	int treasure_row = 0;
	int treasure_col;
	sleep(1);
	while (pid > 0) {
		pid_child_search = wait(&fromChild);
		treasure_col = WEXITSTATUS(fromChild); 
		if(treasure_col > 0) {
			printf("Parent: The treasure was found by child with PID %d at row %d and column %d\n", pid_child_search, treasure_row,treasure_col);
			break;
		}
		treasure_row++;
		
	
	}
	
	
	
	
	printf("The parent is done\n");
	exit(0);
}
