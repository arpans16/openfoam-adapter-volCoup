#include "AlphatAIT.H"
#include "primitivePatchInterpolation.H"

using namespace Foam;

preciceAdapter::AITS::Alphat::Alphat
(
    const Foam::fvMesh& mesh,
    const std::string nameAlphat
)
:
alphat_
(
    const_cast<volScalarField*>
    (
        &mesh.lookupObject<volScalarField>(nameAlphat)
    )
),
mesh_(mesh)
{
    dataType_ = scalar;
}

void preciceAdapter::AITS::Alphat::write(double * buffer, bool meshConnectivity, const unsigned int dim)
{
    int bufferIndex = 0;

    const volScalarField & Alphatptr_ = *alphat_;

    // For every cell of the mesh
    forAll(Alphatptr_, i)
    {
        // Copy the alphat into the buffer
        buffer[bufferIndex++] = Alphatptr_[i];
    }

    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

	const scalarField & AlphatPatch(alphat_->boundaryField()[patchID]);
        // For every cell of the patch
        //forAll(alphat_->boundaryFieldRef()[patchID], i)
        forAll(AlphatPatch, i)
        {
            // Copy the turb therm cond into the buffer
            buffer[bufferIndex++] = AlphatPatch[i];
        }
    }

    //to be added - if mesh connectivity is turned on then interpolations from centers to nodes
}

void preciceAdapter::AITS::Alphat::read(double * buffer, const unsigned int dim)
{
    int bufferIndex = 0;

    volScalarField &Alphatptr_ = *alphat_;

    // For every cell of the mesh
    forAll(Alphatptr_, i)
    {
        // Set the density as the buffer value
        Alphatptr_[i] = buffer[bufferIndex++];
    }
    *alphat_ = Alphatptr_;


    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

        // For every cell of the patch
        forAll(alphat_->boundaryFieldRef()[patchID], i)
        {
            // Set the turb therm cond as the buffer value
            alphat_->boundaryFieldRef()[patchID][i] = buffer[bufferIndex++];
        }
    }
}
