#include "DpdtAIT.H"
#include "primitivePatchInterpolation.H"

using namespace Foam;

preciceAdapter::AITS::Dpdt::Dpdt
(
    const Foam::fvMesh& mesh,
    const std::string nameDpdt
    )
:
Dpdt_(
    const_cast<volScalarField*>
    (
        &mesh.lookupObject<volScalarField>(nameDpdt)
    )
),
mesh_(mesh)
{
    dataType_ = scalar;
}

void preciceAdapter::AITS::Dpdt::write(double * buffer, bool meshConnectivity, const unsigned int dim)
{
    int bufferIndex = 0;

    const volScalarField & Dpdtptr_ = *Dpdt_;

    // For every cell of the mesh
    forAll(Dpdtptr_, i)
    {
        // Copy the rho into the buffer
        buffer[bufferIndex++] = Dpdtptr_[i];
    }

    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

	const scalarField & DpdtPatch(Dpdt_->boundaryField()[patchID]);
        // For every cell of the patch
        //forAll(rho_->boundaryFieldRef()[patchID], i)
        forAll(DpdtPatch, i)
        {
            // Copy the density into the buffer
            buffer[bufferIndex++] = DpdtPatch[i];
        }
    }

    //to be added - if mesh connectivity is turned on then interpolations from centers to nodes
}

void preciceAdapter::AITS::Dpdt::read(double * buffer, const unsigned int dim)
{
    int bufferIndex = 0;

    volScalarField &Dpdtptr_ = *Dpdt_;

    // For every cell of the mesh
    forAll(Dpdtptr_, i)
    {
        // Set the density as the buffer value
        Dpdtptr_[i] = buffer[bufferIndex++];
    }
    *Dpdt_ = Dpdtptr_;


    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

        // For every cell of the patch
        forAll(Dpdt_->boundaryFieldRef()[patchID], i)
        {
            // Set the density as the buffer value
            Dpdt_->boundaryFieldRef()[patchID][i] = buffer[bufferIndex++];
        }
    }
}
