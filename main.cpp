#include <hFramework.h>
#include <hModules/DistanceSensor.h>

static bool programRun=false;

using namespace hModules;

DistanceSensor sens_front(hSens3);
DistanceSensor sens_front_2(hSens4);
DistanceSensor sens_right(hSens1);

const int16_t set_distance=12;
static int16_t act_distance_right;
static int16_t act_distance_left;
static int16_t diff_distanceLeft;
static int16_t diff_distanceRight;
static int16_t act_left_dist;
static int16_t act_right_dist;
static int16_t diff_side;
const int16_t base_speed = -250;


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
	motor_right.setPower(-250);
	motor_left.setPower(-250);
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
	if(diff_distanceLeft < 0 || diff_distanceRight < 0)
	{
		return false;
	}else
	{
		return true;
	}
}


void regulator(const int16_t setPiont, int16_t value, uint32_t gain)
{
	static int16_t feedback; 
	feedback = setPiont - value;
	static int16_t powerDiff;
	if(feedback > -5 && feedback < 5)
	{
		gain*=5;
	}
	powerDiff = feedback * gain;
	 //za blisko
		if(feedback > -25 && feedback < 25 ){
		motor_right.setPower(base_speed - powerDiff);
		motor_left.setPower(base_speed + powerDiff);
		}else if(feedback>=25)
		{
			motorCorrectionLeft();
		}else if(feedback <=-25)
		{
			motorCorrectionRight();
		}


	
}

void hMain()
{
	
	while (true)
	{
		//sys.delay(20);
        hBtn1.setOnPressHandler(onPress);
		act_distance_right = sens_front.getDistance();
		act_distance_left = sens_front_2.getDistance();
		act_right_dist = sens_right.getDistance();
		diff_distanceLeft = set_distance - act_distance_left;
		diff_distanceRight = set_distance - act_distance_right;
		diff_side = act_left_dist - act_right_dist; // ujemna wtedy kiedy blizej lewej
        if(programRun){
		if(!frontCheck())
		{
			regulator(10,act_right_dist,7);
		}else
		{	
			motorTurnLeft();
		}
        }else if(!programRun)
		{
			motorStop();
		}
		Serial.printf("Left: %5d, Right: %5d, Front: %5d \n",act_distance_left, act_distance_right, act_right_dist);
	}
}


