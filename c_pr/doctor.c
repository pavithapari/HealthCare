#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doctor.h"

#ifdef _WIN32
#define OPEN_URL_COMMAND "start %s"
#else
#define OPEN_URL_COMMAND "xdg-open %s"
#endif

// Function to read feedback and calculate average ratings
void loadFeedback(struct Doctor doctors[], int doctorCount) {
    FILE *fb = fopen("feedback.txt", "r");
    if (fb) {
        char line[300];
        char username[100], doctorName[100];
        int rating;

        // Initialize doctor ratings
        for (int i = 0; i < doctorCount; i++) {
            doctors[i].rating = 0.0;
            doctors[i].rating_count = 0;
        }

        while (fgets(line, sizeof(line), fb) != NULL) {
            if (sscanf(line, "User: %s", username) == 1) continue;
            if (sscanf(line, "Doctor: %s", doctorName) == 1) {
                fgets(line, sizeof(line), fb); // Read the "Rating:" line
                if (sscanf(line, "Rating: %d", &rating) == 1) {
                    // Find the doctor and update the rating
                    for (int i = 0; i < doctorCount; i++) {
                        if (strcmp(doctors[i].name, doctorName) == 0) {
                            doctors[i].rating = (doctors[i].rating * doctors[i].rating_count + rating) / (doctors[i].rating_count + 1);
                            doctors[i].rating_count++;
                            break;
                        }
                    }
                }
            }
        }
        fclose(fb);
    }
}

// Comparison function for sorting doctors by rating (descending)
int compareDoctors(const void *a, const void *b) {
    const struct Doctor *docA = (const struct Doctor *)a;
    const struct Doctor *docB = (const struct Doctor *)b;
    if (docB->rating > docA->rating) return 1;
    if (docB->rating < docA->rating) return -1;
    return 0;
}

void getDoctorSuggestion(const char *username) {
    FILE *fp = fopen("doctors.txt", "r");
    if (!fp) {
        printf("Error opening doctor.txt file.\n");
        return;
    }

    struct Doctor allDoctors[MAX];
    int allDoctorCount = 0;

    while (fscanf(fp, "%s %s %s", allDoctors[allDoctorCount].name, allDoctors[allDoctorCount].symptom, allDoctors[allDoctorCount].url) != EOF) {
        allDoctorCount++;
    }
    fclose(fp);

    // Load feedback to calculate ratings
    loadFeedback(allDoctors, allDoctorCount);

    printf("\nChoose a symptom:\n");
    printf("1. Headache\n2. Fever\n3. Cough\n4. Stomach Pain\n5. Skin Rash\n6. Chest Pain\n7. Fatigue\n8. Depression\n9. Shortness of Breath\n");
    printf("Enter your choice (number): ");
    int choice;
    scanf("%d", &choice);
    getchar(); // clear newline

    const char *symptoms[] = {
        "headache", "fever", "cough", "stomachpain", "skinrash",
        "chestpain", "fatigue", "depression", "shortnessofbreath"
    };

    if (choice < 1 || choice > 9) {
        printf("Invalid choice.\n");
        return;
    }

    const char *selectedSymptom = symptoms[choice - 1];

    struct Doctor suggestedDoctors[MAX];
    int suggestedCount = 0;

    for (int i = 0; i < allDoctorCount; i++) {
        if (strcmp(allDoctors[i].symptom, selectedSymptom) == 0) {
            suggestedDoctors[suggestedCount++] = allDoctors[i];
        }
    }

    if (suggestedCount == 0) {
        printf("No doctors found for the selected symptom.\n");
        return;
    }

    // Sort the suggested doctors by rating
    qsort(suggestedDoctors, suggestedCount, sizeof(struct Doctor), compareDoctors);

    printf("\nDoctors for %s (based on ratings):\n", selectedSymptom);
    int indexes[MAX], icount = 0;
    for (int i = 0; i < suggestedCount; i++) {
        printf("%d. Dr. %s (Rating: %.2f) - %s\n", i + 1, suggestedDoctors[i].name, suggestedDoctors[i].rating, suggestedDoctors[i].url);
        indexes[i] = -1; // Initialize
        // Find the original index in allDoctors to store in history
        for (int j = 0; j < allDoctorCount; j++) {
            if (strcmp(allDoctors[j].name, suggestedDoctors[i].name) == 0 && strcmp(allDoctors[j].symptom, suggestedDoctors[i].symptom) == 0) {
                indexes[i] = j;
                break;
            }
        }
        icount++;
    }

    int doctorChoice;
    printf("\nEnter the number to open the doctor’s URL: ");
    scanf("%d", &doctorChoice);
    getchar();

    if (doctorChoice < 1 || doctorChoice > suggestedCount) {
        printf("Invalid doctor selection.\n");
        return;
    }

    // Open the selected doctor URL
    char command[300];
    if (indexes[doctorChoice - 1] != -1) {
        snprintf(command, sizeof(command), OPEN_URL_COMMAND, allDoctors[indexes[doctorChoice - 1]].url);
        system(command);

        // Save user history
        FILE *hist = fopen("history.txt", "a");
        if (hist) {
            fprintf(hist, "%s %s %s\n", username, selectedSymptom, allDoctors[indexes[doctorChoice - 1]].name);
            fclose(hist);
        } else {
            printf("Failed to save history.\n");
        }
    } else {
        printf("Error: Could not find the selected doctor in the main list.\n");
    }
}

void giveFeedback(const char *username) {
    FILE *hist = fopen("history.txt", "r");
    if (!hist) {
        printf("No history found. You are a new user.\n");
        return;
    }

    struct Doctor previousDoctors[MAX];
    char uname[100], usym[50], doctorName[100];
    int count = 0;
    int found = 0;

    while (fscanf(hist, "%s %s %s", uname, usym, doctorName) != EOF) {
        if (strcmp(uname, username) == 0) {
            found = 1;
            strcpy(previousDoctors[count].symptom, usym);
            strcpy(previousDoctors[count].name, doctorName);
            count++;
        }
    }
    fclose(hist);

    if (!found) {
        printf("You are a new user. Please get a doctor suggestion first.\n");
        return;
    }

    printf("\nWelcome back %s! These were your previous doctor recommendations:\n", username);
    for (int i = 0; i < count; i++) {
        printf("%d. Dr. %s (for %s)\n", i + 1, previousDoctors[i].name, previousDoctors[i].symptom);
    }

    printf("Which doctor would you like to give feedback on (enter number)? ");
    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        return;
    }

    int rating;
    printf("Enter your rating for Dr. %s (1 to 5): ", previousDoctors[choice - 1].name);
    scanf("%d", &rating);
    getchar();

    FILE *fb = fopen("feedback.txt", "a");
    if (fb) {
        fprintf(fb, "User: %s\nDoctor: %s\nRating: %d\n---\n",
                username, previousDoctors[choice - 1].name, rating);
        fclose(fb);
        printf("Thank you for your feedback!\n");
    } else {
        printf("Unable to save feedback.\n");
    }
}