// FGA_(VS_2012).cpp : 定義主控台應用程式的進入點。
//

//  Sep. 10th, 2014
//  Floating point GA
//  selected by tournament

#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <iostream>

#include "stdafx.h"

using namespace std;

#define Fit_name "fit.ga"  // save the maximum fitness value in each generation
#define Init_pop_name "init_pop.ga"  // save solutions in the initial population
#define Final_pop_name "final_pop.ga"  // save solutions in the final popolation
#define Best_Individual_name \
  "best_ind.ga"  // save the best solution and its fitness value

#define _dimension 2  // number of optimized variables
#define _popsize 50   // population size
#define _maxgen 1000  // max generation
#define _number 2     // for tournament selection
#define _pi 3.14159

float _pmutation = 0.1;          // mutation probability
float _pcrossover = 0.1;         // crossover probability
const int _lchrom = _dimension;  // length of a chromosome
float FINTNESS = -1;

class Population {
  friend void generation(int, int *);
  friend void Crossover(int, int, int);
  friend void report(float, float, float, char *);
  friend float Eva_Fitness(int, int);
  float chrom[_lchrom + 1], sum_fitness;
 public:
  float fitness;
  Population() { sum_fitness = 0; }
  void clear();
  void initpop(void);
  void Save_variable(FILE *ft);
  void objfunc(float);
};

//inline float max(float a, float b) { return ((a > b) ? a : b); }
inline float max(float a, float b) { return ((a < b) ? a : b); }

template <class Type>
Type mutation(Type ge, int j) {
  int i;

  if (flip(_pmutation)) {
    ge = (rand() % (20001) - 10000) / 1000.0;  // range: -10 to 10
    //ge = rand();
    _nmutation = _nmutation + 1;
  }

  return ge;
}

int select(int);
void initialize(void);
void statistics(float *, float *, float *, float, int);
void generation(int, int *);
void report(float, float, float, char *);
float Eva_Fitness(int, int);
void Save(int);

Population _oldpop[_popsize + 1], _newpop[_popsize + 1];
int _nmutation = 0, _nncross = 0;
float _avg, _max, _min;

int _tmain(int argc, _TCHAR *argv[]) {
  for (int i = 0; i < 10; i++) {
    int max, gen;

    time_t tt;

    srand((unsigned)time(&tt));

    initialize();

    for (gen = 1; gen < _maxgen; gen++) {
      cout << "\n Generation is --- " << gen;
      generation(gen, &max);
    }
    cout << "\n *******  The  End ****** ";

    Save(max);
  }

  return 0;
}

void initialize() {
  int j;
  float fmax_fit = -1E7, fmin_fit = 1E7, ave_fit = 0, fit;
  FILE *fm;
  errno_t err;

  cout << "\n Initial generation   " << endl;

  for (j = 1; j <= _popsize; j++) {
    _newpop[j].initpop();
    _newpop[j].clear();
  }

  for (j = 1; j <= _popsize; j++) {
    fit = Eva_Fitness(j, 0);
    statistics(&fmax_fit, &fmin_fit, &ave_fit, fit, j);
    _newpop[j].objfunc(fit);
  }

  if ((err = fopen_s(&fm, Init_pop_name, "w")) != 0) exit(1);

  for (j = 1; j <= _popsize; j++) _newpop[j].Save_variable(fm);

  fclose(fm);

  report(fmax_fit, fmin_fit, ave_fit, Fit_name);

  for (j = 1; j <= _popsize; j++) {
    _oldpop[j] = _newpop[j];
  }
}

float Eva_Fitness(int jj, int gen) {
  int k;
  float function_value;

  function_value = 0;

  for (k = 1; k <= _dimension; k++) {
    /*function_value = function_value +
                     _newpop[jj].chrom[k] * _newpop[jj].chrom[k] -
                     10 * cos(2 * _pi * _newpop[jj].chrom[k]) + 10;*/
    double pre_cacu = (_newpop[jj + 1].chrom[k] -
                       (_newpop[jj].chrom[k] * _newpop[jj].chrom[k]));
    function_value += (_newpop[jj].chrom[k] - 1) * (_newpop[jj].chrom[k] - 1) +
                      100 * pre_cacu * pre_cacu;
  }
  return 1. / (1. + function_value);
}

void Population::clear() { fitness = 0; }

void Population ::initpop() {
  int i;

  for (i = 1; i <= _lchrom; i++) {
    chrom[i] = (rand() % (20001) - 10000) / 1000.0;  // range: -10 to 10
    //chrom[i] = rand();  // range: -1 to 1
  }
}

void Population ::objfunc(float fit) { fitness = fit; }

void statistics(float *max_fit, float *min_fit, float *ave_fit, float fit,
                int j) {
  if (fit > *max_fit) {
    *max_fit = fit;
  }
  if (fit < *min_fit) {
    *min_fit = fit;
  }
  *ave_fit = *ave_fit + (1.0 / j) * (fit - *ave_fit);
}

int flip(float prob) {
  int i, j;

  i = rand() % (10001);

  if (i < 10000 * prob)
    j = 1;
  else
    j = 0;

  return j;
}

void generation(int _gen, int *max) {
  int i, mate1, mate2;
  float max_f = -1E8;
  //float max_f = 1E8;
  float fmax_fit = -1E8, fmin_fit = 1E8;
  float fit, ave_fit = 0;

  for (i = 1; i <= _popsize; i++) {
    if (_oldpop[i].fitness > max_f) {
    //if (_oldpop[i].fitness < max_f) {
      max_f = _oldpop[i].fitness;
      *max = i;
    }  // end if
  }    // end for i

  for (i = 1; i <= _popsize / 2; i++) {
    mate1 = select(_popsize);
    mate2 = select(_popsize);
    Crossover(i, mate1, mate2);
  }

  _newpop[*max] = _oldpop[*max];

  for (i = 1; i <= _popsize; i++) {
    _newpop[i].clear();
  }

  for (i = 1; i <= _popsize; i++) {
    fit = Eva_Fitness(i, _gen);
    statistics(&fmax_fit, &fmin_fit, &ave_fit, fit, i);
    _newpop[i].objfunc(fit);
  }

  report(fmax_fit, fmin_fit, ave_fit, Fit_name);

  for (i = 1; i <= _popsize; i++) _oldpop[i] = _newpop[i];
}

void Crossover(int i, int mate1, int mate2) {
  int j, jcross;

  if (flip(_pcrossover)) {
    jcross = rand() % (_lchrom - 1) + 1;

    for (j = 1; j <= jcross; j++) {
      _newpop[2 * i - 1].chrom[j] = mutation(_oldpop[mate1].chrom[j], j);

      _newpop[2 * i].chrom[j] = mutation(_oldpop[mate2].chrom[j], j);
    }

    for (j = jcross + 1; j <= _lchrom; j++) {
      _newpop[2 * i - 1].chrom[j] = mutation(_oldpop[mate2].chrom[j], j);
      _newpop[2 * i].chrom[j] = mutation(_oldpop[mate1].chrom[j], j);
    }

  } else {
    for (j = 1; j <= _lchrom; j++) {
      _newpop[2 * i - 1].chrom[j] = _oldpop[mate1].chrom[j];

      _newpop[2 * i].chrom[j] = _oldpop[mate2].chrom[j];
    }
  }
}

int select(int range) {
  int ran[_number + 1];
  float max_value;
  int max, j = 0;

  for (j = 1; j <= _number; j++) ran[j] = rand() % (range) + 1;

  max = ran[1];
  max_value = _oldpop[max].fitness;

  for (j = 2; j <= _number; j++) {
    if (_oldpop[ran[j]].fitness > max_value) {
    //if (_oldpop[ran[j]].fitness < max_value) {
      max = ran[j];
      max_value = _oldpop[ran[j]].fitness;
    }
  }

  return max;
}

void report(float max_fit, float min_fit, float ave_fit, char *p) {
  static int i = 0;
  FILE *fm;
  errno_t err;

  i++;

  cout << "\n max --- " << max_fit << " , min --- " << min_fit << " , avg -- "
       << ave_fit;
  cout << "  Nmutation " << _nmutation;

  if (i == 2) {
    _nmutation = 0;
    i = 0;
  }

  if ((err = fopen_s(&fm, p, "a")) != 0) exit(1);
  fprintf(fm, "%f  %f  %f", max_fit, min_fit, ave_fit);

  fprintf(fm, "\n");

  fclose(fm);
}

void Save(int max) {
  int j;
  FILE *fm;
  errno_t err;

  if ((err = fopen_s(&fm, Best_Individual_name, "a")) != 0) exit(1);
  _newpop[max].Save_variable(fm);
  
  fclose(fm);

  if (_newpop[max].fitness > FINTNESS && _newpop[max].fitness < 1.2) {
    FINTNESS = _newpop[max].fitness;
    if ((err = fopen_s(&fm, Final_pop_name, "w")) != 0) exit(1);
    for (j = 1; j <= _popsize; j++) _newpop[j].Save_variable(fm);
    fclose(fm);
  }
  
}

void Population::Save_variable(FILE *ft) {
  int k;

  for (k = 1; k <= _lchrom; k++) fprintf(ft, " %f", chrom[k]);

  fprintf(ft, " %f", fitness);
  fprintf(ft, "\n");
}