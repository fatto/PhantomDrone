#include "Drone.hpp"

#include <iostream>
#include <system_error>
#include <cassert>
#include <cstdint>
#include <string>

static int portNb = 19997;

Drone::Drone() : reaction() //: end(false)
{
	clientID = simxStart((simxChar*)"127.0.0.1", portNb, 1, 1, 200000, 1);
	if (clientID == -1)
	{
		throw std::system_error(ECONNABORTED, std::system_category(), "no vrep response");
	}

	simxGetObjectHandle(clientID, "Quadricopter_base", &quadcopter, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "Cuboid", &cuboid, simx_opmode_oneshot_wait);

	bbox.half_size = vector4(0.3, 0.3, 0.02, 0.0);

	propeller_velocity = std::array<simxFloat, 4> { 0.f, 0.f, 0.f, 0.f };
	simxGetObjectPosition(clientID, quadcopter, -1 /*absolute position*/, position.data(), simx_opmode_streaming);
	simxGetObjectOrientation(clientID, quadcopter, -1 /*absolute angle*/, angle.data(), simx_opmode_streaming);
	simxGetObjectVelocity(clientID, quadcopter, velocity.data(), rotation.data(), simx_opmode_streaming);
	
	simxGetObjectPosition(clientID, quadcopter, -1 /*absolute position*/, position.data(), simx_opmode_oneshot_wait);
	simxGetObjectOrientation(clientID, quadcopter, -1 /*absolute angle*/, angle.data(), simx_opmode_oneshot_wait);
	simxGetObjectVelocity(clientID, quadcopter, velocity.data(), rotation.data(), simx_opmode_oneshot_wait);

	//simxUChar* signal;
	//simxInt sLength;
	//assert(simxReadStringStream(clientID, "toClient", &signal, &sLength, simx_opmode_oneshot_wait) == simx_return_ok);

	//simxInt dummy_handle;
	//std::array<uint8_t, 12> color{255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255};
	//assert(simxCreateDummy(clientID, 1.0f, color.data(), &dummy_handle, simx_opmode_oneshot_wait) == simx_return_ok);

	//updater = std::thread(&Drone::updateStatus, this);
	simxStartSimulation(clientID, simx_opmode_oneshot_wait);
	simxSynchronous(clientID, 1);
	simxSynchronousTrigger(clientID);
}

Drone::~Drone()
{
	//end.store(true);
	//updater.join();
	simxSynchronous(clientID, 0);
	simxStopSimulation(clientID, simx_opmode_oneshot_wait);
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

void Drone::AddCuboid(std::array<double, 3> _position, std::array<double, 3> _angle)
{
	std::array<simxFloat, 3> fposition = { simxFloat(_position[0]), simxFloat(_position[1]),simxFloat(_position[2]) };
	//std::cout << fposition[0] << " " << fposition[1] << " " << fposition[2] << std::endl;
	std::array<simxFloat, 3> fangle = { simxFloat(_angle[0]), simxFloat(_angle[1]),simxFloat(_angle[2]) };
	simxInt* hand;
	simxInt hand_count;
	simxCopyPasteObjects(clientID, &cuboid, 1, &hand, &hand_count, simx_opmode_oneshot_wait);
	cuboids.push_back(hand[0]);
	boxs.emplace_back();
	simxSetObjectPosition(clientID, hand[0], -1, fposition.data(), simx_opmode_oneshot_wait);
	simxSetObjectOrientation(clientID, hand[0], hand[0], fangle.data(), simx_opmode_oneshot_wait);
}

Drone::StatusStruct Drone::Status() const
{
	return StatusStruct{
		vector4{position[0], position[1], position[2], 0.0},
		vector4{angle[0], angle[1], angle[2], 0.0},
		vector4{velocity[0], velocity[1], velocity[2], 0.0},
		vector4{reaction[0], reaction[1], reaction[2], 0.0}
	};
}

void Drone::Apply(vector4 rotor)
{
	//std::lock_guard<std::mutex> lock(mtx);
	//std::cout << rotor[0] << " " << rotor[1] << " " << rotor[2] << " " << rotor[3] << std::endl;
	propeller_velocity = { simxFloat(rotor.s), simxFloat(rotor.t), simxFloat(rotor.p), simxFloat(rotor.q) }; // no real double support on remoteApi, just a simxDouble definition4
	updateStatus();
}


void Drone::updateStatus()
{
	assert(simxPauseCommunication(clientID, 1) == simx_return_ok);
	for (int i = 0; i < 4; ++i)
	{
		simxSetFloatSignal(clientID, std::string(std::string("velocity") + std::to_string(i + 1)).c_str(), propeller_velocity[i], simx_opmode_oneshot);
	}
	//simxFloat ssssss[] = { 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f };
	//simxUChar signal[sizeof(float)*6];
	//simxInt sLength = sizeof(float)*6;
	//simxWriteStringStream(clientID, "fromClient", (simxUChar*)ssssss, sLength, simx_opmode_oneshot);
	assert(simxPauseCommunication(clientID, 0) == simx_return_ok);

	while (simxSynchronousTrigger(clientID) != simx_return_ok); // HACK TO ENABLE PAUSE ON SIM

	std::array<simxFloat, 3> buff;
	if(simxGetObjectPosition(clientID, quadcopter, -1 /*absolute position*/, buff.data(), simx_opmode_oneshot_wait) == simx_return_ok)
	// if(simxGetObjectPosition(clientID, quadcopter, -1 /*absolute position*/, buff.data(), simx_opmode_buffer) == simx_return_ok)
		position = buff;
	if(simxGetObjectOrientation(clientID, quadcopter, -1 /*absolute angle*/, buff.data(), simx_opmode_oneshot_wait) == simx_return_ok)
	// if(simxGetObjectOrientation(clientID, quadcopter, -1 /*absolute angle*/, buff.data(), simx_opmode_buffer) == simx_return_ok)
		angle = buff;
	// simxGetObjectVelocity(clientID, quadcopter, velocity.data(), rotation.data(), simx_opmode_buffer);
	if (simxGetObjectVelocity(clientID, quadcopter, buff.data(), rotation.data(), simx_opmode_oneshot_wait) == simx_return_ok)
		velocity = buff;


	// collision box update
	double c1 = std::cos(angle[0]), c2 = std::cos(angle[1]), c3 = std::cos(angle[2]);
	double s1 = std::sin(angle[0]), s2 = std::sin(angle[1]), s3 = std::sin(angle[2]);

	bbox.position = vector4(position[0], position[1], position[2], 0.0);
	bbox.x_axis = vector4(c2*c3, c1*s3 + s1*s2*c3, s1*s3 - c1*s2*c3, 0.0);
	bbox.y_axis = vector4(-c2*s3, c1*c3 - s1*s2*s3, s1*c3 + c1*s2*s3, 0.0);
	bbox.z_axis = vector4(s2, -s1*c2, c1*c2, 0.0);
	

	for (decltype(cuboids)::size_type i = 0; i < cuboids.size(); ++i)
	{
		std::array<float, 3> pos, ang;
		//if(simxGetObjectPosition(clientID, cuboids[i], -1 /*absolute position*/, buff.data(), simx_opmode_buffer) == simx_return_ok)
		if (simxGetObjectPosition(clientID, cuboids[i], -1 /*absolute position*/, buff.data(), simx_opmode_oneshot_wait) == simx_return_ok)
			pos = buff;
		//if(simxGetObjectOrientation(clientID, cuboids[i], -1 /*absolute angle*/, buff.data(), simx_opmode_buffer) == simx_return_ok)
		if (simxGetObjectPosition(clientID, cuboids[i], -1 /*absolute position*/, buff.data(), simx_opmode_oneshot_wait) == simx_return_ok)
			ang = buff;
		c1 = std::cos(ang[0]); c2 = std::cos(ang[1]); c3 = std::cos(ang[2]);
		s1 = std::sin(ang[0]); s2 = std::sin(ang[1]); s3 = std::sin(ang[2]);

		boxs[i].position = vector4(pos[0], pos[1], pos[2], 0.0);
		boxs[i].x_axis = vector4(c2*c3, c1*s3 + s1*s2*c3, s1*s3 - c1*s2*c3, 0.0);
		boxs[i].y_axis = vector4(-c2*s3, c1*c3 - s1*s2*s3, s1*c3 + c1*s2*s3, 0.0);
		boxs[i].z_axis = vector4(s2, -s1*c2, c1*c2, 0.0);
		boxs[i].half_size = vector4(0.1, 0.1, 0.1, 0.0);
	}

	vector4 react(0.0); bool coll = false;
	for (auto& b : boxs)
	{
		auto r = sat::testIn(bbox, b);
		react += r.direction;
		coll |= r.collide;
	}

	reaction = std::array<double, 3>{react.x, react.y, react.z};

	std::cout << std::boolalpha << coll << " " << reaction[0] << " " << reaction[1] << " " << reaction[2] << std::endl;
	//std::cout << signal << std::endl;
	//assert(simxGetFloatSignal(clientID, "reactionx", &reaction[0], simx_opmode_oneshot_wait) == simx_return_ok);
	//assert(simxGetFloatSignal(clientID, "reactiony", &reaction[1], simx_opmode_oneshot_wait) == simx_return_ok);
	//assert(simxGetFloatSignal(clientID, "reactionz", &reaction[2], simx_opmode_oneshot_wait) == simx_return_ok);
}
