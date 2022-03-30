
title: volCoup-oneWay
keywords: tutorial, OpenFOAM, volumetric coupling
summary: This tutorial describes how to run a test case for volumetric coupling using preCICE. The preCICE test case for flow-over-heated-plate is used after throwing away the Solid part of the domain. This only serves as a verification case for the preCICE volumetric coupling.
The details of the setup are exactly the same as the fluid part of flow-over-heated-plate. There are three folders in the directory - flowSolve, energySolve, noCoupling. The first two are used in the precice coupling while the third uses a single solver (buoyantPimpleFoam) to simulate the fluid part of flow-over-heated-plate (The plate is kept at 310 K while the inflow is at 300 K) with Navier Stokes and the Energy Equation solved together. In the precice coupling, the first solver (flowSolve) solves the Navier Stokes equations to compute the velocity profile and mass fluxes which are written to the second solver (energySolve) using the precice libraries. The second solver solves the energy equation using these inputs and calculates the Temperature field. For volumetric coupling verification - the Velocity field from flow Solve and the Temperature field from energySolve could be compared with the results from the noCoupling case.

## Setup

This scenario consists of two fluid participants - both solvers are OpenFOAM solvers and derived from buoyantPimpleFoam. The first solver (flowSolve) only solves for velocity and pressure while the second solver (energySolve) solves for energy and calculates temperature. These two solvers work on the same domain discretized with the same mesh. The OF solvers derived from buoyantPimpleFoam, namely flowSolveFoam and energySolveFoam are placed in the solvers folder (openfoam-adapter/tutorial/solvers). These are built to $FOAM_USER_APPBIN in the run scripts of flowSolve and energySolve respectively.

The setup also contains a folder called noCoupling which runs the buoyantPimpleFoam solver itself on the same mesh. The result from this solver can be used to verify the coupled solution obtained by volumetric coupling using preCICE.

