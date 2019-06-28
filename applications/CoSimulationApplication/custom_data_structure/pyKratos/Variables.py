from __future__ import print_function, absolute_import, division  # makes these scripts backward compatible with python 2.6 and 2.7

# Other imports
from copy import deepcopy

class Variable(object):
    def __init__(self, var_name, var_type, zero_val):
        self.__name = var_name
        self.__type = var_type
        self.__zero = zero_val

    def Name(self):
        return self.__name

    def Type(self):
        return self.__type

    def Zero(self):
        # copy to make sure that nothing is referenced wrong
        return deepcopy(self.__zero)

    def __hash__(self):
        return hash(self.__name)

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return 'Variable "{}" of type "{}"'.format(self.__name, self.__type)

def CreateDoubleVariable(name):
    if name in globals():
        raise NameError('Variable "{}" exists already!'.format(name))
    globals()[name] = Variable(name, "Double", 0.0)

def CreateComponentVariable(name):
    if name in globals():
        raise NameError('Variable "{}" exists already!'.format(name))
    globals()[name] = Variable(name, "Component", 0.0)

def CreateArray3Variable(name):
    if name in globals():
        raise NameError('Variable "{}" exists already!'.format(name))

    for comp in ["X", "Y", "Z"]:
        CreateComponentVariable(name + "_" + comp)

    globals()[name] = Variable(name, "Array", [0.0, 0.0, 0.0])

def CreateVectorVariable(name):
    if name in globals():
        raise NameError('Variable "{}" exists already!'.format(name))
    globals()[name] = Variable(name, "Vector", [])

CreateArray3Variable("DISPLACEMENT")
CreateArray3Variable("MESH_DISPLACEMENT")
CreateArray3Variable("ROTATION")
CreateArray3Variable("VELOCITY")
CreateArray3Variable("POINT_LOAD")
CreateArray3Variable("FORCE")
CreateArray3Variable("REACTION")
CreateArray3Variable("EXTERNAL_FORCE")
CreateArray3Variable("TORQUE")
CreateArray3Variable("NORMAL")

CreateDoubleVariable("PRESSURE")
CreateDoubleVariable("YOUNG_MODULUS")
CreateDoubleVariable("POISSON_RATIO")
CreateDoubleVariable("DOMAIN_SIZE")
CreateDoubleVariable("DENSITY")
CreateDoubleVariable("VISCOSITY")
CreateDoubleVariable("TIME")
CreateDoubleVariable("DELTA_TIME")
CreateDoubleVariable("TEMPERATURE")
CreateDoubleVariable("NODAL_MASS")
CreateDoubleVariable("NODAL_ERROR")

CreateVectorVariable("EXTERNAL_FORCES_VECTOR")
