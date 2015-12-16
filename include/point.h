#ifndef __DASHMM_POINT_H__
#define __DASHMM_POINT_H__


/// \file include/point.h
/// \brief Point object interface.


#include <cmath>
#include <cstdlib>


namespace dashmm {


/// An object repesenting a point in 3-space
class Point {
 public:
  /// Construct a point from x, y, z positions.
  Point(double x, double y, double z) : pos_{x, y, z} { }

  /// Copy construct a point.
  Point(const Point &pt) : pos_{pt.x(), pt.y(), pt.z()} { }

  /// Scale the point location.
  ///
  /// This multiplies each coordinate of the Point by \param c and
  /// returns a point with those coordinates.
  ///
  /// \param c - the scaling factor
  ///
  /// \returns The Point scaled
  Point scale(double c) const {
    return Point{c * pos_[0], c * pos_[1], c * pos_[2]};
  }

  /// Access coordinates of the point by index.
  ///
  /// \param i - the index of interest in the range [0,2]. The input is not
  ///            range checked, and so providing an out-of-range index will
  ///            cause undefined behavior.
  ///
  /// \returns the coordinate value
  double operator[](size_t i) const {return pos_[i];}

  /// Return the x coordinate of the Point.
  double x() const {return pos_[0];}

  /// Return the y coordinate of the Point.
  double y() const {return pos_[1];}

  /// Return the z coordinate of the Point.
  double z() const {return pos_[2];}

  /// Return the Euclidean 2-norm of the Point.
  double norm() const {
    return sqrt(pos_[0] * pos_[0] + pos_[1] * pos_[1] + pos_[2] * pos_[2]);
  }

 private:
  double pos_[3];
};


/// Take the dot product of two points.
///
/// This treats the Point object as if it were a vector, and takes the dot
/// product of two Points.
///
/// \param left - one Point
/// \param right - another Point
///
/// \returns - the dot product of the two Points.
double point_dot(const Point &left, const Point &right);

/// Add two points together.
///
/// This performs a component-wise addition of two points.
///
/// \param left - one Point
/// \param right - another Point
///
/// \returns - A Point that is the sum of the inputs
Point point_add(const Point &left, const Point &right);

/// Subtract one point from another.
///
/// This performs a component-wise subtraction of two points.
///
/// \param left - the Point from which \param right is subtracted
/// \param right - the Point subtracted from \param left
///
/// \returns - A Point that is \param left - \param right
Point point_sub(const Point &left, const Point &right);


} //namespace dashmm


#endif // __DASHMM_POINT_H__