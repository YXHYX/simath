#pragma once

#include <tools.h>

namespace fizix::controllers{
	///MPC, PID, quadradic programming, dynamic programming, control barrier functions, Lyapunov control functions, Lyapunov barrier functions, H infinite
	class PIDController
	{
	private:
		double _Kp = 0.0;
		double _Ki = .0;
		double _Kd = .0;

		double _integral = 0;
		double _prevError = 0;
		double _output = 0;
		double _error = 0;
		double _current = 0;
		double _desired;
	public:
		PIDController(double desired, double Kp = 1.0, double Ki = 1.0, double Kd = 1.0);
		~PIDController();

		void step(const float&dt);

		void setDesired(double desired);
		void setCurrent(double current);

		void setKp(double Kp);
		void setKi(double Ki);
		void setKd(double Kd);

		double getOutput();
	};

}