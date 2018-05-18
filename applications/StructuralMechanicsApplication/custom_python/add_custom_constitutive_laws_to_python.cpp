// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:         BSD License
//                   license: structural_mechanics_application/license.txt
//
//  Main authors:    Riccardo Rossi
//


// System includes
#include <pybind11/stl.h>

// External includes

// Project includes
#include "includes/define_python.h"
#include "includes/constitutive_law.h"
#include "custom_python/add_custom_constitutive_laws_to_python.h"
#include "custom_constitutive/truss_constitutive_law.h"
#include "custom_constitutive/beam_constitutive_law.h"
#include "custom_constitutive/elastic_isotropic_3d.h"
#include "custom_constitutive/axisym_elastic_isotropic.h"
#include "custom_constitutive/linear_plane_stress.h"
#include "custom_constitutive/linear_plane_strain.h"
#include "custom_constitutive/elastic_isotropic_plane_stress_uncoupled_shear.h"
#include "custom_constitutive/hyper_elastic_isotropic_kirchhoff_3d.h"
#include "custom_constitutive/hyper_elastic_isotropic_kirchhoff_plane_stress_2d.h"
#include "custom_constitutive/hyper_elastic_isotropic_kirchhoff_plane_strain_2d.h"
#include "custom_constitutive/hyper_elastic_isotropic_neo_hookean_3d.h"
#include "custom_constitutive/hyper_elastic_isotropic_neo_hookean_plane_strain_2d.h"
#include "custom_constitutive/linear_elastic_orthotropic_2D_law.h"

#include "custom_constitutive/linear_j2_plasticity_3d.h"
#include "custom_constitutive/linear_j2_plasticity_plane_strain_2d.h"
#include "custom_constitutive/linear_isotropic_damage_3D_law.h"

// CL
#include "custom_constitutive/generic_small_strain_isotropic_plasticity_3d.h"

// Yield surfaces
#include "custom_constitutive/yield_surfaces/generic_yield_surface.h"
#include "custom_constitutive/yield_surfaces/von_mises_yield_surface.h"
#include "custom_constitutive/yield_surfaces/modified_mohr_coulomb_yield_surface.h"
#include "custom_constitutive/yield_surfaces/rankine_yield_surface.h"
#include "custom_constitutive/yield_surfaces/simo_ju_yield_surface.h"
#include "custom_constitutive/yield_surfaces/drucker_prager_yield_surface.h"
#include "custom_constitutive/yield_surfaces/tresca_yield_surface.h"

// Plastic potentials
#include "custom_constitutive/plastic_potentials/generic_plastic_potential.h"
#include "custom_constitutive/plastic_potentials/von_mises_plastic_potential.h"
#include "custom_constitutive/plastic_potentials/tresca_plastic_potential.h"

// Stress Integrators
#include "custom_constitutive/constitutive_laws_integrators/generic_constitutive_law_integrator_plasticity.h"

namespace Kratos
{
namespace Python
{

using namespace pybind11;


void  AddCustomConstitutiveLawsToPython(pybind11::module& m)
{

    class_< TrussConstitutiveLaw, typename TrussConstitutiveLaw::Pointer, ConstitutiveLaw >
    (m, "TrussConstitutiveLaw").def(init<>() )
    ;

    class_< BeamConstitutiveLaw, typename BeamConstitutiveLaw::Pointer, ConstitutiveLaw >
    (m, "BeamConstitutiveLaw").def(init<>() )
    ;

    class_< LinearPlaneStress, typename LinearPlaneStress::Pointer, ConstitutiveLaw >
    (m, "LinearElasticPlaneStress2DLaw").def(init<>() )
    ;
    
    class_< LinearPlaneStrain, typename LinearPlaneStrain::Pointer, ConstitutiveLaw >
    (m, "LinearElasticPlaneStrain2DLaw").def(init<>() )
    ;

    class_< ElasticIsotropic3D, typename ElasticIsotropic3D::Pointer, ConstitutiveLaw >
    (m, "LinearElastic3DLaw").def(init<>() )
    ;

    class_< AxisymElasticIsotropic, typename AxisymElasticIsotropic::Pointer, ConstitutiveLaw >
    (m, "LinearElasticAxisym2DLaw").def(init<>() )
    ;

    class_< ElasticIsotropicPlaneStressUncoupledShear, typename ElasticIsotropicPlaneStressUncoupledShear::Pointer, ConstitutiveLaw >
    (m, "ElasticPlaneStressUncoupledShear2DLaw").def(init<>() )
    ;

    class_< HyperElasticIsotropicKirchhoff3D, typename HyperElasticIsotropicKirchhoff3D::Pointer, ConstitutiveLaw >
    (m, "KirchhoffSaintVenant3DLaw").def(init<>() )
    ;

    class_< HyperElasticIsotropicKirchhoffPlaneStress2D, typename HyperElasticIsotropicKirchhoffPlaneStress2D::Pointer, ConstitutiveLaw >
    (m, "KirchhoffSaintVenantPlaneStress2DLaw").def(init<>() )
    ;

    class_< HyperElasticIsotropicKirchhoffPlaneStrain2D, typename HyperElasticIsotropicKirchhoffPlaneStrain2D::Pointer, ConstitutiveLaw >
    (m, "KirchhoffSaintVenantPlaneStrain2DLaw").def(init<>() )
    ;

    class_< HyperElasticIsotropicNeoHookean3D, typename HyperElasticIsotropicNeoHookean3D::Pointer, ConstitutiveLaw >
    (m, "HyperElastic3DLaw").def(init<>() )
    ;
    
    class_< HyperElasticIsotropicNeoHookeanPlaneStrain2D, typename HyperElasticIsotropicNeoHookeanPlaneStrain2D::Pointer, ConstitutiveLaw >
    (m, "HyperElasticPlaneStrain2DLaw").def(init<>() )
    ;

    class_< LinearElasticOrthotropic2DLaw, typename LinearElasticOrthotropic2DLaw::Pointer, ConstitutiveLaw >
    (m,"LinearElasticOrthotropic2DLaw").def( init<>())
    ;

    class_< LinearJ2PlasticityPlaneStrain2D, typename LinearJ2PlasticityPlaneStrain2D::Pointer,  ConstitutiveLaw >
    (m,"LinearJ2PlasticityPlaneStrain2DLaw").def( init<>())
    ;

    class_< LinearJ2Plasticity3D, typename LinearJ2Plasticity3D::Pointer,  ConstitutiveLaw >
    (m,"LinearJ2Plasticity3DLaw").def( init<>())
    ;

    class_< LinearIsotropicDamage3D, typename LinearIsotropicDamage3D::Pointer,  ConstitutiveLaw  >
    (m,"LinearIsotropicDamage3DLaw").def( init<>())
    ;
	
	// A.Cornejo
// 	class_< GenericSmallStrainIsotropicPlasticity3D<class YieldSurfaceType, class ConstLawIntegratorType>,
// 	typename GenericSmallStrainIsotropicPlasticity3D<class YieldSurfaceType, class ConstLawIntegratorType>::Pointer,  ConstitutiveLaw  >
//    (m,"GenericSmallStrainIsotropicPlasticity3DLaw").def( init<>())
//    ;
}

}  // namespace Python.
}  // namespace Kratos.
