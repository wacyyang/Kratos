//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main author:     Alex Jarauta
//  Co-author  :     Elaf Mahrous



#include "ULF_application_variables.h"

namespace Kratos
{
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PRESSURE_FORCE)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DISP_FRAC)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VAUX)
    KRATOS_CREATE_VARIABLE(double, TAUONE)
    KRATOS_CREATE_VARIABLE(double, TAUTWO)
    KRATOS_CREATE_VARIABLE(double, NODAL_LENGTH)
    KRATOS_CREATE_VARIABLE(double, MEAN_CURVATURE_2D)
    KRATOS_CREATE_VARIABLE(double, TRIPLE_POINT)
    KRATOS_CREATE_VARIABLE(double, CONTACT_ANGLE)
    KRATOS_CREATE_VARIABLE(double, CONTACT_ANGLE_STATIC)
    KRATOS_CREATE_VARIABLE( double, SURFACE_TENSION_COEF ) 
    KRATOS_CREATE_VARIABLE( double, MEAN_CURVATURE_3D )
    KRATOS_CREATE_VARIABLE( double, GAUSSIAN_CURVATURE )
    KRATOS_CREATE_VARIABLE( double, PRINCIPAL_CURVATURE_1 )
    KRATOS_CREATE_VARIABLE( double, PRINCIPAL_CURVATURE_2 ) 
    KRATOS_CREATE_VARIABLE(double, SUBSCALE_PRESSURE)
    KRATOS_CREATE_VARIABLE(double, INITIAL_MESH_SIZE)
    
    KRATOS_CREATE_VARIABLE(double, DISSIPATIVE_FORCE_COEFF_JM_X)
    KRATOS_CREATE_VARIABLE(double, DISSIPATIVE_FORCE_COEFF_BM_X)
    KRATOS_CREATE_VARIABLE(double, DISSIPATIVE_FORCE_COEFF_SM_X)
    
    KRATOS_CREATE_VARIABLE(double, DISSIPATIVE_FORCE_COEFF_JM_Y)
    KRATOS_CREATE_VARIABLE(double, DISSIPATIVE_FORCE_COEFF_BM_Y)
    KRATOS_CREATE_VARIABLE(double, DISSIPATIVE_FORCE_COEFF_SM_Y)
    
    KRATOS_CREATE_VARIABLE(double, DISSIPATIVE_FORCE_COEFF_JM_Z)
    KRATOS_CREATE_VARIABLE(double, DISSIPATIVE_FORCE_COEFF_BM_Z)
    KRATOS_CREATE_VARIABLE(double, DISSIPATIVE_FORCE_COEFF_SM_Z)
    
    KRATOS_CREATE_VARIABLE(double, TESTFACTA)
    KRATOS_CREATE_VARIABLE(double, TESTFACTB)
    KRATOS_CREATE_VARIABLE(double, TESTFACTC)
    KRATOS_CREATE_VARIABLE(double, TESTFACTD)
    KRATOS_CREATE_VARIABLE(double, SOLID_LIQIUD_SURFTENS_COEFF)
//     KRATOS_CREATE_VARIABLE(double, SOLID_AIR_SURFTENS_COEFF)
//     

    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(RHS_VECTOR)
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(AUX_VECTOR)
    //KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(AUX_VEL)
    //KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(CONVECTION_VELOCITY)
    //KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(AUX_VEL1)

    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( VISCOUS_STRESSX )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( VISCOUS_STRESSY )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( VISCOUS_STRESSZ ) 
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( PRINCIPAL_DIRECTION_1 ) 
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( PRINCIPAL_DIRECTION_2 )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( NORMAL_GEOMETRIC )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( ADHESION_FORCE )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( NORMAL_EQUILIBRIUM )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( NORMAL_CONTACT_LINE_EQUILIBRIUM )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( NORMAL_TRIPLE_POINT )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( NORMAL_CONTACT_LINE )
//     KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( SOLID_FRACTION_GRADIENT )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( SOLID_FRACTION_GRADIENT_PROJECTED )
    KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SUBSCALE_VELOCITY)
    
}
 
