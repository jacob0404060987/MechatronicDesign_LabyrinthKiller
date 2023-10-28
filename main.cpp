#include <hFramework.h>
#include <hModules/DistanceSensor.h>

static bool programRun=false;

using namespace hModules;

DistanceSensor sens_front(hSens3);
DistanceSensor sens_left(hSens4);
DistanceSensor sens_right(hSens1);

const int16_t set_distance=10;
static int16_t act_distance=0;
static int16_t diff_distance;
static int16_t act_left_dist;
static int16_t act_right_dist;
static int16_t diff_side;


void onPress() // instruction executed by clicking the buttson 
{
	if(programRun)
    {
        programRun=false;
    }else if(!programRun)
    {
        programRun=true;
    }
}

void motorStartFwd(void)
{
	motor_right.setPower(-400);
	motor_left.setPower(-400);
}

void motorStartBwd(void)
{
	motor_right.setPower(400);
	motor_left.setPower(400);
}

void motorStop(void)
{
	motor_right.setPower(0);
	motor_left.setPower(0);
}

void motorTurnLeft(void)
{
	motor_right.setPower(-250);
	motor_left.setPower(250);
}

void motorTurnRight(void)
{
	motor_right.setPower(250);
	motor_left.setPower(-250);
}

void motorCorrectionLeft(void)
{
	motor_right.setPower(-400);
	motor_left.setPower(-100);
}

void motorCorrectionRight(void)
{
	motor_right.setPower(-100);
	motor_left.setPower(-400);
}
void hInit(void)
{

}



void pathCorrection(void)
{
	if(diff_side < -5 )
		{
			motorCorrectionRight();
		}else if (diff_side > 5 )
		{
			motorCorrectionLeft();
		}else
		{
			motorStartFwd();
		}
}


bool frontCheck(void)
{
	if(diff_distance < -3)
	{
		return false;
	}else
	{
		return true;
	}
}

void rightCheck(void)
{
	if(act_right_dist < 10)
	{	
		motorCorrectionLeft();
	}else if(act_right_dist > 20)
	{
		motorCorrectionRight();
	}else
	{
		motorStartFwd();
	}
}


void hMain()
{
	hInit();
	while (true)
	{
		sys.delay(20);
        hBtn1.setOnPressHandler(onPress);
		act_distance = sens_front.getDistance();
		act_left_dist = sens_left.getDistance();
		act_right_dist = sens_right.getDistance();
		diff_distance = set_distance - act_distance;
		diff_side = act_left_dist - act_right_dist; // ujemna wtedy kiedy blizej lewej
        if(programRun){
		if(frontCheck())
		{
			motorStop();
			sys.delay(20);
			motorTurnLeft();
			sys.delay(1000);
		}else if(!frontCheck())
		{
			//pathCorrection();
			rightCheck();
		}
        }else if(!programRun)
		{
			motorStop();
		}
		Serial.printf("Left: %5d, Right: %5d, Front: %5d \n",act_left_dist, act_right_dist, act_distance);
	}
}


