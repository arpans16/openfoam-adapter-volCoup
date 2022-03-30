#include "Density.H"
#include "primitivePatchInterpolation.H"

using namespace Foam;

preciceAdapter::FF::Density::Density
(
    const Foam::fvMesh& mesh,
    const std::string nameRho
    )
:
rho_(
    const_cast<volScalarField*>
    (
        &mesh.lookupObject<volScalarField>(nameRho)
    )
),
mesh_(mesh)
{
    dataType_ = scalar;
}

void preciceAdapter::FF::Density::write(double * buffer, bool meshConnectivity, const unsigned int dim)
{
    int bufferIndex = 0;

    const volScalarField & Rhoptr_ = *rho_;

    // For every cell of the mesh
    forAll(Rhoptr_, i)
    {
        // Copy the rho into the buffer
        buffer[bufferIndex++] = Rhoptr_[i];
    }

    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

	const scalarField & RhoPatch(rho_->boundaryField()[patchID]);
        // For every cell of the patch
        //forAll(rho_->boundaryFieldRef()[patchID], i)
        forAll(RhoPatch, i)
        {
            // Copy the density into the buffer
            buffer[bufferIndex++] = RhoPatch[i];
        }
    }

    //to be added - if mesh connectivity is turned on then interpolations from centers to nodes
}

void preciceAdapter::FF::Density::read(double * buffer, const unsigned int dim)
{
    int bufferIndex = 0;

    volScalarField &Rhoptr_ = *rho_;

    // For every cell of the mesh
    forAll(Rhoptr_, i)
    {
        // Set the density as the buffer value
        Rhoptr_[i] = buffer[bufferIndex++];
    }
    *rho_ = Rhoptr_;


    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

        // For every cell of the patch
        forAll(rho_->boundaryFieldRef()[patchID], i)
        {
            // Set the density as the buffer value
            rho_->boundaryFieldRef()[patchID][i] = buffer[bufferIndex++];
        }
    }
}
