#include <PID_v1.h>

class Roller{

    protected:
        int _pinUP;
        int _pinDown;
        int _pinSensor;

        int _maxPosition;
        int _minPosition;
        double _encoderPosition;

        double _targetPosition;

        bool _useSensor;
        enum { _waiting, _movingUp, _movingDown } iState = _waiting;

        double Kp=0.2, Ki=0.2, Kd=0.01;
        double Setpoint, Output;
        
        PID thePID;

    public:
        
        Roller(int pinUP, int pinDown, int pinSensor, bool useSensor);

        int loop();
        int getCurrentPosition();
        int setInitialPosition(int iNewPosition);
        int updatePosition(int iEncoderValue);
        int setMaxPosition(int iNewMaxPosition);
        int setMinPosition(int iNewMinPosition);
        
        int gotoPosition(double iGotoPosition);

        int moveUp();
        int moveDown();
        int stop();

        int openRoller();
        int closeRoller();
        int jogUp();
        int jogDown();
        int setMaxPosition();
        int setMinPosition();

        double getOutput();
        double getTargetPosition();
        void setTuning(double kp, double ki, double kd);

};