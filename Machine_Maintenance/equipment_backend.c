#include <stdlib.h>
#include <stdio.h>
#include "equipment_backend.h"
#define INF 99999


EquipmentData* new_equipment_data(int plan_duration,
  int product_viability, int new_product_cost,
  float * maintenance_costs, float * resale_prices)
  {
    EquipmentData *equipment_data = malloc(sizeof(EquipmentData));
    int i;
    int j;

    equipment_data->cost_matrix = malloc(plan_duration * sizeof(int*) + 1);
    equipment_data->result_matrix = malloc(plan_duration * sizeof(int*) + 1);
    equipment_data->solutions = malloc(plan_duration * sizeof(float)+1);
    equipment_data->plan_duration = plan_duration;
    equipment_data->product_viability = product_viability;
    equipment_data->new_product_cost = new_product_cost;
    equipment_data->maintenance_costs = maintenance_costs;
    equipment_data->resale_prices = resale_prices;

    for(i = 0; i <= plan_duration; i++)
    {
      equipment_data->solutions[i] = INF;
    }

    for(i = 0; i <= plan_duration; i++)
    {
      equipment_data->cost_matrix[i] = malloc(plan_duration * sizeof(int)+1);
      equipment_data->result_matrix[i] = malloc(plan_duration * sizeof(int*)+1);
    }


    for(i = 0; i <= product_viability; i++)
    {
      for(j = 0; j <= plan_duration; j++)
      {
        equipment_data->cost_matrix[i][j] = 0;
        equipment_data->result_matrix[i][j] = 0;
      }
    }

    for(i = 0; i <= plan_duration; i++)
    {
      for(j = 0; j <= plan_duration; j++)
      {
        equipment_data->result_matrix[i][j] = 0;
      }
    }

    calculate_costs(equipment_data);
    calculate_optimal_plans(equipment_data);
    return equipment_data;
  }

void calculate_costs(EquipmentData * equipment_data)
{
  int start_year;
  int end_year = 1;
  int difference = 1;
  float yearly_maintenance_cost = 0;
  float total_cost;

  while( difference <= equipment_data->product_viability)
  {
    yearly_maintenance_cost += equipment_data->maintenance_costs[difference];
    for(start_year = 0; end_year <= equipment_data->plan_duration; start_year++, end_year++)
    {
      total_cost = equipment_data->new_product_cost + yearly_maintenance_cost
      - equipment_data->resale_prices[difference];
      equipment_data->cost_matrix[start_year][end_year] = total_cost;
    }
    difference++;
    end_year = difference;
  }

}

void calculate_optimal_plans(EquipmentData * equipment_data)
{
  equipment_data->solutions[equipment_data->plan_duration] = 0;
  float min_value;
  int start;
  int end;

  for(start = equipment_data->plan_duration-1; start >= 0; start--)
  {
    end = start + 1;

    while(end - start <= equipment_data->product_viability
      && end <= equipment_data->plan_duration)
    {
      min_value = equipment_data->cost_matrix[start][end] + equipment_data->solutions[end];

      if(min_value < equipment_data->solutions[start])
      {
        equipment_data->solutions[start] = min_value;
      }
      end++;
    }

    end = start + 1;

    while(end - start <= equipment_data->product_viability
      && end <= equipment_data->plan_duration)
    {
      min_value = equipment_data->cost_matrix[start][end] + equipment_data->solutions[end];

      if(min_value == equipment_data->solutions[start])
      {
        equipment_data->result_matrix[start][end] = end;
      }
      end++;
    }

  }
}
