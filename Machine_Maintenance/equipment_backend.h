#ifndef EQUIPMENT_BACKEND_H
#define EQUIPMENT_BACKEND_H

typedef struct EquipmentData{
  int plan_duration;
  int product_viability;
  int new_product_cost;
  float *maintenance_costs;
  float *resale_prices;
  float *solutions;
  float **cost_matrix;
  int **result_matrix;
}EquipmentData;

EquipmentData* new_equipment_data(int plan_duration,
  int product_viability, int new_product_cost,
  float * maintenance_costs, float * resale_prices);

void calculate_costs(EquipmentData * equipment_data);
void calculate_optimal_plans(EquipmentData * equipment_data);

#endif
