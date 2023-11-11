#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int wc(char* file_path, char* word){
    FILE *fp;
    int count = 0;
    int ch, len;
    
    if(NULL==(fp=fopen(file_path, "r")))
        return -1;
    len = strlen(word);
    for(;;){
        int i;
        if(EOF==(ch=fgetc(fp))) break;
        if((char)ch != *word) continue;
        for(i=1;i<len;++i){
            if(EOF==(ch = fgetc(fp))) goto end;
            if((char)ch != word[i]){
                fseek(fp, 1-i, SEEK_CUR);
                goto next;
            }
        }
        ++count;
        next: ;
    }
end:
    fclose(fp);
    return count;
}


int main( int argc , char* argv[])
{
   
    char** queries = argv ; 
    int res = 0 ; 
    char * file = argv[1] ; 

    int p[2], nbytes , r ,pid ; 
  
    for (int i = 2 ; i < argc ; i++)
    {
        if(pipe(p) == -1)
        {
            perror( "pipe Failed" );
            continue;
        }

        pid = fork();

        if(pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if(pid == 0)
        { //child code
            close(p[0]);
            int wordcount = wc(file , argv[i]);
            write(p[1],&wordcount ,4 ); 
            printf("query%d: %d\n" , i-1 , wordcount) ; 
            exit(0);
        }

        else{//parent
            
            close(p[1]);
            if (read(p[0],&r,4) <= 0) //read from pipe
            {
                perror( "read failed" );
                exit( EXIT_FAILURE );
            }
            //printf("Im parent %i\n",getpid());
            //printf("r : %d\n",r);
            res += r ; 
            wait(NULL);
        }
    }
    printf("Total occurrences: %d\n", res);
}

