    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <fcntl.h>
	#include <string.h>
	
	int valor, pid1, pid2, pid3, fd0[2], fd1[2], fd2[2], cllegits, bllegits, fd_fitxer, status;
    char buffer[2000];
	
	
	int main(int argc, char *argv[])
	{	
		pipe(fd0);
		pipe(fd1);
		pipe(fd2);
		pid1 = fork();
		
		if (pid1 == 0) {
			pid2 = fork();

			if (pid2 == 0) {
				//Segunda pipe
				close(1);
				dup(fd1[1]);
				close(fd1[0]);
				close(fd1[1]);
				
				//Primera pipe
				close(fd0[0]);
				close(fd0[1]);
				
				//Tercera pipe
				close(fd2[0]);
				close(fd2[1]);
				
				//Comandos
				char *cat_arg = NULL;
				if (argc == 2) {
					cat_arg = malloc(strlen("/proc/") + strlen(argv[1]) + strlen("/status") + 1);
					strcpy(cat_arg, "/proc/");
					strcat(cat_arg, argv[1]);
					strcat(cat_arg, "/status");
					execl("/bin/cat", "cat", cat_arg, (char *)0 ); 

				}else {
					execl("/bin/cat", "cat", "/proc/300149/status", (char *)0 ); 
				}
	
				exit(1);
				
			}else{	
				//Primera pipe
				close(1);
				dup(fd0[1]);
				close(fd0[0]);
				close(fd0[1]);
				
				//Segunda pipe
				close(0);
				dup(fd1[0]);
				close(fd1[0]);
				close(fd1[1]);
				
				//Tercera pipe
				close(fd2[0]);
				close(fd2[1]);

				//Comandos;
				execl("/bin/grep", "grep", "Vm", (char *)0);
                
				exit(1);
			
			}

			
		}else {
			pid3 = fork();
			
			if (pid3 == 0) {
				close(0);
				dup(fd0[0]);
				close(fd0[0]);
				close(fd0[1]);
				
				//Segunda pipe
				close(fd1[0]);
				close(fd1[1]);
				
				//Tercera pipe
				close(1);
				dup(fd2[1]);
				close(fd2[0]);
				close(fd2[1]);
				
				execl("/bin/sort", "sort", "-nrk2", (char *)0);
				exit(1);
				
			}else {
				//Primera pipe
				close(fd0[0]);
				close(fd0[1]);
				
				//Segunda pipe
				close(fd1[0]);
				close(fd1[1]);
				
				//Tercera pipe
				close(0);
				dup(fd2[0]);
				close(fd2[0]);
				close(fd2[1]);
				
				bllegits=read(0,buffer,2000);
				fd_fitxer = open("./sortida.txt", O_WRONLY | O_CREAT | O_TRUNC, 00600);
				write(fd_fitxer, buffer, bllegits);
				
				exit(0);
			
			}
			
		}
	
	}