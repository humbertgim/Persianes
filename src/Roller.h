#include <PID_v1.h>

class Roller{

    protected:
        int _pinUP;
        int _pinDown;
        int _pinSensor;

        int _maxPosition;
        int _minPosition;
        int _encoderPosition;

        int _targetPosition;

        bool _useSensor;
        enum { _waiting, _movingUp, _movingDown } iState = _waiting;

        double Kp=2, Ki=5, Kd=1;
        double Setpoint, Input, Output;
        //PID thePID;

    public:
        Roller(int pinUP, int pinDown, int pinSensor, bool useSensor);
        int loop();
        int getCurrentPosition();
        int setInitialPosition(int iNewPosition);
        int updatePosition(int iEncoderValue);
        int setMaxPosition(int iNewMaxPosition);
        int setMinPosition(int iNewMinPosition);
        
        int gotoPosition(int iGotoPosition);

        int moveUp();
        int moveDown();
        int stop();

        int openRoller();
        int closeRoller();
        int jogUp();
        int jogDown();
        int setMaxPosition();
        int setMinPosition();

};