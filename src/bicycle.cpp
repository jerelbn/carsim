#include "bicycle.h"

namespace bicycle
{


Bicycle::Bicycle()  : t_prev_(-1) {}


Bicycle::Bicycle(const std::string &filename)
  : t_prev_(-1)
{
  load(filename);
}


Bicycle::~Bicycle() {}


void Bicycle::load(const std::string &filename)
{
  // Load all parameters
  xVector x0;
  common::get_yaml_node("name", filename, name_);
  common::get_yaml_node("mass", filename, mass_);
  common::get_yaml_node("inertia", filename, inertia_);
  common::get_yaml_node("length", filename, L_);
  common::get_yaml_node("max_force", filename, max_force_);
  common::get_yaml_node("max_torque", filename, max_torque_);
  common::get_yaml_node("max_steering_angle", filename, max_steering_angle_);
  common::get_yaml_node("drag", filename, drag_);
  common::get_yaml_eigen<xVector>("x0", filename, x0);

  u_.setZero();
  x_ = State(x0);
  dx_.setZero();
}


void Bicycle::propagate(const double &t)
{
  // Time step
  double dt = t - t_prev_;
  if (t_prev_ != t) t_prev_ = t;

  if (t > 0 && dt > 0)
  {
    // 4th order Runge-Kutta integration
    rk4(std::bind(&Bicycle::f, this,
                  std::placeholders::_1,std::placeholders::_2, std::placeholders::_3),
                  dt, x_, u_, dx_);
    x_ += dx_;

    // Wrap angles and enforce steering limits
    x_.psi = common::wrapAngle(x_.psi, M_PI);
    x_.theta = common::saturate(x_.theta, max_steering_angle_, -max_steering_angle_);
  }
}


void Bicycle::f(const State &x, const uVector& u, xVector& dx)
{
  dx(PX) = x.v * cos(x.psi);
  dx(PY) = x.v * sin(x.psi);
  dx(PZ) = 0;
  dx(PSI) = x.v * tan(x.theta) / L_;
  dx(VEL) = u(FORCE) / mass_ - drag_ * x.v;
  dx(THETA) = u(TORQUE) / inertia_;
}


} // namespace bicycle
