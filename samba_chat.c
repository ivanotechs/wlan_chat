/* Write and read a binary file using system calls
   (open, write, read e close) */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* For system calls write, read e close */
 
#include <sys/types.h> /* For system call open */
#include <sys/stat.h>  /* For system call open */
#include <fcntl.h>     /* For system call open */

#define NO_USERS 20
#define MAX_NAME 100
#define MAX_MESSAGES 1000


typedef struct user{
    char name[MAX_NAME];
    char password[MAX_NAME];
    char state;
} user;

//structure to manage the objects message
typedef struct message{
    char sender[MAX_NAME];
    char receiver[MAX_NAME];
    char content[MAX_NAME];
    char state;
} message;



void write_user_file(int user_file, user list_user[]){
    user_file = open("smb://ivano-pc.local/hello/users.bin", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    write(user_file,list_user, MAX_NAME*sizeof(user));
    close(user_file);
}

void write_message_file(int message_file, message list_message[]){
    message_file = open("smb://ivano-pc.local/hello/messages.bin",  O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    write(message_file, list_message, MAX_NAME*sizeof(message));
    close(message_file);
}

void read_message_file(int message_file, message list_message[]){
    message_file = open("smb://ivano-pc.local/hello/messages.bin", O_RDONLY);
    int n_stud = 0;
    while( read(message_file, &list_message[n_stud], sizeof(message)) == sizeof(message) ) {
        n_stud++;
    }
    close(message_file);
}

void read_user_file(int user_file, user list_user[]){
    user_file = open("smb://ivano-pc.local/hello/users.bin", O_RDONLY);
    int n_stud = 0;
    while( read(user_file, &list_user[n_stud], sizeof(user)) == sizeof(user) ) {
        n_stud++;
    }
    close(user_file);
}

void write_numbers_file(int numbers, int num[]){
    numbers = open("smb://ivano-pc.local/hello/numbers.bin", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    write(numbers, num, MAX_NAME*sizeof(int));
    close(numbers);
}

void read_numbers_file(int numbers, int num[]){
    numbers = open("smb://ivano-pc.local/hello/numbers.bin", O_RDONLY);
    int n_stud = 0;
    while( read(numbers, &num[n_stud], sizeof(int)) == sizeof(int) ) {
        n_stud++;
    }
    close(numbers);
}

void header(){
    printf("*************************************************************************************************\n");
    printf("*                                SYSTEM ADMINISTRATION                                          *\n");
    printf("*                                                                                               *\n");
    printf("*                                     SAMBA-CHAT                                                *\n");
    printf("*                                                                                               *\n");
    printf("*                                        V1.0                                                   *\n");
    printf("*************************************************************************************************\n\n\n\n");

    
}

void read_starter(user User, message list_message[], int number[], user list_user[]){
    int mess, us, num;
    read_message_file(mess, list_message);
    read_user_file(us, list_user);
    read_numbers_file(num, number);
}

void write_starter(user User, message list_message[], int number[], user list_user[]){
    int mess, us, num;
    write_message_file(mess, list_message);
    write_user_file(us, list_user);
    write_numbers_file(num, number);

}
user create_user(user list_user[], char name[], char pass[], int number_user){
    user User ;
    strcpy(User.name, name );
    strcpy(User.password, pass);
    User.state = 'o';
    return User;
}

message create_message(message list_message[], char sender[], char receiver[], char content[], int num_message){
    message* Message = (message*)malloc(sizeof(message));
    strcpy(Message->sender, sender);
    strcpy(Message->receiver, receiver);
    strcpy(Message->content, content);
    Message->state = 'u';
    return *Message;
    
}

user login_page(user User, message list_message[], int number[], user list_user[]){
    read_starter(User, list_message, number, list_user);
    char name[MAX_NAME], pass[MAX_NAME];
    printf("                                     LOGIN\n");
    printf("         User Name : ");
    fgets(name, MAX_MESSAGES, stdin);
    name[strcspn(name, "\n")]=0;
    printf("          Password : ");
    fgets(pass, MAX_MESSAGES, stdin);
    pass[strcspn(pass, "\n")]=0;
    int i, found = 0;
    for(i=0; i<=number[0]; i++){
        if(strcmp(name, list_user[i].name) == 0){
                found = 1;
                break;
        }
    }
    if (found == 1){
        if(strcmp(list_user[i].password, pass)==0){
            printf("login successful\n\n");
            return list_user[i];
            
        }
        else{
            printf("Password incorrect....\n");
            login_page(User, list_message, number, list_user);
        }
    }
    else{
        char anas[10];
        printf("User not found. Create account? y/n :");
        fgets(anas, MAX_MESSAGES, stdin);
        anas[strcspn(anas, "\n")]=0;
        if(strcmp(anas, "y")==0){
            number[0]++;
            list_user[number[0]]= create_user(list_user, name, pass, number[0]);
            printf("account registration successfull\n");
            write_starter(User, list_message, number, list_user);
            return list_user[number[0]];
        }
        else{
            login_page(User, list_message, number, list_user);
        }
    }
}
int starter_page(user User, message list_message[], int number[], user list_user[]){
    header();
    printf("    Name : %s                                          State : online\n", User.name );
    printf("                                   MESSAGES RECEIVED\n");

    printf("  Sender            |     Message\n");
    int i;
    for(i = 0; i<=number[1]; i++){
        if(strcmp(list_message[i].receiver, User.name) == 0){
            printf("     %s         :    %s\n", list_message[i].sender, list_message[i].content);
        }
    }
    int answer;
    printf("\n\n              What next? \n              1. New message\n                2. Refresh message List\n              3.Logout\n");
    scanf("%d", &answer);
    if(answer == 1){
            printf("To who do you want to send?\n\n");
            for(i=0; i<=number[1]; i++){
                printf("%d : %s\n", i, list_user[i].name);
            }
            printf("\nEnter the number : ");
            int us;
            scanf("%d", &us);
            printf("\n\nEnter the message >> ");
            char mess[MAX_MESSAGES];
            
            scanf("%s", mess);
            number[1]++;
            list_message[number[1]] = create_message(list_message, User.name, list_user[us].name, mess, number[1]);
            int f;
            write_starter(User, list_message, number, list_user);
            printf("Messages send succeccively \n");
            starter_page(User, list_message, number, list_user);

    }
    else if(answer == 2){
            int mess;
            int us;
            read_message_file(mess, list_message);
            read_user_file(us, list_user);
            starter_page(User, list_message, number, list_user);
    }
    else{
        login_page(User, list_message, number, list_user);
    }

}

int main(){

    user User;
    message list_message[MAX_MESSAGES];
    int number[2];
    user list_user[MAX_MESSAGES];
    read_starter(User, list_message, number, list_user);
    starter_page(login_page(User, list_message, number, list_user), list_message, number, list_user);
   
    return 0;
}



/*int main(){
    printf("i am the error");
    char* name;
    char* pass;
    int n = 0;
    
    user User; 
    message list_message[MAX_MESSAGES];
    Number number;
    number.number_messages = n;
    number.number_users = n;
    user list_user[MAX_MESSAGES];
    
    create_user(list_user, name, pass, n);
    create_message(list_message, name, name, name, n);
    write_starter(User, list_message, number, list_user );
    starter_page(login_page(User, list_message, number, list_user), list_message, number, list_user);
    return 0;
}*/