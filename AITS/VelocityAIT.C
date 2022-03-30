#include "VelocityAIT.H"
#include "primitivePatchInterpolation.H"

using namespace Foam;

preciceAdapter::AITS::Velocity::Velocity
(
    const Foam::fvMesh& mesh,
    const std::string nameU
    )
:
U_(
    const_cast<volVectorField*>
    (
        &mesh.lookupObject<volVectorField>(nameU)
    )
),
mesh_(mesh)
{
    dataType_ = vector;
}

void preciceAdapter::AITS::Velocity::write(double * buffer, bool meshConnectivity, const unsigned int dim)
{
    int bufferIndex = 0;

    const volVectorField & Velptr_ = *U_;

    // For every cell of the mesh
    forAll(Velptr_, i)
    {
        if (i == 0)
            adapterInfo("Write: first value of U at bufferIndex: " + std::to_string(bufferIndex) + " is " + std::to_string(buffer[bufferIndex]));  

        // Copy the velocity components into the buffer
        buffer[bufferIndex++] = Velptr_[i].x();
	buffer[bufferIndex++] = Velptr_[i].y();
	if (dim == 3)
	{
	    buffer[bufferIndex++] = Velptr_[i].z();
	}
    }

    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

	const vectorField & UPatch(U_->boundaryField()[patchID]);
        // For every cell of the patch
        //forAll(U_->boundaryFieldRef()[patchID], i)
        forAll(UPatch, k)
        {
            // Copy the velocity components into the buffer
            buffer[bufferIndex++] = UPatch[k].x(); //U_->boundaryFieldRef()[patchID][i].x();
	    buffer[bufferIndex++] = UPatch[k].y(); //U_->boundaryFieldRef()[patchID][i].y();
	    if (dim == 3)
	    {
	    	buffer[bufferIndex++] = UPatch[k].z(); //U_->boundaryFieldRef()[patchID][i].z();
	    }
        }
    }

    //to be added - if mesh connectivity is turned on then interpolations from centers to nodes
}

void preciceAdapter::AITS::Velocity::read(double * buffer, const unsigned int dim)
{
    int bufferIndex = 0;

    volVectorField &Velptr_ = *U_;

    // For every cell of the mesh
    forAll(Velptr_, i)
    {
        if (i == 0)
            adapterInfo("Read: first value of U at bufferIndex: " + std::to_string(bufferIndex) + " is " + std::to_string(buffer[bufferIndex]));

        // Set the velocity components as the buffer value
        Velptr_[i].x() = buffer[bufferIndex++];
	Velptr_[i].y() = buffer[bufferIndex++];
	if (dim == 3)
	{
	    Velptr_[i].z() = buffer[bufferIndex++];
	}
    }
    *U_ = Velptr_;


    for (uint j = 0; j < patchIDs_.size(); j++ )
    {
        int patchID = patchIDs_.at(j);

	//const vectorField & UPatch(U_->boundaryField()[patchID]);
        // For every cell of the patch
        forAll(U_->boundaryFieldRef()[patchID], k)
        //forAll(UPatch, k)
        {
            // Set the temperature as the buffer value
            U_->boundaryFieldRef()[patchID][k].x() = buffer[bufferIndex++];
	    U_->boundaryFieldRef()[patchID][k].y() = buffer[bufferIndex++];
	    if (dim == 3)
	    {
	    	U_->boundaryFieldRef()[patchID][k].z() = buffer[bufferIndex++];
	    }
        }
    }
}
