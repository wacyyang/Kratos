//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Miguel Maso Sotomayor
//


#if !defined( KRATOS_NESTED_REFINEMENT_UTILITY_H_INCLUDED )
#define KRATOS_NESTED_REFINEMENT_UTILITY_H_INCLUDED


// System includes
#include <string>
#include <iostream>
#include <unordered_map>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/node.h"
#include "includes/element.h"
#include "includes/condition.h"
#include "includes/model_part.h"
#include "includes/key_hash.h"


namespace Kratos
{
///@addtogroup MeshingApplication
///@{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/// Refine a mesh by adding a nested mesh
/** This class fully refine a model part by adding a nested mesh inside each element.
 *  A node is added on each element edge (an additional node is added inside quadrilaterals
 *  and tetrahedrons) to split the elements.
 *  If a higher refinement is needed, the utility can be called recursively.
 */
class NestedRefinementUtility
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of NestedRefinementUtility
    KRATOS_CLASS_POINTER_DEFINITION(NestedRefinementUtility);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    NestedRefinementUtility(ModelPart& rModelPart);

    /// Destructor.
    virtual ~NestedRefinementUtility();


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{

    /**
     * Execute the refinement
     */
    void Refine();


    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    virtual std::string Info() const;

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const;

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const;


    ///@}
    ///@name Friends
    ///@{


    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{


    ///@}
    ///@name Protected member Variables
    ///@{


    ///@}
    ///@name Protected Operators
    ///@{


    ///@}
    ///@name Protected Operations
    ///@{


    ///@}
    ///@name Protected  Access
    ///@{


    ///@}
    ///@name Protected Inquiry
    ///@{


    ///@}
    ///@name Protected LifeCycle
    ///@{


    ///@}

private:
    ///@name Static Member Variables
    ///@{


    ///@}
    ///@name Member Variables
    ///@{
    ModelPart& mrModelPart;


    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{


    ///@}
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    NestedRefinementUtility& operator=(NestedRefinementUtility const& rOther);

    /// Copy constructor.
    NestedRefinementUtility(NestedRefinementUtility const& rOther);


    ///@}

}; // Class NestedRefinementUtility

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


/// input stream function
inline std::istream& operator >> (std::istream& rIStream,
                NestedRefinementUtility& rThis);

/// output stream function
inline std::ostream& operator << (std::ostream& rOStream,
                const NestedRefinementUtility& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

///@} addtogroup block

}  // namespace Kratos.

#endif // KRATOS_NESTED_REFINEMENT_UTILITY_H_INCLUDED  defined
