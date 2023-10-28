#include <hFramework.h>
#include <hModules/DistanceSensor.h>

static bool programRun=false;

using namespace hModules;

DistanceSensor sens_front(hSens3);
DistanceSensor sens_left(hSens4);
DistanceSensor sens_right(hSens1);

const int16_t set_distance=30;
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
	motor_right.setPower(-300);
	motor_left.setPower(-300);
}

void motorStartBwd(void)
{
	motor_right.setPower(300);
	motor_left.setPower(300);
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
	motor_right.setPower(450);
	motor_left.setPower(-450);
}

void motorCorrectionLeft(void)
{
	motor_right.setPower(-350);
	motor_left.setPower(-150);
}

void motorCorrectionRight(void)
{
	motor_right.setPower(-150);
	motor_left.setPower(-350);
}
void hInit(void)
{

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
		/*if(diff_distance>5)
		{
			motorStartBwd();
		}else if(diff_distance<-5)
		{
			motorStartFwd();
		}else{
		motorTurnRight();
		sys.delay(1000);*/
		if(diff_side < -5)
		{
			motorCorrectionRight();
		}else if (diff_side > 5)
		{
			motorCorrectionLeft();
		}else
		{
			motorStartFwd();
		}
        }else if(!programRun)
		{
			motorStop();
		}
		Serial.printf("Left: %5d, Right: %5d, Front: %5d \n",act_left_dist, act_right_dist, act_distance);
	}
}


