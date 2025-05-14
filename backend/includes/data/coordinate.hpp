#pragma once

struct Coordinate {
 public:
  double GetX() const { return x_; }

  double GetY() const { return y_; }

  void SetX(double coord_x) { x_ = coord_x; }

  void SetY(double coord_y) { y_ = coord_y; }

 private:
  double x_{0.};
  double y_{0.};
};
