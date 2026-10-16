#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {
    int N, rounds;

    //step 1 is input

    printf("Enter number of voters: ");
    scanf("%d", &N);
    printf("Enter number of rounds: ");
    scanf("%d", &rounds);


    // step 2 create pipes
    int fd[N][2];       
    int go[N][2];       
    for (int i = 0; i < N; i++) {
        pipe(fd[i]);
        pipe(go[i]);
    }
    //step 3 create votes
    int id = -1;
    for (int i = 0; i < N; i++) {
        if (fork() == 0) {
            id = i;
            break;
        }
    }

    srand(time(NULL) * getpid());
// step 4 voting
    for (int r = 1; r <= rounds; r++) {
        if (id == -1) {
            int yes = 0, vote;
            printf("\n Round %d \n", r);

            for (int i = 0; i < N; i++)
            //start of round
                write(go[i][1], &r, sizeof(r));     

            for (int i = 0; i < N; i++) {
                read(fd[i][0], &vote, sizeof(vote));
                yes += vote;
            }

            if (yes > N - yes)
                printf("Round %d result: YES\n", r);
            else
                printf("Round %d result: NO\n", r);
        } else {
            
            int start;
            //wait on judge
            read(go[id][0], &start, sizeof(start)); 

            int vote = (rand() >> 8) & 1;
            if (vote == 1)
                printf("Round %d: Voter %d votes YES\n", r, id);
            else
                printf("Round %d: Voter %d votes NO\n", r, id);
            write(fd[id][1], &vote, sizeof(vote));
        }
    }
    // judge waits
    if (id == -1) {
        for (int i = 0; i < N; i++)
            wait(NULL);
    }

    return 0;
}