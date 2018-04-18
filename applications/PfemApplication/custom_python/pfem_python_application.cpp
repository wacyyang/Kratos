//
//   Project Name:        KratosPfemApplication     $
//   Created by:          $Author:      JMCarbonell $
//   Last modified by:    $Co-Author:               $
//   Date:                $Date:      February 2016 $
//   Revision:            $Revision:            0.0 $
//
//

#if defined(KRATOS_PYTHON)

// System includes 

// External includes 
#include <boost/python.hpp>

// Project includes 
#include "custom_python/add_custom_processes_to_python.h"
#include "custom_python/add_custom_utilities_to_python.h"
#include "custom_python/add_custom_modelers_to_python.h"
#include "custom_python/add_custom_bounding_to_python.h"

#include "pfem_application.h"
 
namespace Kratos
{

  namespace Python
  {

    using namespace boost::python;
  
    BOOST_PYTHON_MODULE(KratosPfemApplication    )
    {

      class_<KratosPfemApplication    , 
	     KratosPfemApplication    ::Pointer, 
	     bases<KratosApplication>, boost::noncopyable >("KratosPfemApplication")
	  ;

      AddCustomProcessesToPython();
      AddCustomUtilitiesToPython();
      AddCustomModelersToPython();
      AddCustomBoundingToPython();
      
      //registering variables in python ( if must to be seen from python )
      KRATOS_REGISTER_IN_PYTHON_VARIABLE( INITIALIZED_DOMAINS )
      KRATOS_REGISTER_IN_PYTHON_VARIABLE( MESHING_STEP_PERFORMED )
      KRATOS_REGISTER_IN_PYTHON_VARIABLE( LMV )
      KRATOS_REGISTER_IN_PYTHON_VARIABLE( RIGID_WALL )
      KRATOS_REGISTER_IN_PYTHON_VARIABLE( MEAN_ERROR )
      KRATOS_REGISTER_IN_PYTHON_VARIABLE( OFFSET )
      KRATOS_REGISTER_IN_PYTHON_VARIABLE( SHRINK_FACTOR )

    }
  
  
  }  // namespace Python.
  
}  // namespace Kratos.

#endif // KRATOS_PYTHON defined
