//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya (https://github.com/sunethwarna)
//

#if !defined(KRATOS_SPATIAL_VARIANCE_METHOD_H_INCLUDED)
#define KRATOS_SPATIAL_VARIANCE_METHOD_H_INCLUDED

// System includes
#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <tuple>
#include <vector>

// External includes

// Project includes
#include "includes/communicator.h"
#include "includes/define.h"
#include "includes/model_part.h"

// Application includes
#include "custom_utilities/method_utilities.h"

namespace Kratos
{
///@addtogroup RANSApplication
///@{

///@name Kratos Globals
///@{

namespace SpatialMethods
{
template <typename TContainerType, typename TContainerItemType, template <typename T> typename TDataRetrievalFunctor>
class ContainerSpatialMethods
{
public:
    // special overloaded method for flags
    int static CalculateSum(const ModelPart& rModelPart, const Flags& rVariable)
    {
        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);
        int sum = 0;

#pragma omp parallel for reduction(+ : sum)
        for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
        {
            const TContainerItemType& r_item = *(r_container.begin() + i);
            sum += r_item.Is(rVariable);
        }

        sum = rModelPart.GetCommunicator().GetDataCommunicator().SumAll(sum);
        return sum;
    }

    template <typename TDataType>
    TDataType static CalculateSum(const ModelPart& rModelPart,
                                  const Variable<TDataType>& rVariable)
    {
        KRATOS_TRY

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        TDataType global_sum = rVariable.Zero();

        if (r_container.size() > 0)
        {
            const TDataType& r_initial_value =
                TDataRetrievalFunctor<TContainerItemType>()(*r_container.begin(), rVariable);

            MethodUtilities::DataTypeSizeInitializer(global_sum, r_initial_value);

#pragma omp parallel
            {
                TDataType sum = rVariable.Zero();
                MethodUtilities::DataTypeSizeInitializer(sum, r_initial_value);

#pragma omp for
                for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
                {
                    const TContainerItemType& r_item = *(r_container.begin() + i);
                    const TDataType& current_value =
                        TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);
                    MethodUtilities::DataTypeSizeChecker(current_value, sum);
                    sum += current_value;
                }
#pragma omp critical
                {
                    global_sum += sum;
                }
            }
        }

        global_sum = rModelPart.GetCommunicator().GetDataCommunicator().SumAll(global_sum);

        return global_sum;

        KRATOS_CATCH("");
    }

    template <typename TDataType>
    double static CalculateNormSum(const ModelPart& rModelPart,
                                   const Variable<TDataType>& rVariable,
                                   const std::string& rNormType,
                                   Parameters Params)
    {
        KRATOS_TRY

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        double global_sum = 0.0;
        const auto& norm_method =
            MethodUtilities::GetNormMethod<TDataType>(rVariable, rNormType);

#pragma omp parallel
        {
            double sum = 0.0;
#pragma omp for
            for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
            {
                const TContainerItemType& r_item = *(r_container.begin() + i);
                const TDataType& current_value =
                    TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);
                sum += norm_method(current_value);
            }
#pragma omp atomic update
            global_sum += sum;
        }

        global_sum = rModelPart.GetCommunicator().GetDataCommunicator().SumAll(global_sum);
        return global_sum;

        KRATOS_CATCH("");
    }

    template <typename TDataType>
    TDataType static CalculateRootMeanSquare(const ModelPart& rModelPart,
                                             const Variable<TDataType>& rVariable)
    {
        KRATOS_TRY

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        TDataType global_sum = rVariable.Zero();

        if (r_container.size() > 0)
        {
            const TDataType& r_initial_value =
                TDataRetrievalFunctor<TContainerItemType>()(*r_container.begin(), rVariable);

            MethodUtilities::DataTypeSizeInitializer(global_sum, r_initial_value);

#pragma omp parallel
            {
                TDataType sum = rVariable.Zero();
                MethodUtilities::DataTypeSizeInitializer(sum, r_initial_value);

#pragma omp for
                for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
                {
                    const TContainerItemType& r_item = *(r_container.begin() + i);
                    const TDataType& current_value =
                        TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);
                    MethodUtilities::DataTypeSizeChecker(current_value, sum);
                    sum += MethodUtilities::RaiseToPower<TDataType>(current_value, 2);
                }
#pragma omp critical
                {
                    global_sum += sum;
                }
            }
        }

        global_sum = rModelPart.GetCommunicator().GetDataCommunicator().SumAll(global_sum);
        const unsigned int number_of_items =
            rModelPart.GetCommunicator().GetDataCommunicator().SumAll(
                r_container.size());
        global_sum = MethodUtilities::RaiseToPower<TDataType>(
            global_sum * (1.0 / std::max(static_cast<double>(number_of_items), 1.0)), 0.5);

        return global_sum;

        KRATOS_CATCH("");
    }

    template <typename TDataType>
    double static CalculateNormRootMeanSquare(const ModelPart& rModelPart,
                                              const Variable<TDataType>& rVariable,
                                              const std::string& rNormType,
                                              Parameters Params)
    {
        KRATOS_TRY

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        double global_sum = 0.0;
        const auto& norm_method =
            MethodUtilities::GetNormMethod<TDataType>(rVariable, rNormType);

#pragma omp parallel
        {
            double sum = 0.0;
#pragma omp for
            for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
            {
                const TContainerItemType& r_item = *(r_container.begin() + i);
                const TDataType& current_value =
                    TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);
                sum += std::pow(norm_method(current_value), 2);
            }
#pragma omp atomic update
            global_sum += sum;
        }

        global_sum = rModelPart.GetCommunicator().GetDataCommunicator().SumAll(global_sum);
        const unsigned int number_of_items =
            rModelPart.GetCommunicator().GetDataCommunicator().SumAll(
                r_container.size());
        return std::sqrt(global_sum / std::max(static_cast<double>(number_of_items), 1.0));

        KRATOS_CATCH("");
    }

    template <typename TDataType>
    TDataType static CalculateMean(const ModelPart& rModelPart,
                                   const Variable<TDataType>& rVariable)
    {
        const TDataType& sum = CalculateSum<TDataType>(rModelPart, rVariable);
        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        const unsigned int number_of_items =
            rModelPart.GetCommunicator().GetDataCommunicator().SumAll(
                r_container.size());

        return sum * (1.0 / std::max(static_cast<double>(number_of_items), 1.0));
    }

    template <typename TDataType>
    double static CalculateNormMean(const ModelPart& rModelPart,
                                    const Variable<TDataType>& rVariable,
                                    const std::string& rNormType,
                                    Parameters Params)
    {
        const double sum =
            CalculateNormSum<TDataType>(rModelPart, rVariable, rNormType, Params);
        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        const unsigned int number_of_items =
            rModelPart.GetCommunicator().GetDataCommunicator().SumAll(
                r_container.size());

        if (number_of_items > 0)
        {
            return sum * (1.0 / static_cast<double>(number_of_items));
        }

        return 0.0;
    }

    template <typename TDataType>
    std::tuple<TDataType, TDataType> static CalculateVariance(const ModelPart& rModelPart,
                                                              const Variable<TDataType>& rVariable)
    {
        TDataType mean = CalculateMean<TDataType>(rModelPart, rVariable);
        TDataType global_variance = rVariable.Zero();

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        if (r_container.size() > 0)
        {
            const TDataType& r_initial_value =
                TDataRetrievalFunctor<TContainerItemType>()(*r_container.begin(), rVariable);

            MethodUtilities::DataTypeSizeInitializer(global_variance, r_initial_value);

#pragma omp parallel
            {
                TDataType variance = rVariable.Zero();
                MethodUtilities::DataTypeSizeInitializer(variance, r_initial_value);

#pragma omp for
                for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
                {
                    const TContainerItemType& r_item = *(r_container.begin() + i);
                    const TDataType& current_value =
                        TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);
                    MethodUtilities::DataTypeSizeChecker(current_value, variance);

                    variance += MethodUtilities::RaiseToPower(current_value, 2);
                }
#pragma omp critical
                {
                    global_variance += variance;
                }
            }
        }

        global_variance =
            rModelPart.GetCommunicator().GetDataCommunicator().SumAll(global_variance);
        const unsigned int number_of_items =
            rModelPart.GetCommunicator().GetDataCommunicator().SumAll(
                r_container.size());

        if (number_of_items > 0)
        {
            global_variance *= (1.0 / static_cast<double>(number_of_items));
            global_variance -= MethodUtilities::RaiseToPower(mean, 2);
        }

        return std::make_tuple<TDataType, TDataType>(
            std::forward<TDataType>(mean), std::forward<TDataType>(global_variance));
    }

    template <typename TDataType>
    std::tuple<double, double> static CalculateNormVariance(const ModelPart& rModelPart,
                                                            const Variable<TDataType>& rVariable,
                                                            const std::string& rNormType,
                                                            Parameters Params)
    {
        double mean = CalculateNormMean<TDataType>(rModelPart, rVariable, rNormType, Params);

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        double global_variance = 0.0;
        const auto& norm_method =
            MethodUtilities::GetNormMethod<TDataType>(rVariable, rNormType);

#pragma omp parallel
        {
            double variance = 0.0;

#pragma omp for
            for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
            {
                const TContainerItemType& r_item = *(r_container.begin() + i);
                const TDataType& current_value =
                    TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);

                variance += std::pow(norm_method(current_value), 2);
            }
#pragma omp atomic update
            global_variance += variance;
        }
        global_variance =
            rModelPart.GetCommunicator().GetDataCommunicator().SumAll(global_variance);
        const unsigned int number_of_items =
            rModelPart.GetCommunicator().GetDataCommunicator().SumAll(
                r_container.size());

        if (number_of_items > 0)
        {
            global_variance *= (1.0 / static_cast<double>(number_of_items));
            global_variance -= MethodUtilities::RaiseToPower(mean, 2);
        }

        return std::make_tuple<double, double>(
            std::forward<double>(mean), std::forward<double>(global_variance));
    }

    template <typename TDataType>
    std::tuple<double, std::size_t> static GetNormMax(const ModelPart& rModelPart,
                                                      const Variable<TDataType>& rVariable,
                                                      const std::string& rNormType,
                                                      Parameters Params)
    {
        KRATOS_TRY

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        double global_max = std::numeric_limits<double>::lowest();
        std::size_t global_id = 0;
        const auto& norm_method =
            MethodUtilities::GetNormMethod<TDataType>(rVariable, rNormType);

#pragma omp parallel
        {
            double current_max = std::numeric_limits<double>::lowest();
            std::size_t current_id = 0;
#pragma omp for
            for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
            {
                const TContainerItemType& r_item = *(r_container.begin() + i);
                const TDataType& current_value =
                    TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);
                const double value_norm = norm_method(current_value);
                if (value_norm > current_max)
                {
                    current_max = value_norm;
                    current_id = r_item.Id();
                }
            }
#pragma omp critical
            {
                if (current_max > global_max)
                {
                    global_max = current_max;
                    global_id = current_id;
                }
            }
        }

        const DataCommunicator& r_data_communicator =
            rModelPart.GetCommunicator().GetDataCommunicator();
        const std::vector<double> global_max_value_array =
            r_data_communicator.AllGather(std::vector<double>{{global_max}});
        const std::vector<std::size_t> global_max_id_array =
            r_data_communicator.AllGather(std::vector<std::size_t>{{global_id}});

        for (std::size_t i = 0; i < global_max_value_array.size(); ++i)
        {
            if (global_max_value_array[i] > global_max)
            {
                global_max = global_max_value_array[i];
                global_id = global_max_id_array[i];
            }
        }

        return std::make_tuple<double, std::size_t>(
            std::forward<double>(global_max), std::forward<std::size_t>(global_id));

        KRATOS_CATCH("");
    }

    template <typename TDataType>
    std::tuple<double, std::size_t> static GetNormMin(const ModelPart& rModelPart,
                                                      const Variable<TDataType>& rVariable,
                                                      const std::string& rNormType,
                                                      Parameters Params)
    {
        KRATOS_TRY

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        double global_min = std::numeric_limits<double>::max();
        std::size_t global_id = 0;
        const auto& norm_method =
            MethodUtilities::GetNormMethod<TDataType>(rVariable, rNormType);

#pragma omp parallel
        {
            double current_min = std::numeric_limits<double>::max();
            std::size_t current_id = 0;
#pragma omp for
            for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
            {
                const TContainerItemType& r_item = *(r_container.begin() + i);
                const TDataType& current_value =
                    TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);
                const double value_norm = norm_method(current_value);
                if (value_norm < current_min)
                {
                    current_min = value_norm;
                    current_id = r_item.Id();
                }
            }
#pragma omp critical
            {
                if (current_min < global_min)
                {
                    global_min = current_min;
                    global_id = current_id;
                }
            }
        }

        const DataCommunicator& r_data_communicator =
            rModelPart.GetCommunicator().GetDataCommunicator();
        const std::vector<double> global_min_value_array =
            r_data_communicator.AllGather(std::vector<double>{{global_min}});
        const std::vector<std::size_t> global_min_id_array =
            r_data_communicator.AllGather(std::vector<std::size_t>{{global_id}});

        for (std::size_t i = 0; i < global_min_value_array.size(); ++i)
        {
            if (global_min_value_array[i] < global_min)
            {
                global_min = global_min_value_array[i];
                global_id = global_min_id_array[i];
            }
        }

        return std::make_tuple<double, std::size_t>(
            std::forward<double>(global_min), std::forward<std::size_t>(global_id));

        KRATOS_CATCH("");
    }

    template <typename TDataType>
    double static GetNormMedian(const ModelPart& rModelPart,
                                const Variable<TDataType>& rVariable,
                                const std::string& rNormType,
                                Parameters Params)
    {
        KRATOS_TRY

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        const auto& norm_method =
            MethodUtilities::GetNormMethod<TDataType>(rVariable, rNormType);

        std::vector<double> local_values;
        local_values.resize(r_container.size());
        local_values.shrink_to_fit();

#pragma omp parallel for
        for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
        {
            const TContainerItemType& r_item = *(r_container.begin() + i);
            const TDataType& current_value =
                TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);
            local_values[i] = norm_method(current_value);
        }

        std::sort(local_values.begin(), local_values.end());

        const DataCommunicator& r_data_communicator =
            rModelPart.GetCommunicator().GetDataCommunicator();
        const std::vector<std::vector<double>>& global_values =
            r_data_communicator.Gatherv(local_values, 0);

        double median = 0.0;
        if (r_data_communicator.Rank() == 0)
        {
            const std::vector<double>& sorted_values_list =
                MethodUtilities::SortSortedValuesList(global_values);
            const int number_of_values = sorted_values_list.size();

            if (number_of_values > 0)
            {
                if (number_of_values % 2 != 0)
                {
                    median = sorted_values_list[number_of_values / 2];
                }
                else
                {
                    median = (sorted_values_list[(number_of_values - 1) / 2] +
                              sorted_values_list[number_of_values / 2]) *
                             0.5;
                }
            }
        }

        r_data_communicator.Broadcast(median, 0);
        return median;

        KRATOS_CATCH("");
    }

    template <typename TDataType>
    std::tuple<double, double, std::vector<double>, std::vector<int>, std::vector<double>> static GetNormDistribution(

        const ModelPart& rModelPart,
        const Variable<TDataType>& rVariable,
        const std::string& rNormType,
        Parameters Params)
    {
        KRATOS_TRY

        Parameters default_parameters = Parameters(R"(
        {
            "number_of_value_groups" : 10,
            "min_value"              : "min",
            "max_value"              : "max"
        })");

        if (Params.Has("min_value") && Params["min_value"].IsDouble())
        {
            default_parameters["min_value"].SetDouble(0.0);
        }
        if (Params.Has("max_value") && Params["max_value"].IsDouble())
        {
            default_parameters["max_value"].SetDouble(0.0);
        }
        Params.RecursivelyValidateAndAssignDefaults(default_parameters);

        double min_value{0.0};
        if (Params["min_value"].IsDouble())
        {
            min_value = Params["min_value"].GetDouble();
        }
        else if (Params["min_value"].IsString() &&
                 Params["min_value"].GetString() == "min")
        {
            const auto& min_data =
                GetNormMin<TDataType>(rModelPart, rVariable, rNormType, Params);
            min_value = std::get<0>(min_data);
        }
        else
        {
            KRATOS_ERROR << "Unknown min_value. Allowed only double or \"min\" "
                            "string as a value. [ min_value = "
                         << Params["min_value"] << " ]\n.";
        }

        double max_value{0.0};
        if (Params["max_value"].IsDouble())
        {
            max_value = Params["max_value"].GetDouble();
        }
        else if (Params["max_value"].IsString() &&
                 Params["max_value"].GetString() == "max")
        {
            const auto& max_data =
                GetNormMax<TDataType>(rModelPart, rVariable, rNormType, Params);
            max_value = std::get<0>(max_data);
        }
        else
        {
            KRATOS_ERROR << "Unknown max_value. Allowed only double or \"max\" "
                            "string as a value. [ max_value = "
                         << Params["max_value"] << " ]\n.";
        }

        const int number_of_groups = Params["number_of_value_groups"].GetInt();

        const TContainerType& r_container =
            MethodUtilities::GetLocalDataContainer<TContainerType>(rModelPart);

        const auto& norm_method =
            MethodUtilities::GetNormMethod<TDataType>(rVariable, rNormType);

        std::vector<double> group_limits;
        for (int i = 0; i < number_of_groups + 1; ++i)
        {
            group_limits.push_back(
                min_value + (max_value - min_value) * static_cast<double>(i) /
                                static_cast<double>(number_of_groups));
        }
        group_limits[group_limits.size() - 1] += 1.0;
        group_limits.push_back(std::numeric_limits<double>::max());

        group_limits.shrink_to_fit();
        const int number_of_limits = group_limits.size();

        std::vector<int> distribution;
        for (int i = 0; i < number_of_limits; ++i)
        {
            distribution.push_back(0);
        }
        distribution.shrink_to_fit();

#pragma omp parallel
        {
            std::vector<int> local_distribution;
            for (int i = 0; i < number_of_limits; ++i)
            {
                local_distribution.push_back(0);
            }
            local_distribution.shrink_to_fit();

#pragma omp for
            for (int i = 0; i < static_cast<int>(r_container.size()); ++i)
            {
                const TContainerItemType& r_item = *(r_container.begin() + i);
                const TDataType& current_value =
                    TDataRetrievalFunctor<TContainerItemType>()(r_item, rVariable);
                const double value_norm = norm_method(current_value);
                for (int i = 0; i < number_of_limits; ++i)
                {
                    if (value_norm < group_limits[i])
                    {
                        ++local_distribution[i];
                        break;
                    }
                }
            }
#pragma omp critical
            {
                for (int i = 0; i < number_of_limits; ++i)
                {
                    distribution[i] += local_distribution[i];
                }
            }
        }

        std::vector<int> global_distribution =
            rModelPart.GetCommunicator().GetDataCommunicator().SumAll(distribution);

        const int number_of_items = std::max(
            std::accumulate(global_distribution.begin(), global_distribution.end(), 0), 1);
        std::vector<double> global_percentage_distributions;
        for (int i = 0; i < number_of_limits; ++i)
        {
            global_percentage_distributions.push_back(
                static_cast<double>(global_distribution[i]) /
                static_cast<double>(number_of_items));
        }
        group_limits[group_limits.size() - 2] -= 1.0;
        group_limits[group_limits.size() - 1] = max_value;

        return std::make_tuple<double, double, std::vector<double>, std::vector<int>, std::vector<double>>(
            std::forward<double>(min_value), std::forward<double>(max_value),
            std::forward<std::vector<double>>(group_limits),
            std::forward<std::vector<int>>(global_distribution),
            std::forward<std::vector<double>>(global_percentage_distributions));

        KRATOS_CATCH("");
    }
};

using NodeType = ModelPart::NodeType;
using ElementType = ModelPart::ElementType;
using ConditionType = ModelPart::ConditionType;

using NodesContainerType = ModelPart::NodesContainerType;
using ElementsContainerType = ModelPart::ElementsContainerType;
using ConditionsContainerType = ModelPart::ConditionsContainerType;

class HistoricalSpatialMethods
    : public SpatialMethods::ContainerSpatialMethods<NodesContainerType, NodeType, MethodUtilities::HistoricalDataValueRetrievalFunctor>
{
};

class NodalNonHistoricalSpatialMethods
    : public SpatialMethods::ContainerSpatialMethods<NodesContainerType, NodeType, MethodUtilities::NonHistoricalDataValueRetrievalFunctor>
{
};

class ConditionNonHistoricalSpatialMethods
    : public SpatialMethods::ContainerSpatialMethods<ConditionsContainerType, ConditionType, MethodUtilities::NonHistoricalDataValueRetrievalFunctor>
{
};

class ElementNonHistoricalSpatialMethods
    : public SpatialMethods::ContainerSpatialMethods<ElementsContainerType, ElementType, MethodUtilities::NonHistoricalDataValueRetrievalFunctor>
{
};

} // namespace SpatialMethods

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

///@}

///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

///@}

///@} addtogroup block

} // namespace Kratos.

#endif // KRATOS_SPATIAL_VARIANCE_METHOD_H_INCLUDED defined
