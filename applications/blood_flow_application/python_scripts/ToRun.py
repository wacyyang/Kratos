from __future__ import print_function, absolute_import, division
#NVMD3Z
import sys
#output_file=open("python_output","w")
#sys.stdout =output_file
import time
#print (time.strftime("%H:%M:%S"))
#print (time.strftime("%D/%M/%Y"))

domain_size = 3

from KratosMultiphysics import *
from KratosMultiphysics.BloodFlowApplication import *
from KratosMultiphysics.FluidDynamicsApplication import *
from KratosMultiphysics.IncompressibleFluidApplication import *
from KratosMultiphysics.MeshingApplication import *
from KratosMultiphysics.ExternalSolversApplication import *


def GetNodeAfter(table, prop):
	return table[prop - 1][4]

import math
import Only1D


# defining a model part for the fluid and one for the structure
model_part1D = ModelPart("FluidPart")
model_part3D = ModelPart("FluidPart3D")
model_part_centerline = ModelPart("Certerline")

proc_info = model_part1D.ProcessInfo

# defining a model part for the fluid and one for the structure
model_part1D.AddNodalSolutionStepVariable(RADIUS)
model_part1D.AddNodalSolutionStepVariable(BETA)
model_part1D.AddNodalSolutionStepVariable(C0)
model_part1D.AddNodalSolutionStepVariable(NODAL_AREA)
model_part1D.AddNodalSolutionStepVariable(NODAL_MASS)
model_part1D.AddNodalSolutionStepVariable(RHS)
model_part1D.AddNodalSolutionStepVariable(WORK)
model_part1D.AddNodalSolutionStepVariable(FLOW)
model_part1D.AddNodalSolutionStepVariable(VELOCITY)
model_part1D.AddNodalSolutionStepVariable(THICKNESS)
model_part1D.AddNodalSolutionStepVariable(YOUNG_MODULUS)
model_part1D.AddNodalSolutionStepVariable(POISSON_RATIO)
model_part1D.AddNodalSolutionStepVariable(DENSITY)
model_part1D.AddNodalSolutionStepVariable(TERMINAL_RESISTANCE)
model_part1D.AddNodalSolutionStepVariable(FLAG_VARIABLE)
model_part1D.AddNodalSolutionStepVariable(PRESSURE)
model_part1D.AddNodalSolutionStepVariable(PRESSURE_VENOUS)
model_part1D.AddNodalSolutionStepVariable(SYSTOLIC_PRESSURE)
model_part1D.AddNodalSolutionStepVariable(DYASTOLIC_PRESSURE)
model_part1D.AddNodalSolutionStepVariable(AVERAGE_PRESSURE)


import vms_fractional_step_solver as solver
solver.AddVariables(model_part3D)
model_part3D.AddNodalSolutionStepVariable(FLAG_VARIABLE)
model_part3D.AddNodalSolutionStepVariable(DISTANCE)


# ARCHIVE TO SET ::::::::::::::::::::::::::: >>>>> VARIABLES
import config
import simulation_config
import removal_tool
import CouplingTools1D_3Dv6

Config_version="ToRun.PY:VERSION 05_June_2014_local"
print(Config_version)
#print("LOCAAAAAAAAAAAAAAAAAAAAAAAALLLLLLLLLLLLLLLLLLLLLLLLLLL")
#raw_input()

SUMADOR=0.0
SUMADOR_2=0.0

# Set General Conditions
FitValues = simulation_config.FitValues
only1Dtest = simulation_config.only1Dtest
Coupled_Simulation = simulation_config.Coupled_Simulation
blood_viscosity = simulation_config.blood_viscosity
blood_density = simulation_config.blood_density

# Set initial_Radius
FitRadius = simulation_config.FitRadius

# Set Catheter Conditions
CatheterRadius = simulation_config.Catheter_Radius
Use_Catheter = simulation_config.Use_Catheter

# Set Path and variables names
relative_path_3D = config.relative_path_3D
relative_path_1D = config.relative_path_1D
name_model_3D_2 = config.name
artery_type = config.artery_type[0]

# Set computational_time
cardiac_cycle = config.nro_cardiac_cycles # total_time (last value of the Cardiac_cycle
time_cardiac_cycle = config.time_period# True-->Sub_step_control (only for the Coupled_3d_1d)
Sub_steping = simulation_config.Sub_steping
sub_step = simulation_config.sub_step  ## True-->Activate	False-->fix (step_size)
step_size_control = simulation_config.step_size_control
step_size = simulation_config.step_size	 # config.step_size
CardiacCycleConvergence = simulation_config.CardiacCycleConvergence

# Set coupling condition
cycle_to_couple = False
nro_cardiac_cycle = 1
if((cardiac_cycle == 1) and (Coupled_Simulation == True)):
	# 3D cardiac_Cycle will be running
	cardiac_cycle_to_3D = cardiac_cycle + 1
else:
	cardiac_cycle_to_3D = cardiac_cycle	 # 3D cardiac_Cycle will be running

#print("TESSSSSSSSSSSSSSSSSSSSSSSSSS:::cardiac_cycle_to_3D")
#cycle_to_couple = True
#nro_cardiac_cycle = 1

# Pressure conditions
diastolic_pressure = config.diastolic_pressure	# Pa
systolic_pressure = config.systolic_pressure  # Pa
venous_pressure=diastolic_pressure*0.9 #Pa
diastolic_hypermia_pressure = config.diastolic_hypermia_pressure  # Pa
time_period = config.time_period
InletProfileType = simulation_config.inlet_pressure_type

# Set_Results (# Aprox to save results)
final_time = cardiac_cycle_to_3D * time_cardiac_cycle
ascii = simulation_config.ascii_results
save_results = simulation_config.save_results

# Set Aux_variables
# This variable is only for doing test A-B with the 1D model.
Fit_control = False
var_aux = True
#pressure_factor = simulation_config.pressure_factor
Resistence_factor = config.hypermia_Resistence_factor
Condition_Variable = config.hypermia_Condition_Variable
#diastolic_pressure = pressure_factor * diastolic_pressure
#systolic_pressure = pressure_factor * systolic_pressure

# Set Initial_conditions
Q_initial = simulation_config.Q_initial
P_initial = diastolic_pressure

# Write Conditions Used

file_test = str(name_model_3D_2) + "_summary.txt"
summary_file = open(file_test, 'w')
ToWriteIn_Summary = "This case was running, day: " + "\n"
ToWriteIn_Summary += str(time.strftime("%H:%M:%S")) + "\n"
ToWriteIn_Summary += str(time.strftime("%d/%m/%y")) + "\n"
ToWriteIn_Summary += str(Config_version) + "\n"

# Write Conditions Used
file_test = str("Pressure at the Distal Part.txt")
summary_file_pressure_outlet3D= open(file_test, 'w')

# Write Pressure at the Aortic Root Used
file_test = str("Pressure at the Proximal (Aortic Root).txt")
summary_file_pressure_inlet1D= open(file_test, 'w')

# Write Conditions Used
#file_test = str("Flow(first simulation).txt")
#summary_file_pressure_Flow_1D= open(file_test, 'w')

# Write Conditions Used
# file_test = str("Flow at the Aortic Root.txt")
# summary_file_pressure_Flow_Aortic= open(file_test, 'w')

#To Write Flow
i_Flow_file=0
ffit_Flow =[]
ffit_Flow.append(i_Flow_file)
file_test = str("Flow.txt")
ffit_Flow[i_Flow_file] = open(file_test, 'w')

if ((InletProfileType == "coseno") or (InletProfileType == "parabolic") or (InletProfileType == "table")):
	InletPressure = True	
else:
	InletPressure = False

name_model_3D = relative_path_3D + name_model_3D_2
print("-------------------------------------------------------------------------")
print("Name_model_3D:", name_model_3D)
ToWriteIn_Summary += "Name_model_3D: "
ToWriteIn_Summary += str(name_model_3D)
ToWriteIn_Summary += "\n"
print("Final Time:", time_cardiac_cycle)
# Error
if (cardiac_cycle <= 0):
	print("Number of Cardiac Cycles must be > 0")
	print("Please check config file")
	ToWriteIn_Summary += "Number of Cardiac Cycles must be > 0. Please check config file" + "\n"
	summary_file.write(ToWriteIn_Summary)
	sys.exit("Proccess Kill")

if((Coupled_Simulation) and (cardiac_cycle_to_3D < 2)):
	print("Please revise:: cardiac_cycle_to_3D")
	print("Please check config file")
	ToWriteIn_Summary += "Please revise:: cardiac_cycle_to_3D. Please check config file" + "\n"
	summary_file.write(ToWriteIn_Summary)
	sys.exit("Proccess Kill")

if (systolic_pressure < diastolic_pressure):
	print("Please revise:: systolic and diastolic pressure:")
	print("systolic pressure -> ", systolic_pressure,
		  "diastolic_pressure-> ", diastolic_pressure)
	print("Please check config file")
	ToWriteIn_Summary += "Please revise:: systolic and diastolic pressure. Please check config file" + "\n"
	summary_file.write(ToWriteIn_Summary)
	sys.exit("Proccess Kill")

if (diastolic_pressure < diastolic_hypermia_pressure):
	print("Please revise:: diastolic hypermia pressure and diastolic pressure:")
	print("diastolic pressure -> ", systolic_pressure, "must be higher than diastolic hypermia_pressure-> ", diastolic_pressure)
	print("Please check config file")
	ToWriteIn_Summary += "diastolic pressure -> " +  str(systolic_pressure) + " must be higher than diastolic hypermia_pressure-> " + str(diastolic_pressure) + " Please check config file" + "\n"
	summary_file.write(ToWriteIn_Summary)
	sys.exit("Proccess Kill")

# Select 1D mdpa file
if (artery_type == 1):
	input_file_name1 = "Left_Balanced_Dominant"
	input_file_name = relative_path_1D + "Left_Balanced_Dominant"
	print("1D Arterial model:::>>> ", input_file_name)

if (artery_type == 2):
	input_file_name1 = "Left_LCA_Dominant"
	input_file_name = relative_path_1D + "Left_LCA_Dominant"
	print("1D Arterial model:::>>> ", input_file_name)

if (artery_type == 3):
	input_file_name1 = "Left_RCA_Dominant"
	input_file_name = relative_path_1D + "Left_RCA_Dominant"
	print("1D Arterial model:::>>> ", input_file_name)

if (artery_type == 4):
	input_file_name1 = "Left_Small_RCA_Dominant"
	input_file_name = relative_path_1D + "Left_Small_RCA_Dominant"
	print("1D Arterial model:::>>> ", input_file_name)

if (artery_type == 5):
	input_file_name1 = "Right_Balanced_Dominant"
	input_file_name = relative_path_1D + "Right_Balanced_Dominant"
	print("1D Arterial model:::>>> ", input_file_name)

if (artery_type == 6):
	input_file_name1 = "Right_LCA_Dominant"
	input_file_name = relative_path_1D + "Right_LCA_Dominant"
	print("1D Arterial model:::>>> ", input_file_name)

if (artery_type == 7):
	input_file_name1 = "Right_RCA_Dominant"
	input_file_name = relative_path_1D + "Right_RCA_Dominant"
	print("FitValues1D Arterial model:::>>> ", input_file_name)

if (artery_type == 8):
	input_file_name1 = "Right_Small_RCA_Dominant"
	input_file_name = relative_path_1D + "Right_Small_RCA_Dominant"
	print("1D Arterial model:::>>> ", input_file_name)

if (artery_type == 9):
	input_file_name1 = "Right_test"
	input_file_name = relative_path_1D + "Right_test"
	print("1D Arterial model:::>>> ", input_file_name)

if (artery_type > 9 or artery_type < 1):
	print("Please check your config file. Not Artery Type found")
	print("Please check your config.file")
	ToWriteIn_Summary += "Please check your config file. Not Artery Type found.Please check your config.file" 
	summary_file.write(ToWriteIn_Summary)
	sys.exit("Proccess Kill")
	
if ((artery_type < 5)):
	for i in range(0, len(config.deactivate_list)):
		artery_number_id = config.deactivate_list[i]
		print(artery_number_id)
		if (artery_number_id < 15):
			print("Please check your config file. Artery number = ", artery_number_id, ", dont belong to the ", input_file_name1)
			print("Please check your model")
			ToWriteIn_Summary += "Please check your config file. Artery number = "+ str(artery_number_id) + ", dont belong to the " + str(input_file_name1)
			summary_file.write(ToWriteIn_Summary)
			sys.exit("Proccess Kill")
			break
else:
	for i in range(0, len(config.deactivate_list)):
		artery_number_id = config.deactivate_list[i]
		if (artery_number_id > 16):
			print("Please check your config file. Artery number = ", artery_number_id, "dont belong to the ", input_file_name1)
			print("Please check your model")
			ToWriteIn_Summary += "Please check your config file. Artery number = " + str(artery_number_id) + ", dont belong to the " + str(input_file_name1) 
			summary_file.write(ToWriteIn_Summary)
			sys.exit("Proccess Kill")
			break

#print("TESSSSSSSSSSSSSSSSSSSSSSSSSS:::cardiac_cycle_to_3D")
#cardiac_cycle_to_3D=2
			
# select_centerline
ToWriteIn_Summary += "Name_model_1D: "
ToWriteIn_Summary += str(input_file_name)
ToWriteIn_Summary += "\n"
if(Use_Catheter==True):
	input_centerline_name = relative_path_1D + "centerline"
	print("1D CenterLine_Arterial model:::>>> ", input_centerline_name)
	ToWriteIn_Summary += "Name_model_1D_centerline: "
	ToWriteIn_Summary += str(input_centerline_name)
	ToWriteIn_Summary += "\n"

normal = simulation_config.normal_direcction
if (normal == "in"):
    # Normal estan ao donde apuntan las noramles debido al signo de la velocidad de entrada -1 en trasfer 1D3D
    ToWriteIn_Summary += "Surface Elements Normal direction is inward vector. " + "\n"        
elif (normal == "out"):
    ToWriteIn_Summary += "Surface Elements Normal direction is outward vector. " + "\n"        
else:
    print("Please check your simulation_config file.py")
    print("You must check the normal of your model as inward or outward")
    sys.exit("Proccess Kill")
        
ToWriteIn_Summary += "Fit Radius: " + str(FitRadius) + "\n"
ToWriteIn_Summary += "Initial Flow: " + str(Q_initial) + "\n"
ToWriteIn_Summary += "Initial Pressure: " + str(P_initial) + "\n"

# Read_Centerline
if(Use_Catheter==True):
	model_part_io_centerline = ModelPartIO(input_centerline_name)
	model_part_io_centerline.ReadModelPart(model_part_centerline)

# Preparing GiD FILES
gid_mode = GiDPostMode.GiD_PostBinary
multifile = MultiFileFlag.MultipleFiles
deformed_mesh_flag = WriteDeformedMeshFlag.WriteUndeformed
# WriteElementsOnly # WriteConditions
write_conditions = WriteConditionsFlag.WriteConditions #WriteElementsOnly
model_part_io_fluid = ModelPartIO(input_file_name)
model_part_io_fluid.ReadModelPart(model_part1D)

# Fit_control = This variable is only for doing test A-B with the 1D model. CouplingTools1D_3Dv5 is needed to inizializate the radius
# TBR
# if((FitValues) and (Coupled_Simulation == False)):
	#######Coupled_Simulation = True
	#######Fit_control = True
	#######print("Fit_control", Fit_control)
	# raw_input()

#node3D.Fix(VELOCITY_X)

#counter=0
#for cond in model_part1D.Conditions:
	#for node in cond.GetNodes():
		#print("Node and Conditions for the 1D FFR model")
		#print(cond.Id)
		#print(node.Id)
		#print("Parameter A=", cond.GetValue(a))
		#print("Parameter B=", cond.GetValue(b))
		#counter = counter +1
		#raw_input()
#print(counter)
#raw_input()

plot1d = True
plot3d = True
if (Coupled_Simulation == False):
	if (ascii == False):
		gid_io = GidIO(input_file_name, gid_mode, multifile, deformed_mesh_flag, write_conditions)
	else:
		results = str(input_file_name + ".cvpr")
		f1d = open(results, 'w')
	print("Only 1D Problem is running")
	ToWriteIn_Summary += "Only 1D Problem is running"
	ToWriteIn_Summary += "\n"
	plot3d = False	# Plot 1D variables
	Sub_steping = False
	output_step = 1	 # for eachffit.append = 1
else:  # 3D-1D coupled problem is running
	# Plot3d=true---> Plot 3D variables. Plot3d=False---> Plot 1D variables.
	if (plot3d):
		if(ascii == False):
			gid_io_3d = GidIO(name_model_3D, gid_mode, multifile, deformed_mesh_flag, write_conditions)
			if(Use_Catheter==True):
				gid_io_centerline = GidIO(input_centerline_name, gid_mode, multifile, deformed_mesh_flag, write_conditions)
		else:
			results = str(name_model_3D + ".cvpr")
			f3d = open(resuls, 'w')
	if (plot1d):
		if (ascii == False):
			gid_io = GidIO(input_file_name, gid_mode, multifile, deformed_mesh_flag, write_conditions)
		else:
			results = str(input_file_name + ".cvpr")
			f1d = open(results, 'w')
	print("Coupled 3D-1D Problem is running")
	ToWriteIn_Summary += "Coupled 3D-1D Problem is running"
	ToWriteIn_Summary += "\n"
	# To compute FFR Values
	FFR_Inlet_NODES_Values = []
	FFR_Outlet_NODES_Values = []
	[FFR_Inlet_NODES_Values,FFR_Outlet_NODES_Values] = removal_tool.DoRemoval(model_part1D)
	#for i in range(0, len(FFR_Inlet_NODES_Values)):
		#print (i)
		#inlet= FFR_Inlet_NODES_Values[i]
		#print(inlet)
	#for j in range(0, len(FFR_Outlet_NODES_Values)):
		#print (j)
		#outlet= FFR_Outlet_NODES_Values[j]
		#print(outlet)		
	#raw_input()
	# Read 3D model
	
	model_part_io_3D = ModelPartIO(name_model_3D)
	model_part_io_3D.ReadModelPart(model_part3D)
	model_part3D.SetBufferSize(3)
	solver.AddDofs(model_part3D)

# setting up the buffer size: SHOULD BE DONE AFTER READING!!!
model_part1D.SetBufferSize(2)
out = 1
# mesh to be printed
if(ascii == False):
	if(plot1d):
		mesh_name = 0.0
		gid_io.InitializeMesh(mesh_name)
		gid_io.WriteMesh(model_part1D.GetMesh())
		gid_io.WriteNodeMesh(model_part1D.GetMesh())
		gid_io.FinalizeMesh()
		gid_io.InitializeResults(mesh_name, (model_part1D).GetMesh())
		print("Writing 1D Mesh------------------------")
	if(plot3d and only1Dtest == False):
		mesh_name = 0.0
		gid_io_3d.InitializeMesh(mesh_name)
		gid_io_3d.WriteMesh(model_part3D.GetMesh())
		gid_io_3d.FinalizeMesh()
		gid_io_3d.InitializeResults(mesh_name, (model_part3D).GetMesh())
		print("Writing 3D Mesh------------------------")
		if(Use_Catheter==True):
			gid_io_centerline.InitializeMesh(mesh_name)
			gid_io_centerline.WriteMesh(model_part_centerline.GetMesh())
			gid_io_centerline.FinalizeMesh()
			gid_io_centerline.InitializeResults(mesh_name, (model_part_centerline).GetMesh())
			print("Writing Catheter Mesh------------------------")
			gid_io_centerline.FinalizeResults()

# Initial conditions for the 1D model.
integrator = ArteryTimeIntegrator()
integrator.Initialize(model_part1D)
inletconditiontable = model_part1D.GetTable(1)
minlength = 1e+12
minlength = integrator.Element_minLength(model_part1D)

print("Checking 1D model.......................................................")
fixed_flow_nodes = []
fixed_pressure_nodes = []
if (InletPressure == False):
	for node in model_part1D.Nodes:
		if(node.IsFixed(FLOW) and node.GetSolutionStepValue(FLAG_VARIABLE) == 0):
			fixed_flow_nodes.append(node)
			print("Inlet Node:: flow ", node.Id)
			ToWriteIn_Summary += "Inlet Node:: flow " + str(node.Id) + "\n"
else:
	for node in model_part1D.Nodes:
		if(node.IsFixed(PRESSURE) and node.GetSolutionStepValue(FLAG_VARIABLE) == 0):
			fixed_pressure_nodes.append(node)
			print("Inlet Node:: pressure ", node.Id)
			ToWriteIn_Summary +="Inlet Node:: pressure " + str(node.Id) + "\n"

Aortic_Inlet=node.Id
print(Aortic_Inlet)

if (InletPressure == False):
	model1D_control=len(fixed_flow_nodes)
	#print(str(model1D_control))
	if(model1D_control[0]==0):
		print("(WARNING) You are trying to use as PRESSURE inlet in a FLOW model. Please ckeck your 1D model\n\n")
		ToWriteIn_Summary += "(WARNING) You are trying to use PRESSURE inlet in a FLOW model. Please check your 1D model" + "\n"
		summary_file.write(ToWriteIn_Summary)
		sys.exit("Proccess Kill")
else:
	model1D_control=len(fixed_pressure_nodes)
	#print(str(model1D_control))
	if(model1D_control==0):
		ToWriteIn_Summary += "(WARNING) You are trying to use FLOW inlet in a PRESSURE model. Please check your 1D model" + "\n"
		print("(WARNING) You are trying to use FLOW inlet in a PRESSURE model. Please check your 1D model\n\n")
		summary_file.write(ToWriteIn_Summary)
		sys.exit("Proccess Kill")


# Catheter
if (Coupled_Simulation == True):
	if (Use_Catheter == True):
		print("Catheter Simulation ")
		ToWriteIn_Summary += "Catheter Simulation is running "
		ToWriteIn_Summary += "\n"
		ToWriteIn_Summary += "Catheter_Radius: "
		ToWriteIn_Summary += str(CatheterRadius)
		ToWriteIn_Summary += "\n"
		Full_Distance = []
		distance = 1E+20
		# for elements in model_part_centerline.Elements:
			# Full_Distance.append(DistanceCat.DistanceCatheter(elements.Id))
			# print "element", elements.Id
			# for node in elements.GetNodes():
			# nodeid=node.Id
			# node2_id=elements.node[1]
			# Full_Distance.DistanceCatheter.AddNodeinid(Node.Id)
			# print "node", node1
			# print "node", node2
			#nodeX = node.X
			#nodeY = node.Y
			#nodeZ = node.Z
			# for node3D in model_part3D.Nodes:
				# nodes3D_X=node3D.X
				# nodes3D_Y=node3D.Y
				# nodes3D_Z=node3D.Z
				#distance_Point = math.sqrt(pow((nodes3D_X-nodeX),2)+pow((nodes3D_Y-nodeY),2)+pow((nodes3D_Z-nodeZ),2))
				# if (distance_Point<distance):
				# distance=distance_Point
				# print distance
		distance = 1E+20
		for node3D in model_part3D.Nodes:
			node3D.SetSolutionStepValue(DISTANCE, 0, distance)

		for node3D in model_part3D.Nodes:
			nodes3D_X = node3D.X
			nodes3D_Y = node3D.Y
			nodes3D_Z = node3D.Z
			distance = 1E+20
			for node in model_part_centerline.Nodes:
				nodeX = node.X
				nodeY = node.Y
				nodeZ = node.Z
				distance_Point = pow((nodes3D_X - nodeX), 2) + pow(
					(nodes3D_Y - nodeY), 2) + pow((nodes3D_Z - nodeZ), 2)
				if (distance_Point < distance):
					distance = distance_Point
					# print distance
			distance = math.sqrt(distance)
			if (node3D.GetSolutionStepValue(DISTANCE) > distance):
				node3D.SetSolutionStepValue(DISTANCE, 0, distance)
	else:
		print("------------------------Not using Catheter-----------------------")
		ToWriteIn_Summary += "Not Using Catheter "
		ToWriteIn_Summary += "\n"


if (Coupled_Simulation == True):
	ThrowErrors = False
	check_process = TetrahedralMeshOrientationCheck(model_part3D,ThrowErrors)
	check_process.Execute()
	
New_method = True
if (Coupled_Simulation == True):
  	solver3D = solver.IncompressibleFluidSolver(model_part3D, 3)
	if(New_method == True):
		class pressure_config:
			solver_type = "AMGCL"
			scaling = False
			preconditioner_type = "None"
			max_iteration = 500
			tolerance = 1e-4
			smoother_type = "ILU0"
			krylov_type = "CG"
		class velocity_config:
			solver_type = "AMGCL"
			scaling = False
			preconditioner_type = "None"
			max_iteration = 500
			tolerance = 1e-4
			smoother_type = "ILU0"
			krylov_type = "GMRES"
		import linear_solver_factory
		solver3D.velocity_linear_solver = linear_solver_factory.ConstructSolver(velocity_config)
		solver3D.pressure_linear_solver = linear_solver_factory.ConstructSolver(pressure_config)
	solver3D.max_vel_its = 5
	solver3D.max_press_its = 10
	solver3D.predictor_corrector = True
	solver3D.vel_toll = 1e-4
	solver3D.press_toll = 1e-4
	solver3D.dynamic_tau = 1.0 #0.01
	solver3D.compute_reactions = False
	solver3D.activate_smagorinsky(0.2)
	solver3D.Initialize()	
	transfer_obj = CouplingTools1D_3Dv6.TransferTools(model_part1D, model_part3D)
	transfer_obj.Setting3d(P_initial, blood_density, blood_viscosity)
	transfer_obj.Initialize()
	#transfer_obj.Velocity_Initial_Contitions()	# Setting the velocity
	#transfer_obj.Initial_Contitions(diastolic_pressure)
	print("Coupled Simulation Algorithm sets with: ", Coupled_Simulation,"P:", solver3D.max_press_its, "V:", solver3D.max_vel_its)
	ToWriteIn_Summary += "Coupled Simulation Algorithm sets with: " "\n"
	ToWriteIn_Summary += "Pressure Iteration: " + str(solver3D.max_press_its) + "\n"
	ToWriteIn_Summary += "Velocity Iteration: " + str(solver3D.max_vel_its) + "\n"
	print("READY TO SIMULATE.................................")
else:
	print("ONLY 1D SOLVER....................................")
	cycle_to_couple = True
	ToWriteIn_Summary += "Only 1D Solver" + "\n"

if(Condition_Variable):
  ToWriteIn_Summary+="Condition Variable to modify the TERMINAL_RESISTANCE: " + str(Condition_Variable) + "\n"
  #ToWriteIn_Summary+="Pressure imposed: " + str(PRESSURE_DT) + "\n"
  ToWriteIn_Summary+="Resistence_factor: " + str(Resistence_factor) + "\n"
  for cond in model_part1D.Conditions:
	  for node in cond.GetNodes():
		#cond.SetValue(PRESSURE_VENOUS,diastolic_hypermia_pressure)
		#node.SetSolutionStepValue(PRESSURE_VENOUS, 0, diastolic_hypermia_pressure)
		#print("..............................")
		#print(node.GetSolutionStepValue(PRESSURE_VENOUS))
		#print(cond.GetValue(PRESSURE_DT))
		cond.SetValue(PRESSURE_DT, Resistence_factor)
		#print("..............................")
		#print(node.GetSolutionStepValue(PRESSURE_VENOUS))
		#print(cond.GetValue(PRESSURE_DT))
		#raw_input()
		#print (str(cond.GetValue(PRESSURE_DT)))
# Initial Values. Set the intial pressure as reference for the 1D model
# (all nodes take the systolic pressure and the initial flow)

if (InletPressure):
	print("Inlet Profile: Pressure")
	ToWriteIn_Summary += "Inlet Profile: Pressure" + "\n"
	if (InletProfileType == "parabolic"):
		print("Using as InletProfileType ---> PARABOLIC")				
		ToWriteIn_Summary += "Using as InletProfileType ---> PARABOLIC" + "\n"
		pressure_parameter_1 = ((systolic_pressure - diastolic_pressure) * 4) / time_period
		pressure_parameter_2 = (-pressure_parameter_1) / time_period
		pressure_parameter_3 = diastolic_pressure
	elif (InletProfileType == "coseno"):
		print("Using as InletProfileType ---> COSENO")
		ToWriteIn_Summary += "Using as InletProfileType ---> COSENO" + "\n"
		pressure_parameter_1 = (systolic_pressure + diastolic_pressure) / 2
		pressure_parameter_2 = (diastolic_pressure - systolic_pressure) / 2
	#print("Pressure_Factor: ", pressure_factor)
	#ToWriteIn_Summary += "Pressure_Factor: " + str(pressure_factor) + "\n"
	print("Diastolic pressure: ", diastolic_pressure)
	ToWriteIn_Summary += "Diastolic pressure: " + str(diastolic_pressure) + "\n"
	print("Systolic Pressure: ", systolic_pressure)
	ToWriteIn_Summary += "Systolic Pressure: " + str(systolic_pressure) + "\n"
	print("Venous Pressure: ", venous_pressure)
	ToWriteIn_Summary += "Venous Pressure: " + str(venous_pressure) + "\n"
	# diastolic_pressure=inletconditiontable.GetValue(0)
	for node in model_part1D.Nodes:
		if(node.IsFixed(PRESSURE) == False):
			node.SetSolutionStepValue(FLOW, 0, Q_initial)
			node.SetSolutionStepValue(PRESSURE, 0, P_initial)
			node.SetSolutionStepValue(DYASTOLIC_PRESSURE, 0, diastolic_pressure)
			node.SetSolutionStepValue(PRESSURE_VENOUS, 0, venous_pressure)
		else:
			node.SetSolutionStepValue(FLOW, 0, Q_initial)
			node.SetSolutionStepValue(PRESSURE, 0, P_initial)
			node.SetSolutionStepValue(DYASTOLIC_PRESSURE, 0, diastolic_pressure)
			node.SetSolutionStepValue(PRESSURE_VENOUS, 0, venous_pressure)
else:
	print("Inlet Profile: Flow")
	ToWriteIn_Summary += "Inlet Profile: Flow" + "\n"
	Q_initial = inletconditiontable.GetValue(0)
	for node in model_part1D.Nodes:
		if(node.IsFixed(FLOW) == False):
			node.SetSolutionStepValue(FLOW, 0, Q_initial)
			node.SetSolutionStepValue(PRESSURE, 0, P_initial)
			node.SetSolutionStepValue(DYASTOLIC_PRESSURE, 0, diastolic_pressure)
			node.SetSolutionStepValue(PRESSURE_VENOUS, 0, venous_pressure)
		else:
			node.SetSolutionStepValue(FLOW, 0, Q_initial)
			node.SetSolutionStepValue(PRESSURE, 0, P_initial)
			node.SetSolutionStepValue(DYASTOLIC_PRESSURE, 0,diastolic_pressure)
			node.SetSolutionStepValue(PRESSURE_VENOUS, 0, venous_pressure)

# Fit_control = This variable is only for doing test A-B with the 1D model. CouplingTools1D_3Dv5 is needed to inizializate the radius
# TBR
if((FitValues) and (Fit_control)):
	Coupled_Simulation = False

print("Initial conditions set")
print("----------------------------------------------------------------------------------------------------------")

time = 0.0
total_time = 0.0
step = 0.0
mean_flow_1d_value=0.0
mean_pressure_1d_value=0.0
out_pressure_1d_value=0.0
out_pressure_1d_value_FFR=0.0
mean_flow_1d_inlet=0.0
mean_flow_1d_FFR= 0.0
mean_pressure_1d_FFR=0.0
counter= 1.0
inicial = 1.0	
#total_time = total_time + step_size

ToWriteIn_Summary += "Cardiac_cycle Time: " + 	str(time_cardiac_cycle) + " second/s" + "\n"
if ((Coupled_Simulation) and (cardiac_cycle == 1)):
	ToWriteIn_Summary += "Cardiac_cycle defined by the user*: " + \
		str(config.nro_cardiac_cycles) + "\n"
	ToWriteIn_Summary += "*Due to 1D-3D Stabilization, the total number of cardiac_cycle is: "	+ \
		str(cardiac_cycle_to_3D) + "\n"
	ToWriteIn_Summary += "Coupling 1D-3D in the " + \
		str(cardiac_cycle_to_3D) + " cycle" + "\n"
else:
	ToWriteIn_Summary += "Number of cardiac_cycle: " + \
		str(cardiac_cycle_to_3D) + "\n"
	if (Coupled_Simulation):
		ToWriteIn_Summary += "Coupling 1D-3D in the " + \
			str(cardiac_cycle_to_3D) + " cycle" + "\n"

FFR_time = config.nro_FFR_cardiac_cycles			
ToWriteIn_Summary += "Total 1D-3D Coupling Time: " + str(final_time) + " second/s" + "\n"
ToWriteIn_Summary += "Total time to compute FFR values: " + str(FFR_time*time_cardiac_cycle) + " second/s" + "\n"
ToWriteIn_Summary += "Total Simulation time: " + str(final_time + (FFR_time*time_cardiac_cycle)) + " second/s" + "\n"

# Initial Delta Step
if (step_size_control == True):
	print("Using Adaptative Time Step")
	Dt = integrator.EstimateDeltaTime(model_part1D, 0.8, minlength)
	ToWriteIn_Summary += "Using Adaptative Time Step: " + str(Dt) + " (aprox) Seconds" + "\n"
else:
	Dt = step_size
	print("Using Fix Time Step", str(step_size))
	ToWriteIn_Summary += "Using Fix Time Step: " + str(Dt) + " seconds" + "\n"

steps_eval = final_time / Dt

print("----------------------------------------------------------------------------------------------------------")
control_sub_step = 1
if(save_results != 0.0):
	save_Steps = final_time / save_results
	if(save_results < steps_eval):
		output_step = math.floor(steps_eval / save_Steps)
	else:
		out_res = 1
		output_step = math.floor(steps_eval / out_res)
else:
	output_step = 1
		
if(Sub_steping):
	control_sub_step = 1
	# sub_step = 20
	if (output_step < sub_step):
		output_step = sub_step
	# for each time step, ie: 1 save 1 Dt, 100 save 100 Dt
else:
	Sub_steping = True
	control_sub_step = 1
	# sub_step = 200
	if ((output_step < sub_step) and (Coupled_Simulation)):
		output_step = sub_step

# Time controler
myTimer = Timer()

if (Use_Catheter == True):
	for node3D in model_part3D.Nodes:
		distance = node3D.GetSolutionStepValue(DISTANCE) - CatheterRadius
		node3D.SetSolutionStepValue(DISTANCE, 0, distance)
		if (distance < 0):
			node3D.Fix(PRESSURE)
			node3D.Fix(VELOCITY_X)
			node3D.Fix(VELOCITY_Y)
			node3D.Fix(VELOCITY_Z)

	for element3D in model_part3D.Elements:
		negative = 0
		positive = 0
		for node3d in element3D.GetNodes():
			d = node3d.GetSolutionStepValue(DISTANCE)
			if (d >= 0.0):
				positive = positive + 1
			else:
				negative = negative + 1
		if ((negative > 0) and (positive > 0)):
			tmp = Vector(4)
			i = 0
			element3D.SetValue(SPLIT_ELEMENT, True)
			for node3d in element3D.GetNodes():
				d = node3d.GetSolutionStepValue(DISTANCE)
				tmp[i] = d
				i = i + 1
			element3D.SetValue(ELEMENTAL_DISTANCES, tmp)

ToWriteIn_Summary += "Fix Pressure 3D" + "\n"
for node in model_part3D.Nodes:
	if(node.IsFixed(PRESSURE)):
		node.SetValue(IS_STRUCTURE, False)
# START COMPUTATION
# To Write Summary_contions_used
ToWriteIn_Summary += "Running Computation" + "\n"
ToWriteIn_Summary += "... ... ... ... ... ... ... ... ... ... " + "\n"
#ToWriteIn_Summary += "Type of the profile input condition used: " + str(InletProfileType) + "\n"
ToWriteIn_summary_file_pressure_inlet1D="Type of the profile input condition used: "  + str(InletProfileType) + "  Units: Pa " + "\n"
summary_file_pressure_inlet1D.write(ToWriteIn_summary_file_pressure_inlet1D)
summary_file.write(ToWriteIn_Summary)
print("Cardiac_cycle: ------------------------>", nro_cardiac_cycle)
ToWriteIn_Summary = "Cardiac_cycle: ------------------------> " + str(nro_cardiac_cycle) + "\n"
summary_file.write(ToWriteIn_Summary)


#cycle_to_couple = True 
#Coupled_Simulation = True

total_aortic_pressure=0.0
instant_pressure=0.0
counter=0.0
print(final_time)
print(time_cardiac_cycle)
final_time=final_time+0.000001
if(only1Dtest == False):
	while(total_time <= final_time):
		model_part1D.CloneTimeStep(total_time)
		counter=counter+1
		#print(str(step))		
		if (InletPressure == False):
			for node in fixed_flow_nodes:
			# if (Coupled_Simulation == False):
				if(node.IsFixed(FLOW) and node.GetSolutionStepValue(FLAG_VARIABLE) == 0):
					if (step > 1):
						Q = inletconditiontable.GetValue(time)
						Q = 2 * Q - node.GetSolutionStepValue(FLOW, 1)
						node.SetSolutionStepValue(FLOW, 0, Q)
					else:
						Q = inletconditiontable.GetValue(time)
						node.SetSolutionStepValue(FLOW, 0, Q)
		else:
			for node in fixed_pressure_nodes:
				# if (Coupled_Simulation == False):
				if(node.IsFixed(PRESSURE) and node.GetSolutionStepValue(FLAG_VARIABLE) == 0):
					if (InletProfileType == "parabolic"):
						print("Using Parabolic Pressure as input:")
						instant_pressure = (pressure_parameter_2 * time * time) + \
							(pressure_parameter_1 * time) + \
							pressure_parameter_3
						total_aortic_pressure=total_aortic_pressure+instant_pressure
						print("Using Parabolic Pressure Profile as Inlet: ", instant_pressure)
					elif (InletProfileType == "coseno"):
						instant_pressure = (pressure_parameter_1) + ((pressure_parameter_2) * (math.cos(2 * math.pi * total_time / time_period)))
						print("Using Coseno Pressure Profile as Inlet: ", instant_pressure)
						total_aortic_pressure=total_aortic_pressure+instant_pressure
					elif (InletProfileType == "table"):
						instant_pressure = inletconditiontable.GetValue(time)
						print("Using Table Pressure Profile(mdpa) as Inlet: ", instant_pressure)
						total_aortic_pressure=total_aortic_pressure+instant_pressure
					# Transfer pressure in Area & Flow
					A0 = node.GetValue(NODAL_AREA)
					#ToWriteIn_Summary = " A0:	"  + str(A0) + " mm " + "\n"	
					A_aux = node.GetSolutionStepValue(NODAL_AREA, 1)
					#ToWriteIn_Summary += " A_AUX:	"  + str(A_aux) + " mm " + "\n"
					A = (math.sqrt(A0) + (((instant_pressure - diastolic_pressure) * A0) / node.GetSolutionStepValue(BETA, 1))) ** 2
					#ToWriteIn_Summary += " A:	"  + str(A) + " mm " + "\n"
					#summary_file.write(ToWriteIn_Summary)
					A2 = pow(2 * pow(A, 0.25) - pow(A_aux, 0.25), 4)
					#ToWriteIn_Summary = " A2:	"  + str(A2) + " mm " + "\n"
					node.SetSolutionStepValue(NODAL_AREA, 0, A)
					#summary_file.write(ToWriteIn_Summary)
					# node.SetSolutionStepValue(FLOW,0,Q)
				else:
					print("No FIX pressure values are setted")
					sys.exit("Proccess Kill")
					break					
		#ToWriteIn_Summary = " Instant pressure imposed in the inlet node:	"  + str(instant_pressure) + " Pa " + "\n"
		#ToWriteIn_Summary += " Instant area imposed in the inlet node:	"  + str(A) + " mm " + "\n"	
		#summary_file.write(ToWriteIn_Summary)
		##ToWriteIn_summary_file_pressure_Flow_Aortic=str(Q)
		#raw_input()
		ToWriteIn_summary_file_pressure_inlet1D=str(instant_pressure) + "\n"
		summary_file_pressure_inlet1D.write(ToWriteIn_summary_file_pressure_inlet1D)
		#summary_file_pressure_Flow_Aortic(ToWriteIn_summary_file_pressure_Flow_Aortic)
		if (step < 3):
			model_part3D.CloneTimeStep(total_time)
			if (cycle_to_couple == False):
				#"Here I need to save the Flow for the first simulation"
				[mean_flow_1d_value,mean_pressure_1d_value,out_pressure_1d_value]=transfer_obj.Save_Values_For_different_situations(mean_flow_1d_value,mean_pressure_1d_value,out_pressure_1d_value)
			else:
				[mean_flow_1d_value,mean_pressure_1d_value,out_pressure_1d_value]=transfer_obj.Save_Values_For_different_situations(mean_flow_1d_value,mean_pressure_1d_value,out_pressure_1d_value)
				#[mean_flow_1d_FFR,mean_pressure_1d_FFR,out_pressure_1d_value_FFR]=transfer_obj.Save_Values_For_different_situations(mean_flow_1d_FFR,mean_pressure_1d_FFR,out_pressure_1d_value_FFR)
				
		# cardiac_cycle_to_3D)):
		if ((Coupled_Simulation) and (control_sub_step == sub_step) and (cycle_to_couple == True)):
			if(step >= 3):
				print("...............................Solve 3D..................................", step)
				integrator.SolveStep(model_part1D)
				# integrator.ComputePressure(model_part1D)
				integrator.ComputePressure(model_part1D, diastolic_pressure)
				model_part3D.CloneTimeStep(total_time)
				print("Total_time 3d:", total_time)
				print("----------------------------Transfer 1d(tn) to 3d(tn+1)------------------")
				#transfer_obj.Transfer1D_to_3D(diastolic_pressure, summary_file_pressure_outlet3D,summary_file_pressure_Flow_1D)
				transfer_obj.Transfer1D_to_3D(diastolic_pressure, summary_file_pressure_outlet3D)
				print("Solve 3D para ", total_time)
				myTimer.Start("solver3D.Solve()")
				print ("Checking_Pressure")
				transfer_obj.CheckPressure(model_part3D)
				solver3D.Solve()
				myTimer.Stop("solver3D.Solve()")
				print(myTimer)
				print("----------------------------Transfer 3d to 1d----------------------------")
				# transfer_obj.Transfer3D_to_1D()
				# integrator.SolveStep(model_part1D)
				#integrator.ComputePressure(model_part1D, diastolic_pressure)
				# removal_tool.ComputePressure(model_part1D)
				control_sub_step = 0
				var_aux = True
				out = out + 1
		else:
			if ((var_aux) and (Sub_steping)):
				print("Solve 1D ------------------------------> ", total_time)
				print("Sub_step-->", sub_step)
				var_aux = False
			else:
				print("Solve 1D ------------------------------> ", total_time)

			integrator.SolveStep(model_part1D)
			# Only when I saved the results.
			integrator.ComputePressure(model_part1D, diastolic_pressure)
			# removal_tool.ComputePressure(model_part1D) --> This is only needed
			# when the results are saved.			
			control_sub_step = control_sub_step + 1
			out = out + 1
			if (cycle_to_couple == False):
				#"Here I need to save the Flow for the first simulation"
				[mean_flow_1d_value,mean_pressure_1d_value,out_pressure_1d_value]=transfer_obj.Save_Values_For_different_situations(mean_flow_1d_value,mean_pressure_1d_value,out_pressure_1d_value)
			else:
				[mean_flow_1d_value,mean_pressure_1d_value,out_pressure_1d_value]=transfer_obj.Save_Values_For_different_situations(mean_flow_1d_value,mean_pressure_1d_value,out_pressure_1d_value)
				#[mean_flow_1d_FFR,mean_pressure_1d_FFR,out_pressure_1d_value_FFR]=transfer_obj.Save_Values_For_different_situations(mean_flow_1d_FFR,mean_pressure_1d_FFR,out_pressure_1d_value_FFR)
		
		if(save_results == 0.0):
			Writing_results= True
		else:
			if((math.fmod(step, output_step) == 0)):
				Writing_results= True
			else:
				Writing_results= False
		
		if((Writing_results) and (step >= 3) and (cycle_to_couple == True)):
			#integrator.ComputePressure(model_part1D, diastolic_pressure)
			#removal_tool.ComputePressure(model_part1D)			
			#if ((Coupled_Simulation) and (control_sub_step == sub_step) and (cycle_to_couple == True) and (FitValues)):	
			if (FitValues and Coupled_Simulation):	
				if ((Coupled_Simulation) and (step >= 3)):	
					print("Transfer Values for Fitting(time) -------------------------->", str(total_time))
					transfer_obj.FitValues_3d(total_time, ffit_Flow, i_Flow_file)
					transfer_obj.FitValues_1d(total_time)
					#print("hola1")
					#transfer_obj.FitValues_3d(total_time)
					#raw_input()
				else:
					transfer_obj.FitValues_1d(total_time)
					#print("hola2")
					#raw_input()
			#transfer_obj.FitValues_Inlet()
			if (CardiacCycleConvergence):
				print("Check Convergence")
				# Function to check the cardiac_Cycle convergence (move
				# outside)
				time_cardiac_cycle = integrator.CheckCardiacCovergence(model_part1D, time_cardiac_cycle)
			
			if (plot1d):
				if(ascii):
					f1d.write("Time_step \n")
					f1d.write(str(total_time))
					f1d.write("\n")
					# f.write("Number_of_nodes\n")
					f1d.write("Nodal_data_table (Index-NodeId-PRESSURE-VELOCITY_X-RADIUS) \n")
					indextowrite = 1
					for node in model_part1D.Nodes:
						nodewrite = node.Id
						ToWrite = str(indextowrite) + " "
						ToWrite += str(nodewrite) + " " + str(node.GetSolutionStepValue(PRESSURE)) + " "
						ToWrite += str(node.GetSolutionStepValue(VELOCITY_X)) + " " + str(node.GetSolutionStepValue(RADIUS)) + "\n"
						indextowrite = indextowrite + 1
						f1d.write(ToWrite)
					print("Writing CVPR results 1D Results(time)------------------------>", total_time)
				else:
					#gid_io.WriteNodalResults(NODAL_AREA, model_part1D.Nodes, total_time, 0)
					#gid_io.WriteNodalResults(NODAL_MASS, model_part1D.Nodes, total_time, 0)
					gid_io.WriteNodalResults(FLOW, model_part1D.Nodes, total_time, 0)
					#gid_io.WriteNodalResults(RADIUS, model_part1D.Nodes, total_time, 0)
					#gid_io.WriteNodalResults(RHS, model_part1D.Nodes, total_time, 0)
					#gid_io.WriteNodalResults(FLAG_VARIABLE, model_part1D.Nodes, total_time, 0)
					#gid_io.WriteNodalResults(VELOCITY, model_part1D.Nodes, total_time, 0)
					gid_io.WriteNodalResults(PRESSURE, model_part1D.Nodes, total_time, 0)
					#gid_io.WriteNodalResults(WORK, model_part1D.Nodes, total_time, 0)
					print("Writing GiD results 1D Results(time)------------------------>", total_time)
					
			if (Coupled_Simulation == False):
				control_sub_step = 0

			if((plot3d) and (cycle_to_couple == True) and (Coupled_Simulation)):
				transfer_obj.CheckPressure(model_part3D)
				if(ascii):
					f3d.write("Time_step \n")
					f3d.write(str(total_time))
					f3d.write("gid_io_3d.FinalizeResults()\n")
					# f.write("Number_of_nodes\n")
					# f.write(xxxx)
					f3d.write("Nodal_data_table (Index-NodeId-Pressure-Velocity_X-Velocity_Y-Velocity_Z) \n")
					indextowrite = 1
					for node in model_part3D.Nodes:
						nodewrite = node.Id
						pressure_rect = node.GetSolutionStepValue(PRESSURE)
						# +press_1d_out
						ToWrite = str(indextowrite) + " " + str(nodewrite) + " " + str(pressure_rect) + " "
						ToWrite += str(node.GetSolutionStepValue(VELOCITY_X)) + " "
						ToWrite += str(node.GetSolutionStepValue(VELOCITY_Y)) + " "
						ToWrite += str(node.GetSolutionStepValue(VELOCITY_Z)) + "\n"
						indextowrite = indextowrite + 1
						f3d.write(ToWrite)
					print("Writing CVPR results 3D Results(time)------------------------>", total_time)
					#raw_input()
				else:
					gid_io_3d.WriteNodalResults(VELOCITY, model_part3D.Nodes, total_time, 0)
					gid_io_3d.WriteNodalResults(PRESSURE, model_part3D.Nodes, total_time, 0)
					#gid_io_3d.WriteNodalResults(VISCOSITY, model_part3D.Nodes, total_time, 0)
					#gid_io_3d.WriteNodalResults(DENSITY, model_part3D.Nodes, total_time, 0)
					gid_io_3d.WriteNodalResults(FLAG_VARIABLE, model_part3D.Nodes, total_time, 0)
					gid_io_3d.WriteNodalResults(DISTANCE, model_part3D.Nodes, total_time, 0)
					gid_io_3d.WriteNodalResults(NORMAL, model_part3D.Nodes, total_time, 0)
					#gid_io_3d.PrintOnGaussPoints(NORMAL,model_part3D,total_time)
					out = 0
					#raw_input()
					print("Writing GiD results 3D results(step)------------------------>", total_time)

		if(time >= time_cardiac_cycle):
			time = time - time_cardiac_cycle
			nro_cardiac_cycle = nro_cardiac_cycle + 1
			print("nro_cardiac_cycle", str(nro_cardiac_cycle))
			if ((Coupled_Simulation) and (cycle_to_couple == False)):
				if (nro_cardiac_cycle == cardiac_cycle_to_3D):					
					cycle_to_couple = True
					transfer_obj.Initialize()
					# Setting the velocity
					#transfer_obj.Setting3d(P_initial, blood_density, blood_viscosity)
					#transfer_obj.Velocity_Initial_Contitions()
					print(step)
					print(total_aortic_pressure)
					print(counter)
					print(mean_pressure_1d_value)
					print(out_pressure_1d_value)
					print(mean_flow_1d_value)
					transfer_obj.ComputeFFR_Health_Values(total_aortic_pressure,mean_pressure_1d_value,out_pressure_1d_value,mean_flow_1d_value,counter,summary_file)
					total_aortic_pressure=0.0
					mean_pressure_1d_value=0.0
					out_pressure_1d_value=0.0
					mean_flow_1d_value=0.0
					counter=0
					print(step)
					print(total_aortic_pressure)
					print(counter)
					print("Start Coupling:: Only the last Cardiac_cycle ", cardiac_cycle_to_3D)
					ToWriteIn_Summary = "Start Coupling:: Only the last Cardiac_cycle " + str(cardiac_cycle_to_3D) + "\n"			
					summary_file.write(ToWriteIn_Summary)
					print("time",time)
					#raw_input()
			control_sub_step = 0
			out = 0
			print("Cardiac_cycle: ------------------------>", nro_cardiac_cycle)
			ToWriteIn_Summary = "Cardiac_cycle: ------------------------> " + str(nro_cardiac_cycle) + "\n"			
			summary_file.write(ToWriteIn_Summary)
			#raw_input()

		if (step_size_control):
			Dt = integrator.EstimateDeltaTime(model_part1D, 0.6, minlength)
		else:
			Dt = step_size

			
		time = time + Dt
		total_time = total_time + Dt
		out = out + 1
		step = step + 1		
		#counter = counter + 1
		# print "out_step", output_step
		# print "StepCoupled", StepCoupled
		# print "out", out

print("Finish Simulation")

import time
ToWriteIn_Summary = "Finish Computation" + "\n"
ToWriteIn_Summary += str(time.strftime("%H:%M:%S")) + "\n"
ToWriteIn_Summary += "-----------------------------------------------" + "\n"
summary_file.write(ToWriteIn_Summary)
print("---------------------------------------------------------------")

if (Coupled_Simulation == True):
	print("Computing 3D Energy Losses")
	ToWriteIn_Summary = "Computing 3D Energy Losses" + "\n"
	summary_file.write(ToWriteIn_Summary)
	# Setting A & B parameters
	if(only1Dtest == False):
		ab_3d_list = []
		if (FitValues):
			print("Fitting A-B values")
			ab_1d_list = transfer_obj.Fit_ABValues_1D()
			if (Coupled_Simulation):
				ab_3d_list = transfer_obj.Fit_ABValues_3D()
		else:
			print("ONLY TO CHECK HYPERMIA AND HEALTHY MODEL")
			A = 0
			B = 0
			raw_input()
	else:  # only if you want to set A-B parameters manually
		A = simulation_config.A
		B = simulation_config.B
		for j in range(0, len(FFR_Inlet_NODES_Values)):
			ToWriteIn_Summary = "FFR_Inlet_NODES_Values" + str(FFR_Inlet_NODES_Values[j].Id) + "\n"
			summary_file.write(ToWriteIn_Summary)
		for j in range(0, len(FFR_Outlet_NODES_Values)):
			print("FFR_Outlet_NODES_Values", FFR_Outlet_NODES_Values[j].Id)
			ToWriteIn_Summary = "FFR_Outlet_NODES_Values" + str(FFR_Outlet_NODES_Values[j].Id) + "\n"
			summary_file.write(ToWriteIn_Summary)
			ab_3d_list = []
			if (A == 0):
				A = 0
			if (B == 0):
				B = 0
			nodeAB = simulation_config.nodeAB
			ToWriteIn_Summary = "nodeAB" + str(nodeAB) + "\n"
			summary_file.write(ToWriteIn_Summary)
			if (nodeAB == 0):
				nodeAB = FFR_Outlet_NODES_Values[j].Id
			print(nodeAB)
			ab_3d_list.append([nodeAB, A, B])
			print(ab_3d_list)
			ToWriteIn_Summary = "ab_3d_list" + str(ab_3d_list) + "\n"
			summary_file.write(ToWriteIn_Summary)

	#if(Coupled_Simulation == False):
		#for item in ab_1d_list:
			#[node_id, A, B] = item
			#for cond in model_part1D.Conditions:
				#cond_of_interest = False
				#for node in cond.GetNodes():
					#if(node.Id == node_id):
						#cond_of_interest = True
						#print("Node and Conditions for the 1D FFR model")
						#print("Node", node_id)
						#print("Parameter A=", A)
						#print("Parameter B=", B)
				#if(cond_of_interest):
					#cond.SetValue(a, A)
					#cond.SetValue(b, B)
					#print("A and B setted_1D")
					#print("---------------------------------------------------------------------------------------------------------")

	for item in ab_3d_list:
		raw_input()
		[node_id, A, B] = item
		for cond in model_part1D.Conditions:
			cond_of_interest = False
			for node in cond.GetNodes():
				if(node.Id == node_id):
					cond_of_interest = True
					print("Node and Conditions for the 1D FFR model")
					print("Node", node_id)
					print("Parameter A=", A)
					print("Parameter B=", B)
					ToWriteIn_Summary = "Node and Conditions for the 1D FFR model" + "\n"
					ToWriteIn_Summary += "Node "+ str(node_id) + "\n"
					ToWriteIn_Summary += "Parameter A= "+ str(A) + "\n"
					ToWriteIn_Summary += "Parameter B= "+ str(B) + "\n"
					summary_file.write(ToWriteIn_Summary)
			if(cond_of_interest):
				cond.SetValue(a, A)
				cond.SetValue(b, B)
				print("A and B setted_3D")
				ToWriteIn_Summary = "A and B setted_3D"+ "\n"
				summary_file.write(ToWriteIn_Summary)

	print("----------------------------------------------------------------------------------------------------------")
	print("Setting Hypermenia Conditions")
	print("Sistolic_hypermia_pressure", config.systolic_hypermia_pressure)
	print("Diastolic_hypermia_pressure", config.diastolic_hypermia_pressure)
	Venous_hypermia_pressure = config.diastolic_hypermia_pressure * 0.9
	print("Venous_hypermia_pressure ", Venous_hypermia_pressure)
	#ToWriteIn_Summary = "-----------------------------------------------" + "\n"	
	#ToWriteIn_Summary += "Setting Hypermenia Conditions"+ "\n"
	#ToWriteIn_Summary += "Diastolic_hypermia_pressure: "+ str(diastolic_hypermia_pressure) + "\n"
	#summary_file.write(ToWriteIn_Summary)
	## TBR
	if(only1Dtest):
		for cond in model_part1D.Conditions:
			for node in cond.GetNodes():
				# cond.SetValue(PRESSURE_VENOUS,diastolic_hypermia_pressure)
				node.SetSolutionStepValue(PRESSURE_VENOUS, 0, Venous_hypermia_pressure)
				cond.SetValue(PRESSURE_DT, Resistence_factor)
				# print diastolic_pressure
				# print PRESSURE_VENOUS
				# print node.Id
				# print cond.Id
	print("----------------------------------------------------------------------------------------------------------")

	#-------------------------------------------------------------------------
	#-------------------------------------------------------------------------
	#-------------------------------------------------------------------------
	#To Check where A,B are imposed. 
	#for cond in model_part1D.Conditions:
		#for node in cond.GetNodes():
			#print("Node and Conditions for the 1D FFR model")
			#print(cond.Id)
			#print(node.Id)
			#ToWriteIn_Summary += "Node "+ str(node.Id) + "\n"
			#ToWriteIn_Summary += "Condition " + str(cond.Id) + "\n"
			#summary_file.write(ToWriteIn_Summary)
			
	
	# 3D reduced model is setted in transfer_obj.Fit_ABValues_3D
	print("Finish Compute 3D Energy Losses")
	print("Computing FFR Analysis")
	ToWriteIn_Summary = " Finish Compute 3D Energy Losses" + "\n"
	ToWriteIn_Summary += "-----------------------------------------------" + "\n"
	ToWriteIn_Summary += "Computing FFR Analysis" + "\n"	
	ToWriteIn_Summary += str(time.strftime("%H:%M:%S")) + "\n"
	summary_file.write(ToWriteIn_Summary)
	
	[mean_flow_1d_FFR]=Only1D.Only1D(model_part1D,model_part3D,total_time, config, simulation_config,input_file_name, FFR_Inlet_NODES_Values, FFR_Outlet_NODES_Values,summary_file,mean_flow_1d_value,Aortic_Inlet)
	#Only1D.Only1D(model_part1D, total_time, config, simulation_config,input_file_name, FFR_Inlet_NODES_Values, FFR_Outlet_NODES_Values,summary_file)
	#FFR_FLOW = mean_flow_1d_value/mean_flow_1d_FFR_2
	#print(mean_flow_1d_value)
	#print(mean_flow_1d_FFR)
	#print("CHECK::::FFR_FLOW",FFR_FLOW )
	print("Finish FFR Analysis")
	#-------------------------------------------------------------------------
	#-------------------------------------------------------------------------
	#-------------------------------------------------------------------------
	#-------------------------------------------------------------------------
	# m=node.GetSolutionStepValue(NODAL_MASS)
	# node.SetSolutionStepValue(NODAL_MASS,0,m+C)

	if (plot1d):
		if (ascii == False):
			gid_io.FinalizeResults()
		else:
			f1d.close()

	if (plot3d):
		if (ascii == False):
			gid_io_3d.FinalizeResults()
		else:
			f3d.close()
	
	import time		
	ToWriteIn_Summary = "Finish FFR Computation" + "\n"
	ToWriteIn_Summary += str(time.strftime("%H:%M:%S")) + "\n"
	ToWriteIn_Summary += "-----------------------------------------------" + "\n"
	summary_file.write(ToWriteIn_Summary)
	#ffit_Flow.close()


ToWriteIn_Summary = "----------------------------------------------------------------------"+ "\n"
ToWriteIn_Summary += "FINISH"+ "\n"
summary_file.write(ToWriteIn_Summary)
summary_file.close()
print("---------------------------------------------END-------------------------------------------")
