#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 100

typedef struct {
    char doctorname[50];
    int symptom_number;
    char url[100];
} Doctor;

const char* symptom_names[] = {
    "", "Headache", "Fever", "Cough", "Stomach Pain",
    "Skin Rash", "Chest Pain", "Fatigue", "Depression", "Shortness of Breath"
};

void search_doctor() {
    FILE *userFile = fopen("user_data.txt", "r");
    FILE *doctorFile = fopen("doctors.txt", "r");

    if (!userFile || !doctorFile) {
        printf("Error opening user or doctor file.\n");
        return;
    }

    char userLine[MAX_LINE];
    while (fgets(userLine, sizeof(userLine), userFile)) {
        char name[50];
        int symptoms[10];
        int count = 0;

        char *token = strtok(userLine, " \n");
        if (token == NULL) continue;
        strcpy(name, token); // user's name

        // Read symptom numbers
        while ((token = strtok(NULL, " \n")) != NULL && count < 10) {
            symptoms[count++] = atoi(token);
        }

        printf("\nPatient: %s\n", name);

        for (int i = 0; i < count; i++) {
            int symptom_num = symptoms[i];
            const char* symptom_name = (symptom_num >= 1 && symptom_num <= 9) ? symptom_names[symptom_num] : "Unknown";

            // rewind doctor file for each symptom search
            rewind(doctorFile);
            char doctorLine[MAX_LINE];
            int found = 0;

            while (fgets(doctorLine, sizeof(doctorLine), doctorFile)) {
                Doctor doc;
                sscanf(doctorLine, "%s %d %s", doc.doctorname, &doc.symptom_number, doc.url);

                if (doc.symptom_number == symptom_num) {
                    printf("Symptom %d (%s) -> Doctor: %s, URL: %s\n", symptom_num, symptom_name, doc.doctorname, doc.url);
                    found = 1;
                    break;
                }
            }

            if (!found) {
                printf("Symptom %d (%s) -> No doctor found.\n", symptom_num, symptom_name);
            }
        }
    }

    fclose(userFile);
    fclose(doctorFile);
}
