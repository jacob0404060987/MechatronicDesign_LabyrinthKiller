#include <hFramework.h>

static bool programRun=false;


void encoder()
{
	while (true)
	{
		Serial.printf("pos: %d\r\n", hMot1.getEncoderCnt()); // print the current position of Motor 1 (no. of encoder ticks)
		hLED1.toggle();
		sys.delay(100);
	}
}

void onPress() // instruction executed by clicking the button 
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
	motor_right.setPower(300);
	motor_left.setPower(300);
}

void motorStartBwd(void)
{
	motor_right.setPower(-300);
	motor_left.setPower(-300);
}

void motorStop(void)
{
	motor_right.setPower(0);
	motor_left.setPower(0);
}

void motorTurnRight(void)
{
	motor_right.setPower(-300);
	motor_left.setPower(300);
}

void motorTurnLeft(void)
{
	motor_right.setPower(300);
	motor_left.setPower(-300);
}
void hMain()
{
	sys.taskCreate(encoder); // this creates a task that will execute `encoder` concurrently
	while (true)
	{
        hBtn1.setOnPressHandler(onPress);
        if(programRun){
		motorStartFwd();
		sys.delay(2000);
		motorStartBwd();
		sys.delay(2000);
		motorTurnLeft();
		sys.delay(2000);
		motorTurnRight();
		sys.delay(2000);
        }else if(!programRun)
		{
			motorStop();
		}
	}
}


