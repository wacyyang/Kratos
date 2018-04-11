//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Carlos A. Roig
//
//

// External includes

// Project includes
#include "includes/define.h"
#include "includes/define_python.h"
#include "input_output/logger.h"



namespace Kratos {
namespace Python {

    using namespace pybind11;
/**
 * Prints the arguments from the python script using the Kratos Logger class. Implementation
 * @args tuple  representing the arguments of the function The first argument is the label
 * @kwargs dictionary  resenting key-value pairs for
 * @severity Logger::Severity The message level of severity @see Logger::Severity
 * @useKwargLabel bool Indicates if the label must be gather from kwargs (true) or is the first argument of the call (false)
 * name arguments
 **/
void printImpl(pybind11::args args, pybind11::kwargs kwargs, Logger::Severity severity, bool useKwargLabel) {
    if(len(args) == 0)
        std::cout << "ERROR" << std::endl;
    
    std::stringstream buffer;
    Logger::Severity severityOption = severity;
    Logger::Category categoryOption = Logger::Category::STATUS;

    std::string label;
//     const char* label;

    // Get the label
    unsigned int to_skip = 0; //if the kwargs label is false, consider the first entry of the args as the label
    if(useKwargLabel) {
        if(kwargs.contains("label")) {
            label = str(kwargs["label"]);
        } else {
            label = "undefined";
        }
    } else {
        label = str(args[0]); //if the kwargs label is false, consider the first entry of the args as the label
        to_skip = 1;
    }
    
    unsigned int counter = 0;
    for(auto item : args)
    {
        if(counter >= to_skip)
        {
            buffer << item;
            if(counter < len(args))
                buffer << " ";
        }
        counter++;
    }
        

//     // Extract the pybind11::args part
//     for(int i = (useKwargLabel ? 0 : 1); i < len(args); ++i) {
//         object curArg = args[i];
//         if(curArg) {
//             buffer << (str(args[i])) << ((i != len(args)) ? " " : "");
// //             buffer << extract<const char *>(boost::python::str(args[i])) << ((i != len(args)) ? " " : "");
//         }
//     }

    // Extract the options
    if(kwargs.contains("severity")) {
//         severityOption = extract<Logger::Severity>(kwargs["severity"]);
        severityOption = cast<Logger::Severity>(kwargs["severity"]);
    }

    if(kwargs.contains("category")) {
//         categoryOption = extract<Logger::Category>(kwargs["category"]);
        categoryOption = cast<Logger::Category>(kwargs["category"]);
    }

    // Send the message and options to the logger
    Logger(label) << buffer.str() << severityOption << categoryOption << std::endl;

}

/**
 * Prints the arguments from the python script using the Kratos Logger class. Default function uses INFO severity.
 * @args pybind11::args boost::python::object representing the arguments of the function The first argument is the label
 * @kwargs pybind11::dictionary of boost::python::objects resenting key-value pairs for
 * name arguments
 **/
void printDefault(pybind11::args args, pybind11::kwargs kwargs) {
    printImpl(args, kwargs, Logger::Severity::INFO, true);
}

/**
 * Prints the arguments from the python script using the Kratos Logger class using INFO severity.
 * @args pybind11::args boost::python::object representing the arguments of the function The first argument is the label
 * @kwargs pybind11::dictionary of boost::python::objects resenting key-value pairs for
 * name arguments
 **/
void printInfo(pybind11::args args, pybind11::kwargs kwargs) {
    printImpl(args, kwargs, Logger::Severity::INFO, false);
}

/**
 * Prints the arguments from the python script using the Kratos Logger class using WARNING severity.
 * @args pybind11::args boost::python::object representing the arguments of the function The first argument is the label
 * @kwargs pybind11::dictionary of boost::python::objects resenting key-value pairs for
 * name arguments
 **/
void printWarning(pybind11::args args, pybind11::kwargs kwargs) {
    printImpl(args, kwargs, Logger::Severity::WARNING, false);
}

void  AddLoggerToPython(pybind11::module& m) {

    class_<LoggerOutput, Kratos::shared_ptr<LoggerOutput>>(m,"LoggerOutput")
    .def("SetMaxLevel", &LoggerOutput::SetMaxLevel)
    .def("GetMaxLevel", &LoggerOutput::GetMaxLevel)
    .def("SetSeverity", &LoggerOutput::SetSeverity)
    .def("GetSeverity", &LoggerOutput::GetSeverity)
    .def("SetCategory", &LoggerOutput::SetCategory)
    .def("GetCategory", &LoggerOutput::GetCategory)
    ;

    class_<Logger, Kratos::shared_ptr<Logger>> logger_scope(m,"Logger");
    logger_scope.def(init<std::string const &>());
    logger_scope.def("Print", printDefault); // raw_function(printDefault,1))
    logger_scope.def_static("PrintInfo",printInfo); // raw_function(printInfo,1))
    logger_scope.def_static("PrintWarning", printWarning); //raw_function(printWarning,1))
    logger_scope.def_static("GetDefaultOutput", &Logger::GetDefaultOutputInstance, return_value_policy::reference); //_internal )
    ;

    // Enums for Severity
    enum_<Logger::Severity>(logger_scope,"Severity")
    .value("WARNING", Logger::Severity::WARNING)
    .value("INFO", Logger::Severity::INFO)
    .value("DETAIL", Logger::Severity::DETAIL)
    .value("DEBUG", Logger::Severity::DEBUG)
    .value("TRACE", Logger::Severity::TRACE);

    // Enums for Category
    enum_<Logger::Category>(logger_scope,"Category")
    .value("STATUS", Logger::Category::STATUS)
    .value("CRITICAL", Logger::Category::CRITICAL)
    .value("STATISTICS", Logger::Category::STATISTICS)
    .value("PROFILING", Logger::Category::PROFILING)
    .value("CHECKING", Logger::Category::CHECKING);
}

}  // namespace Python.
} // Namespace Kratos
