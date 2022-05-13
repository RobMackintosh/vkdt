#include "cfa-pca.h"
#include "cfa-gauss.h"
#include "cfa-sigmoid.h"
#include "cfa-plain.h"
#include <strings.h>

static inline double
cfa_smoothness(
    int model,
    int num,
    const double *p)
{
  switch(model)
  {
    case 1: return cfa_pca_smoothness(num, p);
    case 2: return cfa_gauss_smoothness(num, p);
    case 3: return cfa_sigmoid_smoothness(num, p);
    case 4: return cfa_plain_smoothness(num, p);
    default: return 0;
  }
}

static inline double
cfa_red(
    int           model,
    int           num,         // number of params (max 10 at this time)
    const double *p,           // parameters
    double        wavelength)  // evaluate at this wavelength
{
  switch(model)
  {
    case 1: return cfa_pca_red(num, p, wavelength);
    case 2: return cfa_gauss_red(num, p, wavelength);
    case 3: return cfa_sigmoid_red(num, p, wavelength);
    case 4: return cfa_plain_red(num, p, wavelength);
    default: return 0;
  }
}

static inline double
cfa_green(
    int           model,
    int           num,         // number of params (max 10 at this time)
    const double *p,           // parameters
    double        wavelength)  // evaluate at this wavelength
{
  switch(model)
  {
    case 1: return cfa_pca_green(num, p, wavelength);
    case 2: return cfa_gauss_green(num, p, wavelength);
    case 3: return cfa_sigmoid_green(num, p, wavelength);
    case 4: return cfa_plain_green(num, p, wavelength);
    default: return 0;
  }
}
static inline double
cfa_blue(
    int           model,
    int           num,         // number of params (max 10 at this time)
    const double *p,           // parameters
    double        wavelength)  // evaluate at this wavelength
{
  switch(model)
  {
    case 1: return cfa_pca_blue(num, p, wavelength);
    case 2: return cfa_gauss_blue(num, p, wavelength);
    case 3: return cfa_sigmoid_blue(num, p, wavelength);
    case 4: return cfa_plain_blue(num, p, wavelength);
    default: return 0;
  }
}

static inline int
cfa_model_parse(const char *c)
{
  if(!strcasecmp(c, "pca"))     return 1;
  if(!strcasecmp(c, "gauss"))   return 2;
  if(!strcasecmp(c, "sigmoid")) return 3;
  if(!strcasecmp(c, "plain"))   return 4;
  return 1; // default pca
}

static inline const char*
cfa_model_str(model)
{
  switch(model)
  {
    case 1: return "pca";
    case 2: return "gauss";
    case 3: return "sigmoid";
    case 4: return "plain";
    default: return "invalid";
  }
}

