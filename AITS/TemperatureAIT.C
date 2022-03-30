#include "TemperatureAIT.H"
#include "primitivePatchInterpolation.H"

using namespace Foam;

preciceAdapter::AITS::Temperature::Temperature
(
    const Foam::fvMesh& mesh,
    const std::string nameT
    )
:
T_(
    const_cast<volScalarField*>
    (
        &mesh.lookupObject<volScalarField>(nameT)
    )
),
mesh_(mesh)
{
    dataType_ = scalar;
}

void preciceAdapter::AITS::Temperature::write(double * buffer, bool meshConnectivity, const unsigned int dim)
{
    int bufferIndex = 0;

    const volScalarField & Temprtr_ = *T_;

    // For every cell of the mesh
    forAll(Temprtr_, i)
    {
        // Copy the temperature into the buffer
        buffer[bufferIndex++] = Temprtr_[i];
    }

    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

	const scalarField & TPatch(T_->boundaryField()[patchID]);
        // For every cell of the patch
        //forAll(T_->boundaryFieldRef()[patchID], i)
        forAll(TPatch, i)
        {
            // Copy the temperature into the buffer
            buffer[bufferIndex++] = TPatch[i];
        }
    }

    //to be added - if mesh connectivity is turned on then interpolations from centers to nodes
}

void preciceAdapter::AITS::Temperature::read(double * buffer, const unsigned int dim)
{
    int bufferIndex = 0;

    volScalarField &Temprtr_ = *T_;

    // For every cell of the mesh
    forAll(Temprtr_, i)
    {
        // Set the temperature as the buffer value
        Temprtr_[i] = buffer[bufferIndex++];
    }
    *T_ = Temprtr_;


    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

        // For every cell of the patch
        forAll(T_->boundaryFieldRef()[patchID], i)
        {
            // Set the temperature as the buffer value
            T_->boundaryFieldRef()[patchID][i] = buffer[bufferIndex++];
        }
    }
}
