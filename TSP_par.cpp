#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <unistd.h>

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>

/*
Kompilowanie: g++ -fopenmp TSP_par.cpp
*/

#define n_miast 1000
#define n_matrix 10

using namespace std;

void algorytm(int **miasta, int nMiast){
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
}

int main() {

    int *** miasta = (int ***)malloc(n_matrix*sizeof(int**));
    for (int i = 0; i< n_matrix; i++) {
        miasta[i] = (int **) malloc(n_miast*sizeof(int *));
        for (int j = 0; j < n_miast; j++) {
            miasta[i][j] = (int *)malloc(n_miast*sizeof(int));
        }

    }

    srand(time(0));
    for(int m = 0; m<n_matrix; m++){
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

    
    double timeStart = omp_get_wtime();
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 200);
    }
    double timeEnd = omp_get_wtime();
    cout<<"200 Time: " << (timeEnd - timeStart)/n_matrix<< " seconds"<<endl;

    timeStart = omp_get_wtime();
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 400);
    }
    timeEnd = omp_get_wtime();
    cout<<"400 Time: " << (timeEnd - timeStart)/n_matrix<< " seconds"<<endl;

    timeStart = omp_get_wtime();
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 600);
    }
    timeEnd = omp_get_wtime();
    cout<<"600 Time: " << (timeEnd - timeStart)/n_matrix<< " seconds"<<endl;

    timeStart = omp_get_wtime();;
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 800);
    }
    timeEnd = omp_get_wtime();
    cout<<"800 Time: " << (timeEnd - timeStart)/n_matrix<< " seconds"<<endl;

    timeStart = omp_get_wtime();
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 1000);
    }
    timeEnd = omp_get_wtime();
    cout<<"1000 Time: " << (timeEnd - timeStart)/n_matrix<< " seconds"<<endl;


    for (int i = 0; i< n_matrix; i++) {
        for (int j = 0; j < n_miast; j++) {
            free(miasta[i][j]);
        }
        free(miasta[i]);
    }
    free(miasta);
    
   return 0;
   
   
}