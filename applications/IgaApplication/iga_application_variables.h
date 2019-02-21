/*
//  KRATOS  _____________
//         /  _/ ____/   |
//         / // / __/ /| |
//       _/ // /_/ / ___ |
//      /___/\____/_/  |_| Application
//
//  Main authors:   Thomas Oberbichler
*/

#if !defined(KRATOS_IGA_APPLICATION_VARIABLES_H_INCLUDED)
#define  KRATOS_IGA_APPLICATION_VARIABLES_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "includes/variables.h"
#include "includes/kratos_application.h"

namespace Kratos
{

KRATOS_DEFINE_VARIABLE(double, NURBS_CONTROL_POINT_WEIGHT)

KRATOS_DEFINE_VARIABLE(Vector, COORDINATES)
KRATOS_DEFINE_VARIABLE(double, CROSSSECTION_ROTATION)
// KRATOS_DEFINE_VARIABLE(Vector, TANGENTS)

KRATOS_DEFINE_VARIABLE(double, CROSS_AREA)
KRATOS_DEFINE_VARIABLE(double, PRESTRESS_CAUCHY)

KRATOS_DEFINE_VARIABLE(Vector, SHAPE_FUNCTION_VALUES)
KRATOS_DEFINE_VARIABLE(Vector, SHAPE_FUNCTION_LOCAL_DER_1)
KRATOS_DEFINE_VARIABLE(Vector, SHAPE_FUNCTION_LOCAL_DER_2)
KRATOS_DEFINE_VARIABLE(Vector, SHAPE_FUNCTION_LOCAL_DER_3)
KRATOS_DEFINE_VARIABLE(Matrix, SHAPE_FUNCTION_LOCAL_DERIVATIVES)
KRATOS_DEFINE_VARIABLE(Matrix, SHAPE_FUNCTION_LOCAL_SECOND_DERIVATIVES)

KRATOS_DEFINE_VARIABLE(double, RAYLEIGH_ALPHA)
KRATOS_DEFINE_VARIABLE(double, RAYLEIGH_BETA)

KRATOS_DEFINE_VARIABLE(double, DISPLACEMENT_ROTATION)
KRATOS_DEFINE_VARIABLE(double, REACTION_ROTATION)

KRATOS_DEFINE_VARIABLE(std::string, DEBUG_EXPECTED_DATA)

KRATOS_DEFINE_VARIABLE(double, SHEAR_MODULUS)
KRATOS_DEFINE_VARIABLE(double, SHEAR_A)
KRATOS_DEFINE_VARIABLE(double, SHEAR_B)

KRATOS_DEFINE_VARIABLE(double, PHI)
KRATOS_DEFINE_VARIABLE(double, PHI_DER_1)
KRATOS_DEFINE_VARIABLE(double, PHI_DER_2)

KRATOS_DEFINE_VARIABLE(Vector, T0)
KRATOS_DEFINE_VARIABLE(Vector, T0_DER)
KRATOS_DEFINE_VARIABLE(Vector, N0)

KRATOS_DEFINE_VARIABLE(Vector, A1)
KRATOS_DEFINE_VARIABLE(Vector, A2)
KRATOS_DEFINE_VARIABLE(Vector, A3)
KRATOS_DEFINE_VARIABLE(Vector, A1_1)
KRATOS_DEFINE_VARIABLE(Vector, A2_1)
KRATOS_DEFINE_VARIABLE(Vector, A3_1)

KRATOS_DEFINE_VARIABLE(double, MOMENT_OF_INERTIA_Y)
KRATOS_DEFINE_VARIABLE(double, MOMENT_OF_INERTIA_Z)
KRATOS_DEFINE_VARIABLE(double, MOMENT_OF_INERTIA_T)

KRATOS_DEFINE_VARIABLE(Vector, LOAD_VECTOR_MOMENT)
KRATOS_DEFINE_VARIABLE(double, LOAD_MOMENT)

KRATOS_DEFINE_VARIABLE(int, DIRICHLET_CONDITION_TYPE)
KRATOS_DEFINE_VARIABLE(double, PENALTY_DISPLACEMENT)
KRATOS_DEFINE_VARIABLE(double, PENALTY_ROTATION)
KRATOS_DEFINE_VARIABLE(double, PENALTY_TORSION)

// KRATOS_DEFINE_VARIABLE(double, NORMAL_STRESS)
// KRATOS_DEFINE_VARIABLE(Vector, TANGENTIAL_STRESS)
KRATOS_DEFINE_VARIABLE(Vector, BENDING_STRESS)


} // namespace Kratos

#endif // !defined(KRATOS_IGA_APPLICATION_VARIABLES_H_INCLUDED)