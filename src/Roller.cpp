#include <Roller.h>
#include <Arduino.h>


#define PINHIGH 1
#define PINLOW 0

#define JOG_INCREMENTS 10000



Roller::Roller(int pinUP, int pinDown, int pinSensor, bool useSensor = false){
    _pinDown = pinDown;
    _pinUP = pinUP;
    _pinSensor = pinSensor;
    _useSensor = useSensor;
    //thePID = PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, P_ON_E, DIRECT);
    //thePID.SetMode(AUTOMATIC);
};

int Roller::loop(){
    //thePID.Compute();
    switch (iState)
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
    }
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
int Roller::gotoPosition(int iGotoPosition)
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
    digitalWrite(_pinUP, PINHIGH);
    digitalWrite(_pinDown, PINLOW);

    //analogWrite(_pinUP, Output);
    //analogWrite(_pinDown, 0);
    return 0;
}

int Roller::moveDown()
{
    digitalWrite(_pinUP, PINLOW);
    digitalWrite(_pinDown, PINHIGH);

    //analogWrite(_pinUP, 0);
    //analogWrite(_pinDown, Output);
    return 0;
};

int Roller::stop(){
    //analogWrite(_pinUP, 0);
    //analogWrite(_pinDown, 0);

    digitalWrite(_pinUP, PINLOW);
    digitalWrite(_pinDown, PINLOW);
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
