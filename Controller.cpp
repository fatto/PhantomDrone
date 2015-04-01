#include "Controller.hpp"

#include "ArrayMath.hpp"
#include <cmath>
#include <iostream>

Controller::Controller(Drone& d) : drone(d), t(timer::now()), file("plot.dat")
{
	file << "status_angle\tangle\tmin_angle\teuler\tdiff" << std::endl;
};

void Controller::Go(std::array<double, 3> position, std::array<double, 3> angle)
{
	t = timer::now();
	auto status = drone.Status();

	// vertical
	double e = position[2] - status.position[2];
	cumul = cumul + e;
	double pv = pParam * e;
	double thrust = 5.335 + pv + iParam*cumul + dParam*(e - lastE) + status.velocity[2] * vParam;
	lastE = e;

	// horizzontal
	array3d sp = position - status.position;
	auto ang = status.angle;
	double c1 = std::cos(ang[0]), c2 = std::cos(ang[1]), c3 = std::cos(ang[2]);
	double s1 = std::sin(ang[0]), s2 = std::sin(ang[1]), s3 = std::sin(ang[2]);

	array3d vx = { c2*c3, c1*s3 + s1*s2*c3, s1*s3 - c1*s2*c3 };
	array3d vy = { -c2*s3, c1*c3 - s1*s2*s3, s1*c3 + c1*s2*s3 };
	double alphaE = vy[2];
	double betaE = vx[2];
	double alphaCorr = 0.25*alphaE + 2.1*(alphaE - pAlphaE);
	double betaCorr = -0.25*betaE - 2.1*(betaE - pBetaE);
	pAlphaE = alphaE;
	pBetaE = betaE;

	auto disp_x = sp[0] * vx[0] + sp[1] * vx[1];
	auto disp_y = sp[0] * vy[0] + sp[1] * vy[1];
	alphaCorr = alphaCorr + disp_y * 0.005 + 1.0 * (disp_y - psp1);
	betaCorr = betaCorr - disp_x * 0.005 - 1 * (disp_x - psp0);
	psp1 = disp_y;
	psp0 = disp_x;


	// rotational
	double min_angle = std::acos(std::cos(status.angle[2] - angle[2]));
	double ct = std::cos(angle[2]), st = std::sin(angle[2]);
	double euler = std::copysign(min_angle, (s3*ct - c3*st));
	double rotCorr = euler * 0.1 + 2 * (euler - prevEuler); // positive rotation clockwise
	prevEuler = euler;

	std::array<double, 4> rotor;
	rotor[0] = thrust*(1 - alphaCorr + betaCorr + rotCorr);
	rotor[1] = thrust*(1 - alphaCorr - betaCorr - rotCorr);
	rotor[2] = thrust*(1 + alphaCorr - betaCorr + rotCorr);
	rotor[3] = thrust*(1 + alphaCorr + betaCorr - rotCorr);


	drone.Apply(rotor);
}
