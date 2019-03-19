//-------------------------------------------------------------
//         ___  __           ___ _      _    _
//  KRATOS| _ \/ _|___ _ __ | __| |_  _(_)__| |
//        |  _/  _/ -_) '  \| _|| | || | / _` |
//        |_| |_| \___|_|_|_|_| |_|\_,_|_\__,_|DYNAMICS
//
//  License:(BSD)    PfemFluidMechanicsApplication/license.txt
//
//  Main authors:    Josep Maria Carbonell
//                   Alessandro Franci
//                   Miquel Angel Celigueta
//-------------------------------------------------------------
//
//   Project Name:        KratosPfemFluidDynamicsApplication $
//   Created by:          $Author:               JMCarbonell $
//   Last modified by:    $Co-Author:                        $
//   Date:                $Date:               February 2016 $
//   Revision:            $Revision:                     0.0 $
//
//


#if !defined(KRATOS_PFEM_FLUID_DYNAMICS_APPLICATION_VARIABLES_H_INCLUDED )
#define  KRATOS_PFEM_FLUID_DYNAMICS_APPLICATION_VARIABLES_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "includes/kratos_application.h"
#include "includes/variables.h"
#include "includes/cfd_variables.h"
#include "includes/dem_variables.h"
#include "delaunay_meshing_application_variables.h"

namespace Kratos
{
  ///@name Type Definitions
  ///@{
  ///@}

  ///@name Kratos Globals
  ///@{

  // some post process variables + stress invariants
  /* KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, M_MODULUS ) */
  /* KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, int, PATCH_INDEX ) */
  /* KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, NORMVELOCITY ) */
  KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, bool, FREESURFACE )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, INITIAL_DELTA_TIME )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, CURRENT_DELTA_TIME )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, bool, TIME_INTERVAL_CHANGED )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, bool, BAD_VELOCITY_CONVERGENCE )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, bool, BAD_PRESSURE_CONVERGENCE )

    //Papanastasiou variables
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, FLOW_INDEX )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, YIELDED)
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, YIELD_SHEAR )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, ADAPTIVE_EXPONENT )

    //mu(I)-rheology variables
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, STATIC_FRICTION )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, DYNAMIC_FRICTION )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, INERTIAL_NUMBER_ZERO )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, GRAIN_DIAMETER )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, GRAIN_DENSITY )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, REGULARIZATION_COEFFICIENT )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, INFINITE_FRICTION )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, INERTIAL_NUMBER_ONE )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, ALPHA_PARAMETER )

    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, PRESSURE_VELOCITY )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, PRESSURE_ACCELERATION )

    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, NODAL_VOLUME )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_CAUCHY_STRESS )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_DEVIATORIC_CAUCHY_STRESS )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_SFD_NEIGHBOURS )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_SFD_NEIGHBOURS_ORDER )
    /* KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_SFD_NEIGHBOURS_PRESSURE ) */
    /* KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_SFD_NEIGHBOURS_PRESSURE_ID ) */
    /* KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_SFD_NEIGHBOURS_VELOCITY_X_ID ) */
    /* KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_SFD_NEIGHBOURS_VELOCITY_Y_ID ) */
    /* KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_SFD_NEIGHBOURS_VELOCITY_Z_ID ) */
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Matrix, NODAL_DEFORMATION_GRAD )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Matrix, NODAL_DEFORMATION_GRAD_VEL )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_SPATIAL_DEF_RATE )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, Vector, NODAL_SPATIAL_DEF_RATE_BIS )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, NODAL_VOLUMETRIC_DEF_RATE )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, NODAL_MEAN_MESH_SIZE )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, NODAL_FREESURFACE_AREA )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, FIRST_LAME_TYPE_COEFFICIENT )
    KRATOS_DEFINE_APPLICATION_VARIABLE( PFEM_FLUID_DYNAMICS_APPLICATION, double, SECOND_LAME_TYPE_COEFFICIENT )


    //Define Variables
    //Define Variables

    ///@}

    }

#endif	/* KRATOS_PFEM_FLUID_DYNAMICS_APPLICATION_VARIABLES_H_INCLUDED */
