# ==============================================================================
#  KratosShapeOptimizationApplication
#
#  License:         BSD License
#                   license: ShapeOptimizationApplication/license.txt
#
#  Main authors:    Baumgaertner Daniel, https://github.com/dbaumgaertner
#                   Geiser Armin, https://github.com/armingeiser
#
# ==============================================================================

# Making KratosMultiphysics backward compatible with python 2.6 and 2.7
from __future__ import print_function, absolute_import, division

# Kratos Core and Apps
import KratosMultiphysics as km
import KratosMultiphysics.ShapeOptimizationApplication as KSO

from KratosMultiphysics.ShapeOptimizationApplication import model_part_controller_factory

# Additional imports
from KratosMultiphysics.StructuralMechanicsApplication import structural_response_function_factory as csm_response_factory
from .analyzer_base import AnalyzerBaseClass
from KratosMultiphysics.StructuralMechanicsApplication import structural_response
from KratosMultiphysics.StructuralMechanicsApplication.structural_mechanics_analysis import StructuralMechanicsAnalysis
from KratosMultiphysics import Parameters
import time as timer
import shutil
import glob, os

# ==============================================================================
class KratosInternalAnalyzer( AnalyzerBaseClass ):
    # --------------------------------------------------------------------------
    def __init__( self, specified_responses, model_part_controller ):
        # self.response_functions = {}
        self.model_part_controller = model_part_controller
        self.specified_responses = specified_responses
        self.model = model_part_controller.GetModel()
        #print("::Response Object Creation Skipped::")
        
        # for (response_id, response_settings) in specified_responses:
        #     #print("CALLED:::", response_id, response_settings)           
            
        #     self.response_functions[response_id] = csm_response_factory.CreateResponseFunction(response_id, response_settings, model)    
    #---------------------------------------------------------------------------
    def DirForOptimization(self, OptiFile, itr , FolderName):
        shutil.copy(OptiFile, str(itr)+ "_ITR" + ".post.bin")            
        dir = "/home/jey/Desktop/CodeWorld/KRATOS/JeyExamples/MultiObjectiveThickShell/ITR_Results/"

        for file in glob.glob("*_ITR.post.bin"):
            dst = dir + "" + file.replace(".post.bin", FolderName)
            os.mkdir(dst)
            #print("RESULT FOLDER CREATED--- !!")
            shutil.move(file, dst)
    
    def InitializeBeforeOptimizationLoop( self ):
        # for response in self.response_functions.values():
        #     response.Initialize()
        print("::InitOpti Ignored")
    # --------------------------------------------------------------------------
    def AnalyzeDesignAndReportToCommunicator( self, currentDesign, optimizationIteration, communicator ):
        
        optimization_model_part = self.model_part_controller.GetOptimizationModelPart()
        model_part_nodes = optimization_model_part.Nodes
        print("::OPTI MODEL::")
        x = []
        y = []
        z = []
        for node in model_part_nodes:
            x.append(node.X)
            y.append(node.Y)
            z.append(node.Z)
            # if node.Id < 6:
            #     print(node.Id, x[node.Id-1], y[node.Id-1], z[node.Id-1])
            # elif node.Id == 88:
            #     print(node.Id, node.X, node.Y, node.Z)
            print(node.Id, x[node.Id-1], y[node.Id-1], z[node.Id-1])
       
        time_before_analysis = optimization_model_part.ProcessInfo.GetValue(km.TIME)
        step_before_analysis = optimization_model_part.ProcessInfo.GetValue(km.STEP)
        delta_time_before_analysis = optimization_model_part.ProcessInfo.GetValue(km.DELTA_TIME)

        
        if optimizationIteration == 1:
            self.response_functions = {}
            for (response_id, response_settings) in self.specified_responses:
                self.response_functions[response_id] = csm_response_factory.CreateResponseFunction(response_id, response_settings, self.model, optimizationIteration)  
        
        else:    
            for identifier, response in self.response_functions.items():
                response.model.DeleteModelPart(response.primal_model_part.Name)
                print("::ModelPart Deleted::", response.primal_model_part.Name)
            for (response_id, response_settings) in self.specified_responses:
                self.response_functions[response_id] = csm_response_factory.CreateResponseFunction(response_id, response_settings, self.model, optimizationIteration)

        
        for identifier, response in self.response_functions.items():        

            # Reset step/time iterators such that they match the optimization iteration after calling CalculateValue (which internally calls CloneTimeStep)
            optimization_model_part.ProcessInfo.SetValue(km.STEP, step_before_analysis-1)
            optimization_model_part.ProcessInfo.SetValue(km.TIME, time_before_analysis-1)
            optimization_model_part.ProcessInfo.SetValue(km.DELTA_TIME, 0)

            # print("::OPTI PART 2::", response.primal_model_part)
            response.SetCoordinatesUpdate(x, y, z)

            response.Initialize()

            # for node in response.primal_model_part.Nodes:
            #     node.X = x[node.Id-1]
            #     node.Y = y[node.Id-1]
            #     node.Z = z[node.Id-1]

            # KSO.MeshControllerUtilities(response.primal_model_part).SetReferenceMeshToMesh()
            # print("::Nodes Transfered from OPTI MODELPART TO PRIMAL MODELPART::")
            
            response.InitializeSolutionStep()

            # response values
            if communicator.isRequestingValueOf(identifier):
                response.CalculateValue()
                communicator.reportValue(identifier, response.GetValue())
            
            # response gradients
            if communicator.isRequestingGradientOf(identifier):
                response.CalculateGradient()
                communicator.reportGradient(identifier, response.GetShapeGradient())
            
            response.FinalizeSolutionStep()

            # Clear results or modifications on model part
            optimization_model_part.ProcessInfo.SetValue(km.STEP, step_before_analysis)
            optimization_model_part.ProcessInfo.SetValue(km.TIME, time_before_analysis)
            optimization_model_part.ProcessInfo.SetValue(km.DELTA_TIME, delta_time_before_analysis)

            self.model_part_controller.SetMeshToReferenceMesh()

            self.model_part_controller.SetDeformationVariablesToZero()

            KSO.MeshControllerUtilities(response.primal_model_part).SetMeshToReferenceMesh()
            
            KSO.MeshControllerUtilities(response.primal_model_part).SetDeformationVariablesToZero()        

    # --------------------------------------------------------------------------
    def FinalizeAfterOptimizationLoop( self ):
        for response in self.response_functions.values():
            response.Finalize()

    # --------------------------------------------------------------------------
    @staticmethod
    def __CreateResponseFunctions( specified_responses, model ):
        response_functions = {}

        available_csm_response_functions = ["strain_energy", "mass", "eigenfrequency", "adjoint_local_stress", "adjoint_max_stress"]

        for (response_id, response_settings) in specified_responses:
            if response_id in response_functions.keys():
                raise NameError("There are multiple response functions with the following identifier: " + response_id)

            if response_settings["response_type"].GetString() in available_csm_response_functions:
                response_functions[response_id] = csm_response_factory.CreateResponseFunction(response_id, response_settings, model)
            else:
                raise NameError("The following structural response function is not available: " + response_id)

        return response_functions