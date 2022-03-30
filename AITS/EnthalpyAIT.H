#ifndef AITS_ENTHALPY_H
#define AITS_ENTHALPY_H

#include "CouplingDataUser.H"
#include "mixedFvPatchFields.H"
#include "fvCFD.H"

namespace preciceAdapter
{
namespace AITS
{

//- Class that writes and reads enthalpy.
class Enthalpy : public CouplingDataUser
{

private:

    //- Enthalpy field
    Foam::volScalarField * enthalpy_;
    const Foam::fvMesh& mesh_;

public:

    //- Constructor
    Enthalpy
    (
        const Foam::fvMesh& mesh,
        const std::string nameEnthalpy
    );

    //- Write the temperature values into the buffer
    void write(double * buffer, bool meshConnectivity, const unsigned int dim);

    //- Read the temperature values from the buffer
    void read(double * buffer, const unsigned int dim);

};

}
}

// NOTE: In order to couple another variable, you need to add another
// subclass of the CouplingDataUser. Take this file and Temperature.C
// as an example and look for more notes in the Adapter.C and Adapter.H.
// Essentially, the new class needs to implement a constructor (which also
// sets the dataType_) and implement the write() and read() methods.
// Then, you need to create objects of this class in the Adapter.C,
// whenever requested from the configuration file (see notes).

#endif
