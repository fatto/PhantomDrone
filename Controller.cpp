#include "Controller.hpp"

#include "ArrayMath.hpp"
#include <cmath>
#include <iostream>

Controller::Controller(Drone& d) : drone(d), t(timer::now()), file("plot.dat")
{
	file << "status_angle\tangle\tmin_angle\teuler\tdiff" << std::endl;
};

void Controller::Go(vector4 position, vector4 angle)
{
	t = timer::now();
	auto status = drone.Status();

	//std::cout << status.position.x << " " << status.position.y << " " << status.position.z << std::endl;

	// vertical
	double e = position.z - status.position.z;
	cumul = cumul + e;
	double pv = pParam * e;
	double thrust = 5.335 + pv + iParam*cumul + dParam*(e - lastE) + status.velocity.z * vParam;
	lastE = e;

	// horizontal
	vector4 sp = position - status.position;
	auto ang = status.angle;
	double c1 = std::cos(ang.x), c2 = std::cos(ang.y), c3 = std::cos(ang.z);
	double s1 = std::sin(ang.x), s2 = std::sin(ang.y), s3 = std::sin(ang.z);

	vector4 vx = { c2*c3, c1*s3 + s1*s2*c3, s1*s3 - c1*s2*c3, 0.0 };
	vector4 vy = { -c2*s3, c1*c3 - s1*s2*s3, s1*c3 + c1*s2*s3, 0.0 };
	double alphaE = vy.z;
	double betaE = vx.z;
	double alphaCorr = 0.25*alphaE + 2.1*(alphaE - pAlphaE);
	double betaCorr = -0.25*betaE - 2.1*(betaE - pBetaE);
	pAlphaE = alphaE;
	pBetaE = betaE;

	auto disp_x = sp.x * vx.x + sp.y * vx.y;
	auto disp_y = sp.x * vy.x + sp.y * vy.y;
	alphaCorr = alphaCorr + disp_y * 0.005 + 1.0 * (disp_y - psp1);
	betaCorr = betaCorr - disp_x * 0.005 - 1 * (disp_x - psp0);
	psp1 = disp_y;
	psp0 = disp_x;


	// rotational
	double min_angle = std::acos(std::cos(status.angle.z - angle.z));
	double ct = std::cos(angle.z), st = std::sin(angle.z);
	double euler = std::copysign(min_angle, (s3*ct - c3*st));
	double rotCorr = euler * 0.1 + 2 * (euler - prevEuler); // positive rotation clockwise
	prevEuler = euler;

	vector4 rotor = {
		thrust*(1 - alphaCorr + betaCorr + rotCorr),
		thrust*(1 - alphaCorr - betaCorr - rotCorr),
		thrust*(1 + alphaCorr - betaCorr + rotCorr),
		thrust*(1 + alphaCorr + betaCorr - rotCorr)
	};

	//std::cout << rotor.x << " " << rotor.y << " " << rotor.z << " " << rotor.w << std::endl;


	drone.Apply(rotor);
}
