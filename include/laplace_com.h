#ifndef __DASHMM_LAPLACE_COM_EXPANSION_H__
#define __DASHMM_LAPLACE_COM_EXPANSION_H__


#include <cmath>
#include <complex>
#include <memory>
#include <vector>

#include "include/expansion.h"
#include "include/ids.h"
#include "include/index.h"
#include "include/particle.h"
#include "include/point.h"


namespace dashmm {


struct LaplaceCOMData {
  int reserved;
  int type;
  double mtot;
  double xcom[3];
  double Q[6];
};


class LaplaceCOM : public Expansion {
 public:
  explicit LaplaceCOM(Point center);
  LaplaceCOM(LaplaceCOMData *ptr, size_t bytes)
      : data_{ptr}, bytes_{sizeof(LaplaceCOMData)} { }
  ~LaplaceCOM();

  void *release() override {
    LaplaceCOMData *retval = data_;
    data_ = nullptr;
    return retval;
  }
  size_t bytes() const override {return bytes_;}
  bool valid() const override {return data_ != nullptr;}
  int type() const override {return kExpansionLaplaceCOM;}
  bool provides_L() const override {return false;}
  bool provides_exp() const override {return false;}
  size_t size() const override {return 10;}
  Point center() const override {
    assert(valid());
    return Point{data_->xcom[0], data_->xcom[1], data_->xcom[2]};
  }
  std::complex<double> term(size_t i) const override;


  std::unique_ptr<Expansion> S_to_M(Point center, Source *first,
                                    Source *last) const override;
  std::unique_ptr<Expansion> S_to_L(Point center, Source *first,
                                    Source *last) const override {
    return std::unique_ptr<Expansion>{new LaplaceCOM{nullptr, 0}};
  }

  std::unique_ptr<Expansion> M_to_M(int from_child,
                                    double s_size) const override;
  std::unique_ptr<Expansion> M_to_L(Index s_index, double size,
                                    Index t_index) const override {
    return std::unique_ptr<Expansion>{new LaplaceCOM{nullptr, 0}};
  }
  std::unique_ptr<Expansion> L_to_L(int to_child,
                                    double t_size) const override {
    return std::unique_ptr<Expansion>{new LaplaceCOM{nullptr, 0}};
  }

  void M_to_T(Target *first, Target *last) const override;
  void L_to_T(Target *first, Target *last) const override {  }
  void S_to_T(Source *s_first, Source *s_last,
              Target *t_first, Target *t_last) const override;

  void add_expansion(const Expansion *temp1) override;

  std::unique_ptr<Expansion> get_new_expansion(Point center) const override {
    return std::unique_ptr<Expansion>{new LaplaceCOM{center}};
  }

  void set_mtot(double m) {data_->mtot = m;}
  void set_xcom(const double c[3]) {
    data_->xcom[0] = c[0]; data_->xcom[1] = c[1]; data_->xcom[2] = c[2];
  }
  void set_Q(const double q[6]) {
    data_->Q[0] = q[0]; data_->Q[1] = q[1]; data_->Q[2] = q[2];
    data_->Q[3] = q[3]; data_->Q[4] = q[4]; data_->Q[5] = q[5];
  }

 private:
  void calc_mtot(Source *first, Source *last);
  void calc_xcom(Source *first, Source *last);
  void calc_Q(Source *first, Source *last);

  LaplaceCOMData *data_;
  size_t bytes_;
};


} //namespace dashmm


#endif // __DASHMM_LAPLACE_COM_EXPANSION_H__