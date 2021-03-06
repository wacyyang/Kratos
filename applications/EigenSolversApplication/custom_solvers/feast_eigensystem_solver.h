/*
//  KRATOS _______
//        / ____(_)___ ____  ____
//       / __/ / / __ `/ _ \/ __ \
//      / /___/ / /_/ /  __/ / / /
//     /_____/_/\__, /\___/_/ /_/ SolversApplication
//             /____/
//
//  Author:  Quirin Aumann
*/

#if !defined(KRATOS_FEAST_EIGENSYSTEM_SOLVER_H_INCLUDED)
#define KRATOS_FEAST_EIGENSYSTEM_SOLVER_H_INCLUDED

// External includes

// Project includes
#include "includes/define.h"
#include "includes/kratos_parameters.h"
#include "linear_solvers/linear_solver.h"
#include "includes/ublas_interface.h"
#include "includes/ublas_complex_interface.h"

extern "C" {
    #include <feast.h>
    #include <feast_sparse.h>
}

namespace Kratos {

namespace { // helpers namespace
    template<typename TScalar>
    struct SettingsHelper
    {
        SettingsHelper(Parameters SolverParams) : mParam(SolverParams) {};

        Parameters GetDefaultParameters();
        void CheckParameters();
        TScalar GetE1();
        double GetE2();
        private:
            Parameters mParam;
    };

    template<>
    Parameters SettingsHelper<double>::GetDefaultParameters()
    {
        return Parameters(R"({

        })");
    }

    template<>
    Parameters SettingsHelper<std::complex<double>>::GetDefaultParameters()
    {
        return Parameters(R"({

        })");
    }

    template<>
    void SettingsHelper<double>::CheckParameters()
    {
        KRATOS_ERROR_IF( mParam["search_lowest_eigenvalues"].GetBool() && mParam["search_highest_eigenvalues"].GetBool() ) <<
            "Cannot search for highest and lowest eigenvalues at the same time" << std::endl;

        KRATOS_ERROR_IF( mParam["e_max"].GetDouble() <= mParam["e_min"].GetDouble() ) <<
            "Invalid eigenvalue limits provided" << std::endl;

        KRATOS_INFO_IF( "FEASTEigensystemSolver",
            mParam["e_mid_re"].GetDouble() != 0.0 || mParam["e_mid_im"].GetDouble() != 0.0 || mParam["e_r"].GetDouble() != 0.0 ) <<
            "Manually defined e_mid_re, e_mid_im, e_r are not used for real symmetric matrices" << std::endl;
    }

    template<>
    void SettingsHelper<std::complex<double>>::CheckParameters()
    {
        KRATOS_ERROR_IF( mParam["e_r"].GetDouble() <= 0.0 ) <<
            "Invalid search radius provided" << std::endl;

        KRATOS_INFO_IF( "FEASTEigensystemSolver",
            mParam["e_min"].GetDouble() != 0.0 || mParam["e_max"].GetDouble() != 0.0 ) <<
            "Manually defined e_min, e_max are not used for complex symmetric matrices" << std::endl;

        KRATOS_INFO_IF( "FEASTEigensystemSolver",
            mParam["search_lowest_eigenvalues"].GetBool() || mParam["search_highest_eigenvalues"].GetBool() ) <<
            "Search for extremal eigenvalues is only available for Hermitian problems" << std::endl;
    }

    template<> double SettingsHelper<double>::GetE1() {return mParam["e_min"].GetDouble();}
    template<> std::complex<double> SettingsHelper<std::complex<double>>::GetE1() {return std::complex<double>(mParam["e_mid_re"].GetDouble(), mParam["e_mid_im"].GetDouble());}

    template<>double SettingsHelper<double>::GetE2() {return mParam["e_max"].GetDouble();}
    template<> double SettingsHelper<std::complex<double>>::GetE2() {return mParam["e_r"].GetDouble();}
}

template<
    bool TSymmetric,
    typename TScalarIn,
    typename TScalarOut,
    class TSparseSpaceTypeIn = TUblasSparseSpace<TScalarIn>,
    class TDenseSpaceTypeIn = TUblasDenseSpace<TScalarIn>,
    class TSparseSpaceTypeOut = TUblasSparseSpace<TScalarOut>,
    class TDenseSpaceTypeOut = TUblasDenseSpace<TScalarOut>>
class FEASTEigensystemSolver
    : public LinearSolver<TSparseSpaceTypeIn, TDenseSpaceTypeOut>
{
    Parameters mParam;

  public:
    KRATOS_CLASS_POINTER_DEFINITION(FEASTEigensystemSolver);

    typedef LinearSolver<TSparseSpaceTypeIn, TDenseSpaceTypeIn> BaseType;

    typedef typename TSparseSpaceTypeIn::MatrixType SparseMatrixType;

    typedef typename TDenseSpaceTypeOut::VectorType DenseVectorType;

    typedef typename TDenseSpaceTypeOut::MatrixType DenseMatrixType;

    typedef matrix<TScalarOut, column_major> FEASTMatrixType;

    typedef TScalarIn ValueTypeIn;

    typedef TScalarOut ValueTypeOut;

    FEASTEigensystemSolver(
        Parameters param
    ) : mParam(param)
    {
        Parameters default_params(R"(
        {
            "solver_type" : "feast",
            "symmetric" : true,
            "number_of_eigenvalues" : 0,
            "search_lowest_eigenvalues" : false,
            "search_highest_eigenvalues" : false,
            "e_min" : 0.0,
            "e_max" : 0.0,
            "e_mid_re" : 0.0,
            "e_mid_im" : 0.0,
            "e_r" : 0.0,
            "subspace_size" : 0,
            "max_iteration" : 20,
            "tolerance" : 1e-12,
            "echo_level" : 0
        })");

        default_params.AddMissingParameters(SettingsHelper<TScalarOut>(mParam).GetDefaultParameters());

        mParam.ValidateAndAssignDefaults(default_params);

        KRATOS_ERROR_IF( mParam["number_of_eigenvalues"].GetInt() < 0 ) <<
            "Invalid number of eigenvalues provided" << std::endl;

        KRATOS_ERROR_IF( mParam["subspace_size"].GetInt() < 0 ) <<
            "Invalid subspace size provided" << std::endl;

        KRATOS_ERROR_IF( mParam["max_iteration"].GetInt() < 1 ) <<
            "Invalid maximal number of iterations provided" << std::endl;

        KRATOS_ERROR_IF( (mParam["search_lowest_eigenvalues"].GetBool() || mParam["search_highest_eigenvalues"].GetBool())
            &&  mParam["number_of_eigenvalues"].GetInt() == 0 ) << "Please specify the number of eigenvalues to be found" << std::endl;

        KRATOS_ERROR_IF( mParam["subspace_size"].GetInt() == 0 && mParam["number_of_eigenvalues"].GetInt() == 0 ) <<
            "Please specify either \"subspace_size\" or \"number_of_eigenvalues\"" << std::endl;

        KRATOS_INFO_IF( "FEASTEigensystemSolver",
            mParam["number_of_eigenvalues"].GetInt() > 0  && mParam["subspace_size"].GetInt() > 0 ) <<
            "Manually defined subspace size will be overwritten to match the defined number of eigenvalues" << std::endl;

        SettingsHelper<TScalarOut>(mParam).CheckParameters();
    }

    ~FEASTEigensystemSolver() override = default;

    /**
     * Solve the generalized eigenvalue problem using FEAST
     * @param rK first input matrix
     * @param rM second input matrix
     * @param rEigenvalues eigenvalues
     * @param rEigenvectors row-aligned eigenvectors [n_evs,n_dofs]
     */
    void Solve(
        SparseMatrixType& rK,
        SparseMatrixType& rM,
        DenseVectorType& rEigenvalues,
        DenseMatrixType& rEigenvectors) override
    {
        // settings
        const std::size_t system_size = rK.size1();
        size_t subspace_size;

        if( mParam["search_lowest_eigenvalues"].GetBool() || mParam["search_highest_eigenvalues"].GetBool() ) {
            subspace_size = 2 * static_cast<size_t>(mParam["number_of_eigenvalues"].GetInt());
        } else if( mParam["subspace_size"].GetInt() == 0 ) {
            subspace_size = 1.5 * static_cast<size_t>(mParam["number_of_eigenvalues"].GetInt());
        } else {
            subspace_size = static_cast<size_t>(mParam["subspace_size"].GetInt());
        }

        // create column based matrix for the fortran routine
        FEASTMatrixType tmp_eigenvectors(system_size, subspace_size);
        DenseVectorType tmp_eigenvalues(subspace_size);
        DenseVectorType residual(subspace_size);

        // set FEAST settings
        int fpm[64] = {};
        feastinit(fpm);

        mParam["echo_level"].GetInt() > 0 ? fpm[0] = 1 : fpm[0] = 0;

        fpm[2] = -std::log10(mParam["tolerance"].GetDouble());
        fpm[3] = mParam["max_iteration"].GetInt();

        // compute only right eigenvectors
        if( !TSymmetric ) {
            fpm[14] = 1;
        }

        if( mParam["search_lowest_eigenvalues"].GetBool() ) {
            fpm[39] = -1;
        }
        if( mParam["search_highest_eigenvalues"].GetBool() ) {
            fpm[39] = 1;
        }

        char UPLO = 'F';
        int N = static_cast<int>(system_size);

        // provide matrices in array form. fortran indices start with 1, must be int
        double* A = reinterpret_cast<double*>(rK.value_data().begin());

        std::vector<int> IA(N+1);
        CreateFortranIndices(rK.index1_data(), IA);

        std::vector<int> JA(IA[N]-1);
        CreateFortranIndices(rK.index2_data(), JA);

        double* B = reinterpret_cast<double*>(rM.value_data().begin());

        std::vector<int> IB(N+1);
        CreateFortranIndices(rM.index1_data(), IB);

        std::vector<int> JB(IB[N]-1);
        CreateFortranIndices(rM.index2_data(), JB);

        double epsout;
        int loop;
        TScalarOut E1 = SettingsHelper<TScalarOut>(mParam).GetE1();
        double E2 = SettingsHelper<TScalarOut>(mParam).GetE2();
        double* Emin = reinterpret_cast<double*>(&E1);
        double* Emax = reinterpret_cast<double*>(&E2);
        int M0 = static_cast<int>(subspace_size);
        double* E = reinterpret_cast<double*>(tmp_eigenvalues.data().begin());
        double* X = reinterpret_cast<double*>(tmp_eigenvectors.data().begin());
        int M;
        double* res = reinterpret_cast<double*>(residual.data().begin());
        int info;

        // call feast
        auto feast = CreateFeast<TScalarIn>(TSymmetric);
        feast(&UPLO, &N, A, IA.data(), JA.data(), B, IB.data(), JB.data(), fpm, &epsout, &loop, Emin, Emax, &M0, E, X, &M, res, &info);

        KRATOS_ERROR_IF(info < 0 || info > 99) << "FEAST encounterd error " << info << ". Please check FEAST output." << std::endl;
        KRATOS_INFO_IF("FeastEigensystemSolver", info > 1 && info < 6) << "FEAST finished with warning " << info << ". Please check FEAST output." << std::endl;
        KRATOS_ERROR_IF(info == 1) << "FEAST finished with warning " << info << ", no eigenvalues could be found in the given search interval." << std::endl;
        KRATOS_ERROR_IF(info == 7) << "FEAST finished with warning " << info << ", no extremal eigenvalues could be found. Please check FEAST output." << std::endl;

        // truncate non-converged results
        tmp_eigenvalues.resize(M, true);
        tmp_eigenvectors.resize(system_size, M, true);

        // copy eigenvalues to result vector
        rEigenvalues.swap(tmp_eigenvalues);

        // copy eigenvectors to result matrix
        if( rEigenvectors.size1() != tmp_eigenvectors.size1() || rEigenvectors.size2() != tmp_eigenvectors.size2() )
            rEigenvectors.resize(tmp_eigenvectors.size1(), tmp_eigenvectors.size2(), false);

        noalias(rEigenvectors) = tmp_eigenvectors;

        // the eigensolver strategy expects an eigenvector matrix of shape [n_eigenvalues, n_dofs], so FEAST's eigenvector matrix has to be transposed
        rEigenvectors = trans(rEigenvectors);
    }
    /**
     * Print information about this object.
     */
    void PrintInfo(std::ostream &rOStream) const override
    {
        rOStream << "FEASTEigensystemSolver";
    }

    /**
     * Print object's data.
     */
    void PrintData(std::ostream &rOStream) const override
    {
    }

  private:

    typedef void (feast_ptr)(char*, int*, double*, int*, int*, double*, int*, int*, int*, double*, int*, double*, double*, int*, double*, double*, int*, double*, int*);

    /**
     * The FEAST functions for symmetric and unsymmetric eigenvalue problems do not have the same signature;
     * for the symmetric case, the first parameter is a char, the rest are the same for the symmetric and general case.
     *
     * Here we define a function pointer with the signature of the symmetric FEAST function (which is longer) and bind
     * the functions providing all 19 arguments for the symmetric case (dfeast_scsrgv and zfeast_scsrgv) while only the
     * last 18 arguments for the general case.

     * With these placeholders _1, ..., _N we could change the order of the provided arguments in the call of the function
     * pointer. Here we omit the first parameters.
     *
     * @see https://en.cppreference.com/w/cpp/utility/functional/bind
     */
    template<typename TScalar, typename std::enable_if<std::is_same<double, TScalar>::value, int>::type = 0>
    std::function<feast_ptr> CreateFeast(bool symmetric)
    {
        using namespace std::placeholders;

        if( symmetric ) {
            return std::bind(dfeast_scsrgv, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19);
        } else {
            return std::bind(dfeast_gcsrgv, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19);
        }
    }

    template<typename TScalar, typename std::enable_if<std::is_same<std::complex<double>, TScalar>::value, int>::type = 0>
    std::function<feast_ptr> CreateFeast(bool symmetric)
    {
        using namespace std::placeholders;

        if( symmetric ) {
            return std::bind(zfeast_scsrgv, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19);
        } else {
            return std::bind(zfeast_gcsrgv, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19);
        }
    }

    template<typename IndexDataType>
    void CreateFortranIndices(const IndexDataType& rIndexData, std::vector<int>& rFortranIndices)
    {
        #pragma omp parallel for
        for( int i=0; i<static_cast<int>(rFortranIndices.size()); ++i ) {
            rFortranIndices[i] = static_cast<int>(rIndexData[i]) + 1;
        }
    }

}; // class FEASTEigensystemSolver


/**
 * input stream function
 */
template<bool TSymmetric, typename TScalarIn, typename TScalarOut>
inline std::istream& operator >>(
    std::istream& rIStream,
    FEASTEigensystemSolver<TSymmetric, TScalarIn, TScalarOut>& rThis)
{
    return rIStream;
}

/**
 * output stream function
 */
template<bool TSymmetric, typename TScalarIn, typename TScalarOut>
inline std::ostream& operator <<(
    std::ostream& rOStream,
    const FEASTEigensystemSolver<TSymmetric, TScalarIn, TScalarOut>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}

} // namespace Kratos

#endif // defined(KRATOS_FEAST_EIGENSYSTEM_SOLVER_H_INCLUDED)
