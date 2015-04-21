#include "Phantom.hpp"
#include "ArrayMath.hpp"

#include <iostream>

Phantom::Phantom() : start_position(0, 0, 0), position(0, 0, 0), offset_position(0,0,0), update_force(0, 0, 0), start_gimbal(0, 0, 0), gimbal(0, 0, 0), offset_gimbal(0,0,0), max_force(0)
{
	hHD = hdInitDevice(HD_DEFAULT_DEVICE);
	testDevice();

	gCallbackHandle = hdScheduleAsynchronous(&Phantom::monitorDevice, this, HD_MAX_SCHEDULER_PRIORITY);

	hdEnable(HD_FORCE_OUTPUT);
	hdGetDoublev(HD_NOMINAL_MAX_FORCE, &max_force);

	hdStartScheduler();
	testDevice();
}
Phantom::~Phantom()
{
	hdStopScheduler();
	//hdUnschedule(gCallbackHandle);
	hdDisableDevice(hHD);
}

Phantom::StatusStruct Phantom::Status()
{
	std::lock_guard<std::mutex> lock(device_mutex);
	hduVector3Dd pos_diff;
	hduVecSubtract(pos_diff, position, start_position);
	hduVecAdd(pos_diff, pos_diff, offset_position);
	hduVector3Dd gim_diff;
	hduVecSubtract(gim_diff, gimbal, start_gimbal);
	hduVecAdd(gim_diff, gim_diff, offset_gimbal);

	//std::cout << pos_diff[0] * 0.01 << " " << -pos_diff[2] * 0.01 << " " << pos_diff[1] * 0.01 << " " << - gim_diff[2] << std::endl;
	return StatusStruct{
		button,
		vector4{pos_diff[0]*0.01, -pos_diff[2] * 0.01, pos_diff[1] * 0.01, 0.0},
		vector4{gim_diff[0], gim_diff[1], -gim_diff[2], 0.0} // in rads
	};
}
//std::array<double, 3> Phantom::Position()
//{
//	std::lock_guard<std::mutex> lock(device_mutex);
//	return std::array<double,3>{position[0], -position[2], position[1]};
//}
//
//std::array<double, 3> Phantom::Displacement()
//{
//	std::lock_guard<std::mutex> lock(device_mutex);
//	hduVector3Dd displacement; 
//	hduVecSubtract(displacement, position, anchor);
//	return std::array<double,3>{displacement[0], -displacement[2], displacement[1]};
//}

void Phantom::Force(vector4 f)
{
	f.x = clamp(f.x * 5.0, -max_force, max_force);
	f.y = clamp(f.y * 5.0, -max_force, max_force);
	f.z = clamp(f.z * 5.0, -max_force, max_force);

	//std::cout << f.x << " " << f.y << " " << f.z << std::endl;
	std::lock_guard<std::mutex> lock(device_mutex);
	update_force = hduVector3Dd(f.x, f.z, -f.y);
}

bool Phantom::testDevice()
{
	bool valid = false;
	auto error = hdGetError().errorCode;
	switch (error)
	{
	case HD_INVALID_ENUM: std::cout << "HD_INVALID_ENUM" << std::endl; break;
	case HD_INVALID_VALUE: std::cout << "HD_INVALID_VALUE" << std::endl; break;
	case HD_INVALID_OPERATION: std::cout << "HD_INVALID_OPERATION" << std::endl; break;
	case HD_INVALID_INPUT_TYPE: std::cout << "HD_INVALID_INPUT_TYPE" << std::endl; break;
	case HD_BAD_HANDLE: std::cout << "HD_BAD_HANDLE" << std::endl; break;

	case HD_WARM_MOTORS: std::cout << "HD_WARM_MOTORS" << std::endl; break;
	case HD_EXCEEDED_MAX_FORCE: std::cout << "HD_EXCEEDED_MAX_FORCE" << std::endl; break;
	case HD_EXCEEDED_MAX_FORCE_IMPULSE: std::cout << "HD_EXCEEDED_MAX_FORCE_IMPULSE" << std::endl; break;
	case HD_EXCEEDED_MAX_VELOCITY: std::cout << "HD_EXCEEDED_MAX_VELOCITY" << std::endl; break;
	case HD_FORCE_ERROR: std::cout << "HD_FORCE_ERROR" << std::endl; break;

	case HD_DEVICE_FAULT: std::cout << "HD_DEVICE_FAULT" << std::endl; break;
	case HD_DEVICE_ALREADY_INITIATED: std::cout << "HD_DEVICE_ALREADY_INITIATED" << std::endl; break;
	case HD_COMM_ERROR: std::cout << "HD_COMM_ERROR" << std::endl; break;
	case HD_COMM_CONFIG_ERROR: std::cout << "HD_COMM_CONFIG_ERROR" << std::endl; break;
	case HD_TIMER_ERROR: std::cout << "HD_TIMER_ERROR" << std::endl; break;

	case HD_ILLEGAL_BEGIN: std::cout << "HD_ILLEGAL_BEGIN" << std::endl; break;
	case HD_ILLEGAL_END: std::cout << "HD_ILLEGAL_END" << std::endl; break;
	case HD_FRAME_ERROR: std::cout << "HD_FRAME_ERROR" << std::endl; break;

	case HD_INVALID_PRIORITY: std::cout << "HD_INVALID_PRIORITY" << std::endl; break;
	case HD_SCHEDULER_FULL: std::cout << "HD_SCHEDULER_FULL" << std::endl; break;

	case HD_INVALID_LICENSE: std::cout << "HD_INVALID_LICENSE" << std::endl; break;

	default: break;
	}
	if (error == HD_SUCCESS) valid = true;
	return valid;
}

HDCallbackCode HDCALLBACK Phantom::monitorDevice(void *pUserData)
{
	Phantom* device = static_cast<Phantom*>(pUserData);
	std::lock_guard<std::mutex> lock(device->device_mutex);

	HDint nCurrentButtons, nLasButons;

	hdMakeCurrentDevice(device->hHD);
	hdBeginFrame(hdGetCurrentDevice());

	hdGetDoublev(HD_CURRENT_POSITION, device->position);
	hdGetDoublev(HD_CURRENT_GIMBAL_ANGLES, device->gimbal);

	hdGetIntegerv(HD_CURRENT_BUTTONS, &nCurrentButtons);
	hdGetIntegerv(HD_LAST_BUTTONS, &nLasButons);
	device->button[0] = nCurrentButtons & HD_DEVICE_BUTTON_1;
	device->button[1] = nCurrentButtons & HD_DEVICE_BUTTON_2;

	if (!(nCurrentButtons & HD_DEVICE_BUTTON_1))
	{
		if ((nLasButons & HD_DEVICE_BUTTON_1))
		{
			hduVector3Dd pos_diff;
			hduVecSubtract(pos_diff, device->position, device->start_position);
			hduVecAdd(device->offset_position, device->offset_position, pos_diff);
		}
		memcpy(device->start_position, device->position, sizeof(hduVector3Dd));
	}
	if (!(nCurrentButtons & HD_DEVICE_BUTTON_2))
	{
		if ((nLasButons & HD_DEVICE_BUTTON_2))
		{
			hduVector3Dd gim_diff;
			hduVecSubtract(gim_diff, device->gimbal, device->start_gimbal);
			hduVecAdd(device->offset_gimbal, device->offset_gimbal, gim_diff);
		}
		memcpy(device->start_gimbal, device->gimbal, sizeof(hduVector3Dd));
	}

	hdSetDoublev(HD_CURRENT_FORCE, device->update_force);

	hdEndFrame(hdGetCurrentDevice());

	if (!device->testDevice()) return HD_CALLBACK_DONE;

	return HD_CALLBACK_CONTINUE;
}
