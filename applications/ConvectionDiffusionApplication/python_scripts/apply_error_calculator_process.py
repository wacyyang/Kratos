from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7

# Importing the Kratos Library
import KratosMultiphysics
import KratosMultiphysics.MeshingApplication as KratosMesh
import KratosMultiphysics.ConvectionDiffusionApplication as KratosConvDiff
import KratosMultiphysics.FluidDynamicsApplication as KratosFluid

def Factory(settings, Model): 
    if(type(settings) != KratosMultiphysics.Parameters): 
        raise Exception("expected input shall be a Parameters object, encapsulating a json string") 
    return ApplyErrorCalculatorProcess(Model, settings["Parameters"])

## All the processes python should be derived from "Process" 
class ApplyErrorCalculatorProcess(KratosMultiphysics.Process):

    def __init__(self, Model, settings):
        KratosMultiphysics.Process.__init__(self)

        default_settings = KratosMultiphysics.Parameters(""" 
            {
                "model_part_name"                       : "MainModelPart",
                "minimal_size"                        : 0.01,
                "maximal_size"                        : 10.0,
                "refinement_strategy"                 : "Simple_Error_Calculator",
                "reference_variable_name"             : "ERROR_RATIO",
                "echo_level"                          : 0
            }
            """
        )
        settings.ValidateAndAssignDefaults(default_settings)

        self.model_part = Model[settings["model_part_name"].GetString()]

        """ KratosMultiphysics.VariableUtils().SetNonHistoricalVariable(
                KratosMultiphysics.ConvectionDiffusionApplication.NODAL_TEMP_GRADIENT,
                0.0,
                self.model_part.Nodes)
        
        KratosMultiphysics.VariableUtils().SetNonHistoricalVariable(
                KratosMultiphysics.ConvectionDiffusionApplication.NODAL_ERROR_PROJ,
                0.0,
                self.model_part.Nodes) """
        
        self.model_part.AddNodalSolutionStepVariable(KratosConvDiff.NODAL_TEMP_GRADIENT)
        self.model_part.AddNodalSolutionStepVariable(KratosConvDiff.NODAL_ERROR_PROJ)

        #Process Parameters
        error_calc_params = KratosMultiphysics.Parameters("""{}""")
        error_calc_params.AddValue("minimal_size", settings["minimal_size"])
        error_calc_params.AddValue("maximal_size", settings["maximal_size"])
        error_calc_params.AddValue("refinement_strategy", settings["refinement_strategy"])
        error_calc_params.AddValue("reference_variable_name", settings["reference_variable_name"])
        error_calc_params.AddValue("echo_level", settings["echo_level"])

        if (self.model_part.ProcessInfo[KratosMultiphysics.DOMAIN_SIZE] == 2):
            self.error_calc = KratosConvDiff.SimpleErrorCalculatorProcess2D(self.model_part, error_calc_params)
        else:
            self.error_calc = KratosConvDiff.SimpleErrorCalculatorProcess3D(self.model_part, error_calc_params)
    
    def ExecuteInitialize(self):
        pass

    def ExecuteBeforeSolutionLoop(self):
        pass

    def ExecuteInitializeSolutionStep(self):
        pass

    def ExecuteFinalizeSolutionStep(self):
        pass

    def ExecuteBeforeOutputStep(self):
        pass

    def ExecuteAfterOutputStep(self):
        pass

    def ExecuteFinalize(self):
        # We execute our process
        self.error_calc.Execute()

    def Clear(self):
        pass


