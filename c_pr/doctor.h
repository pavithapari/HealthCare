#ifndef DOCTOR_H
#define DOCTOR_H

#define MAX 100

struct Doctor {
    char name[100];
    char symptom[50];
    char url[200];
    float rating;
    int rating_count;
};

void getDoctorSuggestion(const char *username);
void giveFeedback(const char *username);

#endif