#pragma once
const double xyz_to_srgb[3][3] = {
    { 3.240479, -1.537150, -0.498535 },
    {-0.969256,  1.875991,  0.041556 },
    { 0.055648, -0.204043,  1.057311 }
};

const double srgb_to_xyz[3][3] = {
    { 0.412453, 0.357580, 0.180423 },
    { 0.212671, 0.715160, 0.072169 },
    { 0.019334, 0.119193, 0.950227 }
};

const double xyz_to_xyz[3][3] = {
  { 1.0, 0.0, 0.0 },
  { 0.0, 1.0, 0.0 },
  { 0.0, 0.0, 1.0 },
};

const double xyz_to_ergb[3][3] = {
  {  2.689989, -1.276020, -0.413844},
  { -1.022095,  1.978261,  0.043821},
  {  0.061203, -0.224411,  1.162859},
};

const double ergb_to_xyz[3][3] = {
  { 0.496859,  0.339094,  0.164047 },
  { 0.256193,  0.678188,  0.065619 },
  { 0.023290,  0.113031,  0.863978 },
};

const double xyz_to_prophoto_rgb[3][3] = {
    { 1.3459433,  -0.2556075, -0.0511118 },
    { -0.5445989,  1.5081673,  0.0205351 },
    {  0.0000000,  0.0000000,  1.2118128 }
};

const double prophoto_rgb_to_xyz[3][3] = {
    { 0.7976749,  0.1351917,  0.0313534 },
    { 0.2880402,  0.7118741,  0.0000857 },
    { 0.0000000,  0.0000000,  0.8252100 }
};

const double xyz_to_aces2065_1[3][3] = {
    {  1.0498110175, 0.0000000000, -0.0000974845 },
    { -0.4959030231, 1.3733130458, 0.0982400361 },
    {  0.0000000000, 0.0000000000, 0.9912520182 }
};

const double aces2065_1_to_xyz[3][3] = {
    { 0.9525523959, 0.0000000000, 0.0000936786 },
    { 0.3439664498, 0.7281660966, -0.0721325464 },
    { 0.0000000000, 0.0000000000, 1.0088251844 }
};

const double xyz_to_aces_ap1[3][3] = {
    { 1.645704, -0.324803, -0.219683},
    {-0.665556,  1.615332,  0.015570},
    { 0.011755, -0.008284,  0.918405}
};

const double aces_ap1_to_xyz[3][3] = {
    { 0.66057003,  0.13362288,  0.15574338},
    { 0.27222872,  0.67408153,  0.05368935},
    {-0.00599938,  0.00436992,  1.08733511}
};

const double xyz_to_rec2020[3][3] = {
    {  1.7166511880, -0.3556707838, -0.2533662814 },
    { -0.6666843518,  1.6164812366,  0.0157685458 },
    {  0.0176398574, -0.0427706133,  0.9421031212 }
};

const double rec2020_to_xyz[3][3] = {
    { 0.6369580483, 0.1446169036, 0.1688809752 },
    { 0.2627002120, 0.6779980715, 0.0593017165 },
    { 0.0000000000, 0.0280726930, 1.0609850577 }
};

static inline double 
mat3_det(const double a[3][3])
{
#define A(y, x) a[y - 1][x - 1]
  return
    A(1, 1) * (A(3, 3) * A(2, 2) - A(3, 2) * A(2, 3)) -
    A(2, 1) * (A(3, 3) * A(1, 2) - A(3, 2) * A(1, 3)) +
    A(3, 1) * (A(2, 3) * A(1, 2) - A(2, 2) * A(1, 3));
}

static inline double
mat3_inv(const double a[3][3], double inv[3][3])
{
  const double det = mat3_det(a);
  if(!(det != 0.0)) return 0.0;
  const double invdet = 1.0 / det;

  inv[0][0] =  invdet * (A(3, 3) * A(2, 2) - A(3, 2) * A(2, 3));
  inv[0][1] = -invdet * (A(3, 3) * A(1, 2) - A(3, 2) * A(1, 3));
  inv[0][2] =  invdet * (A(2, 3) * A(1, 2) - A(2, 2) * A(1, 3));

  inv[1][0] = -invdet * (A(3, 3) * A(2, 1) - A(3, 1) * A(2, 3));
  inv[1][1] =  invdet * (A(3, 3) * A(1, 1) - A(3, 1) * A(1, 3));
  inv[1][2] = -invdet * (A(2, 3) * A(1, 1) - A(2, 1) * A(1, 3));

  inv[2][0] =  invdet * (A(3, 2) * A(2, 1) - A(3, 1) * A(2, 2));
  inv[2][1] = -invdet * (A(3, 2) * A(1, 1) - A(3, 1) * A(1, 2));
  inv[2][2] =  invdet * (A(2, 2) * A(1, 1) - A(2, 1) * A(1, 2));
  return det;
#undef A
}

static inline void
mat3_mulv(
    const double a[3][3],
    const double v[3],
    double res[3])
{
  res[0] = res[1] = res[2] = 0.0f;
  for(int j=0;j<3;j++)
    for(int i=0;i<3;i++)
      res[j] += a[j][i] * v[i];
}

static inline void
mat3_mul(
    const double a[3][3],
    const double b[3][3],
    double res[3][3])
{
  for(int j=0;j<3;j++) for(int i=0;i<3;i++)
  {
    res[i][j] = 0.0;
    for(int k=0;k<3;k++)
      res[i][j] += a[i][k] * b[k][j];
  }
}
