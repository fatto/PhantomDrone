//#include <Windows.h>

#ifdef  _WIN64
#pragma warning (disable:4996)
#endif

#include <cstdio>
#include <iostream>

#include <thread>

//#include <HD/hd.h>
//#include <HDU/hduError.h>
//#include <HDU/hduVector.h>

//extern "C" {
//    #include "extApi.h"
///*	#include "extApiCustom.h" if you wanna use custom remote API functions! */
//}
//
//static int portNb = 19997;


//#include "Phantom.hpp"
//#include "XInput.hpp"
#include "Drone.hpp"
#include "Controller.hpp"
#include "ArrayMath.hpp"
#include "Pad.hpp"
#include <cmath>

//static HDdouble gSpringStiffness = 0.05;
//static HDdouble gMaxStiffness = 1.0;
//static HDdouble gStiffnessIncrement = 0.025;
//
//HDSchedulerHandle gCallbackHandle = 0;
//
//void mainLoop();
//HDCallbackCode HDCALLBACK SetSpringStiffnessCallback(void *pUserData);
//HDCallbackCode HDCALLBACK AnchoredSpringForceCallback(void *pUserData);

/******************************************************************************
 Main function.
******************************************************************************/
int main(int argc, char* argv[])
{
//	vec4_simd<float> one(1.0f, 0.0f, 0.0f, 0.0f), two(0.0f, 1.0f, 0.0f, 0.0f);
	
//	auto d = dot(one, two);
//	vec4_simd<float> three(2.0f, 0.0, 0.0f, 0.0f);
//	auto dd = dot(one, three);
//	std::cout << std::endl;

	vec4 banana = {1.f, 1.f, 1.f, 0.f};
	vec4 limoni = banana * banana;
	banana /= limoni;
	
	Pad pd;
	
	while(true)
	{
		auto status = pd.Status();
		if(status.button[0])
			std::cout << status.position[2] << std::endl;
	}
//	one += two;
//	one += 1.0f;
//	vec4_simd<float> three = one*1.0f + (one*two) + (one / two);
//	two = one + vec4_simd<float>(0.0f, 0.0f, 0.0f, 1.0f);
//	two = one;
//	auto compa = all(eq(one,two));
//	if (compa)
//		std::cout << "one > two" << std::endl;
//	else
//		std::cout << "one <= two" << std::endl;
//	two = one;
	return 0;
	/*int clientID = simxStart((simxChar*)"127.0.0.1", portNb, 1, 1, 2000, 5);
	if(clientID == -1)
	{

		return -1;
	}*/

	// Phantom phantom;
//	XInput pad;
//	Drone drone;
//	Controller controller(drone);
//
//	short keystate;
//	auto destination = std::array<double, 3>{ 0.0, 0.0, 0.8};
//	//auto last_displ = std::array<double, 3>{0.0, 0.0, 0.0};
//	auto angle = std::array<double, 3>{0.0, 0.0, 0.0};
//	//auto last_angle = 0.0;
//	bool update_destination = false, update_angle = false;
//	do
//	{
//		/*keystate = GetAsyncKeyState(VK_CONTROL);*/ // VK_ESCAPE
//		keystate = GetAsyncKeyState(VK_ESCAPE);
//		std::chrono::milliseconds dura(2);
//		//std::this_thread::sleep_for(dura);
//
//		//auto pos = test.Position();
//		// auto haptic_status = phantom.Status();
//		auto haptic_status = pad.Status();
//		auto drone_status = drone.Status();
//		// auto displ = std::array<double, 3>{ haptic_status.position[0] * 0.01, haptic_status.position[1] * 0.01, haptic_status.position[2] * 0.01};
//		// auto displ = std::array<double, 3>{ haptic_status.position[0], haptic_status.position[1], haptic_status.position[2]};
//		std::array<double, 3> dest;
//		std::array<double, 3> ang;
//
//		dest = destination + haptic_status.position;
//		ang = angle + haptic_status.gimbal;
//		// if (haptic_status.button[0])
//		// {
//		// 	dest = destination + displ;
//		// 	last_displ = displ;
//		// 	update_destination = true;
//		// }
//		// else if (update_destination)
//		// {
//		// 	destination = destination + last_displ;
//		// 	dest = destination;
//		// 	update_destination = false;
//		// }
//		// else
//		// {
//		// 	dest = destination;
//		// }
//
//		// if (haptic_status.button[1])
//		// {
//		// 	ang = angle + haptic_status.gimbal[2];
//		// 	last_angle = haptic_status.gimbal[2];
//		// 	update_angle = true;
//		// }
//		// else if (update_angle)
//		// {
//		// 	angle = angle + last_angle;
//		// 	ang = angle;
//		// 	update_angle = false;
//		// }
//		// else
//		// {
//		// 	ang = angle;
//		// }
//		//displ[0] *= 0.05;
//		//displ[1] *= 0.05;
//		//displ[2] *= 0.05;
//		//std::cout << pos[0] << " " << pos[1] << " " << pos[2] << " " << displ[0] << " " << displ[1] << " " << displ[2] << std::endl;
//		//test.Force(displ);
//		//float force = float(displ[1])*2.f;
//		/*drone.Apply({ force, force, force, force });*/
//		/*controller.Go({0.0f, 0.0f, 0.0138f});*/
//		//controller.Go({ 0.0f, 0.0f, 0.8f });
//		//destination = destination + std::array<float,3>{ float(displ[0]*0.01), float(displ[1] * 0.01), float(displ[2] * 0.01)};
//		//auto destination = std::array<float, 3>{ 0.f, 0.f, 0.8f };
//		//std::cout << destination[0] << " " << destination[1] << " " << destination[2] << " " << haptic_status.gimbal[2] << std::endl;
//		//controller.Go(destination + std::array<float, 3>{ float(displ[0] * 0.01), float(displ[1] * 0.01), float(displ[2] * 0.01)}, float(haptic_status.gimbal[2]));
//		//test.Pool();
//
//		//std::cout << "x " << dest[0] << " y " << dest[1] << " z " << dest[2] << std::endl;
//		controller.Go(dest, ang );
//		drone_status = drone.Status();
//		//phantom.Force(drone_status.reaction);
//	} while (!((1 << 16) & keystate));
//
//
//	//HDErrorInfo error;
//
//	//HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);
//	//if (HD_DEVICE_ERROR(error = hdGetError()))
//	//{
//	//    //hduPrintError(stderr, &error, "Failed to initialize haptic device");
//
//	//    fprintf(stderr, "\nPress any key to quit.\n");
//	//    return -1;
//	//}
//
//	//printf("Anchored Spring Force Example\n");
//
//	///* Schedule the haptic callback function for continuously monitoring the
//	//   button state and rendering the anchored spring force. */
//	//gCallbackHandle = hdScheduleAsynchronous(
//	//    AnchoredSpringForceCallback, 0, HD_MAX_SCHEDULER_PRIORITY);
//
//	//hdEnable(HD_FORCE_OUTPUT);
//
//	///* Query the max closed loop control stiffness that the device
//	//   can handle. Using a value beyond this limit will likely cause the 
//	//   device to buzz. */
//	//hdGetDoublev(HD_NOMINAL_MAX_STIFFNESS, &gMaxStiffness);
//
//	///* Start the haptic rendering loop. */
//	//hdStartScheduler();
//	//if (HD_DEVICE_ERROR(error = hdGetError()))
//	//{
//	//    //hduPrintError(stderr, &error, "Failed to start scheduler");
//	//    fprintf(stderr, "\nPress any key to quit.\n");
//	//    return -1;
//	//}
//
//	///* Start the main application loop. */
//	//mainLoop();
//
//	///* Cleanup by stopping the haptics loop, unscheduling the asynchronous
//	//   callback, disabling the device. */
//	//hdStopScheduler();
//	//hdUnschedule(gCallbackHandle);
//	//hdDisableDevice(hHD);

	return 0;
}

/******************************************************************************
 Main loop.
 Detects and interprets keypresses.  Monitors and initiates error recovery if
 necessary.
******************************************************************************/
//void mainLoop()
//{
//    HDdouble stiffness = gSpringStiffness;
//    int keypress;
//
//    printf("\nPress button and drag to feel anchored spring force.\n");
//    printf("Press +/- to modify spring stiffness or press Q to quit.\n\n");
//
//    printf("Spring stiffness: %.3f N/mm\n", stiffness);
//
//    while (HD_TRUE)
//    {       
//        /* Check for keyboard input. */
//        //if (_kbhit())
//        //{
//        //    keypress = getch();
//        //    keypress = toupper(keypress);
//        //    
//        //    if (keypress == 'Q')
//        //    {
//        //        return;
//        //    }
//        //    else if (keypress == '+' || keypress == '=')
//        //    {
//        //        stiffness += gStiffnessIncrement;
//        //        if (stiffness > gMaxStiffness)
//        //            stiffness = gMaxStiffness;
//        //        
//        //            /* Make a thread-safe call to change the spring stiffness 
//        //               being used for rendering forces. */
//        //        hdScheduleSynchronous(SetSpringStiffnessCallback, &stiffness,
//        //            HD_DEFAULT_SCHEDULER_PRIORITY);
//        //        
//        //        printf("Spring stiffness: %.3f N/mm\n", stiffness);
//        //    }
//        //    else if (keypress == '-' || keypress == '_')
//        //    {
//        //        stiffness -= gStiffnessIncrement;
//        //        if (stiffness < 0)
//        //            stiffness = 0;
//        //        
//        //            /* Make a thread-safe call to change the spring stiffness 
//        //               being used for rendering forces. */
//        //        hdScheduleSynchronous(SetSpringStiffnessCallback, &stiffness,
//        //            HD_DEFAULT_SCHEDULER_PRIORITY);            
//        //        
//        //        printf("Spring stiffness: %.3f N/mm\n", stiffness);
//        //    }
//        //    else
//        //    {
//        //        printf("\nInvalid keypress\n");
//        //        printf("Press +/- to modify spring stiffness or press Q to quit.\n\n");
//        //    }
//        //}
//
//        /* Check if the main scheduler callback has exited. */
//        if (!hdWaitForCompletion(gCallbackHandle, HD_WAIT_CHECK_STATUS))
//        {
//            fprintf(stderr, "\nThe main scheduler callback has exited\n");
//            fprintf(stderr, "\nPress any key to quit.\n");
//            return;
//        }
//    }
//}
//
//
///******************************************************************************
// * Main scheduler callback for rendering the anchored spring force.
// *****************************************************************************/
//HDCallbackCode HDCALLBACK AnchoredSpringForceCallback(void *pUserData)
//{
//    static hduVector3Dd anchor;
//    static HDboolean bRenderForce = HD_FALSE;
//    HDErrorInfo error;
//
//    HDint nCurrentButtons, nLastButtons;
//    hduVector3Dd position;
//	hduVector3Dd force = { 0, 0, 0 };
//
//    hdBeginFrame(hdGetCurrentDevice());
//
//    hdGetDoublev(HD_CURRENT_POSITION, position);
//    
//    hdGetIntegerv(HD_CURRENT_BUTTONS, &nCurrentButtons);
//    hdGetIntegerv(HD_LAST_BUTTONS, &nLastButtons);
//
//    if ((nCurrentButtons & HD_DEVICE_BUTTON_1) != 0 &&
//        (nLastButtons & HD_DEVICE_BUTTON_1) == 0)
//    {
//        /* Detected button down */
//        memcpy(anchor, position, sizeof(hduVector3Dd));
//        bRenderForce = HD_TRUE;
//    }
//    else if ((nCurrentButtons & HD_DEVICE_BUTTON_1) == 0 &&
//             (nLastButtons & HD_DEVICE_BUTTON_1) != 0)
//
//    {
//        /* Detected button up */
//        bRenderForce = HD_FALSE;
//
//        /* Send zero force to the device, or else it will just continue
//           rendering the last force sent */
//        hdSetDoublev(HD_CURRENT_FORCE, force);
//    }
//
//    if (bRenderForce)
//    {
//        /* Compute spring force as F = k * (anchor - pos), which will attract
//           the device position towards the anchor position */
//        hduVecSubtract(force, anchor, position);
//        hduVecScaleInPlace(force, gSpringStiffness);
//                
//        hdSetDoublev(HD_CURRENT_FORCE, force);
//    }
//
//    hdEndFrame(hdGetCurrentDevice());
//
//    /* Check if an error occurred while attempting to render the force */
//    if (HD_DEVICE_ERROR(error = hdGetError()))
//    {
//        //if (hduIsForceError(&error))
//        //{
//        //    bRenderForce = HD_FALSE;
//        //}
//        //else if (hduIsSchedulerError(&error))
//        //{
//            return HD_CALLBACK_DONE;
//        //}
//    }
//
//    return HD_CALLBACK_CONTINUE;
//}
//
///******************************************************************************
// * Scheduler callback for synchronously changing the stiffness value being
//   used by the anchored spring force.
// *****************************************************************************/
//HDCallbackCode HDCALLBACK SetSpringStiffnessCallback(void *pUserData)
//{
//    HDdouble *pStiffness = (HDdouble *) pUserData;
//        
//    gSpringStiffness = *pStiffness;        
//
//    return HD_CALLBACK_DONE;    
//}

/*****************************************************************************/
