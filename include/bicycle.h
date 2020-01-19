#pragma once

#include "common_cpp/common.h"
#include "common_cpp/quaternion.h"


namespace bicycle
{


// State indices
enum
{
  PX,
  PY,
  PZ,
  VEL,
  PSI,
  THETA,
  NUM_STATES
};

// Input indices
enum
{
  FORCE,
  TORQUE,
  COMMAND_SIZE
};

// Convenient definitions
typedef Eigen::Matrix<double, NUM_STATES, 1> xVector, dxVector;
typedef Eigen::Matrix<double, COMMAND_SIZE, 1> uVector;

struct State
{

  Eigen::Vector3d p;
  double v;
  double psi; // heading
  double theta; // steering angle

  State()
  {
    p.setZero();
    v = 0;
    psi = 0;
    theta =0;
  }

  State(const xVector &x0)
  {
    p = x0.segment<3>(PX);
    v = x0(VEL);
    psi = x0(PSI);
    theta = x0(THETA);
  }

  State operator+(const dxVector &delta) const
  {
    State x;
    x.p = p + delta.segment<3>(PX);
    x.v = v + delta(VEL);
    x.psi = psi + delta(PSI);
    x.theta = theta + delta(THETA);
    return x;
  }

  void operator+=(const dxVector &delta)
  {
    *this = *this + delta;
  }

  Eigen::Matrix<double, NUM_STATES, 1> toEigen() const
  {
    Eigen::Matrix<double, NUM_STATES, 1> x;
    x << p, v, psi, theta;
    return x;
  }

};

inline void rk4(std::function<void(const State&, const uVector&, dxVector&)> f,
                                   const double& dt, const State& x, const uVector& u, dxVector& dx)
{
  dxVector k1, k2, k3, k4;
  f(x, u, k1);
  f(x + k1 * dt / 2, u, k2);
  f(x + k2 * dt / 2, u, k3);
  f(x + k3 * dt, u, k4);
  dx = (k1 + 2 * k2 + 2 * k3 + k4) * dt / 6.0;
}


class Bicycle
{

public:

  Bicycle();
  Bicycle(const std::string &filename);
  ~Bicycle();

  void load(const std::string &filename);
  void propagate(const double &t);
  
  const std::string& name() const { return name_; }
  const State& state() const { return x_; }
  const double& x() const { return x_.p(0); }
  const double& y() const { return x_.p(1); }
  const double& psi() const { return x_.psi; }
  const double& theta() const { return x_.theta; }
  double& force() { return u_(0); }
  double& torque() { return u_(1); }
  const double& max_force() const { return max_force_; }
  const double& max_torque() const { return max_torque_; }


private:

  void f(const State& x, const uVector& u, xVector& dx);

  std::string name_;
  uVector u_;
  State x_;
  dxVector dx_;

  double t_prev_;
  double mass_;
  double inertia_;
  double L_;
  double max_force_;
  double max_torque_;
  double max_steering_angle_;
  double drag_;

};


} // namespace bicycle
