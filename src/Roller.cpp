#include <Roller.h>
#include <Arduino.h>


#define PINHIGH 1
#define PINLOW 0

#define JOG_INCREMENTS 10000


Roller::Roller(int pinUP, int pinDown, int pinSensor, bool useSensor){
    _pinDown = pinDown;
    _pinUP = pinUP;
    _pinSensor = pinSensor;
    _useSensor = useSensor;
    thePID = PID(&_encoderPosition, &Output, &_targetPosition, Kp, Ki, Kd, P_ON_E, DIRECT);
    thePID.SetMode(AUTOMATIC);
    thePID.SetSampleTime(10);
    thePID.SetOutputLimits(-255,255);
}


int Roller::loop(){
    thePID.Compute();
    if(abs(_targetPosition - _encoderPosition) < 100) Output = 0;
        if(Output > 0){
            analogWrite(_pinUP, abs(Output));
            analogWrite(_pinDown, 0);
        }else{
            analogWrite(_pinUP, 0);
            analogWrite(_pinDown, abs(Output));

        }
    
    /*switch (iState)
    {
    case _waiting:
        stop();
        break;
    case _movingUp:
        if(_encoderPosition >= _targetPosition){
            iState = _waiting;
            stop();
        }else{
            moveUp();
        }
        
        break;
    case _movingDown:
        
        if(_encoderPosition <= _targetPosition){
            iState = _waiting;
            stop();
        }else{
            moveDown();
        }
        
        break;
    
    default:
        iState = _waiting;
        break;
    }*/
    return 0;
}
int Roller::getCurrentPosition()
{
    return _encoderPosition;
}
int Roller::setInitialPosition(int iNewPosition)
{
    _encoderPosition = iNewPosition;
    return 0;
}
int Roller::updatePosition(int iEncoderValue)
{
    _encoderPosition  = iEncoderValue;
    return 0;
}
int Roller::setMaxPosition(int iNewMaxPosition)
{
    _maxPosition = iNewMaxPosition;
    return 0;
}
int Roller::setMinPosition(int iNewMinPosition)
{
    _minPosition = iNewMinPosition;
    return 0;
}
int Roller::gotoPosition(double iGotoPosition)
{
    _targetPosition = iGotoPosition;
    if(_targetPosition > _encoderPosition){
        iState = _movingUp;
    }else{
        iState = _movingDown;
    }
    return 0;
}
int Roller::moveUp()
{
    //digitalWrite(_pinUP, PINHIGH);
    //digitalWrite(_pinDown, PINLOW);

    analogWrite(_pinUP, abs(Output));
    analogWrite(_pinDown, 0);
    return 0;
}

int Roller::moveDown()
{
    //digitalWrite(_pinUP, PINLOW);
    //digitalWrite(_pinDown, PINHIGH);

    analogWrite(_pinUP, 0);
    analogWrite(_pinDown, abs(Output));
    return 0;
}

int Roller::stop(){
    analogWrite(_pinUP, 0);
    analogWrite(_pinDown, 0);

    //digitalWrite(_pinUP, PINLOW);
    //digitalWrite(_pinDown, PINLOW);
    _targetPosition = _encoderPosition;
    return 0;
}

int Roller::openRoller()
{
    return gotoPosition(_maxPosition);
}

int Roller::closeRoller()
{
    return gotoPosition(_minPosition);
}

int Roller::jogUp()
{
    return gotoPosition(_encoderPosition + JOG_INCREMENTS);
}

int Roller::jogDown()
{
    return gotoPosition(_encoderPosition - JOG_INCREMENTS);
}

int Roller::setMaxPosition()
{
    _maxPosition = _encoderPosition;
    return 0;
}
int Roller::setMinPosition()
{
    _minPosition = _encoderPosition;
    return 0;
}

double Roller::getOutput()
{
    return Output;
}

double Roller::getTargetPosition(){
    return _targetPosition;
}

void Roller::setTuning(double kp, double ki, double kd)
{
    Kp = kp;
    Kd = kd;
    Ki = ki;
    thePID.SetTunings(Kp,Ki,Kd);
}
