#include "PIDController.h"

namespace fizix::controllers {
	PIDController::PIDController(double desired, double Kp, double Ki, double Kd)
		:_desired(desired), _Kp(Kp), _Ki(Ki), _Kd(Kd)
	{

	}

	PIDController::~PIDController()
	{

	}


	void PIDController::step(const float& dt)
	{
		this->_error = this->_desired - this->_current;
		double proportional = this->_Kp * this->_error;
		this->_integral += this->_Ki * this->_error * dt;

		double derivative = this->_Kd * (this->_error - this->_prevError) / dt;
		this->_output = proportional + this->_integral + derivative;
		this->_prevError = this->_error;
	}

	void PIDController::setDesired(double desired)
	{
		this->_desired = desired;
	}

	void PIDController::setCurrent(double current)
	{
		this->_current = current;
	}

	void PIDController::setKp(double Kp)
	{
		this->_Kp = Kp;
	}

	void PIDController::setKi(double Ki)
	{
		this->_Ki = Ki;
	}

	void PIDController::setKd(double Kd)
	{
		this->_Kd = Kd;
	}

	double PIDController::getOutput()
	{
		return this->_output;
	}
}