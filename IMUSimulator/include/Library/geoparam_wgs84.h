#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>

#include "std_IMUSimulator.h"

namespace IMUSimulator {


	class WGS84Ellipsoid {

		private: 
			double WIE_E;
			double SM_AXIS;
			double E_SQR;
			double NORMAL_GRV;
			double GRV_CONS;
			double FLATTENING;
			double M_FAKTOR;

		public:
			double llh[3];

			/*Constructors*/
			WGS84Ellipsoid(void);
			WGS84Ellipsoid(double[], CoordiateFrame);

			
			void setCoordinates(double[], CoordiateFrame);
			void getParams(double&, double&, double&, double&, double&, double&);
			void get_Gravity_and_WIE_E(double&, double&);

	private:
		void setWGS84Params(void);

	};


}