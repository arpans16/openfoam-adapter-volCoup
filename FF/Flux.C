#include "Flux.H"
#include "primitivePatchInterpolation.H"

using namespace Foam;

preciceAdapter::FF::Flux::Flux
(
    const Foam::fvMesh& mesh,
    const std::string namephi
    )
:
phi_(
    const_cast<surfaceScalarField*>
    (
        &mesh.lookupObject<surfaceScalarField>(namephi)
    )
),
mesh_(mesh)
{
    dataType_ = scalar;
}

void preciceAdapter::FF::Flux::write(double * buffer, bool meshConnectivity, const unsigned int dim)
{
    int bufferIndex = 0;

    const surfaceScalarField & phiptr_ = *phi_;

    // For every cell of the mesh
    forAll(phiptr_, i)
    {
        // Copy the temperature into the buffer
        buffer[bufferIndex++] = phiptr_[i];
    }

    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

	const scalarField & phiPatch(phi_->boundaryField()[patchID]);
        // For every cell of the patch
        //forAll(phi_->boundaryFieldRef()[patchID], i)
        forAll(phiPatch, i)
        {
            // Copy the temperature into the buffer
            buffer[bufferIndex++] = phiPatch[i]; //phi_->boundaryFieldRef()[patchID][i];
        }
    }

    //to be added - if mesh connectivity is turned on then interpolations from centers to nodes
}

void preciceAdapter::FF::Flux::read(double * buffer, const unsigned int dim)
{
    int bufferIndex = 0;

    surfaceScalarField &phiptr_ = *phi_;

    // For every cell of the mesh
    forAll(phiptr_, i)
    {
        // Set the phi as the buffer value
        phiptr_[i] = buffer[bufferIndex++];
    }
    *phi_ = phiptr_;


    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

        // For every cell of the patch
        forAll(phi_->boundaryFieldRef()[patchID], i)
        {
            // Set the phi as the buffer value
            phi_->boundaryFieldRef()[patchID][i] = buffer[bufferIndex++];
        }
    }
}
