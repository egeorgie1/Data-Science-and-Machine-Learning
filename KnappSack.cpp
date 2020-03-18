#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iostream>
using namespace std;

const int BUDGET = 5000;
const int N = 24; // number artefacts
const int POPULATION_SIZE = 100;
const int MAX_ITER = 1000;
const int K = 10; //crossovers per iteration
int artefactsPrices[N] = {100,120,1600,700,150,680,270,385,230,520,1700,500,240,480,730,420,430,220,70,180,40,300,900,2000};
int artefactsValues[N] = {150,40,200,160,60,45,60,48,30,10,400,300,15,10,40,70,75,80,20,12,50,10,20,150};

class chromosome {
 public:
     int taken[N]; //0 or 1 means taken or not
     int fitness;
     int totalPrice;

     chromosome(){
         for(int i=0; i<N; i++)
            taken[i] = rand() % 2;

         calcTotalPrice();
         calcFitness();
     }

     void calcTotalPrice(){
       int sum = 0;
       for(int i=0; i<N; i++)
         sum += taken[i] * artefactsPrices[i];

       totalPrice = sum;
     }

     void calcFitness(){
       if(totalPrice > BUDGET)
          fitness = 0;
       else {
          int sum = 0;
          for(int i=0; i<N; i++)
            sum += taken[i] * artefactsValues[i];

          fitness = sum;
       }
     }

     void mutation(){ // swap 2 random positions
         int pos1 = rand() % N;
         int pos2 = rand() % N;

         swap(taken[pos1], taken[pos2]);
         //recalculate totalPrice and fitness
         calcTotalPrice();
         calcFitness();
     }

      chromosome& operator=(const chromosome& c){
         if(this != &c){
            for(int i=0; i<N; i++)
              taken[i] = c.taken[i];

            totalPrice = c.totalPrice;
            fitness = c.fitness;
         }
         return *this;
      }

      bool operator<(const chromosome& c) const{
         return fitness < c.fitness;
      }

};

void crossover(int p1, int p2, vector<chromosome>& population, int pos){
  //Uniform Crossover
  chromosome child1, child2;

  for(int i=0; i<N; i++){
    if(rand() % 2){ // remainder 1
        child1.taken[i] = population[p2].taken[i];
        child2.taken[i] = population[p1].taken[i];
    }
    else{ // remainder 0
        child1.taken[i] = population[p1].taken[i];
        child2.taken[i] = population[p2].taken[i];
    }
  }

  child1.calcTotalPrice();
  child1.calcFitness();

  child2.calcTotalPrice();
  child2.calcFitness();

  population[pos] = child1;
  population[K + pos] = child2;

}

void geneticAlg(){
  vector<chromosome> population;
  //generate random population
  for (int i = 0; i < POPULATION_SIZE; i++) {
	  chromosome c;
	  population.push_back(c);
  }
  // sort in ascending order according to the fitness function
  sort(population.begin(),population.end());

  for(int i=0; i<MAX_ITER; i++){
    //crossover K times => 2*K new children
    //we choose randomly 2 parents among the 10 best candidates (right end of the vector because of the asc. order)

    for(int j=0; j<K; j++){
        int p1 = rand() % 10; //random number between 0 and 9
        int p2 = rand() % 10; //random number between 0 and 9

        // the 2 new children will be at positions j and K+j on the left side of the population vector, replacing
        // two of the worst individuals in the population
        crossover((POPULATION_SIZE - 1) - p1, (POPULATION_SIZE - 1) - p2, population, j);
    }
    //mutation with speed 3%, i.e. 3% of the population will mutate
    int mutations = (int) (0,03 * POPULATION_SIZE);
    for(int x=0; x<mutations; x++){
        int mutIndex = rand() % POPULATION_SIZE; // the individual at this position will mutate
        population[mutIndex].mutation();
    }

    sort(population.begin(),population.end());
  }

  //after MAX_ITER iterations
  //print best solution, it is at position POPULATION_SIZE - 1
  for(int i=0; i<N; i++)
    cout << population[POPULATION_SIZE - 1].taken[i];
  cout << endl;

}
int main(){
  geneticAlg();
return 0;
}
