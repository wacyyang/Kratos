// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:             BSD License
//                                       license: StructuralMechanicsApplication/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//

// System includes

// External includes

// Project includes
#include "contact_structural_mechanics_application_variables.h"
#include "custom_processes/alm_fast_init_process.h"

namespace Kratos
{
void ALMFastInit::Execute()
{
    KRATOS_TRY;
    
    // First we reorder the conditions ids (may methods and utilities assume that conditions are ordered)
    ConditionsArrayType& r_root_conditions_array = mrThisModelPart.GetRootModelPart().Conditions();
    const auto it_root_cond_begin = r_root_conditions_array.begin();

    #pragma omp parallel for
    for(int i = 0; i < static_cast<int>(r_root_conditions_array.size()); ++i) {
        auto it_cond = it_root_cond_begin + i;
        it_cond->SetId(i + 1);
    }

    // We differentiate between frictional or frictionless
    const bool is_frictional = mrThisModelPart.Is(SLIP);
    
    // We initialize the penalty parameter
    const double epsilon = mrThisModelPart.GetProcessInfo()[INITIAL_PENALTY];
    
    // Auxiliar zero array
    const array_1d<double, 3> zero_array = ZeroVector(3);

    // We iterate over the nodes
    NodesArrayType& r_nodes_array = mrThisModelPart.Nodes();
    const auto it_node_begin = r_nodes_array.begin();
    
    #pragma omp parallel for
    for(int i = 0; i < static_cast<int>(r_nodes_array.size()); ++i) {
        auto it_node = it_node_begin + i;
        
        // Weighted values
        it_node->FastGetSolutionStepValue(WEIGHTED_GAP) = 0.0;
        if (is_frictional)
            it_node->FastGetSolutionStepValue(WEIGHTED_SLIP) = zero_array;
        
        // Penalty parameter
        it_node->SetValue(INITIAL_PENALTY, epsilon);
        
        // Auxiliar values
        it_node->SetValue(DYNAMIC_FACTOR, 1.0);
        it_node->SetValue(AUGMENTED_NORMAL_CONTACT_PRESSURE, 0.0);
        if (is_frictional)
            it_node->SetValue(AUGMENTED_TANGENT_CONTACT_PRESSURE, zero_array);
    }
    
    // Now we iterate over the conditions
    ConditionsArrayType& r_conditions_array = mrThisModelPart.Conditions();
    const auto it_cond_begin = r_conditions_array.begin();
    
    #pragma omp parallel for
    for(int i = 0; i < static_cast<int>(r_conditions_array.size()); ++i) {
        auto it_cond = it_cond_begin + i;
        it_cond->SetValue(NORMAL, zero_array); // The normal and tangents vectors
    }

    if (is_frictional) {
        // We initialize the frictional coefficient. The evolution of the frictional coefficient it is supposed to be controled by a law
        #pragma omp parallel for
        for(int i = 0; i < static_cast<int>(r_nodes_array.size()); ++i) {
            auto it_node = it_node_begin + i;
            it_node->SetValue(FRICTION_COEFFICIENT, 0.0);
            it_node->SetValue(NODAL_AREA, 0.0);
        }

        #pragma omp parallel for
        for(int i = 0; i < static_cast<int>(r_conditions_array.size()); ++i) {
            auto it_cond = it_cond_begin + i;

            auto p_prop = it_cond->pGetProperties();
            const double friction_coefficient = p_prop->GetValue(FRICTION_COEFFICIENT);
            auto& r_geom = it_cond->GetGeometry();

            for (auto& r_node : r_geom) {
                double& r_friction_coefficient = r_node.GetValue(FRICTION_COEFFICIENT);
                #pragma omp atomic
                r_friction_coefficient += friction_coefficient;

                double& r_nodal_area = r_node.GetValue(NODAL_AREA);
                #pragma omp atomic
                r_nodal_area += 1.0;
            }
        }

        #pragma omp parallel for
        for(int i = 0; i < static_cast<int>(r_nodes_array.size()); ++i) {
            auto it_node = it_node_begin + i;
            double& friction_coefficient = it_node->GetValue(FRICTION_COEFFICIENT);
            friction_coefficient /= it_node->GetValue(NODAL_AREA);
        }
    }

    KRATOS_CATCH("");
} // class ALMFastInit
} // namespace Kratos
