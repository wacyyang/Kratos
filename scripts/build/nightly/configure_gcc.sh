BOOST_DIR="${HOME}/CompiledLibs/boost_1_59_0"
PYTHON_VERSION_S=3
PYTHON_VERSION_M=5

cmake .. \
-DCMAKE_C_COMPILER=gcc                                                                          \
-DCMAKE_INSTALL_RPATH="${HOME}/Kratos/libs"                                                     \
-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE                                                        \
-DCMAKE_CXX_COMPILER=g++                                                                        \
<<<<<<< HEAD
=======
-DCMAKE_BUILD_TYPE=Custom                                                                      \
>>>>>>> ef6c07007... Added custom build mode
-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -msse3 -fopenmp"                                              \
-DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -msse3 -std=c++11 -fopenmp"                               \
-DBOOST_ROOT="${BOOST_DIR}"                                                                     \
-DPYTHON_LIBRARY="/usr/lib/python${PYTHON_VERSION_S}.${PYTHON_VERSION_M}/config-${PYTHON_VERSION_S}.${PYTHON_VERSION_M}m-x86_64-linux-gnu/libpython${PYTHON_VERSION_S}.${PYTHON_VERSION_M}m.so"                            \
-DPYTHON_INCLUDE_DIR="/usr/include/python${PYTHON_VERSION_S}.${PYTHON_VERSION_M}"               \
-DALE_APPLICATION=ON                                                                            \
-DADJOINT_FLUID_APPLICATION=OFF                                                                 \
-DCONVECTION_DIFFUSION_APPLICATION=ON                                                           \
-DDAM_APPLICATION=ON                                                                            \
-DDEM_APPLICATION=ON                                                                            \
-DEMPIRE_APPLICATION=ON                                                                         \
-DEXTERNAL_SOLVERS_APPLICATION=ON                                                               \
-DFSI_APPLICATION=ON                                                                            \
-DFLUID_DYNAMICS_APPLICATION=ON                                                                 \
-DMESHING_APPLICATION=ON                                                                        \
-DMULTISCALE_APPLICATION=OFF                                                                    \
-DPARTICLE_MECHANICS_APPLICATION=ON                                                             \
-DPFEM_APPLICATION=ON                                                                           \
-DCONTACT_MECHANICS_APPLICATION=ON                                                              \
-DPFEM_FLUID_DYNAMICS_APPLICATION=ON                                                            \
-DPFEM_SOLID_MECHANICS_APPLICATION=ON                                                           \
-DPOROMECHANICS_APPLICATION=ON                                                                  \
-DSHAPE_OPTIMIZATION_APPLICATION=ON                                                             \
-DSOLID_MECHANICS_APPLICATION=ON                                                                \
-DSTRUCTURAL_MECHANICS_APPLICATION=ON                                                           \
-DSWIMMING_DEM_APPLICATION=ON                                                                   \
-DTHERMO_MECHANICAL_APPLICATION=ON                                                              \
-DCONTACT_STRUCTURAL_MECHANICS_APPLICATION=ON                                                   \
-DMAPPING_APPLICATION=ON                                                                        \
-DMKL_SOLVERS_APPLICATION=OFF                                                                   \
-DMKLSOLVER_INCLUDE_DIR=\"UNSET\"                                                               \
-DMKLSOLVER_LIB_DIR=\"UNSET\"                                                                   \
-DMETIS_APPLICATION=OFF                                                                         \
-DPARMETIS_ROOT_DIR=\"UNSET\"                                                                   \
-DTRILINOS_APPLICATION=OFF                                                                      \
-DTRILINOS_ROOT=\"UNSET\"                                                                       \
-DINSTALL_EMBEDDED_PYTHON=ON                                                                    \
-DINCLUDE_FEAST=ON                                                                              \
