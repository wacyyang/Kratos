//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Ruben Zorrilla
//

#if !defined(KRATOS_DIVIDE_GEOMETRY)
#define KRATOS_DIVIDE_GEOMETRY

// System includes

// External includes

// Project includes
#include "includes/node.h"
#include "geometries/point.h"
#include "geometries/geometry.h"
#include "geometries/geometry_data.h"
#include "utilities/indexed_object.h"
#include "containers/pointer_vector_set.h"

namespace Kratos
{
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

class KRATOS_API(KRATOS_CORE) IndexedPoint : public Point, public IndexedObject
{
public:

    ///@name Type Definitions
    ///@{

    /// Pointer definition of IndexedPoint
    KRATOS_CLASS_POINTER_DEFINITION(IndexedPoint);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Empty constructor
    IndexedPoint();

    /// Auxiliar constructor
    IndexedPoint(const unsigned int Id);

    /// Default constructor
    IndexedPoint(const array_1d<double,3>& rCoords, const unsigned int Id);

    /// Destructor
    ~IndexedPoint();

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
    std::string Info() const override;

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override;

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override;

    ///@}
    ///@name Friends
    ///@{

    ///@}
    ///@name Member variables
    ///@{

    ///@}
    ///@name Operations
    ///@{

    ///@}

private:
    ///@name Static Member Variables
    ///@{

    ///@}
    ///@name Member Variables
    ///@{

    ///@}
    ///@name Serialization
    ///@{

    friend class Serializer;

    void save(Serializer& rSerializer) const override {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, Point);
        KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, IndexedObject);
    };

    void load(Serializer& rSerializer) override {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, Point);
        KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, IndexedObject);
    };

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

    ///@}
};

/// input stream function
inline std::istream& operator >> (std::istream& rIStream,
                                  IndexedPoint& rThis) {
    return rIStream;
};

/// output stream function
inline std::ostream& operator << (std::ostream& rOStream,
                                  const IndexedPoint& rThis) {
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
};

class KRATOS_API(KRATOS_CORE) DivideGeometry
{
public:

    ///@name Type Definitions
    ///@{

    /// Pointer definition of DivideGeometry
    KRATOS_CLASS_POINTER_DEFINITION(DivideGeometry);

    // General type definitions
    typedef Geometry < Node<3> >                                    GeometryType;
    typedef IndexedPoint                                            IndexedPointType;
    typedef IndexedPoint::Pointer                                   IndexedPointPointerType;
    typedef Geometry < IndexedPoint >                               IndexedPointGeometryType;
    typedef Geometry < IndexedPoint >::Pointer                      IndexedPointGeometryPointerType;
    typedef PointerVectorSet<IndexedPointType, IndexedObject>       IndexedPointsContainerType;

    bool mIsSplit;          // True if the element is split.

    int mSplitEdgesNumber;  // Number of split edges.
    int mDivisionsNumber;   // Number of generated subdivisions.

    IndexedPointsContainerType mAuxPointsContainer;                         // Indexed points container to store the original plus the intersection points.
    std::vector < IndexedPointGeometryPointerType > mPositiveSubdivisions;  // Array to store the generated positive subdivisions geometries.
    std::vector < IndexedPointGeometryPointerType > mNegativeSubdivisions;  // Array to store the generated negative subdivisions geometries.
    std::vector < IndexedPointGeometryPointerType > mPositiveInterfaces;    // Array to store the generated positive interfaces geometries.
    std::vector < IndexedPointGeometryPointerType > mNegativeInterfaces;    // Array to store the generated negative interfaces geometries.
    std::vector < unsigned int > mPositiveInterfacesParentIds;              // Array to store the parent subgeometries ids of the generated positive interfaces.
    std::vector < unsigned int > mNegativeInterfacesParentIds;              // Array to store the parent subgeometries ids of the generated negative interfaces.

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor
    DivideGeometry(const GeometryType& rInputGeometry, const Vector& rNodalDistances);

    /// Destructor
    ~DivideGeometry();

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
    ///@name Operations
    ///@{

    Vector GetNodalDistances() const;

    GeometryType GetInputGeometry() const;

    /**
     * Divides the input geometry according to the provided distance data.
     */
    virtual void GenerateDivision() = 0;

    /**
     * Generates a list containing the intersection interface geometries for either the positive or the negative element subdivisions.
     */
    virtual void GenerateIntersectionsSkin() = 0;

    /**
     * Generates a list containing the exterior (boundary) faces geometries for the positive element subdivisions.
     */
    virtual std::vector < IndexedPointGeometryPointerType > GeneratePositiveExteriorFaces() = 0;

    /**
     * Generates a list containing the exterior (boundary) faces geometries for the negative element subdivisions.
     */
    virtual std::vector < IndexedPointGeometryPointerType > GenerateNegativeExteriorFaces() = 0;

    /**
     * Given a father face id, generates a list containing the exterior (boundary)
     * faces geometries belonging to the positive side of that that father face.
     * @param FatherFaceId: Father face in where the positive exterior faces are to be obtained
     */
    virtual std::vector < IndexedPointGeometryPointerType > GeneratePositiveExteriorFaces(const unsigned int FatherFaceId) = 0;

    /**
     * Given a father face id, generates a list containing the exterior (boundary)
     * faces geometries belonging to the negative side of that that father face.
     * @param FatherFaceId: Father face in where the negative exterior faces are to be obtained
     */
    virtual std::vector < IndexedPointGeometryPointerType > GenerateNegativeExteriorFaces(const unsigned int FatherFaceId) = 0;

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

    /**
    * Returns true if the element is split and false otherwise.
    */
    void IsSplit();

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

    const GeometryType& mrInputGeometry;
    const Vector& mrNodalDistances;

    ///@}
    ///@name Serialization
    ///@{

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
    DivideGeometry& operator=(DivideGeometry const& rOther);

    /// Copy constructor.
    DivideGeometry(DivideGeometry const& rOther)
        : mrInputGeometry(rOther.mrInputGeometry) , mrNodalDistances(rOther.mrNodalDistances) {};

    ///@}

};// class DivideGeometry

}//namespace Kratos
#endif /* KRATOS_DIVIDE_GEOMETRY defined */
