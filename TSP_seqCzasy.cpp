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
Kompilowanie: g++ TSP_par.cpp
*/

#define n_miast 1000
#define n_matrix 10

using namespace std;

void algorytm(int **miasta, int nMiast){
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
    cout << bestTime << endl;
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

    
    double timeStart = clock();
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 200);
    }
    double timeEnd = clock();
    cout<<"200 Time: " << (timeEnd - timeStart)/CLOCKS_PER_SEC/n_matrix<< " seconds"<<endl;

    timeStart = clock();
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 400);
    }
    timeEnd = clock();
    cout<<"400 Time: " << (timeEnd - timeStart)/CLOCKS_PER_SEC/n_matrix<< " seconds"<<endl;

    timeStart = clock();
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 600);
    }
    timeEnd = clock();
    cout<<"600 Time: " << (timeEnd - timeStart)/CLOCKS_PER_SEC/n_matrix<< " seconds"<<endl;

    timeStart = clock();
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 800);
    }
    timeEnd = clock();
    cout<<"800 Time: " << (timeEnd - timeStart)/CLOCKS_PER_SEC/n_matrix<< " seconds"<<endl;

    timeStart = clock();
    for(int i = 0; i<n_matrix; i++){
        algorytm(miasta[i], 1000);
    }
    timeEnd = clock();
    cout<<"1000 Time: " << (timeEnd - timeStart)/CLOCKS_PER_SEC/n_matrix<< " seconds"<<endl;


    for (int i = 0; i< n_matrix; i++) {
        for (int j = 0; j < n_miast; j++) {
            free(miasta[i][j]);
        }
        free(miasta[i]);
    }
    free(miasta);
    

        for (int i = 0; i< n_matrix; i++) {
        for (int j = 0; j < n_miast; j++) {
            free(miasta[i][j]);
        }
        free(miasta[i]);
    }
    free(miasta);
    return 0;
   
   
}