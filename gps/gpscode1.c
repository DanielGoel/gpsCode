#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MaxLine 255

typedef struct
{

    char name [100];
    double lat, lon, alt, time;

}user_t;

typedef struct
{

    char name [100];
    double dist;
    
}user_d;

int findUsers (char filename [50])
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("FILE ERROR");
    }

    char buffer [MaxLine];
    fgets(buffer, MaxLine, file);
    int numUsers;

    sscanf(buffer, "%d\n", &numUsers);
    fgets(buffer, MaxLine, file);

    numUsers = numUsers + 1;

    fclose(file);

    return(numUsers);
}

user_t scan_user (char filename [50], int user, int numUsers)
{
    user_t users [numUsers];

    FILE *file = fopen (filename, "r");

    if (file == NULL)
    {
        printf("FILE ERROR");
    }

    char buffer [100];
    fgets(buffer, 100, file );
    fgets(buffer, 100, file);

    for (int i=0; i < numUsers; i ++)
    {
        sscanf(buffer, "%s\n", users[i].name);
        fgets(buffer, 100, file);

        sscanf(buffer, "%lf\n", &users[i].time);
        fgets(buffer, 100, file);

        sscanf(buffer, "%lf\n", &users[i].lat);
        fgets(buffer, 100, file);

        sscanf(buffer, "%lf\n", &users[i].lon);
        fgets(buffer, 100, file);

        sscanf(buffer, "%lf\n", &users[i].alt);
        fgets(buffer, 100, file);
    }
    fclose(file);
    return(users[user]);
}

user_d dist;

user_d distance (user_t ourUser, user_t otherUser)
{
    dist.dist = sqrt(((ourUser.lat - otherUser.lat)*(ourUser.lat - otherUser.lat)+(ourUser.lon - otherUser.lon)*(ourUser.lon - otherUser.lon)+(ourUser.alt - otherUser.alt)*(ourUser.alt - otherUser.alt)));
    strcpy(dist.name, otherUser.name);
    return (dist);
}

int closest(user_d dist_user[], int number)
{
   
    double low = dist_user[0].dist;
    int user = 0;

    for(int i=0; i < number-2; i++)
    {
        if(low > dist_user[i+1].dist)
        {
            low = dist_user[i + 1].dist;
            user = i + 1;
        }

    }
    
    return(user);
}

int main(void){
    char filename[50];

    printf("enter the name of the file:");
    scanf("%s", filename);

    int numUsers = findUsers(filename);
    
    user_t ourUser;
    user_t otherUser[numUsers -1];
    user_d dist_user[numUsers -1];
    
    int nearestUser;
    ourUser = scan_user(filename,0,numUsers);

    for(int x=0; x<numUsers; x++){
        otherUser[x] = scan_user(filename, x+1, numUsers);
    }
    for(int y=0; y<numUsers; y++){
       dist_user[y] =  distance(ourUser, otherUser[y]);
    }
    int number=numUsers;
    nearestUser = closest(dist_user, number);

    printf("%s is the closest\n", otherUser[nearestUser]);
    printf("And they are located at the folowing position in the form (latitude, longitude, altitude): (%f, %f, %f)", otherUser[nearestUser].lat, otherUser[nearestUser].lon, otherUser[nearestUser].alt);
return 0;
}


