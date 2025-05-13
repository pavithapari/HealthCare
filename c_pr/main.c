#include <stdio.h>
#include "doctor.h"

int main() {
    char username[100];
    printf("Enter your username: ");
    scanf("%s", username);

    while (1) {
        printf("\n1. Get Doctor Suggestion\n2. Give Feedback\n3. Exit\nChoose an option: ");
        int option;
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1:
                getDoctorSuggestion(username);
                break;
            case 2:
                giveFeedback(username);
                break;
            case 3:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid option.\n");
        }
    }

    return 0;
}