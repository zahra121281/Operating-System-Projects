#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define NUM_THREADS 1000
pthread_t threads[NUM_THREADS];


typedef struct {
    char* search_query;
    char* path2file ; 
    // int num ; 
} SearchInfo;


void print_struct(SearchInfo* s )
{
    printf("%s %s\n" , s->search_query , s->path2file ) ; 
}


int match(const char* text, const char* pattern) {
    int textLen = strlen(text);
    int patternLen = strlen(pattern);
    int j, i ;
    for (i = 0; i <= textLen - patternLen; i++) {

        // Check if pattern matches the text starting at position i
        for (j = 0; j < patternLen; j++) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }

        if (j == patternLen) {
            // Pattern found at position i
            return i;
        }
    }
    return -1;
}


char* AppendString(char* str1 , char* str2 )
{   
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char slash = '/' ; 
    
    char* merged = (char*)malloc((len1 + len2 + 3) * sizeof(char));
    if (merged == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    strcpy(merged, str1); // Copy the first string to the merged string 
    //strcat(merged, &slash) ;  
    strcat(merged, str2); // Concatenate the second string to the merged string 
    return merged;
}


void* Search(void* args )
{
    SearchInfo * Info = (SearchInfo*) args ; 
    char* search_query = Info->search_query ;   
    int t_count = 0 ; 
    char* c = "/" ; 
    char* path2file = AppendString( Info->path2file , c ) ;
    DIR *dir = opendir(path2file);
    //printf("%s\n" , path2file) ; 
    if (dir == NULL) {
        return NULL ; 
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (entry->d_type != DT_DIR) 
        {
            if( match( entry->d_name , search_query ) != -1  )
            {
                char* new_path_to_file = AppendString( path2file , entry->d_name ) ; 
                printf("[+] Found:\n %s\n", new_path_to_file );
            }
        } 
        else
        {
            char* new_path = AppendString( path2file , entry->d_name ) ; 
            SearchInfo new_info = { search_query , new_path } ; 
            int out = pthread_create(&threads[t_count] , NULL,Search,&new_info);
            t_count +=1 ; 
        }
        
    }
    closedir(dir);

    for (int i = 0; i < t_count ; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
}

int main(int argc , char* argv[])
{
    char* start_dir= argv[2];
    char* search_query=argv[3];
    char command[] = "search" ; 
    // scanf("search %s %s", start_dir, search_query);
    SearchInfo searchInfo = {search_query , start_dir  } ; 
    if ( strcmp( command , argv[1]) == 0 ) 
        Search(&searchInfo) ; 
    else
        printf("Incorrect command!!") ; 
    return 0 ; 
}




