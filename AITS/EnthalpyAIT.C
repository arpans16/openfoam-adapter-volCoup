#include "EnthalpyAIT.H"
#include "primitivePatchInterpolation.H"

using namespace Foam;

preciceAdapter::AITS::Enthalpy::Enthalpy
    (
    const Foam::fvMesh& mesh,
    const std::string nameEnthalpy
    )
:
enthalpy_(
    const_cast<volScalarField*>
    (
        &mesh.lookupObject<volScalarField>(nameEnthalpy)
    )
),
mesh_(mesh)
{
    dataType_ = scalar;
}

void preciceAdapter::AITS::Enthalpy::write(double * buffer, bool meshConnectivity, const unsigned int dim)
{
    int bufferIndex = 0;

    const volScalarField & Enthalpyptr_ = *enthalpy_;

    // For every cell of the mesh
    forAll(Enthalpyptr_, i)
    {
        // Copy the enthalpy into the buffer
        buffer[bufferIndex++] = Enthalpyptr_[i];
    }

    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

	const scalarField & EnthalpyPatch(enthalpy_->boundaryField()[patchID]);
        // For every cell of the patch
        //forAll(enthalpy_->boundaryFieldRef()[patchID], i)
        forAll(EnthalpyPatch, i)
        {
            // Copy the density into the buffer
            buffer[bufferIndex++] = EnthalpyPatch[i];
        }
    }

    //to be added - if mesh connectivity is turned on then interpolations from centers to nodes
}

void preciceAdapter::AITS::Enthalpy::read(double * buffer, const unsigned int dim)
{
    int bufferIndex = 0;

    volScalarField &Enthalpyptr_ = *enthalpy_;

    // For every cell of the mesh
    forAll(Enthalpyptr_, i)
    {
        // Set the enthalpy as the buffer value
        Enthalpyptr_[i] = buffer[bufferIndex++];
    }
    *enthalpy_ = Enthalpyptr_;


    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

        // For every cell of the patch
        forAll(enthalpy_->boundaryFieldRef()[patchID], i)
        {
            // Set the enthalpy as the buffer value
            enthalpy_->boundaryFieldRef()[patchID][i] = buffer[bufferIndex++];
        }
    }
}
