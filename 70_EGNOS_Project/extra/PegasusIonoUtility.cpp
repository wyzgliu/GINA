#include "Runnables.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include "navMain.hpp"
#include "SaasTropModel.hpp"
#include "NeillTropModel.hpp"

#include "RTKPOST_Header.hpp"
#include "RTKPOST_Stream.hpp"
#include "RTKPOST_Data.hpp"

using namespace std;

int main(int argc, char **argv) {

	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";

	if (argc != 4 ) {

		std::cout << " Invalid arguments. Expected arguments: " << std::endl;
		std::cout << "#1 Ionex FileName w Path - input " << std::endl;
		std::cout << "#2 Pegasus FileName w Path - input " << std::endl;
		std::cout << "#3 Pegasus FileName w Path - output" << std::endl;
		exit(1);
	}

	std::string IonexFileNamewPath = argv[1];
	std::string PegasusFileNamewPath = argv[2];
	std::string outPegasusFileNamewPath = argv[3];

	//std::string IonexFileNamewPath = ROOT "\\70_EGNOS_Project\\core\\files\\DebugFiles\\CODG1040.19I";
	//std::string PegasusFileNamewPath = ROOT "\\70_EGNOS_Project\\core\\files\\DebugFiles\\sig1.txt";
	//std::string outPegasusFileNamewPath = ROOT "\\70_EGNOS_Project\\core\\files\\DebugFiles\\sig1_Out.txt";

	double heightOfIonoLayerinMeter = 350000;

		// Stream creation
		gpstk::IonexStream strm(IonexFileNamewPath.c_str(), std::ios::in);
		
		if (!strm) {

			gpstk::FileMissingException e("File " + IonexFileNamewPath +
				" could not be opened.");
			GPSTK_THROW(e);
		}

		// create the header object
		gpstk::IonexHeader header;
		strm >> header;

		if (!header.valid) {

			gpstk::FileMissingException e("File " + IonexFileNamewPath +
				" could not be opened. Check again " +
				"the path or the name provided!");
			GPSTK_THROW(e);
		}

		heightOfIonoLayerinMeter = header.hgt[0]*1000;
		EGNOS::IonexModel ionexModel(heightOfIonoLayerinMeter);

		// If the iono model is not a single layer one, we throw an error.
		if (header.hgt[2] != 0) {
			throw domain_error("Iono model is not a single layer model. EGNOS::IonexModel class is not able to handle this case correctly");
		}

		header.firstEpoch.setTimeSystem(gpstk::TimeSystem::GPS);
		header.lastEpoch.setTimeSystem(gpstk::TimeSystem::GPS);

		// keep an inventory of the loaded files 
		gpstk::IonexStore ionoStore;
		ionoStore.addFile(IonexFileNamewPath, header);

		// this map is useful in finding DCB value
		ionoStore.inxDCBMap[header.firstEpoch] = header.svsmap;

		// object data. If valid, add to the map
		gpstk::IonexData iod;
		while (strm >> iod && iod.isValid()) {

			iod.time.setTimeSystem(gpstk::TimeSystem::GPS);
			ionoStore.addMap(iod);
		}

		ionexModel.addIonexStore(ionoStore);

	std::string WN, sow, ippLat, ippLon, SatId, SIG_TOTAL, SIG_ION, SIG_TRO, SIG_FLT, SIG_AIR, elevation, azimuth;
	std::string line;
	
	std::ifstream pegasusReadFile;
	std::ofstream pegasusWriteFile;

	pegasusReadFile.open(PegasusFileNamewPath.c_str());
	pegasusWriteFile.open(outPegasusFileNamewPath.c_str());

	EGNOS::IonCorrandVar ionoCorr;
	while (std::getline(pegasusReadFile, line))
	{

		std::stringstream   ss(line);
		ss >> WN >> sow >> ippLat >> ippLon >> SatId >> SIG_TOTAL >> SIG_ION >> SIG_TRO >> SIG_FLT >> SIG_AIR >> elevation >> azimuth;
		
		gpstk::GPSWeekSecond epoch(stod(WN), stod(sow));

		try
		{
			ionoCorr = ionexModel.getCorrection(epoch, stod(ippLat), stod(ippLon), stod(elevation));
			
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			
			pegasusWriteFile << WN << " " << sow << " " << ippLat << " " << ippLon << " " << SatId << " " << SIG_TOTAL << " " << SIG_ION << " " << SIG_TRO << " " << SIG_FLT << " " << SIG_AIR << " " << elevation << " " << azimuth << std::endl;

			continue;
		}
	
		pegasusWriteFile  << WN << " " << sow << " " << ippLat << " " << ippLon << " " << SatId << " " << SIG_TOTAL << " " << SIG_ION << " " << SIG_TRO << " " << SIG_FLT << " " << SIG_AIR << " " << elevation << " " << azimuth << " " << std::fixed << std::setprecision(3) << ionoCorr.CorrinMeter << " " << std::fixed << std::setprecision(3) << std::sqrt(ionoCorr.Variance) << std::endl;
	}
	
	pegasusReadFile.close();
	pegasusWriteFile.close();

	return 0;
}