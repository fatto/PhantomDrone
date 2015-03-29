#include "Drone.hpp"

#include <iostream>
#include <system_error>
#include <cassert>
#include <string>

static int portNb = 19997;

Drone::Drone() //: end(false)
{
	clientID = simxStart((simxChar*)"127.0.0.1", portNb, 1, 1, 2000, 5);
	if (clientID == -1)
	{
		throw std::system_error(ECONNABORTED, std::system_category(), "no vrep response");
	}

	assert(simxGetObjectHandle(clientID, "Quadricopter_base", &quadcopter, simx_opmode_oneshot_wait) == simx_return_ok);

	propeller_velocity = std::array<simxFloat, 4> { 0.f, 0.f, 0.f, 0.f };
	assert(simxGetObjectPosition(clientID, quadcopter, -1 /*absolute position*/, position.data(), simx_opmode_oneshot_wait) == simx_return_ok);
	assert(simxGetObjectOrientation(clientID, quadcopter, -1 /*absolute angle*/, angle.data(), simx_opmode_oneshot_wait) == simx_return_ok);
	assert(simxGetObjectVelocity(clientID, quadcopter, velocity.data(), rotation.data(), simx_opmode_oneshot_wait) == simx_return_ok);

	//updater = std::thread(&Drone::updateStatus, this);
	assert(simxStartSimulation(clientID, simx_opmode_oneshot_wait) == simx_return_ok);
	assert(simxSynchronous(clientID, 1) == simx_return_ok);
}

Drone::~Drone()
{
	//end.store(true);
	//updater.join();
	assert(simxStopSimulation(clientID, simx_opmode_oneshot_wait) == simx_return_ok);
	/*switch (err)
	{
	case simx_return_ok: std::cout << "simx_return_ok" << std::endl; break;
	case simx_return_novalue_flag: std::cout << "simx_return_novalue_flag" << std::endl; break;
	case simx_return_timeout_flag: std::cout << "simx_return_timeout_flag" << std::endl; break;
	case simx_return_illegal_opmode_flag: std::cout << "simx_return_illegal_opmode_flag" << std::endl; break;
	case simx_return_remote_error_flag: std::cout << "simx_return_remote_error_flag" << std::endl; break;
	case simx_return_split_progress_flag: std::cout << "simx_return_split_progress_flag" << std::endl; break;
	case simx_return_local_error_flag: std::cout << "simx_return_local_error_flag" << std::endl; break;
	case simx_return_initialize_error_flag: std::cout << "simx_return_initialize_error_flag" << std::endl; break;
	}*/
	//extApi_sleepMs(5);
	simxFinish(clientID);
}

Drone::StatusStruct Drone::status() const
{
	return StatusStruct{
		std::array<double,3>{position[0], position[1], position[2]},
		std::array<double,3>{angle[0], angle[1], angle[2]},
		std::array<double,3>{velocity[0], velocity[1], velocity[2]}
	};
}

void Drone::apply(std::array<double, 4> rotor)
{
	//std::lock_guard<std::mutex> lock(mtx);
	//std::cout << rotor[0] << " " << rotor[1] << " " << rotor[2] << " " << rotor[3] << std::endl;
	propeller_velocity = { simxFloat(rotor[0]), simxFloat(rotor[1]), simxFloat(rotor[2]), simxFloat(rotor[3]) }; // no real double support on remoteApi, just a simxDouble definition4
	updateStatus();
}

void Drone::updateStatus()
{
	assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
	for (int i = 0; i < 4; ++i)
	{
		simxSetFloatSignal(clientID, std::string(std::string("velocity") + std::to_string(i + 1)).c_str(), propeller_velocity[i], simx_opmode_oneshot);
	}
	assert(simxPauseCommunication(clientID, 0) == simx_return_ok);

	while (simxSynchronousTrigger(clientID) != simx_return_ok); // HACK TO ENABLE PAUSE ON SIM

	assert(simxGetObjectPosition(clientID, quadcopter, -1 /*absolute position*/, position.data(), simx_opmode_oneshot_wait) == simx_return_ok);
	assert(simxGetObjectOrientation(clientID, quadcopter, -1 /*absolute angle*/, angle.data(), simx_opmode_oneshot_wait) == simx_return_ok);
	assert(simxGetObjectVelocity(clientID, quadcopter, velocity.data(), rotation.data(), simx_opmode_oneshot_wait) == simx_return_ok);
}
