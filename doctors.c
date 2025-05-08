#include <stdio.h>
#include <string.h>

#define MAX_LEN 100 

typedef struct {
    char doctorname[50];
    char symptoms[50];
    char url[50];
} doctor;

int main() {
    FILE *fp = fopen("doctors.txt", "r");
    if (!fp) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    char line[MAX_LEN];
    doctor record;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;  // Remove newline
        char* token;
        token=strtok(line,"  ");
        if(token)
        strcpy(record.doctorname,token);
        token=strtok(NULL,"  ");
        if(token)
        strcpy(record.symptoms,token);
        token=strtok(NULL,"  ");
        if(token)
        strcpy(record.url,token);


        printf("Doctor: %s\n", record.doctorname);
        printf("Symptom: %s\n", record.symptoms);
        printf("URL: %s\n\n", record.url);
    }

    fclose(fp);
    return 0;
}
