#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


typedef struct user //main structure
{
    double latitude;
    double longitude;
    double altitude;
    double time;
    char name[100];
} user_t;
typedef struct distToUser //distance structure
{
    char name[100];
    double dist;
} intervalUser_t;

user_t my_user(double, double, double, double, char *);
void scan_user(user_t *);
void getFile(char *);
int howManyOtherPpl(char[]);
void fillInOtherData(char[], int, user_t[]);
void findDist(user_t, int, user_t[], intervalUser_t[]);
user_t closestDistance(int, user_t[], intervalUser_t[]);
void scan_user(user_t *scanned_user)
{
    double lat, lon, alt, time;
    char name[100];

    printf("\n what is your name?: ");
    scanf("%s", &name);
    printf("Please enter your latitude, longitude, altitude, and time in this order please: ");
    scanf("%lf, %lf, %lf, %lf", &lat, &lon, &alt, &time); // get the suer info
    printf("%s (%lf, %lf, %lf)  %lf nanoseconds is the inputed values.\n", name, lat, lon, alt, time); //make sure its right
    *scanned_user = my_user(lat, lon, alt, time, name);
}
user_t my_user(double lat, double lon, double alt, double time, char *name) //makes stuff into structures
{
user_t temp_User = {lat, lon, alt, time, "\0"};
strcpy(temp_User.name, name);
return temp_User;
}
void getFile(char *fileName){  //gets the file name

    printf("\n what is the name of the file? :\n");
    fgets(fileName, 64, stdin); //gives file name with enter at the end
    fileName[strlen(fileName) - 1] = '\0'; // gets rid of the enter at the end
}
    int howManyOtherPpl(char address[64])
{
    FILE *filePntr = NULL;
    char tempChar;
    int amntUser = 0;
    filePntr = fopen(address, "r"); // opens the file
    if (filePntr == NULL)
    {
        printf("\n you misspelled the file name or didnt put it in the right folder idiot\n");
    }
    else
    {
        tempChar = fgetc(filePntr); // grab the first char

        while ((tempChar > 47 && tempChar < 58)) //scans the whole line to find the value does it in ascii
        {
            amntUser += (tempChar - '0'); // turn the ascii to normal digits
            tempChar = fgetc(filePntr);
            amntUser *= 10; // moves up one decimal place
        }
        amntUser /= 10; //moves the decimal place back
    }

    fclose(filePntr); // closes the file
    return amntUser;
}
void fillInOtherData(char storage[64], int amntUsers, user_t other_users[amntUsers])
{
    FILE *filePntr = NULL;
    char name[64];                                          //this one too
    double time, lat, lon, alt; // variables for filling in data^
        filePntr = fopen(storage, "r"); //opens file
        fscanf(filePntr, "%*s"); // skips the first line
        for (int i = 0; i < amntUsers; i++)  //
        {
            fscanf(filePntr, "%s", name);
            fscanf(filePntr, "%lf", &time);
            fscanf(filePntr, "%lf", &lat);
            fscanf(filePntr, "%lf", &lon);
            fscanf(filePntr, "%lf", &alt);

            other_users[i] = my_user(lat, lon, alt, time, name);
        }
    fclose(filePntr);
}

void findDist(user_t main_user, int amntUserss, user_t otherUsers[amntUserss], intervalUser_t otherdists[amntUserss])
{
    double lat = main_user.latitude, lon = main_user.longitude, alt = main_user.altitude;
    for (int i = 0; i < amntUserss; i++)
    {
        otherdists[i].dist = sqrt(pow((lat - otherUsers[i].latitude), 2) + pow((lon - otherUsers[i].longitude), 2) + pow((alt - otherUsers[i].altitude), 2));
        strcpy(otherdists[i].name, otherUsers[i].name); //long distance formula
    }
}

user_t closestDistance(int amntUsers, user_t otherUsers[amntUsers], intervalUser_t otherdists[amntUsers])
{
    int closestPerson = 0; // place holder number

    for (int i = 0; i < amntUsers; i++){
        if (otherdists[i].dist < otherdists[closestPerson].dist){
            closestPerson = i;
        }
    }
    return otherUsers[closestPerson];
}


int main(void)
{

    user_t scanned_user, theClosestUser;
    int amnthUsers;                            // the number of other users
    char fileName[100] = {'\0'}; // set the adress to delimeters
    getFile(fileName);
    scan_user(&scanned_user);

    amnthUsers = howManyOtherPpl(fileName);

    user_t other_users[amnthUsers];
    intervalUser_t other_user_dists[amnthUsers];

    fillInOtherData(fileName, amnthUsers, other_users);

    for (int i = 0; i < amnthUsers; i++)  // prints all their data
    {
         printf("\nName: %s\nTime: %.0lf\nLat: %lg\nLong: %lg\nAlt: %lg\n", other_users[i].name, other_users[i].time, other_users[i].latitude, other_users[i].longitude, other_users[i].altitude);
    }

    findDist(scanned_user, amnthUsers, other_users, other_user_dists);
    theClosestUser = closestDistance(amnthUsers, other_users, other_user_dists);

    printf("\n %s is the closest.", theClosestUser.name);
    printf(" And they are located at the folowing position in the form (latitude, longitude, altitude): (%0.2lf, %0.2lf, %0.2lf)\n", theClosestUser.latitude, theClosestUser.longitude, theClosestUser.altitude);

    return 0;
}