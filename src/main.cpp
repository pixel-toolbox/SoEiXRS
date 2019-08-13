#include "G4RunManager.hh"

#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#include "include.hh"

#include "FTFP_BERT.hh"

#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4EmPenelopePhysics.hh"

using namespace SoEiXRS;

int main(int argc, char* argv[]) {

	if (argc != 8) {
		std::cout << "Expects 7 arguments: " << std::endl;
		std::cout << "  beam energy (in keV)" << std::endl;
		std::cout << "  beam energy fluctuations (in keV), gaussian distribution" << std::endl;
		std::cout << "  target thickness (in mm)" << std::endl;
		std::cout << "  target angle (in degrees, 0 means perpendicular)" << std::endl;
		std::cout << "  detector position (\"transmission\" or \"perpendicular\")" << std::endl;
		std::cout << "  detector opening angle (in degrees)" << std::endl;
		std::cout << "  create data directory [yes/no]" << std::endl;
		return 0;
	}

	double beamEnergy = std::stod(argv[1]);
	double beamEnergyFluc = std::stod(argv[2]);
	double targetThickness = std::stod(argv[3]);
	double targetAngle = std::stod(argv[4]);
	DetectorPosition detectorPosition;
	std::string s1(argv[5]);
	if (s1 == "transmission") {
		detectorPosition = transmission;
	} else if (s1 == "perpendicular") {
		detectorPosition = perpendicular;
	} else {
		std::cout << "unknown detector position" << std::endl;
		exit(0);
	}
	double detectorOpeningAngle = std::stod(argv[6]);
	bool createDataDirectory = (std::string(argv[7]) == "yes");

	std::cout << "#initial data:" << std::endl;
	std::cout << "# beam energy: " << beamEnergy << "keV" << std::endl;
	std::cout << "# beam energy width: " << beamEnergyFluc << "keV" << std::endl;
	std::cout << "# target thickness: " << targetThickness << "mm" << std::endl;
	std::cout << "# target angle: " << targetAngle << "°" << std::endl;
	std::cout << "# detector position: " << s1 << std::endl;
	std::cout << "# detector opening angle: " << detectorOpeningAngle << "°" << std::endl;

	std::string allEnergyFileName = "allEnergy.dat";
	std::string detEnergyFileName = "detEnergy.dat";

	if (createDataDirectory) {
		std::ostringstream is;

		is << "sim_v1_" << beamEnergy << "pm" << beamEnergyFluc << "keV_"
				<< targetThickness << "mm_" << targetAngle << "deg_" << s1
				<< "_" << detectorOpeningAngle << "deg";

		std::string dirName(is.str());

		if (fs::is_directory(dirName) || fs::exists(dirName)) {
			fs::remove(dirName);
		}

		fs::create_directory(dirName); // create src folder

		allEnergyFileName = is.str() + "/" + allEnergyFileName;
		detEnergyFileName = is.str() + "/" + detEnergyFileName;
	}

	// construct the default run manager
	G4RunManager* runManager = new G4RunManager();

	// set mandatory initialization classes
	G4VModularPhysicsList* physicsList = new FTFP_BERT;
	runManager->SetUserInitialization(physicsList);

	runManager->SetUserInitialization(
			new DetectorConstruction(targetThickness, targetAngle));
	runManager->SetUserInitialization(
			new ActionInitialization(beamEnergy, beamEnergyFluc,
					allEnergyFileName.c_str(), detEnergyFileName.c_str(),
					detectorPosition, detectorOpeningAngle));

	// get the pointer to the UI manager and set verbosities
	G4UImanager* UI = G4UImanager::GetUIpointer();
	UI->ApplyCommand("/run/verbose 0");
	UI->ApplyCommand("/event/verbose 0");
	UI->ApplyCommand("/tracking/verbose 0");
	UI->ApplyCommand("/run/setCut  0.00001 mm");
	UI->ApplyCommand("/cuts/setLowEdge 250 eV");
	UI->ApplyCommand("/process/em/fluo true");

	// initialize G4 kernel
	runManager->Initialize();

	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialise();

	UI->ApplyCommand("/control/execute vis.mac");
	UI->ApplyCommand("/vis/scene/endOfEventAction accumulate -1");

	// start a run
	int numberOfEvent = 10;
	runManager->BeamOn(numberOfEvent);

	std::string s;

	std::cin >> s;
	/*while (s != "exit" && s != "e") {
		//std::cin >> s;
		//UI->ApplyCommand(s);
		runManager->BeamOn(numberOfEvent);
		//UI->ApplyCommand("/vis/viewer/rebuild");
	}*/

	// job termination
	delete runManager;
	delete visManager;

	return 0;
}
