#include "series_model.h"
#include <stdio.h>

/*Constructor like function, used to set probabilities*/
void set_probabilities(double probH_A_value, double probH_B_value,
  double probA_A_value,double probA_B_value)
  {
    probH_A = probH_A_value;
    probH_B = probH_B_value;
    probA_A = probA_A_value;
    probA_B = probA_B_value;
  }

/*Calculate result matrix, then return it*/
void calculate_results(int * series_arrangement, int best_of_games,
double result_matrix[best_of_games+1][best_of_games+1])
{
  int i = 0;
  int j = 0;
  int games_left = 0;
  double bellman_result = 0;
  result_matrix[0][0] = 0;

  //Set first row to 1s, simplest case 1
  for(i = 1; i <= best_of_games; i++)
  {
    result_matrix[0][i] = 1;
  }

  //Set first column to 0s, simplest case2
  for(i = 1; i <= best_of_games; i++)
  {
    result_matrix[i][0] = 0;
  }

  //use belllman equation to calulate the rest of the matrix;
  for(i = 1; i <= best_of_games; i++)
  {
    for(j = 1; j <= best_of_games; j++)
    {
      games_left = (best_of_games - i) + (best_of_games - j);
      if(series_arrangement[games_left] == 1)
      {
        bellman_result = calc_probability(result_matrix[i-1][j],
        result_matrix[i][j-1],probH_A,probA_B);
      }
      else
      {
        bellman_result = calc_probability(result_matrix[i-1][j],
        result_matrix[i][j-1],probA_A,probH_B);
      }
      result_matrix[i][j] = bellman_result;
    }
  }
}

double calc_probability(double up, double left, double prob1, double prob2)
{
  double final_sum = (up * prob1) + (left * prob2);
  return final_sum;
}
