#!/bin/bash
cd ${0%/*} || exit 1    		    		# Run from this directory
. $WM_PROJECT_DIR/bin/tools/RunFunctions    		# Tutorial run functions

# Fluid participant
# Run this script in one terminal and the "runSolid" script in another terminal.
# These scripts present how the two participants would be started manually.
# Alternatively, you may execute the "Allrun" script in one terminal.
# Run this script with "-parallel" for parallel simulations
# The script "Allclean" cleans-up the result and log files.
# 1 for true, 0 for false

#rm -rfv $FOAM_USER_APPBIN/energySolveFoam
wmake ../../solvers/energySolveFoam/

echo "Preparing and running the energySolve participant..."
rm -rfv 0/
cp -r 0.orig/ 0/
blockMesh
checkMesh
touch fluid-openfoam.foam
solver=$(getApplication)

#procs=$(getNumberOfProcessors)
#decomposePar -force
#mpirun -np $procs $solver -parallel
#reconstructPar -latestTime

$solver

. ../../tools/openfoam-remove-empty-dirs.sh && openfoam_remove_empty_dirs
