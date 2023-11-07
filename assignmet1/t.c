#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MaxNameLength 55
#define MaxEmployeeCount 200

void Add(char Employees[MaxEmployeeCount][MaxNameLength], char* name, int position, int Size){
    int i ; 
	for( i  = Size - 1; i >= position; i--){
		char temp[MaxEmployeeCount];
		strcpy(temp, Employees[i]);
		strcpy(Employees[i], Employees[i + 1]);
		strcpy(Employees[i + 1], temp);
	}
	strcpy(Employees[position], name);
}

void Swap(char Employees[MaxEmployeeCount][MaxNameLength], char* name1, char* name2, int Size){
	int index1;
	int index2;
    int i ; 
	for( i  = 0; i < Size; i++){
		if(!strcmp(Employees[i], name1))
			index1 = i;
		if(!strcmp(Employees[i], name2))
			index2 = i;
	}
	char temp[MaxEmployeeCount];
	strcpy(temp, Employees[index1]);
	strcpy(Employees[index1], Employees[index2]);
	strcpy(Employees[index2], temp);
}
void Delete(char Employees[MaxEmployeeCount][MaxNameLength], char* target, int Size){
	int ripple = 0;
    int i ; 
	for( i  = 0; i < Size - 1; i++)
		if(!strcmp(Employees[i], target) || ripple == 1){
			char temp[MaxEmployeeCount];
			strcpy(temp, Employees[i]);
			strcpy(Employees[i], Employees[i + 1]);
			strcpy(Employees[i + 1], temp);
		}
}

void print(char Employees[MaxEmployeeCount][MaxNameLength], int Size){
    int i ; 
	for( i  = 0; i < Size; i++)
		printf("%s ", Employees[i]);
	printf("\n");
}

int main(int argc, char *argv[]) {
	int n;
	scanf("%d", &n);
    int i ; 
	char Employees[MaxEmployeeCount][MaxNameLength];
	for(i = 0; i < n; i++)
		scanf("%s", Employees[i]);
	int CommandCount;
	scanf("%d", &CommandCount);
	for(i = 0; i < CommandCount; i++){
		char command[10];
		scanf("%s", command);
		if(!strcmp(command, "add")){
			int position;
			char name[MaxNameLength];
			scanf("%s", name);
			scanf("%d", &position);
			Add(Employees, name, position, n);
			n++;
		}
		if(!strcmp(command, "swap")){
			char name1[MaxNameLength];
			char name2[MaxNameLength];
			scanf("%s", name1);
			scanf("%s", name2);
			Swap(Employees, name1, name2, n);
		}
		if(!strcmp(command, "delete")){
			char name[MaxNameLength];
			scanf("%s", name);
			Delete(Employees, name, n);
			n--;		
		}
	}
	print(Employees, n);
	return 0;
}
