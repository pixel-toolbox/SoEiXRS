#include "G4RunManager.hh"

#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#include "include.hh"
#include "define.hh"

#include "FTFP_BERT.hh"

#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4EmPenelopePhysics.hh"

using namespace SoEiXRS;

int main(int argc, char* argv[]) {

	if (argc != 10) {
		std::cout << "Expects 9 arguments: " << std::endl;
		std::cout << "  beam energy (in keV)" << std::endl;
		std::cout << "  beam energy fluctuations (in keV), gaussian distribution" << std::endl;
		std::cout << "  target thickness (in mm)" << std::endl;
		std::cout << "  target angle (in degrees, 0 means perpendicular)" << std::endl;
		std::cout << "  detector position (\"transmission\" or \"perpendicular\")" << std::endl;
		std::cout << "  detector opening angle (in degrees)" << std::endl;
		std::cout << "  create data directory [yes/no]" << std::endl;
		std::cout << "  Geant4 Material, e.g. G4_W, G4_Mo, G4_Au" << std::endl;
		std::cout << "  Number of events" << std::endl;
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
	std::string material = std::string(argv[8]);
	int eventNum = std::stoi(argv[9]);

	std::cout << "#initial data:" << std::endl;
	std::cout << "# beam energy: " << beamEnergy << "keV" << std::endl;
	std::cout << "# beam energy width: " << beamEnergyFluc << "keV" << std::endl;
	std::cout << "# target thickness: " << targetThickness << "mm" << std::endl;
	std::cout << "# target angle: " << targetAngle << "°" << std::endl;
	std::cout << "# detector position: " << s1 << std::endl;
	std::cout << "# detector opening angle: " << detectorOpeningAngle << "°" << std::endl;
	std::cout << "# create Data directory: " << createDataDirectory << std::endl;
	std::cout << "# material: " << material << std::endl;
	std::cout << "# Event Number: " << eventNum << std::endl;

	std::string allEnergyFileName = "allEnergy.dat";
	std::string detEnergyFileName = "detEnergy.dat";
	std::string detInfoFileName = "info.dat";

	if (createDataDirectory) {
		std::ostringstream is;

		is << "sim_v2_" << beamEnergy << "pm" << beamEnergyFluc << "keV_"
				<< targetThickness << "mm_" << targetAngle << "deg_" << s1
				<< "_" << detectorOpeningAngle << "deg_" << material << "_" << eventNum << "events";

		std::string dirName(is.str());

		if (fs::is_directory(dirName) || fs::exists(dirName)) {
			fs::remove_all(dirName);
		}

		fs::create_directory(dirName); // create src folder

		allEnergyFileName = is.str() + "/" + allEnergyFileName;
		detEnergyFileName = is.str() + "/" + detEnergyFileName;
		detInfoFileName = is.str() + "/" + detInfoFileName;
	}

	std::ofstream detInfoFile(detInfoFileName);

	detInfoFile << "PARTICLES_IN_GUN=" << PARTICLES_IN_GUN << "\n";
	detInfoFile << "NUMBER_OF_EVENTS=" << eventNum << "\n";

	detInfoFile.close();

	// construct the default run manager
	G4RunManager* runManager = new G4RunManager();

	// set mandatory initialization classes
	G4VModularPhysicsList* physicsList = new FTFP_BERT;
	runManager->SetUserInitialization(physicsList);

	runManager->SetUserInitialization(
			new DetectorConstruction(targetThickness, targetAngle, material));
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

	/*G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialise();

	UI->ApplyCommand("/control/execute vis.mac");
	UI->ApplyCommand("/vis/scene/endOfEventAction accumulate -1");*/

	// start a run
	runManager->BeamOn(eventNum);

	/*std::string s;

	std::cin >> s;*/
	/*while (s != "exit" && s != "e") {
		//std::cin >> s;
		//UI->ApplyCommand(s);
		runManager->BeamOn(numberOfEvent);
		//UI->ApplyCommand("/vis/viewer/rebuild");
	}*/

	// job termination
	delete runManager;
	//delete visManager;

	return 0;
}
