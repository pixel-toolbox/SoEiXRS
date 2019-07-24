#ifndef SoEiXRS_CLASSES_ACTIONINITIALIZATION_HPP_
#define SoEiXRS_CLASSES_ACTIONINITIALIZATION_HPP_ 1

#include "G4VUserActionInitialization.hh"

namespace SoEiXRS {

class ActionInitialization : public G4VUserActionInitialization {
	public:
		ActionInitialization(double energy, double energyFluc);

		virtual void Build() const;

		virtual ~ActionInitialization();

	private:
		double energy;
		double energyFluc;
};

} /* namespace SoEiXRS */

#endif /* SoEiXRS_CLASSES_ACTIONINITIALIZATION_HPP_ */
