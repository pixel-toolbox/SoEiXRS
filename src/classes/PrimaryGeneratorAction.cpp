#include "PrimaryGeneratorAction.hpp"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace SoEiXRS {

PrimaryGeneratorAction::PrimaryGeneratorAction(double energy, double energyFluc) :
		G4VUserPrimaryGeneratorAction(), fParticleGun(0), fEnvelopeBox(0), de(
				time(0)), nd(energy, energyFluc) {
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);

	G4ParticleDefinition* particle = G4Electron::ElectronDefinition();
	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
	fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -10 * cm));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
	delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
	fParticleGun->SetParticleEnergy(nd(de) * keV);

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

} /* namespace SoEiXRS */
