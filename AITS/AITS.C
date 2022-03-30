#include "AITS.H"

#include "Utilities.H"

using namespace Foam;

preciceAdapter::AITS::AITSolver::AITSolver
(
    const Foam::fvMesh& mesh
)
:
mesh_(mesh)
{}

bool preciceAdapter::AITS::AITSolver::configure(const IOdictionary& adapterConfig)
{
    DEBUG(adapterInfo("Configuring the AITS module..."));

    // Read the AITS-specific options from the adapter's configuration file
    if (!readConfig(adapterConfig)) return false;

    // NOTE: If you want to add a new solver type, which you can manually
    // specify in the configuration, add it here. See also the methods
    // addWriters() and addReaders().
    // Check the solver type and determine it if needed
    cout << "SolverType: |" << solverType_ << "|" << endl;
    if (
        solverType_.compare("compressible") == 0 ||
        solverType_.compare("incompressible") == 0 ||
        solverType_.compare("basic") == 0
    )
    {
        DEBUG(adapterInfo("Known solver type: " + solverType_));
    }
    else if (solverType_.compare("none") == 0)
    {
        DEBUG(adapterInfo("Determining the solver type..."));
        solverType_ = determineSolverType();
    }
    else
    {
        DEBUG(adapterInfo("Unknown solver type. Determining the solver type..."));
        solverType_ = determineSolverType();
    }

    return true;
}

bool preciceAdapter::AITS::AITSolver::readConfig(const IOdictionary& adapterConfig)
{

    const dictionary AITSdict = adapterConfig.subOrEmptyDict("AITS");

    // Read the solver type (if not specified, it is determined automatically)
    solverType_ = AITSdict.lookupOrDefault<word>("solverType", "");
    DEBUG(adapterInfo("    user-defined solver type : " + solverType_));

    // Read the name of the temperature field (if different)
    nameT_ = AITSdict.lookupOrDefault<word>("nameT", "T");
    DEBUG(adapterInfo("    temperature field name : " + nameT_));

    // Read the name of the velocity field (if different)
    nameU_ = AITSdict.lookupOrDefault<word>("nameU", "U");
    DEBUG(adapterInfo("    velocity field name : " + nameU_));

    // Read the name of the phi field (if different)
    namephi_ = AITSdict.lookupOrDefault<word>("namephi", "phi");
    DEBUG(adapterInfo("    phi field name : " + namephi_));

    // Read the name of the density field (if different)
    nameRho_ = AITSdict.lookupOrDefault<word>("nameRho", "rho");
    DEBUG(adapterInfo("    rho field name : " + nameRho_));

    // Read the name of the turb therm cond field (if different)
    nameAlphat_ = AITSdict.lookupOrDefault<word>("nameAlphat", "alphat");
    DEBUG(adapterInfo("    alphat field name : " + nameAlphat_));

    // Read the name of the dpdt field (if different)
    nameDpdt_ = AITSdict.lookupOrDefault<word>("nameDpdt", "Dpdt");
    DEBUG(adapterInfo("    Dpdt field name : " + nameDpdt_));

    // Read the name of the enthalpy field (if different)
    nameEnthalpy_ = AITSdict.lookupOrDefault<word>("nameEnthalpy", "enthalpy");
    DEBUG(adapterInfo("    enthalpy field name : " + nameEnthalpy_));
 
    return true;
}

std::string preciceAdapter::AITS::AITSolver::determineSolverType()
{
    // NOTE: When coupling a different variable, you may want to
    // add more cases here. Or you may provide the solverType in the config.

    std::string solverType = "unknown";

    // Determine the solver type: Compressible, Incompressible or Basic - add pressure dimensions

    solverType = "compressible";

    return solverType;
}

void preciceAdapter::AITS::AITSolver::addWriters(std::string dataName, Interface * interface)
{
    if (dataName.find("Temperature") == 0)
    {
        interface->addCouplingDataWriter
        (
            dataName,
            new Temperature(mesh_, nameT_)
        );
        DEBUG(adapterInfo("Added writer: Temperature."));
    }
    else if (dataName.find("Velocity") == 0)
    {
        interface->addCouplingDataWriter
        (
            dataName,
            new Velocity(mesh_, nameU_)
        );
        DEBUG(adapterInfo("Added writer: Velocity."));
    }
    else if (dataName.find("Flux") == 0)
    {
        interface->addCouplingDataWriter
        (
            dataName,
            new Flux(mesh_, namephi_)
        );
        DEBUG(adapterInfo("Added writer: Flux."));
    }
    else if (dataName.find("Rho") == 0)
    {
        interface->addCouplingDataWriter
        (
            dataName,
            new Rho(mesh_, nameRho_)
        );
        DEBUG(adapterInfo("Added writer: Rho."));
    }
    else if (dataName.find("Alphat") == 0)
    {
	interface->addCouplingDataWriter
	(
	    dataName,
	    new Alphat(mesh_, nameAlphat_)
	);
	DEBUG(adapterInfo("Added writer: Alphat."));
    }
    else if (dataName.find("Dpdt") == 0)
    {
        interface->addCouplingDataWriter
        (
            dataName,
            new Dpdt(mesh_, nameDpdt_)
        );
        DEBUG(adapterInfo("Added writer: Dpdt."));
    }
    else if (dataName.find("Enthalpy") == 0)
    {
        interface->addCouplingDataWriter
        (
            dataName,
            new Enthalpy(mesh_, nameEnthalpy_)
        );
        DEBUG(adapterInfo("Added writer: Enthalpy."));
    }
    else
    {
        adapterInfo("Unknown data type - cannot add " + dataName +".", "error");
    }

    // NOTE: If you want to couple another variable, you need
    // to add your new coupling data user as a coupling data
    // writer here (and as a reader below).
    // The argument of the dataName.compare() needs to match
    // the one provided in the adapter's configuration file.
}

void preciceAdapter::AITS::AITSolver::addReaders(std::string dataName, Interface * interface)
{

    if (dataName.find("Temperature") == 0)
    {
        interface->addCouplingDataReader
        (
            dataName,
            new Temperature(mesh_, nameT_)
        );
        DEBUG(adapterInfo("Added reader: Temperature."));
    }
    else if (dataName.find("Velocity") == 0)
    {
        interface->addCouplingDataReader
        (
            dataName,
            new Velocity(mesh_, nameU_)
        );
        DEBUG(adapterInfo("Added reader: Velocity."));
    }
    else if (dataName.find("Flux") == 0)
    {
        interface->addCouplingDataReader
        (
            dataName,
            new Flux(mesh_, namephi_)
        );
        DEBUG(adapterInfo("Added reader: Flux."));
    }
    else if (dataName.find("Rho") == 0)
    {
        interface->addCouplingDataReader
        (
            dataName,
            new Rho(mesh_, nameRho_)
        );
        DEBUG(adapterInfo("Added reader: Rho."));
    }
    else if (dataName.find("Alphat") == 0)
    {
	interface->addCouplingDataReader
	(
	    dataName,
	    new Alphat(mesh_, nameAlphat_)
	);
	DEBUG(adapterInfo("Added reader: Alphat."));
    }
    else if (dataName.find("Dpdt") == 0)
    {
        interface->addCouplingDataReader
        (
            dataName,
            new Dpdt(mesh_, nameDpdt_)
        );
        DEBUG(adapterInfo("Added reader: Dpdt."));
    }
    else if (dataName.find("Enthalpy") == 0)
    {
        interface->addCouplingDataReader
        (
            dataName,
            new Enthalpy(mesh_, nameEnthalpy_)
        );
        DEBUG(adapterInfo("Added reader: Enthalpy."));
    }
    else
    {
        adapterInfo("Unknown data type - cannot add " + dataName +".", "error");
    }

    // NOTE: If you want to couple another variable, you need
    // to add your new coupling data user as a coupling data
    // reader here (and as a writer above).
    // The argument of the dataName.compare() needs to match
    // the one provided in the adapter's configuration file.
}
