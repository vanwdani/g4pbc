#pragma once

#include "G4Step.hh"
#include "G4DynamicParticle.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalPhoton.hh"
#include "G4ParticleChangeForPeriodic.hh"
#include "G4TransportationManager.hh"
#include "G4VDiscreteProcess.hh"

#include "globals.hh"

enum G4PeriodicBoundaryProcessStatus {
  Undefined,
  Reflection,
  Cycling,
  StepTooSmall,
  NotAtBoundary
 };

class G4PeriodicBoundaryProcess : public G4VDiscreteProcess {

public:

  G4PeriodicBoundaryProcess(const G4String& processName = "CycBoundary",
    G4ProcessType type = fNotDefined, bool per_x = true,
    bool per_y = true, bool per_z = false, bool ref_walls = false);
  ~G4PeriodicBoundaryProcess();

private:

  G4PeriodicBoundaryProcess(const G4PeriodicBoundaryProcess &right);

  G4PeriodicBoundaryProcess& operator=(const G4PeriodicBoundaryProcess &right);

public:

  G4bool IsApplicable(const G4ParticleDefinition& );

  G4double GetMeanFreePath(const G4Track& ,
                           G4double ,
                           G4ForceCondition* condition);
  // Returns infinity; i. e. the process does not limit the step,
  // but sets the 'Forced' condition for the DoIt to be invoked at
  // every step. However, only at a boundary will any action be
  // taken.

  G4PeriodicBoundaryProcessStatus GetStatus() const;

  G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

protected:

  G4ParticleChangeForPeriodic fParticleChange;

private:

  void BoundaryProcessVerbose(void) const;

  G4PeriodicBoundaryProcessStatus theStatus;
  G4ThreeVector OldPosition;
	G4ThreeVector NewPosition;
  G4ThreeVector OldMomentum;
	G4ThreeVector NewMomentum;
  G4ThreeVector OldPolarization;
	G4ThreeVector NewPolarization;
	G4ThreeVector theGlobalNormal;
  G4double kCarTolerance;

  G4bool reflecting_walls;

  bool periodic_x; bool periodic_y; bool periodic_z;

};

inline G4bool G4PeriodicBoundaryProcess::IsApplicable(const G4ParticleDefinition&
  aParticleType)
{
  /*the process is not applicable to optical photons. If a logicalskinsurface
  is defined, then the optical photon would be cycled at any logicalskinsurface
  
  TODO add applicability to optical photons by testing name of logicalskinsurface
  , for example.
  */
   return ( &aParticleType != G4OpticalPhoton::OpticalPhoton() );

}

inline G4PeriodicBoundaryProcessStatus G4PeriodicBoundaryProcess::GetStatus() const
{
   return theStatus;
}