#ifndef SoEiXRS_CLASSES_ACTIONINITIALIZATION_HPP_
#define SoEiXRS_CLASSES_ACTIONINITIALIZATION_HPP_ 1

#include "G4VUserActionInitialization.hh"
#include "DetectorPosition.hh"

namespace SoEiXRS {

class ActionInitialization: public G4VUserActionInitialization {
public:
	ActionInitialization(double energy, double energyFluc,
			const char* detectorEnergyOutFile, const char* detectorEnergyElectronOutFile,
			DetectorPosition detPos, double detectorAngle);

	virtual void Build() const;

	virtual ~ActionInitialization();

private:
	double energy;
	double energyFluc;

	const char* detectorEnergyOutFile;
	const char* detectorEnergyElectronOutFile;

	DetectorPosition detPos;
	double detectorAngle;

};

} /* namespace SoEiXRS */

#endif /* SoEiXRS_CLASSES_ACTIONINITIALIZATION_HPP_ */
