#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Hall
{
    /*struct definition of a hall*/
    char Hall_Name[80];
    char Movie_Name[80];
    int Hall_Width;
    int Hall_Height;
    char Last_Seat_Letter;/*maksimum letter of seat in hall*/
    char** Hall_Sitting_Plan;
    int sum;/*total income the hall*/
    int Number_of_Student;
    int Number_of_FullFare;

};
/*Function definitions*/
void Create_Halls(struct Hall* All_Halls, char* command_list, int Line_Count, int j);
void Buy_Ticket(struct Hall* All_Halls, char* command_list, int hall_count, int Line_Count);
int Hall_Count(char** command_list, int Line_Count);
void Read_File(char* s, char** matrix);
void Cancel_Ticket(struct Hall* All_Halls, char *command_list, int hall_count, int Line_Count);
int Find_Letter_of_Seat(char letter);
int File_Line_Count(char* s);
void Statistic(struct Hall* All_Halls, int hall_count);
void Show_Hall(struct Hall* All_Halls, int hall_count, char *input_line);

int main(int argc, char *argv[])
{
    /* Definitions */
    char* input_file = argv[1];
    char** command_list;
    int i = 0;
    int hall_index = 0;
    int hall_count;
    struct Hall* All_Halls;
    int Line_Count = File_Line_Count(input_file);/*Function counts number of lines and return that value in an integer.*/
    remove("output.txt");/*if there is already exist a output.txt remove it.*/
    command_list = (char**)calloc(Line_Count, sizeof(char*));
    for(i = 0; i < Line_Count; i++)
        command_list[i] = (char *)calloc(200, sizeof(char));
    Read_File(input_file, command_list);      /*take input lines from input file and return with command_list matrix*/
    hall_count = Hall_Count(command_list, Line_Count);/*Function counts CREATEHALL commands and return that value in an integer.*/
    All_Halls = (struct Hall*)calloc(hall_count, sizeof(struct Hall));/*This array contains all halls in struct hall type.*/

    for(i = 0; i < Line_Count; i++){                                            /*This loop does calls functipns and operate commands.*/
        if(command_list[i][0] == 'C' && command_list[i][1] == 'R'){
            Create_Halls(All_Halls, command_list[i], Line_Count, hall_index);/* This function create a hall*/
            hall_index++;
        }
        else if(command_list[i][0] == 'B' && command_list[i][1] == 'U'){
            Buy_Ticket(All_Halls, command_list[i], hall_count, Line_Count);/*This function buy tickets from related hall*/

        }
        else if(command_list[i][0] == 'C' && command_list[i][1] == 'A'){
            Cancel_Ticket(All_Halls, command_list[i], hall_count, Line_Count);/*This function cancel tickets from related hall*/

        }
        else if(command_list[i][0] == 'S' && command_list[i][1] == 'T'){
            Statistic(All_Halls, hall_count);                               /*This function print statistics related hall*/
        }
        else if(command_list[i][0] == 'S' && command_list[i][1] == 'H'){
            Show_Hall(All_Halls, hall_count, command_list[i]);              /*This function print sitting plan related hall*/
        }
    }

    for(i=0; i<hall_count; i++){
        free(All_Halls[i].Hall_Sitting_Plan);           /* Free all sitting plan,  command_list and All_Halls matrises*/
    }
    free(command_list);
    free(All_Halls);
    return 0;
}
int Hall_Count(char** command_list, int Line_Count)
{
    int count = 0;
    int i;
    for(i = 0; i<Line_Count; i++){
        if(command_list[i][0] == 'C' && command_list[i][1] == 'R'){
            count++;
        }
    }
    return count;
}

void Create_Halls(struct Hall* All_Halls, char *input_line, int Line_Count, int j){
    /*Definitions*/
    int k = 0;
    int x = 0;
    int y = 0;
    char Seat_Letter;
    int line_number = 1;
    char* token = strtok(input_line, " \"\n");      /*Split the command line and assign  halls attributes*/

    for (k = 0; token != NULL; k++){
        if(k == 1){
            strcpy(All_Halls[j].Hall_Name, token);
        }
        else if(k == 2){
            strcpy(All_Halls[j].Movie_Name, token);
        }
        else if(k == 3){
            All_Halls[j].Hall_Width = 2*atoi(token)+3;
        }
        else if(k == 4){
            All_Halls[j].Hall_Height = 2*atoi(token)+3;
        }
        token = strtok(NULL, " \"\n");

    }
    All_Halls[j].sum = 0;
    All_Halls[j].Number_of_FullFare = 0;
    All_Halls[j].Number_of_Student = 0;
    int ht = All_Halls[j].Hall_Height;
    int wd = All_Halls[j].Hall_Width;

    All_Halls[j].Hall_Sitting_Plan = (char**)calloc(ht, sizeof(char*));  /*Create a matrix for halls sitting plan*/
    for(k = 0; k <ht; k++){
        All_Halls[j].Hall_Sitting_Plan[k] = (char*)calloc(wd, sizeof(char));
    }

    for(x = 0; x<All_Halls[j].Hall_Height; x++){
        if(x%2 == 0&&x<All_Halls[j].Hall_Height-2){
            for(y = 0; y<All_Halls[j].Hall_Width; y++){
                if(y < 2){
                    All_Halls[j].Hall_Sitting_Plan[x][y] = ' ';     /*This part separetes rows with "---"*/
                }
                else{
                    All_Halls[j].Hall_Sitting_Plan[x][y] = '-';
                }
            }
        }
        else if(x<All_Halls[j].Hall_Height-2){
            char line_number_convert[7];

            for(y = 0; y<All_Halls[j].Hall_Width; y++){
                sprintf(line_number_convert, "%d", line_number);
                if(line_number < 10 && y == 0){
                    All_Halls[j].Hall_Sitting_Plan[x][0] = line_number_convert[0];      /*This part creates seats with "|" and assign number of rows*/
                    All_Halls[j].Hall_Sitting_Plan[x][1] = ' ';
                    line_number++;
                }
                else if(line_number >= 10 && y == 0){
                    All_Halls[j].Hall_Sitting_Plan[x][0] = line_number_convert[0];
                    All_Halls[j].Hall_Sitting_Plan[x][1] = line_number_convert[1];

                    line_number++;
                }
                else if(y%2 == 0){
                    All_Halls[j].Hall_Sitting_Plan[x][y] = '|';
                }
                else if(y > 1){
                    All_Halls[j].Hall_Sitting_Plan[x][y] = ' ';
                }
            }

        }
        else if(x == All_Halls[j].Hall_Height-2){
            Seat_Letter = 'A';
            for(y=0; y < All_Halls[j].Hall_Width; y++){
                if(y%2 == 1&&y>2){
                    All_Halls[j].Hall_Sitting_Plan[x][y] = Seat_Letter;
                    if(y == All_Halls[j].Hall_Width-1){
                        All_Halls[j].Last_Seat_Letter = Seat_Letter;        /* this part assign letters of seat on last row before curtain*/
                    }
                    Seat_Letter++;
                }
                else{
                    All_Halls[j].Hall_Sitting_Plan[x][y] = ' ';
                }
            }
            All_Halls[j].Last_Seat_Letter = Seat_Letter-1;
        }
        else if(x == All_Halls[j].Hall_Height-1){
            for(y = 0; y < All_Halls[j].Hall_Width; y++){               /*this part assign curtain on middle of hall */
                All_Halls[j].Hall_Sitting_Plan[x][y] = ' ';
            }
            All_Halls[j].Hall_Sitting_Plan[x][All_Halls[j].Hall_Width/2-5] = 'C';
            All_Halls[j].Hall_Sitting_Plan[x][All_Halls[j].Hall_Width/2-3] = 'U';
            All_Halls[j].Hall_Sitting_Plan[x][All_Halls[j].Hall_Width/2-1] = 'R';
            All_Halls[j].Hall_Sitting_Plan[x][All_Halls[j].Hall_Width/2+1] = 'T';
            All_Halls[j].Hall_Sitting_Plan[x][All_Halls[j].Hall_Width/2+3] = 'A';
            All_Halls[j].Hall_Sitting_Plan[x][All_Halls[j].Hall_Width/2+5] = 'I';
            All_Halls[j].Hall_Sitting_Plan[x][All_Halls[j].Hall_Width/2+7] = 'N';
        }
    }
}
void Buy_Ticket(struct Hall* All_Halls, char *input_line, int hall_count, int Line_Count){
    /*Definitions*/
    int k;
    int j;
    char Movie_Name[80];
    char Seat_Label_Number[80];
    int Seat_Number;
    char Seat_Label[80];
    char Seat_Label_Letter;
    char Ticket_Type[80];
    int  Number_of_Ticket;
    FILE *output;
    output=fopen("output.txt", "a");
    char* token = strtok(input_line, " \"\n");
    for(k = 0; token!=NULL; k++){           /*This loop split command line*/
        if(k == 1){                         /*and assign commands in related constants*/
            strcpy(Movie_Name, token);
        }
        else if(k == 2){
            Seat_Label_Letter = token[0];
            strcpy(Seat_Label, token);
            strcpy(Seat_Label_Number, token);
            for(j=0; j < 79; j++){
                Seat_Label_Number[j] = Seat_Label_Number[j+1];
            }
            Seat_Number = atoi(Seat_Label_Number);
        }
        else if(k == 3){
            strcpy(Ticket_Type, token);
        }
        else if(k == 4){
            Number_of_Ticket = atoi(token);
        }
        token = strtok(NULL, " \"\n");
    }
    if(k == 5){ /*This condition control command line errors.For example BUYTICKET "" D8 student 2*/
        for(k=0; k < hall_count; k++){
            if(strcmp(All_Halls[k].Movie_Name,Movie_Name) == 0){
                if(Seat_Label_Letter+Number_of_Ticket-1<=All_Halls[k].Last_Seat_Letter && Seat_Number<=(All_Halls[k].Hall_Height-3)/2){/* this condition control seats*/
                    int index = Find_Letter_of_Seat(Seat_Label_Letter);                                                                /*are defined or not*/
                    int height_number = atoi(Seat_Label_Number)+atoi(Seat_Label_Number)-1;
                    int control = 0;
                    for(j = 0; j<2*Number_of_Ticket; j=j+2){                                /*this loop control there is seats exist enough*/
                        if(All_Halls[k].Hall_Sitting_Plan[height_number][index+j] != ' '){
                            control = 1;
                        }

                    }
                    if(control == 0){
                        printf("%s [%s] Seat(s) ", All_Halls[k].Hall_Name, All_Halls[k].Movie_Name);            /* this part buys ticket and fills seats*/
                        fprintf(output, "%s [%s] Seat(s) ", All_Halls[k].Hall_Name, All_Halls[k].Movie_Name);   /*then write ouputs in a file and screen*/

                        for(j=0; j<2*Number_of_Ticket; j=j+2){
                            if(j+2 == 2*Number_of_Ticket){
                            printf("%c%s ", Seat_Label_Letter, Seat_Label_Number);
                            fprintf(output,"%c%s ", Seat_Label_Letter, Seat_Label_Number);
                            }
                            else{
                            printf("%c%s,", Seat_Label_Letter, Seat_Label_Number);
                            fprintf(output,"%c%s,", Seat_Label_Letter, Seat_Label_Number);
                            }
                            if(strcmp(Ticket_Type,"Student") == 0){
                                All_Halls[k].Hall_Sitting_Plan[height_number][index] = 's';
                                All_Halls[k].Number_of_Student++;
                                All_Halls[k].sum = All_Halls[k].sum+7;
                            }
                            else{
                                All_Halls[k].Hall_Sitting_Plan[height_number][index] = 'f';
                                All_Halls[k].Number_of_FullFare++;
                                All_Halls[k].sum = All_Halls[k].sum+10;
                            }
                            index = index+2;
                            Seat_Label_Letter++;
                        }
                        printf("successfully sold.\n");
                        fprintf(output, "successfully sold.\n");
                    }
                    else{
                        printf("ERROR:Specified seat(s) in %s are not available! They have been already taken.\n", All_Halls[k].Hall_Name);
                        fprintf(output,"ERROR:Specified seat(s) in %s are not available! They have been already taken.\n", All_Halls[k].Hall_Name);
                    }
                }
                else{
                    printf("ERROR:Seat %s is not defined at %s\n", Seat_Label, All_Halls[k].Hall_Name);
                    fprintf(output, "ERROR:Seat %s is not defined at %s\n", Seat_Label, All_Halls[k].Hall_Name);
                }
            }

        }
    }
    else{
        printf("ERROR: Movie name cannot be empty\n");
        fprintf(output, "ERROR: Movie name cannot be empty\n");
    }
    fclose(output);
}
/* Find_Letter_of_Seat function take letter of seat and return an index for hall sitting plan matrix*/
int Find_Letter_of_Seat(char letter){
    int result_index = 0;

    if(letter == 'A')      result_index = 3;
    else if(letter == 'B') result_index = 5;
    else if(letter == 'C') result_index = 7;
    else if(letter == 'D') result_index = 9;
    else if(letter == 'E') result_index = 11;
    else if(letter == 'F') result_index = 13;
    else if(letter == 'G') result_index = 15;
    else if(letter == 'H') result_index = 17;
    else if(letter == 'I') result_index = 19;
    else if(letter == 'J') result_index = 21;
    else if(letter == 'K') result_index = 23;
    else if(letter == 'L') result_index = 25;
    else if(letter == 'M') result_index = 27;
    else if(letter == 'N') result_index = 29;
    else if(letter == 'O') result_index = 31;
    else if(letter == 'P') result_index = 33;
    else if(letter == 'Q') result_index = 35;
    else if(letter == 'R') result_index = 37;
    else if(letter == 'S') result_index = 39;
    else if(letter == 'T') result_index = 41;
    else if(letter == 'U') result_index = 43;
    else if(letter == 'V') result_index = 45;
    else if(letter == 'W') result_index = 47;
    else if(letter == 'X') result_index = 49;
    else if(letter == 'Y') result_index = 51;
    else if(letter == 'Z') result_index = 53;

    return result_index;


}
void Cancel_Ticket(struct Hall* All_Halls, char* input_line, int hall_count, int Line_Count){
    /*Definitions*/
    int k;
    int j;
    FILE *output;
    output = fopen("output.txt", "a");
    char Movie_Name[80];
    char Seat_Label[80];
    char Seat_Label_Number[80];
    char Seat_Label_Letter;
    /*This part splits command line and assign related constants*/

    char* token = strtok(input_line," \"\n");
    for(k = 0; token != NULL; k++){
        if(k == 1){
            strcpy(Movie_Name,token);
        }
        if(k == 2){
            Seat_Label_Letter = token[0];
            strcpy(Seat_Label, token);
            strcpy(Seat_Label_Number, token);
            for(j=0; j< 79; j++){
                Seat_Label_Number[j] = Seat_Label_Number[j+1];
            }
        }
        token = strtok(NULL, " \"\n");
    }
    if(k == 3){ /* This condition control error for wrong command lines.For example CANCELTICKET "" C3*/
        for(k = 0; k < hall_count; k++){
            if(strcmp(All_Halls[k].Movie_Name,Movie_Name) == 0){ /*This condition find hall for that movie name*/
                if(Seat_Label_Letter <= All_Halls[k].Last_Seat_Letter){/* this condition control seats are defined or not in hall*/
                    int index = Find_Letter_of_Seat(Seat_Label_Letter);
                    int height_number = atoi(Seat_Label_Number) + atoi(Seat_Label_Number) - 1;
                    if(All_Halls[k].Hall_Sitting_Plan[height_number][index] != ' '){/*This part control seats are full or not*/
                        if(All_Halls[k].Hall_Sitting_Plan[height_number][index] == 's'){
                            All_Halls[k].Number_of_Student--;
                            All_Halls[k].sum = All_Halls[k].sum - 7;
                        }
                        else{
                            All_Halls[k].Number_of_FullFare--;
                            All_Halls[k].sum = All_Halls[k].sum - 10;
                        }
                        All_Halls[k].Hall_Sitting_Plan[height_number][index] = ' ';
                        printf("%s [%s] purchase is cancelled. Seat %s is now free.\n", All_Halls[k].Hall_Name, All_Halls[k].Movie_Name, Seat_Label);
                        fprintf(output, "%s [%s] purchase is cancelled. Seat %s is now free.\n", All_Halls[k].Hall_Name, All_Halls[k].Movie_Name, Seat_Label);
                    }
                    else{
                        printf("ERROR: Seat %s in %s was not sold.\n", Seat_Label, All_Halls[k].Hall_Name);
                        fprintf(output, "ERROR: Seat %s in %s was not sold.\n", Seat_Label, All_Halls[k].Hall_Name);
                    }
                }
            }
        }
    }
    else{
        printf("ERROR: Movie name cannot be empty\n");
        fprintf(output, "ERROR: Movie name cannot be empty\n");

    }

    fclose(output);
}
/*Statistic function print statistics a hall*/
void Statistic(struct Hall* All_Halls, int hall_count){
    FILE *output;
    output=fopen("output.txt", "a");
    int i;
    for(i=0; i < hall_count; i++){
        printf("%s %d student(s), %d full fare(s), sum:%d TL\n", All_Halls[i].Movie_Name, All_Halls[i].Number_of_Student, All_Halls[i].Number_of_FullFare, All_Halls[i].sum);
        fprintf(output, "%s %d student(s), %d full fare(s), sum:%d TL\n", All_Halls[i].Movie_Name, All_Halls[i].Number_of_Student, All_Halls[i].Number_of_FullFare, All_Halls[i].sum);
    }
    fclose(output);
}
/*This function print sitting plan a hall*/
void Show_Hall(struct Hall* All_Halls, int hall_count, char *input_line){
    FILE *output;
    output=fopen("output.txt", "a");
    int i;
    int k;
    int j;
    char Hall_Name[80];
    char* token = strtok(input_line, " \"\n");
    for(k = 0; token != NULL; k++){
        if(k == 1){
            strcpy(Hall_Name, token);
        }
        token = strtok(NULL, " \"\n");
    }
    if(k == 2){/* This condition control error for wrong command.For example SHOWHALL ""*/
        for(i = 0; i < hall_count; i++){
            if(strcmp(All_Halls[i].Hall_Name, Hall_Name) == 0){/*This condition find hall with hall name*/
                printf("%s sitting plan\n", All_Halls[i].Hall_Name);
                fprintf(output,"%s sitting plan\n", All_Halls[i].Hall_Name);
                for(j = 0; j < All_Halls[i].Hall_Height; j++){
                    printf("%s\n", All_Halls[i].Hall_Sitting_Plan[j]);
                    fprintf(output, "%s\n", All_Halls[i].Hall_Sitting_Plan[j]);
                }
            }
        }
    }
    else{
        printf("ERROR: Hall name cannot be empty\n");
        fprintf(output, "ERROR: Hall name cannot be empty\n");
    }
    fclose(output);
}
/*This function counts number of line in input file and return that value in an integer*/
int File_Line_Count(char* s){
    int count = 0;
    FILE* input_file;
    char buff[500];
    input_file = fopen(s, "r");
    if(!input_file){
        return -1;
    }
    while(fgets(buff, 500, input_file) != NULL){
        count++;
    }
    fclose(input_file);
    return count;
}
/*This function reads input file and assign these lines in a matrix*/
void Read_File(char* s, char** matrix){
    int i = 0;
    FILE* input_file;
    char buff[500];
    input_file = fopen(s, "r");
    if(!input_file){
        return;
    }
    while(fgets(buff, 500, input_file) != NULL){
        strcpy(matrix[i], buff);
        i = i+1;
    }
    fclose(input_file);
}


