// =============================================================================
//  Dynamic Adaptive System for Hierarchical Multipole Methods (DASHMM)
//
//  Copyright (c) 2015-2017, Trustees of Indiana University,
//  All rights reserved.
//
//  This software may be modified and distributed under the terms of the BSD
//  license. See the LICENSE file for details.
//
//  This software was created at the Indiana University Center for Research in
//  Extreme Scale Technologies (CREST).
// =============================================================================


/// \file
/// \brief Implementation of precomputed tables for Laplace


#include "builtins/laplace_table.h"


namespace dashmm {


std::unique_ptr<LaplaceTable> builtin_laplace_table_;


LaplaceTable::LaplaceTable(int n_digits, double size) {
  n_digits_ = n_digits; 
  size_ = size; 
  scale_ = 1.0 / size;
  int expan_length[] = {0, 4, 7, 9, 13, 16, 18, 23, 26, 29,
                        33, 36, 40, 43, 46};
  p_ = expan_length[n_digits];
  generate_sqf();
  generate_sqbinom();
  generate_wigner_dmatrix();

  switch (n_digits) {
  case 3:
    s_ = 9;
    nexp_ = 57;
    lambda_ = new double[9]{0.99273996739714473469540223504736787e-01,
                            0.47725674637049431137114652301534079e+00,
                            0.10553366138218296388373573790886439e+01,
                            0.17675934335400844688024335482623428e+01,
                            0.25734262935147067530294862081063911e+01,
                            0.34482433920158257478760788217186928e+01,
                            0.43768098355472631055818055756390095e+01,
                            0.53489575720546005399569367000367492e+01,
                            0.63576578531337464283978988532908261e+01};
    weight_ = new double[9]{0.24776441819008371281185532097879332e+00,
                            0.49188566500464336872511239562300034e+00,
                            0.65378749137677805158830324216978624e+00,
                            0.76433038408784093054038066838984378e+00,
                            0.84376180565628111640563702167128213e+00,
                            0.90445883985098263213586733400006779e+00,
                            0.95378613136833456653818075210438110e+00,
                            0.99670261613218547047665651916759089e+00,
                            0.10429422730252668749528766056755558e+01};
    m_ = new int[9]{4, 8, 12, 16, 20, 20, 24, 8, 2};
    sm_ = new int[10]{0, 2, 6, 12, 20, 30, 40, 52, 56, 57};
    f_ = new int[9]{2, 3, 4, 5, 5, 4, 5, 4, 1};
    smf_ = new int[10]{0, 4, 16, 40, 80, 130, 170, 230, 246, 247};
    break;

  case 6:
    s_ = 18;
    nexp_ = 284;
    lambda_ = new double[18]{0.52788527661177607475107009804560221e-01,
                             0.26949859838931256028615734976483509e+00,
                             0.63220353174689392083962502510985360e+00,
                             0.11130756427760852833586113774799742e+01,
                             0.16893949614021379623807206371566281e+01,
                             0.23437620046953044905535534780938178e+01,
                             0.30626998290780611533534738555317745e+01,
                             0.38356294126529686394633245072327554e+01,
                             0.46542473432156272750148673367220908e+01,
                             0.55120938659358147404532246582675725e+01,
                             0.64042126837727888499784967279992998e+01,
                             0.73268800190617540124549122992902994e+01,
                             0.82774009925823861522076185792684555e+01,
                             0.92539718060248947750778825138695538e+01,
                             0.10255602723746401139237605093512684e+02,
                             0.11282088297877740146191172243561596e+02,
                             0.12334067909676926788620221486780792e+02,
                             0.13414920240172401477707353478763252e+02};
    weight_ = new double[18]{0.13438265914335215112096477696468355e+00,
                             0.29457752727395436487256574764614925e+00,
                             0.42607819361148618897416895379137713e+00,
                             0.53189220776549905878027857397682965e+00,
                             0.61787306245538586857435348065337166e+00,
                             0.68863156078905074508611505734734237e+00,
                             0.74749099381426187260757387775811367e+00,
                             0.79699192718599998208617307682288811e+00,
                             0.83917454386997591964103548889397644e+00,
                             0.87570092283745315508980411323136650e+00,
                             0.90792943590067498593754180546966381e+00,
                             0.93698393742461816291466902839601971e+00,
                             0.96382546688788062194674921556725167e+00,
                             0.98932985769673820186653756536543369e+00,
                             0.10143828459791703888726033255807124e+01,
                             0.10400365437416452252250564924906939e+01,
                             0.10681548926956736522697610780596733e+01,
                             0.11090758097553685690428437737864442e+01};
    m_ = new int[18]{6, 8, 12, 16, 20, 26, 30, 34, 38, 44, 48, 52,
                     56, 60, 60, 52, 4, 2};
    sm_ = new int[19]{0, 3, 7, 13, 21, 31, 44, 59, 76, 95, 117, 141,
                     167, 195, 225, 255, 281, 283, 284};
    f_ = new int[18]{3, 5, 6, 7, 7, 8, 9, 9, 10, 10,
                     11, 11, 11, 11, 11, 11, 7, 1};
    smf_ = new int[19]{0, 9, 29, 65, 121, 191, 295, 430, 583, 773, 993,
                      1257, 1543, 1851, 2181, 2511, 2797, 2811, 2812};

  case 9:
    s_ = 27; 
    nexp_ = 699;
    lambda_ = new double[27]{0.35834993247954083361861421508365311e-01, 
                             0.18605352495124757861155728733137948e+00,
                             0.44642194330546725034025712375296280e+00,
                             0.80414605853435716653621057048439980e+00,
                             0.12462239924414626468518463298096322e+01,
                             0.17611257461239471222569363817456178e+01,
                             0.23391453785205094106913747964426875e+01,
                             0.29722080942457780317056403873721138e+01,
                             0.36535650947301854252202701900387183e+01,
                             0.43775326951870896508012265258003026e+01,
                             0.51393010711012010460763121955096722e+01,
                             0.59347963207816585295972799940500408e+01,
                             0.67605758319435445002909546019509435e+01,
                             0.76137438021356000916739503736607730e+01,
                             0.84918804165657331139982488821260631e+01,
                             0.93929821745587691594892021385021508e+01,
                             0.10315412711899460518338855763431638e+02,
                             0.11257864229560141211550217121839523e+02,
                             0.12219330080395609527954547957051545e+02,
                             0.13199089643751765521528795943595469e+02,
                             0.14196707628360135444722800457384437e+02,
                             0.15212051628819105886236684455070645e+02,
                             0.16245334166526408381514556822367013e+02,
                             0.17297187705846923222452460322529078e+02,
                             0.18368783996172027173088281415402889e+02,
                             0.19462114300213894324542707181535661e+02,
                             0.20582673852051787122263704077340662e+02};
    weight_ = new double[27]{0.91634452557743162337544617912499234e-01,
                             0.20725798653776011937210910218709614e+00,
                             0.31128301536359448409285732850548811e+00,
                             0.40197254484414085551335915624804329e+00,
                             0.48025776040426659818649568478576839e+00,
                             0.54793227641803410321585943165700883e+00,
                             0.60676658418922713167376059573143721e+00,
                             0.65823842224192508076896501734154299e+00,
                             0.70353143374483517735740178977721371e+00,
                             0.74360437862720618618084245099453256e+00,
                             0.77925510412986809249957786960294470e+00,
                             0.81116302338313539532776985652162693e+00,
                             0.83991454783642205406124503497267142e+00,
                             0.86601910131774351153666202662861906e+00,
                             0.88992107452719526605733335600234568e+00,
                             0.91201046747319902685546821885509416e+00,
                             0.93263337746385499560375365035724826e+00,
                             0.95210285562789409574691035231808200e+00,
                             0.97071063776060639849418976154993288e+00,
                             0.98874065527360133209811010601697490e+00,
                             0.10064860790234053666836189222522080e+01,
                             0.10242732841383590525197178067173809e+01,
                             0.10424992963077313046227345694205724e+01,
                             0.10616963916554709168593717549811117e+01,
                             0.10826735650889751649827985602314584e+01,
                             0.11072236156410122376314575376454741e+01,
                             0.11468295974452529240039666547090746e+01};
    m_ = new int[27]{6, 10, 14, 18, 22, 26, 30, 34, 40, 44, 48, 54, 58,
                     62, 66, 72, 76, 80, 84, 90, 92, 96, 98, 96, 76, 4, 2}; 
    sm_ = new int[28]{0, 3, 8, 15, 24, 35, 48, 63, 80, 100, 122, 146, 
                      173, 202, 233, 266, 302, 340, 380, 422, 467, 513, 
                      561, 610, 658, 696, 698, 699}; 
    f_ = new int[27]{4, 6, 8, 8, 10, 10, 12, 12, 14, 14, 14, 16, 16, 
                     16, 16, 16, 18, 18, 18, 18, 18, 18, 18, 16, 16, 12, 2}; 
    smf_ = new int[28]{0, 12, 42, 98, 170, 280, 410, 590, 794, 1074, 1382,
                       1718, 2150, 2614, 3110, 3638, 4214, 4898, 5618, 
                       6374, 7184, 8012, 8876, 9758, 10526, 11134, 11158, 
                       11160}; 
  default:
    break;
  }

  generate_xs();
  generate_ys();
  generate_zs();
  generate_lambdaknm();
  generate_ealphaj();
}


LaplaceTable::~LaplaceTable() {
  delete [] sqf_;
  delete [] sqbinom_;
  for (auto it = dmat_plus_->begin(); it != dmat_plus_->end(); ++it) {
    delete [] it->second;
  }
  for (auto it = dmat_minus_->begin(); it != dmat_minus_->end(); ++it) {
    delete [] it->second;
  }
  delete dmat_plus_;
  delete dmat_minus_;

  delete [] lambda_;
  delete [] weight_;
  delete [] m_;
  delete [] sm_;
  delete [] f_;
  delete [] smf_;
  delete [] xs_;
  delete [] ys_;
  delete [] zs_;
  delete [] lambdaknm_;
  delete [] ealphaj_;
}

void LaplaceTable::generate_sqf() {
  sqf_ = new double[2 * p_ + 3];
  sqf_[0] = 1.0;
  for (int i = 1; i <= p_ * 2 + 2; ++i) {
    sqf_[i] = sqf_[i - 1] * sqrt(i);
  }
}

void LaplaceTable::generate_sqbinom() {
  int N = 2 * p_;
  int total = (N + 1) * (N + 2) / 2;
  sqbinom_ = new double[total];

  // Set the first three binomial coefficients
  sqbinom_[0] = 1; // binom(0, 0)
  sqbinom_[1] = 1; // binom(1, 0)
  sqbinom_[2] = 1; // binom(1, 1)

  // Compute the rest binomial coefficients
  for (int n = 2; n <= N; ++n) {
    // Get address of binom(n, 0) in current
    // Get address of binom(n - 1, 0) in previous
    double *current = &sqbinom_[midx(n, 0)];
    double *previous = &sqbinom_[midx(n - 1, 0)];

    current[0] = 1; // binom(n, 0);
    for (int m = 1; m < n; ++m) {
      current[m] = previous[m] + previous[m - 1];
    }
    current[n] = 1; // binom(n, n)
  }

  // Compute the square root of the binomial coefficients
  for (int i = 0; i < total; ++i) {
    sqbinom_[i] = sqrt(sqbinom_[i]);
  }
}

void LaplaceTable::generate_wigner_dmatrix() {
  dmat_plus_ = new builtin_map_t;
  dmat_minus_ = new builtin_map_t;

  double cbeta[24] = {1.0 / sqrt(5.0), 1.0 / sqrt(6.0), 1.0 / sqrt(9.0),
                      1.0 / sqrt(10.0), 1.0 / sqrt(11.0), 1.0 / sqrt(14.0),
                      1.0 / sqrt(19.0), 2.0 / sqrt(5.0), sqrt(2.0 / 3.0),
                      sqrt(1.0 / 2.0), sqrt(4.0 / 9.0), sqrt(1.0 / 3.0),
                      sqrt(4.0 / 13.0), sqrt(2.0 / 7.0), sqrt(4.0 / 17.0),
                      sqrt(2.0 / 11.0), sqrt(9.0 / 10.0), sqrt(9.0 / 11.0),
                      sqrt(9.0 / 13.0), sqrt(9.0 / 14.0), sqrt(9.0 / 17.0),
                      sqrt(9.0 / 19.0), sqrt(9.0 / 22.0), 0.0};
  int nd = (p_ + 1) * (4 * p_ * p_ + 11 * p_ + 6) / 6;
  for (int i = 0; i < 24; ++i) {
    double beta = acos(cbeta[i]);
    double *dp_data = new double[nd];
    double *dm_data = new double[nd];
    generate_dmatrix_of_beta(beta, dp_data, dm_data);
    (*dmat_plus_)[cbeta[i]] = dp_data;
    (*dmat_minus_)[cbeta[i]] = dm_data;
  }

  for (int i = 0; i < 23; i++) {
    double beta = acos(-cbeta[i]);
    double *dp_data = new double[nd];
    double *dm_data = new double[nd];
    generate_dmatrix_of_beta(beta, dp_data, dm_data);
    (*dmat_plus_)[-cbeta[i]] = dp_data;
    (*dmat_minus_)[-cbeta[i]] = dm_data;
  }
}

void LaplaceTable::generate_dmatrix_of_beta(double beta,
                                            double *dp, double *dm) {
  double cbeta = cos(beta);
  double sbeta = sin(beta);
  double s2beta2 = (1 - cbeta) / 2; // sin^2(beta / 2)
  double c2beta2 = (1 + cbeta) / 2; // cos^2(beta / 2)

  // Set d_0^{0, 0} to 1
  dp[0] = 1;
  dm[0] = 1;

  // Set d_1^{0, m}
  dp[1] = -sbeta / sqrt(2); // d_1^{0, -1}
  dp[2] = cbeta; // d_1^{0, 0}
  dp[3] = sbeta / sqrt(2); // d_1^{0, 1}
  dm[1] = -dp[1];
  dm[2] = dp[2];
  dm[3] = -dp[3];

  // Set d_1^{1, m}
  dp[4] = s2beta2; // d_1^{1, -1}
  dp[5] = -sbeta / sqrt(2); // d_1^{1, 0}
  dp[6] = c2beta2; // d_1^{1, 1}
  dm[4] = dp[4];
  dm[5] = -dp[5];
  dm[6] = dp[6];

  // Compute d_n^{0, m} for 2 <= n <= P
  for (int n = 2; n <= p_; ++n) {
    double *dpc = NULL, *dpp = NULL, *dmc = NULL;
    int m;
    // Get address of d_n^{0, 0} for angle beta, saved in dpc
    // Get address of d_{n - 1}^{0, 0} for angle beta, saved in dpp
    // Get address of d_n^{0, 0} for angle -beta, saved in dmc
    dpc = &dp[didx(n, 0, 0)];
    dpp = &dp[didx(n - 1, 0, 0)];
    dmc = &dm[didx(n, 0, 0)];

    // Compute d_n^{0, -n}
    m = -n;
    dpc[m] = -sbeta * 0.5 * sqrt((n - m) * (n - m - 1)) / n * dpp[m + 1];
    dmc[m] = dpc[m] * pow_m1(m);

    // Compute d_n^{0, -n + 1}
    m = -n + 1;
    dpc[m] = (-sbeta * 0.5 * sqrt((n - m) * (n - m - 1)) * dpp[m + 1] +
              cbeta * sqrt((n - m) * (n + m)) * dpp[m]) / n;
    dmc[m] = dpc[m] * pow_m1(m);

    // Handle d_n^{0, m}
    for (m = -n + 2; m <= n - 2; ++m) {
      dpc[m] = (-sbeta * 0.5 * sqrt((n - m) * (n - m - 1)) * dpp[m + 1] +
                cbeta * sqrt((n - m) * (n + m)) * dpp[m] +
                sbeta * 0.5 * sqrt((n + m) * (n + m - 1)) * dpp[m - 1]) / n;
      dmc[m] = dpc[m] * pow_m1(m);
    }

    // Handle d_n^{0, n - 1}
    m = n - 1;
    dpc[m] = (cbeta * sqrt((n - m) * (n + m)) * dpp[m] +
              sbeta * 0.5 * sqrt((n + m) * (n + m - 1)) * dpp[m - 1]) / n;
    dmc[m] = dpc[m] * pow_m1(m);

    // Handle d_n^{0, n}
    m = n;
    dpc[m] = sbeta * 0.5 * sqrt((n + m) * (n + m - 1)) / n * dpp[m - 1];
    dmc[m] = dpc[m] * pow_m1(m);

    // Compute d_n^{mp, m} for 1 <= mp <= n
    for (int mp = 1; mp <= n; mp++) {
      // Get address of d_n^{mp, 0} for angle beta, saved in dpc
      // Get address of d_{n - 1}^{mp - 1, 0} for angle beta, saved in dpp
      // Get address of d_n^{mp, 0} for angle -beta, saved in dmc
      dpc = &dp[didx(n, mp, 0)];
      dpp = &dp[didx(n - 1, mp - 1, 0)];
      dmc = &dm[didx(n, mp, 0)];

      double factor = 1.0 / sqrt((n + mp) * (n + mp - 1));

      // Compute d_n^{mp, -n}
      m = -n;
      dpc[m] = s2beta2 * sqrt((n - m) * (n - m - 1)) * factor * dpp[m + 1];
      dmc[m] = dpc[m] * pow_m1(mp - m);

      // Compute d_n^{mp, -n + 1}
      m = -n + 1;
      dpc[m] = (s2beta2 * sqrt((n - m) * (n - m - 1)) * dpp[m + 1] -
                sbeta * sqrt((n + m) * (n - m)) * dpp[m]) * factor;
      dmc[m] = dpc[m] * pow_m1(mp - m);

      // Compute d_n^{mp, m}
      for (m = -n + 2; m <= n - 2; ++m) {
        dpc[m] = (s2beta2 * sqrt((n - m) * (n - m - 1)) * dpp[m + 1]
                  + c2beta2 * sqrt((n + m) * (n + m - 1)) * dpp[m - 1]
                  - sbeta * sqrt((n + m) * (n - m)) * dpp[m]) * factor;
        dmc[m] = dpc[m] * pow_m1(mp - m);
      }

      // Compute d_n^{mp, n - 1}
      m = n - 1;
      dpc[m] = (-sbeta * sqrt((n + m) * (n - m)) * dpp[m] +
                c2beta2 * sqrt((n + m) * (n + m - 1)) * dpp[m - 1]) * factor;
      dmc[m] = dpc[m] * pow_m1(mp - m);

      // Compute d_n^{mp, n}
      m = n;
      dpc[m] = c2beta2 * sqrt((n + m) * (n + m - 1)) * factor * dpp[m - 1];
      dmc[m] = dpc[m] * pow_m1(mp - m);
    }
  }
}

void LaplaceTable::generate_xs() {
  xs_ = new dcomplex_t[7 * nexp_];
  int offset = 0;
  for (int k = 0; k < s_; ++k) {
    double alpha = 2 * M_PI / m_[k];
    for (int j = 1; j <= m_[k] / 2; ++j) {
      double alphaj = j * alpha;
      double calphaj = cos(alphaj);
      for (int m = -3; m <= 3; ++m) {
        double arg = lambda_[k] * m * calphaj;
        xs_[offset++] = dcomplex_t{cos(arg), sin(arg)};
      }
    }
  }
}

void LaplaceTable::generate_ys() {
  ys_ = new dcomplex_t[7 * nexp_];
  int offset = 0;
  for (int k = 0; k < s_; ++k) {
    double alpha = 2 * M_PI / m_[k];
    for (int j = 1; j <= m_[k] / 2; ++j) {
      double alphaj = j * alpha;
      double salphaj = sin(alphaj);
      for (int m = -3; m <= 3; ++m) {
        double arg = lambda_[k] * m * salphaj;
        ys_[offset++] = dcomplex_t{cos(arg), sin(arg)};
      }
    }
  }
}

void LaplaceTable::generate_zs() {
  zs_ = new double[4 * s_];
  int offset = 0;
  for (int k = 0; k < s_; ++k) {
    for (int m = 0; m <= 3; ++m) {
      zs_[offset++] = exp(-lambda_[k] * m);
    }
  }
}

void LaplaceTable::generate_lambdaknm() {
  lambdaknm_ = new double[s_ * (p_ + 1) * (p_ + 2) / 2];
  double *temp = new double[2 * p_ + 1];
  temp[0] = 1.0;
  for (int i = 1; i <= p_ * 2; ++i) {
    temp[i] = temp[i - 1] * sqrt(i);
  }

  int offset = 0;
  for (int k = 0; k < s_; ++k) {
    for (int n = 0; n <= p_; ++n) {
      double lambdakn = pow(lambda_[k], n);
      for (int m = 0; m <= n; ++m) {
        lambdaknm_[offset++] = lambdakn / temp[n - m] / temp[n + m];
      }
    }
  }

  delete [] temp;
}

void LaplaceTable::generate_ealphaj() {
  ealphaj_ = new dcomplex_t[smf_[s_]];
  int offset = 0;
  for (int k = 0; k < s_; ++k) {
    double alpha = 2 * M_PI / m_[k];
    for (int j = 1; j <= m_[k] / 2; ++j) {
      double alpha_j = j * alpha;
      for (int m = 1; m <= f_[k]; ++m) {
        double arg = m * alpha_j;
        ealphaj_[offset++] = dcomplex_t{cos(arg), sin(arg)};
      }
    }
  }
}

void update_laplace_table(int n_digits, double size) {
  // Once we are fully distrib, this must be wrapped up somehow in SharedData
  // or something similar.

  if (builtin_laplace_table_ == nullptr) {
    // Create the table if it does not exist
    builtin_laplace_table_ =
      std::unique_ptr<LaplaceTable>{new LaplaceTable{n_digits, size}};
  } else if (builtin_laplace_table_->n_digits() != n_digits) {
    // Replace the existing table with one for the new accuracy requirement
    builtin_laplace_table_.reset(new LaplaceTable{n_digits, size});
  } else if (builtin_laplace_table_->size() != size) {
    // Just need to update the scaling factor
    builtin_laplace_table_->update(size);
  }
}


} // namespace dashmm
