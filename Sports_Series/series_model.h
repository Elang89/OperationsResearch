#ifndef SERIES_MODEL_H
#define SERIES_MODEL_H

double probH_A;
double probH_B;
double probA_A;
double probA_B;

void set_probabilities(double probH_A_value, double probH_B_value,
  double probA_A_value,double probA_B_value);
void calculate_results(int * series_arrangement, int best_of_games,
  double result_matrix[best_of_games+1][best_of_games+1]);
double calc_probability(double up, double left, double prob1, double prob2);


#endif
