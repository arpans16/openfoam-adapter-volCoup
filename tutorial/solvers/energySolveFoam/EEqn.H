{
    volScalarField alphaEff("alphaEff", turbulence->alpha() + alphat);

    volScalarField& he = thermo.he();

    K = 0.5*magSqr(U);
    dpdt = Dpdt;

    fvScalarMatrix EEqn
    (
        fvm::ddt(rho, he) + fvm::div(phi, he)
      + fvc::ddt(rho, K) + fvc::div(phi, K)
      + (
            he.name() == "e"
          ? fvc::div
            (
                fvc::absolute(phi/fvc::interpolate(rho), U),
                p,
                "div(phiv,p)"
            )
          : -dpdt
        )
      - fvm::laplacian(alphaEff, he)
     ==
        rho*(U&g)
      + radiation->Sh(thermo, he)
      + fvOptions(rho, he)
    );

    EEqn.relax();

    fvOptions.constrain(EEqn);

    EEqn.solve();

    fvOptions.correct(he);

    thermo.correct();
    radiation->correct();

    Info<< "min/max T:" << min(thermo.T()).value() << ' ' << max(thermo.T()).value() << endl;
}
