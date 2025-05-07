#include <stdio.h>
#include <string.h>

void get_input() {
    char name[10];
    int symptoms[10];
    int i = 0, num;
    FILE *fp1;

    printf("\nEnter your name: ");
    scanf("%s",name);
    printf("1.headache 2.fever 3.cough 4.stomach pain 5.skin rash 6.chest pain 7.fatigue 8.depression 9.shortness of breath");
    printf("Enter the symptoms number and press 0 to end\n");

    while (1) {
        scanf("%d", &num);
        if (num == 0) break;
        if (i < 10) {
            symptoms[i++] = num;
        } else {
            printf("Max 10 symptoms allowed.\n");
            break;
        }
    }

    fp1 = fopen("user_data.txt", "a");
    if (fp1 != NULL) {
        fprintf(fp1, "%s ", name); 
        for (int j = 0; j < i; j++) {
            fprintf(fp1, "%d ", symptoms[j]);  
        }
        fprintf(fp1, "\n");  
        fclose(fp1);
    } else {
        printf("Error opening file!\n");
    }
}

int main() {
    char buffer[300];
    printf("Hellow! Welcome to our symptoms checler!\n");
    get_input();

    printf("Patient data saved successfully!\n");
    FILE *fp=popen("./doctors","r");
    int count=1;
    while(fgets(buffer,sizeof(buffer),fp)!=NULL){
        printf("%d %s\n",count,buffer);
        count++
    }
    pclose(fp);
    int choice;
    printf("Enter a choice to open ")


    return 0;
}