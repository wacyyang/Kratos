//   
//   Project Name:        			KratosDamApplication $
//   Last Modified by:    $Author:    	  Lorenzo Gracia $
//   Date:                $Date:           	January 2016 $
//   Revision:            $Revision:                 1.0 $
//

#if !defined(KRATOS_UP_CONDITION_H_INCLUDED )
#define  KRATOS_UP_CONDITION_H_INCLUDED

// System includes
#include <cmath>

// Project includes
#include "includes/define.h"
#include "includes/condition.h"
#include "includes/serializer.h"
#include "includes/process_info.h"

// Application includes
#include "custom_utilities/condition_utilities.hpp"
#include "dam_application_variables.h"

namespace Kratos
{

template< unsigned int TDim, unsigned int TNumNodes >
class UPCondition : public Condition
{

public:

    KRATOS_CLASS_POINTER_DEFINITION( UPCondition );
    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Default constructor
    UPCondition() : Condition() {}
    
    // Constructor 1
    UPCondition( IndexType NewId, GeometryType::Pointer pGeometry ) : Condition(NewId, pGeometry) {}
    
    // Constructor 2
    UPCondition( IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties ) : Condition(NewId, pGeometry, pProperties) 
    {
        mThisIntegrationMethod = this->GetGeometry().GetDefaultIntegrationMethod();
    }

    // Destructor
    virtual ~UPCondition() {}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    Condition::Pointer Create(IndexType NewId,NodesArrayType const& ThisNodes,PropertiesType::Pointer pProperties ) const;
 
    void GetDofList(DofsVectorType& rConditionDofList,ProcessInfo& rCurrentProcessInfo );

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix,VectorType& rRightHandSideVector,ProcessInfo& rCurrentProcessInfo );
    
    void CalculateLeftHandSide(MatrixType& rLeftHandSideMatrix,ProcessInfo& rCurrentProcessInfo );
    
    void CalculateRightHandSide(VectorType& rRightHandSideVector,ProcessInfo& rCurrentProcessInfo );

    void EquationIdVector(EquationIdVectorType& rResult,ProcessInfo& rCurrentProcessInfo );

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

protected:  

    struct ConditionVariables
    {
        ///Properties variables
        double Density;
        
        ///ProcessInfo variables
        double AcelerationCoefficient;
          
        ///Variables computed at each GP
        Vector Np;
        Vector NormalVector;
        boost::numeric::ublas::bounded_matrix<double,TDim, TNumNodes*TDim> Nu;
		double IntegrationCoefficient;
		
		///Nodal variables
        array_1d<double,TNumNodes> PressureVector;
        Vector AccelerationVector;

        ///Auxiliary Variables
        boost::numeric::ublas::bounded_matrix<double,TNumNodes*TDim,TNumNodes> UPMatrix;
        boost::numeric::ublas::bounded_matrix<double,TNumNodes,TNumNodes*TDim> PUMatrix;
        array_1d<double,TNumNodes*TDim> UVector;
        array_1d<double,TNumNodes> PVector;
    };
        
    // Member Variables
    
    GeometryData::IntegrationMethod mThisIntegrationMethod;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    void CalculateAll( MatrixType& rLeftHandSideMatrix, VectorType& rRightHandSideVector, const ProcessInfo& rCurrentProcessInfo);
    
    void CalculateLHS( MatrixType& rLeftHandSideMatrix, const ProcessInfo& rCurrentProcessInfo);

    void CalculateRHS( VectorType& rRightHandSideVector, const ProcessInfo& rCurrentProcessInfo);
    
    void CalculateNormalVector(VectorType& rNormalVector,  const Matrix& Jacobian);
        
    void CalculateIntegrationCoefficient(double& rIntegrationCoefficient, const Matrix& Jacobian, const double& weight);
       
    void InitializeConditionVariables(ConditionVariables& rVariables, const GeometryType& Geom, const PropertiesType& Prop, const ProcessInfo& rCurrentProcessInfo);
    
    void GetAccelerationVector( Vector& rValues, int Step );
    
    void CalculateLHSContribution(MatrixType& rLeftHandSideMatrix, ConditionVariables& rVariables);    
    
    void CalculateRHSContribution(VectorType& rRightHandSideVector, ConditionVariables& rVariables);    

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

private:
    
    // Serialization
    
    friend class Serializer;
    
    virtual void save(Serializer& rSerializer) const
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, Condition )
    }

    virtual void load(Serializer& rSerializer)
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, Condition )
    }
    
}; // class UPCondition.

} // namespace Kratos.

#endif // KRATOS_UP_CONDITION_H_INCLUDED defined 
