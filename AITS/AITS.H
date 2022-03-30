#ifndef AITS_H
#define AITS_H

#include "Interface.H"

#include "AITS/TemperatureAIT.H"
#include "AITS/VelocityAIT.H"
#include "AITS/FluxAIT.H"
#include "AITS/RhoAIT.H"
#include "AITS/AlphatAIT.H"
#include "AITS/DpdtAIT.H"
#include "AITS/EnthalpyAIT.H"
#include "fvCFD.H"

namespace preciceAdapter
{
namespace AITS
{

class AITSolver
{

protected:

    //- OpenFOAM fvMesh object
    const Foam::fvMesh& mesh_;

    // TODO: Create a struct for all the parameter names

    //- Solver type
    std::string solverType_ = "none";

    //- Name of the temperature field
    std::string nameT_ = "T";

    //- Name of the velocity field
    std::string nameU_ = "U";

    //- Name of the phi field
    std::string namephi_ = "phi";

    //- Name of the density field
    std::string nameRho_ = "rho";

    //- Name of the turb therm cond field
    std::string nameAlphat_ = "alphat";

    //- Name of the Dpdt field
    std::string nameDpdt_ = "Dpdt";

    //- Name of the enthalpy field
    std::string nameEnthalpy_ = "enthalpy";
    
protected:

    //- Determine the solver type
    std::string determineSolverType();

    //- Read the AITS-related options from the adapter's configuraiton file
    bool readConfig(const IOdictionary& adapterConfig);

public:

    //- Constructor
    AITSolver(const Foam::fvMesh& mesh);

    //- Configure
    bool configure(const IOdictionary& adapterConfig);

    //- Add coupling data writers
    void addWriters(std::string dataName, Interface * interface);

    //- Add coupling data readers
    void addReaders(std::string dataName, Interface * interface);
};

}
}

#endif
