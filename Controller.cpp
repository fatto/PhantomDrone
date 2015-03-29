#include "Controller.hpp"

#include "ArrayMath.hpp"
#include <cmath>
#include <iostream>

//Controller::Controller(Drone& d) : drone(d), t(timer::now()), file("plot.dat")
//{
//	//file << "vx0\tvx1\tvx2\tvy0\tvy1\tvy2" << std::endl;
//	//file << "alphaCorr\tbetaCorr\trotCorr" << std::endl;
//	//file << "error_x\terror_y\terror_angle" << std::endl;
//	//file << "angle_diff\teuler\ttarget_angle\tcurrent_angle" << std::endl;
//	file << "rot0\trot1\trot2\trot3" << std::endl;
//}

//void Controller::Go(std::array<float, 3> position, float angle)
//{
//	//std::cout << (timer::now() - t).count() * double(timer::period::num) / double(timer::period::den) * 1000.0 << std::endl;
//	t = timer::now();
//	auto status = drone.Status();
//
//	// vertical
//	float e = position[2] - status.position[2];
//	cumul = cumul + e;
//	float pv = pParam * e;
//	float thrust = 5.335f + pv + iParam*cumul + dParam*(e - lastE) + status.velocity[2] * vParam;
//	lastE = e;
//
//	// horizzontal
//	array3f sp = position - status.position;
//	//array3f sp = status.position - position;
//	auto ang = status.angle;
//	float c1 = std::cos(ang[0]), c2 = std::cos(ang[1]), c3 = std::cos(ang[2]);
//	float s1 = std::sin(ang[0]), s2 = std::sin(ang[1]), s3 = std::sin(ang[2]);
//
//	array3f vx = { c2*c3, c1*s3 + s1*s2*c3, s1*s3 - c1*s2*c3 };
//	array3f vy = { -c2*s3, c1*c3 - s1*s2*s3, s1*c3 + c1*s2*s3 };
//	//file << vx[0] << "\t" << vx[1] << "\t" << vx[2] << "\t" << vy[0] << "\t" << vy[1] << "\t" << vx[2] << std::endl;
//	/*vx = vx + status.position;
//	vy = vy + status.position;*/
//	float alphaE = vy[2]; // - status.position[2];
//	float betaE = vx[2]; // -status.position[2];
//	float alphaCorr = 0.25*alphaE + 2.1*(alphaE - pAlphaE);
//	float betaCorr = -0.25*betaE - 2.1*(betaE - pBetaE);
//	pAlphaE = alphaE;
//	pBetaE = betaE;
//	/*alphaCorr = alphaCorr + sp[1] * 0.005 + 1 * (sp[1] - psp1);
//	betaCorr = betaCorr - sp[0] * 0.005 - 1 * (sp[0] - psp0);*/
//	/*alphaCorr = alphaCorr + 0.5 * (sp[1] * 0.005 + 1.0 * (sp[1] - psp1));
//	betaCorr = betaCorr + 0.5 * (-sp[0] * 0.005 - 1.0 * (sp[0] - psp0));
//	psp1 = sp[1];
//	psp0 = sp[0];*/
//	auto disp_x = sp[0] * vx[0] + sp[1] * vx[1];
//	auto disp_y = sp[0] * vy[0] + sp[1] * vy[1];
//	alphaCorr = alphaCorr + disp_y * 0.005 + 1.0 * (disp_y - psp1);
//	betaCorr = betaCorr - disp_x * 0.005 - 1 * (disp_x - psp0);
//	psp1 = disp_y;
//	psp0 = disp_x;
//
//
//	// rotational
//	//array3f euler = status.angle - array3f{0.f, 0.f, angle};
//	//float euler = M_PI - std::fabs(std::fmod(std::fabs(status.angle[2] - angle + 2.0*M_PI), 2.0 * M_PI) - M_PI);
//	float angle_diff = status.angle[2] - angle;
//	float c_ang = std::cos(status.angle[2] - angle);
//	float min_angle = std::acos(c_ang);
//	/*float s_ang = std::sin(min_angle);
//	float dir = c3*st - s3*ct, angle_diff = dir;
//	float euler = dir? min_angle : - min_angle;*/
//	float ct = std::cos(angle), st = std::sin(angle);
//	float euler = (c3*st - s3*ct) * min_angle;
//	//euler = (angle - status.angle[2]) < ((status.angle[2]+M_PI) - angle) ? -euler : euler;
//	euler = fabs(status.angle[2] - angle) == euler ? euler : -euler;
//	//array3f euler = status.angle - array3f{ 0.f, 0.f, 3.14f * (2.f/5.f) *0.f };
//	//float rotCorr = euler[2] * 0.1 + 2 * (euler[2] - prevEuler);
//	float rotCorr = euler * 0.1 + 2 * (euler - prevEuler);
//	//rotCorr *= 0.1;
//	//std::cout << status.angle[2] << "\t\t" << angle << "\t\t" << rotCorr << std::endl;
//	//std::cout << angle << "\t\t" << euler[2] * 0.1 << "\t\t" << 2 * (euler[2] - prevEuler) << std::endl;
//	prevEuler = euler;
//	//file << angle_diff << "\t" << euler << "\t" << angle << "\t" << status.angle[2] << std::endl;
//
//	std::array<float, 4> rotor;
//	rotor[0] = thrust*(1 - alphaCorr + betaCorr + rotCorr);
//	rotor[1] = thrust*(1 - alphaCorr - betaCorr - rotCorr);
//	rotor[2] = thrust*(1 + alphaCorr - betaCorr + rotCorr);
//	rotor[3] = thrust*(1 + alphaCorr + betaCorr - rotCorr);
//
//	/*rotor[0] = thrust*(1 + rotCorr);
//	rotor[1] = thrust*(1 - rotCorr);
//	rotor[2] = thrust*(1 + rotCorr);
//	rotor[3] = thrust*(1 - rotCorr);*/
//	file << rotor[0] << "\t" << rotor[1] << "\t" << rotor[2] << "\t" << rotor[3] << std::endl;
//
//	drone.Apply(rotor);
//}

//
//-- Horizontal control :
//--sp = simGetObjectPosition(targetObj, d)
//--m = simGetObjectMatrix(d, -1)
//--vx = { 1,0,0 }
//--vx = simMultiplyVector(m, vx)
//--vy = { 0,1,0 }
//--vy = simMultiplyVector(m, vy)
//--alphaE = (vy[3] - m[12])
//--alphaCorr = 0.25*alphaE + 2.1*(alphaE - pAlphaE)
//--betaE = (vx[3] - m[12])
//--betaCorr = -0.25*betaE - 2.1*(betaE - pBetaE)
//--pAlphaE = alphaE
//-- pBetaE = betaE
//-- alphaCorr = alphaCorr + sp[2] * 0.005 + 1 * (sp[2] - psp2)
//--betaCorr = betaCorr - sp[1] * 0.005 - 1 * (sp[1] - psp1)
//--psp2 = sp[2]
//--psp1 = sp[1]
//
//--Rotational control :
//--euler = simGetObjectOrientation(d, targetObj)
//--rotCorr = euler[3] * 0.1 + 2 * (euler[3] - prevEuler)
//--prevEuler = euler[3]
//
//--Decide of the motor velocities :
//--particlesTargetVelocities[1] = thrust*(1 - alphaCorr + betaCorr + rotCorr)
//--particlesTargetVelocities[2] = thrust*(1 - alphaCorr - betaCorr - rotCorr)
//--particlesTargetVelocities[3] = thrust*(1 + alphaCorr - betaCorr + rotCorr)
//--particlesTargetVelocities[4] = thrust*(1 + alphaCorr + betaCorr - rotCorr)
//
//--Send the desired motor velocities to the 4 rotors:
//-- for i = 1, 4, 1 do
//--simSetScriptSimulationParameter(propellerScripts[i], 'particleVelocity', particlesTargetVelocities[i])
//--end



//--Vertical control :
//--targetPos = simGetObjectPosition(targetObj, -1)
//--pos = simGetObjectPosition(d, -1)
//--l = simGetVelocity(heli)
//--e = (targetPos[3] - pos[3])
//--cumul = cumul + e
//-- pv = pParam*e
//-- thrust = 5.335 + pv + iParam*cumul + dParam*(e - lastE) + l[3] * vParam
//-- lastE = e