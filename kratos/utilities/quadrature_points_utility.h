//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License 
//                   Kratos default license: kratos/license.txt
//
//  Main author:    
//

#if !defined(KRATOS_QUADRATURE_POINTS_UTILITY_H_INCLUDED)
#define KRATOS_QUADRATURE_POINTS_UTILITY_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "geometries/geometry.h"
#include "geometries/quadrature_point_geometry.h"
#include "geometries/quadrature_point_curve_on_surface_geometry.h"

namespace Kratos
{
    ///@name Kratos Classes
    ///@{

    /// A Class for the creation of integration points
    template<class TPointType>
    class CreateQuadraturePointsUtility
    {
    public:
        ///@name Type Definitions
        ///@{

        typedef Geometry<TPointType> GeometryType;
        typedef typename Geometry<TPointType>::Pointer GeometryPointerType;

        typedef std::size_t SizeType;
        typedef std::size_t IndexType;

        typedef PointerVector<TPointType> PointsArrayType;

        ///@}
        ///@name Operations
        ///@{

        static GeometryPointerType CreateQuadraturePointCurveOnSurface(
            GeometryShapeFunctionContainer<GeometryData::IntegrationMethod>& rShapeFunctionContainer,
            PointsArrayType rPoints,
            double LocalTangentU,
            double LocalTangentV,
            GeometryType* pGeometryParent)
        {
            return Kratos::make_shared<
                QuadraturePointCurveOnSurfaceGeometry<TPointType>>(
                    rPoints,
                    rShapeFunctionContainer,
                    LocalTangentU,
                    LocalTangentV,
                    pGeometryParent);
        }

        static GeometryPointerType CreateQuadraturePointCurveOnSurface(
            GeometryShapeFunctionContainer<GeometryData::IntegrationMethod>& rShapeFunctionContainer,
            PointsArrayType rPoints,
            double LocalTangentU,
            double LocalTangentV)
        {
            return Kratos::make_shared<
                QuadraturePointCurveOnSurfaceGeometry<TPointType>>(
                    rPoints,
                    rShapeFunctionContainer,
                    LocalTangentU,
                    LocalTangentV);
        }

        static GeometryPointerType CreateQuadraturePoint(
            SizeType WorkingSpaceDimension,
            SizeType LocalSpaceDimension,
            GeometryShapeFunctionContainer<GeometryData::IntegrationMethod>& rShapeFunctionContainer,
            PointsArrayType rPoints,
            GeometryType* pGeometryParent)
        {
            if (WorkingSpaceDimension == 1 && LocalSpaceDimension == 1)
                return Kratos::make_shared<
                    QuadraturePointGeometry<TPointType, 1>>(
                        rPoints,
                        rShapeFunctionContainer,
                        pGeometryParent);
            if (WorkingSpaceDimension == 2 && LocalSpaceDimension == 2)
                return Kratos::make_shared<
                    QuadraturePointGeometry<TPointType, 2>>(
                        rPoints,
                        rShapeFunctionContainer,
                        pGeometryParent);
            else if (WorkingSpaceDimension == 3 && LocalSpaceDimension == 2)
                return Kratos::make_shared<
                    QuadraturePointGeometry<TPointType, 3, 2>>(
                        rPoints,
                        rShapeFunctionContainer,
                        pGeometryParent);
            else if (WorkingSpaceDimension == 3 && LocalSpaceDimension == 3)
                return Kratos::make_shared<
                    QuadraturePointGeometry<TPointType, 3>>(
                        rPoints,
                        rShapeFunctionContainer,
                        pGeometryParent);
            else{
                KRATOS_ERROR << "Working/ Local space dimension combinations are "
                    << "not provieded for QuadraturePointGeometry. WorkingSpaceDimension: "
                    << WorkingSpaceDimension << ", LocalSpaceDimension: " << LocalSpaceDimension
                    <<  std::endl;
            }
        }

        static GeometryPointerType CreateQuadraturePoint(
            SizeType WorkingSpaceDimension,
            SizeType LocalSpaceDimension,
            GeometryShapeFunctionContainer<GeometryData::IntegrationMethod>& rShapeFunctionContainer,
            PointsArrayType rPoints)
        {
            if (WorkingSpaceDimension == 1 && LocalSpaceDimension == 1)
                return Kratos::make_shared<
                QuadraturePointGeometry<TPointType, 1>>(
                    rPoints,
                    rShapeFunctionContainer);
            if (WorkingSpaceDimension == 2 && LocalSpaceDimension == 2)
                return Kratos::make_shared<
                QuadraturePointGeometry<TPointType, 2>>(
                    rPoints,
                    rShapeFunctionContainer);
            else if (WorkingSpaceDimension == 3 && LocalSpaceDimension == 2)
                return Kratos::make_shared<
                QuadraturePointGeometry<TPointType, 3, 2>>(
                    rPoints,
                    rShapeFunctionContainer);
            else if (WorkingSpaceDimension == 3 && LocalSpaceDimension == 3)
                return Kratos::make_shared<
                QuadraturePointGeometry<TPointType, 3>>(
                    rPoints,
                    rShapeFunctionContainer);
            else {
                KRATOS_ERROR << "Working/ Local space dimension combinations are "
                    << "not provieded for QuadraturePointGeometry. WorkingSpaceDimension: "
                    << WorkingSpaceDimension << ", LocalSpaceDimension: " << LocalSpaceDimension
                    << std::endl;
            }
        }

        static std::vector<GeometryPointerType> Create(
            GeometryPointerType pGeometry) {
            KRATOS_TRY;

            auto integration_points = pGeometry->IntegrationPoints();
            auto default_method = pGeometry->GetDefaultIntegrationMethod();
            auto r_N = pGeometry->ShapeFunctionsValues();

            std::vector<GeometryPointerType> geometry_pointer_vector(integration_points.size());

            for (IndexType i = 0; i < integration_points.size(); ++i)
            {
                Matrix N_i = ZeroMatrix(1, pGeometry->size());
                for (IndexType j = 0; j < pGeometry->size(); ++j)
                {
                    N_i(0, j) = r_N(0, j);
                }

                GeometryShapeFunctionContainer<GeometryData::IntegrationMethod> data_container(
                    default_method,
                    integration_points[i],
                    N_i,
                    pGeometry->ShapeFunctionLocalGradient(i));

                geometry_pointer_vector[i] = CreateQuadraturePoint(
                    pGeometry->WorkingSpaceDimension(), pGeometry->LocalSpaceDimension(),
                    data_container, pGeometry->Points(), pGeometry.get());
            }
            return geometry_pointer_vector;

            KRATOS_CATCH("");
        }

        static std::vector<GeometryPointerType> Create(
            GeometryPointerType pGeometry,
            GeometryData::IntegrationMethod ThisIntegrationMethod) {
            KRATOS_TRY;

            auto integration_points = pGeometry->IntegrationPoints(ThisIntegrationMethod);
            auto r_N = pGeometry->ShapeFunctionsValues(ThisIntegrationMethod);

            std::vector<GeometryPointerType> geometry_pointer_vector(integration_points.size());

            for (IndexType i = 0; i < integration_points.size(); ++i)
            {
                Matrix N_i = ZeroMatrix(1, pGeometry->size());
                for (IndexType j = 0; j < pGeometry->size(); ++j)
                {
                    N_i(0, j) = r_N(0, j);
                }

                GeometryShapeFunctionContainer<GeometryData::IntegrationMethod> data_container(
                    ThisIntegrationMethod,
                    integration_points[i],
                    N_i,
                    pGeometry->ShapeFunctionLocalGradient(i, ThisIntegrationMethod));

                geometry_pointer_vector[i] = CreateQuadraturePoint(
                    pGeometry->WorkingSpaceDimension(), pGeometry->LocalSpaceDimension(),
                    data_container, pGeometry->Points(), pGeometry.get());
            }
            return geometry_pointer_vector;

            KRATOS_CATCH("");
        }

        ///@}

    };
    ///@} // Kratos Classes
} // namespace Kratos.

#endif // KRATOS_QUADRATURE_POINTS_UTILITY_H_INCLUDED defined
