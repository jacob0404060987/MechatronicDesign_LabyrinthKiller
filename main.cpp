#include <hFramework.h>
#include <hModules/DistanceSensor.h>

#define IDLE_STATE 0
#define FRONT_CHECK_STATE 1
#define TURNING_LEFT_STATE 2

static bool programRun=false;
static bool stepWorkMode=false;

using namespace hModules;

DistanceSensor sens_front(hSens3);
DistanceSensor sens_front_2(hSens4);
DistanceSensor sens_right(hSens1);

const int16_t set_distance=20;
static int16_t act_distance_right;
static int16_t act_distance_left;
static int16_t diff_distanceLeft;
static int16_t diff_distanceRight;
static int16_t act_left_dist;
static int16_t act_right_dist;
static int16_t diff_side;
const int16_t base_speed = -250;

static uint8_t stepCounter;



void onPress(void) // instruction executed by clicking the buttson 
{
	if(!stepWorkMode){
	if(programRun)
    {
        programRun=false;
    }else if(!programRun)
    {
        programRun=true;
    }
	}else{
		if(stepCounter==FRONT_CHECK_STATE){
		stepCounter=TURNING_LEFT_STATE;
		}else if(stepCounter == TURNING_LEFT_STATE)
		{
		stepCounter=IDLE_STATE;
		}
	}
}

void stepWork(void)
{
	programRun=false;
	if(stepWorkMode)
    	{
   	     stepWorkMode=false;
    	}else if(!stepWorkMode)
   	 {
        stepWorkMode=true;
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
	if(diff_distanceLeft < -3 || diff_distanceRight <-3 )
	{
		return false;
	}else
	{
		return true;
	}
}

void rightCheck(void)
{
	if(act_right_dist < 16)
	{	
		motorCorrectionLeft();
	}else if(act_right_dist > 20 && act_right_dist <= 70)
	{
		motorCorrectionRight();
	}else if(act_right_dist > 70)
	{
	motorCorrectionRight();
		}
}

void regulator(const int16_t setPiont, int16_t value, uint32_t gain)
{
	static int16_t feedback; 
	feedback = setPiont - value;
	static int16_t powerDiff;
	powerDiff = feedback * gain;
	 //za blisko
		if(feedback > -15 && feedback < 15 ){
		motor_right.setPower(base_speed - powerDiff);
		motor_left.setPower(base_speed + powerDiff);
		}else if(feedback>=15)
		{
			motorCorrectionLeft();
		}else if(feedback <=-15)
		{
			motorCorrectionRight();
		}
		if(act_right_dist == -1)
		{
			motorTurnLeft();//DO SPRAWDZENIA CZY TO SIE DZIEJE
		}


	
}

void motorSpinLeft(void)
{
	while(act_distance_left<13 || act_distance_right <13 ){
			motorTurnLeft();
		
				act_distance_right = sens_front.getDistance();
				act_distance_left = sens_front_2.getDistance();
			}
}

void stepWorkStateMachine(uint8_t* ctr)
{
	switch (*ctr)
	{
	case IDLE_STATE:
		regulator(15,act_right_dist,7);
		if(frontCheck || act_distance_left ==-1 || act_distance_right ==-1)
		{
			*ctr++;
		}
		break;
	case FRONT_CHECK_STATE:
		motorStop();
		break;
	case TURNING_LEFT_STATE:
		motorSpinLeft();
		motorStop();
		break;
	default:
		break;
	}
}

void hMain()
{
	hInit();
	while (true)
	{
		
        hBtn1.setOnPressHandler(onPress);
		hBtn2.setOnPressHandler(stepWork);
		act_distance_right = sens_front.getDistance();
		act_distance_left = sens_front_2.getDistance();
		act_right_dist = sens_right.getDistance();
		diff_distanceLeft = set_distance - act_distance_left;
		diff_distanceRight = set_distance - act_distance_right;
		diff_side = act_left_dist - act_right_dist; // ujemna wtedy kiedy blizej lewej
        if(programRun && !stepWorkMode){
		if(frontCheck() || act_distance_left ==-1 || act_distance_right ==-1)
		{

				motorSpinLeft();

		}else if(!frontCheck())
		{
			
			
			regulator(15,act_right_dist,7);
		}
        }else if(!programRun && !stepWorkMode)
		{
			motorStop();
		}
		if(stepWorkMode)
		{
			stepWorkStateMachine(&stepCounter);
		}
	
		
	}
}


