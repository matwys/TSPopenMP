#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <unistd.h>

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>

using namespace std;

// compilowanie# g++ -fopenmp TSP.cpp
// Przykładowe uruchomienie# ./a.out 100

void algorytmSeq(int **miasta, int nMiast){
    int bestTime = -1;


    {

        for(int i = 0; i < nMiast; i++){
            
            int sum = 0;

            int odwiedzony[nMiast] = {0};
            odwiedzony[i] = 1;
            int actual = i;
            int kolejnosc[nMiast+1];
            kolejnosc[0]=actual;

            for(int n = 0; n < nMiast-1; n++){

                int min = miasta[actual][actual];
                int best = actual;

                for(int j = 0; j < nMiast; j++){
                    if(odwiedzony[j] == 0 && min > miasta[actual][j]){
                        best = j;
                        min = miasta[actual][j];
                    }
                }

                sum = sum + min;
                if( (sum > bestTime && bestTime != -1)){
                    break;
                }
                odwiedzony[best] = 1;
                actual = best;
                kolejnosc[n+1] = actual;
            }
            sum = sum + miasta[actual][i];
            
            if(bestTime == -1 || bestTime > sum){
                bestTime = sum;
            }
        }
    }
    cout <<"Cykl Hamiltona sekwencyjnie: " << bestTime << endl;
}

void algorytmPar(int **miasta, int nMiast){
    int bestTime;
    int globalBestTime;

    omp_set_num_threads(8);
    #pragma omp parallel private(bestTime) shared(globalBestTime) 
    {
        bestTime = -1;

        globalBestTime = -1;
        #pragma omp for
        for(int i = 0; i < nMiast; i++){

            int sum = 0;

            int odwiedzony[nMiast] = {0};
            odwiedzony[i] = 1;
            int actual = i;
            int kolejnosc[nMiast+1];
            kolejnosc[0]=actual;

            for(int n = 0; n < nMiast-1; n++){
                
                int min = miasta[actual][actual];
                int best = actual;

                for(int j = 0; j < nMiast; j++){
                    if(odwiedzony[j] == 0 && min > miasta[actual][j]){
                        best = j;
                        min = miasta[actual][j];
                    }
                }
                
                sum = sum + min;
                if( (sum > bestTime && bestTime != -1) || (sum > globalBestTime && globalBestTime != -1)){
                    break;
                }
                odwiedzony[best] = 1;
                actual = best;
                kolejnosc[n+1] = actual;
            }
            sum = sum + miasta[actual][i];
            
            if(bestTime == -1 || bestTime > sum){
                bestTime = sum;
            }
        }
        #pragma omp critical
        if(globalBestTime == -1 || globalBestTime > bestTime){
            globalBestTime = bestTime;
        }
    }
    cout <<"Cykl Hamiltona równolegle: " << globalBestTime << endl;
}

int main(int argc, char *argv[]){

    int n_miast = atoi(argv[1]);

    int *** miasta = (int ***)malloc(1*sizeof(int**));
    for (int i = 0; i< 1; i++) {
        miasta[i] = (int **) malloc(n_miast*sizeof(int *));
        for (int j = 0; j < n_miast; j++) {
            miasta[i][j] = (int *)malloc(n_miast*sizeof(int));
        }

    }

    srand(time(0));
    for(int m = 0; m<1; m++){
        for(int i = 0; i < n_miast; i++){
            for(int j = 0; j < n_miast; j++){
                if(i==j){
                    miasta[m][i][j] = 999;
                } else if(j < i){
                    miasta[m][i][j] = miasta[m][j][i];
                } else {
                    miasta[m][i][j] = rand() % 10 + 10;
                }
            }
        }
    }

    double timeStart = clock();
    algorytmSeq(miasta[0], n_miast);
    double timeEnd = clock();
    cout<<"Time: " << (timeEnd - timeStart)/CLOCKS_PER_SEC << " seconds"<<endl;

    timeStart = omp_get_wtime();
    algorytmPar(miasta[0], n_miast);
    timeEnd = omp_get_wtime();
    cout<<"Time: " << (timeEnd - timeStart) << " seconds"<<endl;


    for (int i = 0; i< 1; i++) {
        for (int j = 0; j < n_miast; j++) {
            free(miasta[i][j]);
        }
        free(miasta[i]);
    }
    free(miasta);
    return 0;
}