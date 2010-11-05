#include "whipple.h"
int eomwrapper(double t, const double x[10], double f[10], void * params)
{
  Whipple * p = (Whipple *) params;
  // Assign the states of the Whipple object
  p->setState(x);
  // Evaluate the RHS of the ODE's representing the equations of motion
  p->eoms();
  // Assign the right hand sides of the ODE's to the array passed
  f[0] = p->q0p;
  f[1] = p->q1p;
  f[2] = p->q3p;
  f[3] = p->q4p;
  f[4] = p->q5p;
  f[5] = p->q6p;
  f[6] = p->q7p;
  f[7] = p->u1p;
  f[8] = p->u3p;
  f[9] = p->u5p;
  // Return the status
  return GSL_SUCCESS;
} // eomwrapper()

double hc_f(double q2, void * params)
{
  Whipple * p = (Whipple *) params;
  return
      p->rft + p->ls*cos(p->q1)*cos(q2) +
      p->lf*(sin(p->q1)*sin(p->q3)-sin(q2)*cos(p->q1)*cos(p->q3)) +
      p->rf*sqrt((1-pow((sin(p->q1)*cos(p->q3)+sin(q2)*sin(p->q3)*cos(p->q1)),2))) -
      p->rrt - p->rr*cos(p->q1) - p->lr*sin(q2)*cos(p->q1);
} // hc_f()

double hc_df(double q2, void * params)
{
  Whipple * p = (Whipple *) params;
  return
      -cos(p->q1)*(p->lr*cos(q2)+p->ls*sin(q2)+p->lf*cos(q2)*cos(p->q3)+p->rf*sin(p->q3)*cos(q2)*(
                  sin(p->q1)*cos(p->q3)+sin(q2)*sin(p->q3)*cos(p->q1))/sqrt((1-pow((sin(p->q1)*cos(p->q3)+sin(q2)*
                              sin(p->q3)*cos(p->q1)),2))));
} // hc_df()

void hc_fdf(double q2, void * params, double * f, double * df)
{
  *f = hc_f(q2, params);
  *df =  hc_df(q2, params);
} // hc_fdf()

ostream &operator<<(ostream &file, const Whipple * discs)
{
  file.write((char *) &(discs->t), sizeof discs->t);
  file.write((char *) &discs->q0, sizeof discs->q0);
  file.write((char *) &discs->q1, sizeof discs->q1);
  file.write((char *) &discs->q2, sizeof discs->q2);
  file.write((char *) &discs->q3, sizeof discs->q3);
  file.write((char *) &discs->q4, sizeof discs->q4);
  file.write((char *) &discs->q5, sizeof discs->q5);
  file.write((char *) &discs->q6, sizeof discs->q6);
  file.write((char *) &discs->q7, sizeof discs->q7);
  file.write((char *) &discs->u0, sizeof discs->u0);
  file.write((char *) &discs->u1, sizeof discs->u1);
  file.write((char *) &discs->u2, sizeof discs->u2);
  file.write((char *) &discs->u3, sizeof discs->u3);
  file.write((char *) &discs->u4, sizeof discs->u4);
  file.write((char *) &discs->u5, sizeof discs->u5);
  file.write((char *) discs->no_fn, sizeof discs->no_fn);
  file.write((char *) &discs->Rx, sizeof discs->Rx);
  file.write((char *) &discs->Ry, sizeof discs->Ry);
  file.write((char *) &discs->Rz, sizeof discs->Rz);
  file.write((char *) &discs->Fx, sizeof discs->Fx);
  file.write((char *) &discs->Fy, sizeof discs->Fy);
  file.write((char *) &discs->Fz, sizeof discs->Fz);
  file.write((char *) &discs->ke, sizeof discs->ke);
  file.write((char *) &discs->pe, sizeof discs->pe);
  file.write((char *) &discs->fa_yaw, sizeof discs->fa_yaw);
  file.write((char *) &discs->fa_lean, sizeof discs->fa_lean);
  file.write((char *) &discs->fa_pitch, sizeof discs->fa_pitch);
  file.write((char *) discs->constraints, sizeof discs->constraints);
  return file;
} // ostream &operator<<()

void Whipple::writeSimRecord_dt(const char * filename) const
{
  ofstream fp(filename, ios::out);
  if (fp.is_open()) {
    fp << "import numpy as np" << endl;
    fp << "sim_dt = np.dtype([('t', np.float64)," << endl <<
          "                   ('q0', np.float64)," << endl <<
          "                   ('q1', np.float64)," << endl <<
          "                   ('q2', np.float64)," << endl <<
          "                   ('q3', np.float64)," << endl <<
          "                   ('q4', np.float64)," << endl <<
          "                   ('q5', np.float64)," << endl <<
          "                   ('q6', np.float64)," << endl <<
          "                   ('q7', np.float64)," << endl <<
          "                   ('u0', np.float64)," << endl <<
          "                   ('u1', np.float64)," << endl <<
          "                   ('u2', np.float64)," << endl <<
          "                   ('u3', np.float64)," << endl <<
          "                   ('u4', np.float64)," << endl <<
          "                   ('u5', np.float64)," << endl <<
          "                   ('fnx', np.float64)," << endl <<
          "                   ('fny', np.float64)," << endl <<
          "                   ('fnz', np.float64)," << endl <<
          "                   ('Rx', np.float64)," << endl <<
          "                   ('Ry', np.float64)," << endl <<
          "                   ('Rz', np.float64)," << endl <<
          "                   ('Fx', np.float64)," << endl <<
          "                   ('Fy', np.float64)," << endl <<
          "                   ('Fz', np.float64)," << endl <<
          "                   ('ke', np.float64)," << endl <<
          "                   ('pe', np.float64)," << endl <<
          "                   ('fa_yaw', np.float64)," << endl <<
          "                   ('fa_lean', np.float64)," << endl <<
          "                   ('fa_pitch', np.float64)," << endl <<
          "                   ('nh1', np.float64)," << endl <<
          "                   ('nh2', np.float64)," << endl <<
          "                   ('nh3', np.float64)])" << endl;
    fp.close();
  } else {
    cerr << "Unable to open " << filename << "for writing." << endl;
    cerr << "Aborting." << endl;
    exit(0);
  }
} // writeSimRecord_dt()

void Whipple::writeEvalRecord_dt(const char * filename) const
{
  ofstream fp(filename, ios::out);
  if (fp.is_open()) {
    fp << "import numpy as np" << endl;
    fp << "eval_dt = np.dtype([('v', np.float64)," << endl <<
          "                    ('lambda1', np.float64)," << endl <<
          "                    ('lambda2', np.float64)," << endl <<
          "                    ('lambda3', np.float64)," << endl <<
          "                    ('lambda4', np.float64)])" << endl;
    fp.close();
  } else {
    cerr << "Unable to open " << filename << "for writing." << endl;
    cerr << "Aborting." << endl;
    exit(0);
  }
} // writeEvalRecord_dt()

void Whipple::printState(void) const
{
  cout.precision(16);
  cout << "q0 = " << q0 << " (ignorable)" << endl
       << "q1 = " << q1 << endl
       << "q2 = " << q2 << " (dependent)" << endl
       << "q3 = " << q3 << endl
       << "q4 = " << q4 << " (ignorable)" << endl
       << "q5 = " << q5 << " (ignorable)" << endl
       << "q6 = " << q6 << " (ignorable)" << endl
       << "q7 = " << q7 << " (ignorable)" << endl
       << "u0 = " << u0 << " (dependent)" << endl
       << "u1 = " << u1 << endl
       << "u2 = " << u2 << " (dependent)" << endl
       << "u3 = " << u3 << endl
       << "u4 = " << u4 << " (dependent)" << endl
       << "u5 = " << u5 << endl;
} // printState()

void Whipple::printParameters(void) const
{
  cout.precision(16);
  cout << "rr   = " << rr << endl
       << "rrt  = " << rrt << endl
       << "rf   = " << rf << endl
       << "rft  = " << rft << endl
       << "lr   = " << lr << endl
       << "ls   = " << ls << endl
       << "lf   = " << lf << endl
       << "mr   = " << mr << endl
       << "mf   = " << mf << endl
       << "ICyy = " << ICyy << endl
       << "IDxx = " << IDxx << endl
       << "IDyy = " << IDyy << endl
       << "IDzz = " << IDzz << endl
       << "IDxz = " << IDxz << endl
       << "IExx = " << IExx << endl
       << "IEyy = " << IEyy << endl
       << "IEzz = " << IEzz << endl
       << "IExz = " << IExz << endl
       << "IFyy = " << IFyy << endl
       << "lrx  = " << lrx << endl
       << "lrz  = " << lrz << endl
       << "lfx  = " << lfx << endl
       << "lfz  = " << lfz << endl
       << "g    = " << g << endl;
} // printParameters()

void Whipple::writeState(const char * filename) const
{
  ofstream fp(filename, ios::out);
  fp.precision(16);
  if (fp.is_open()) {
  fp << "q0 = " << q0 << " (ignorable coordinate)" << endl
     << "q1 = " << q1 << endl
     << "q2 = " << q2 << " (dependent coordinate)" << endl
     << "q3 = " << q3 << endl
     << "q4 = " << q4 << " (ignorable coordinate)" << endl
     << "q5 = " << q5 << " (ignorable coordinate)" << endl
     << "q6 = " << q6 << " (ignorable coordinate)" << endl
     << "q7 = " << q7 << " (ignorable coordinate)" << endl
     << "u0 = " << u0 << " (dependent speed)" << endl
     << "u1 = " << u1 << endl
     << "u2 = " << u2 << " (dependent speed)" << endl
     << "u3 = " << u3 << endl
     << "u4 = " << u4 << " (dependent speed)" << endl
     << "u5 = " << u5 << endl;
  } else {
    cerr << "Unable to open " << filename << "for writing." << endl;
    cerr << "Aborting." << endl;
    exit(0);
  }
} // writeState()

void Whipple::writeParameters(const char * filename) const
{
  ofstream fp(filename, ios::out);
  fp.precision(16);
  if (fp.is_open()) {
  fp << "rr   = " << rr << endl
     << "rrt  = " << rrt << endl
     << "rf   = " << rf << endl
     << "rft  = " << rft << endl
     << "lr   = " << lr << endl
     << "ls   = " << ls << endl
     << "lf   = " << lf << endl
     << "mr   = " << mr << endl
     << "mf   = " << mf << endl
     << "ICyy = " << ICyy << endl
     << "IDxx = " << IDxx << endl
     << "IDyy = " << IDyy << endl
     << "IDzz = " << IDzz << endl
     << "IDxz = " << IDxz << endl
     << "IExx = " << IExx << endl
     << "IEyy = " << IEyy << endl
     << "IEzz = " << IEzz << endl
     << "IExz = " << IExz << endl
     << "IFyy = " << IFyy << endl
     << "lrx  = " << lrx << endl
     << "lrz  = " << lrz << endl
     << "lfx  = " << lfx << endl
     << "lfz  = " << lfz << endl
     << "g    = " << g << endl;
  } else {
    cerr << "Unable to open " << filename << "for writing." << endl;
    cerr << "Aborting." << endl;
    exit(0);
  }
} // writeParameters()

Whipple::Whipple()
{
  // Setup the root finder and the numerical integrator
  initRootFinder();
  initODESolver();

  // Camera settings
  theta = M_PI / 4.0;
  phi = 0.0;
  d = 1.0;
  ctx = .35;
  cty = .35; 
  ctz = 0.0;
  
  // Default inputs, parameters, and initial state
  Trw = Tfw = Ts = 0.0;
  setBenchmarkParameters();
  evalConstants();          // evaluate the constant z's
  setBenchmarkState();      // set the state
  eoms();                   // compute du_i/dt, and intermediate z's
  computeOutputs();         // compute remaining z's and all outputs

  // Initialize the eigenvalues and eigenvectors
  evals = gsl_vector_complex_alloc(4);
  evecs = gsl_matrix_complex_alloc(4, 4);
  m = gsl_matrix_alloc(4, 4);
  gsl_matrix_set_zero(m);
  m->data[2] = 1.0;
  m->data[7] = 1.0;
  w = gsl_eigen_nonsymmv_alloc(4);
} // constructor

Whipple::~Whipple()
{
  gsl_root_fdfsolver_free(fdf_s); 
  gsl_odeiv_evolve_free(e);
  gsl_odeiv_control_free(c);
  gsl_odeiv_step_free(s);
  gsl_vector_complex_free(evals);
  gsl_matrix_complex_free(evecs);
  gsl_matrix_free(m);
  gsl_eigen_nonsymmv_free(w);
} // destructor

void Whipple::setParameters(WhippleParams * p)
{
  ICyy = p->ICyy;
  IDxx = p->IDxx;
  IDxz = p->IDxz;
  IDyy = p->IDyy;
  IDzz = p->IDzz;
  IExx = p->IExx;
  IExz = p->IExz;
  IEyy = p->IEyy;
  IEzz = p->IEzz;
  IFyy = p->IFyy;
  g = p->g;
  lf = p->lf;
  lfx = p->lfx;
  lfz = p->lfz;
  lr = p->lr;
  lrx = p->lrx;
  lrz = p->lrz;
  ls = p->ls;
  mr = p->mr;
  mf = p->mf;
  rf = p->rf;
  rft = p->rft;
  rr = p->rr;
  rrt = p->rrt;
} // setParameters()

void Whipple::setState(const double state[10])
{
  q0 = state[0];  // Yaw
  q1 = state[1];  // Lean
  q3 = state[2];  // Steer
  calcPitch();    // Solve holonomic constraint for pitch angle
  q4 = state[3];
  q5 = state[4];
  q6 = state[5];
  q7 = state[6];
  u1 = state[7];
  u3 = state[8];
  u5 = state[9];
} // setState()

// TODO: fix inertias and cm distances
void Whipple::setBenchmarkParameters(void)
{
  rr=0.3;
  rrt=0.0;
  rf=0.35;
  rft=0.0;
  lr=0.9534570696121847;
  ls=0.2676445084476887;
  lf=0.0320714267276193;
  mr=87.0;
  mf=7.0;
  ICyy=0.0603;
  IDxx=7.178169776497895;
  IDyy=11;
  IDzz=4.821830223502103;
  IDxz=3.822553593835788;
  IExx=0.05841337700152973;
  IEyy=0.06;
  IEzz=0.007586622998470264;
  IExz=0.009119225261946296;
  IFyy=0.28;
  lrx=0.4707271515135145;
  lrz=-0.477928811464608;
  lfx=-0.005970833924186833;
  lfz=-0.3699518200282974;
  g=9.81;
} // setBenchmarkParameters()

void Whipple::setBenchmarkState(void)
{
  q0 = q1 = q3 = q4 = q5 = q6 = q7 = 0.0;
  q2 = M_PI/10.0;
  u1 = 0.5;
  u3 = 0.0;
  u5 = -4.6/(rf+rft);
} // setBenchmarkState

void Whipple::initRootFinder(void)
{
  // Newton-Raphson setup
  FDF.f = &hc_f;
  FDF.df = &hc_df;
  FDF.fdf = &hc_fdf;
  FDF.params = this;
  fdf_T = gsl_root_fdfsolver_newton;
  fdf_s = gsl_root_fdfsolver_alloc(fdf_T);
} // initRootFinder()

void Whipple::initODESolver(void)
{
  // Set integration settings
  t = 0.0;
  tf = 5.0;
  h = 0.001;
  fps = 100;
  T = gsl_odeiv_step_rk8pd;
  s = gsl_odeiv_step_alloc(T, 10);
  c = gsl_odeiv_control_y_new(1e-6, 1e-9);
  e = gsl_odeiv_evolve_alloc(10);
  sys.function = eomwrapper;
  sys.jacobian = NULL;
  sys.dimension = 10;
  sys.params = this;
} // initODESolver()

void Whipple::calcPitch(void)
{ 
  iter = 0;
  // Newton's method
  gsl_root_fdfsolver_set(fdf_s, &FDF, q2);
  do
  {
    gsl_root_fdfsolver_iterate(fdf_s);
    status = gsl_root_test_residual(
              hc_f(gsl_root_fdfsolver_root(fdf_s), this), 1e-16);
  } while(status == GSL_CONTINUE && ++iter < 100);
  q2 = gsl_root_fdfsolver_root(fdf_s);
} // calcPitch

void Whipple::calcEvals(void)
{
  m->data[0] = m->data[1] = m->data[3] = 
    m->data[4] = m->data[5] = m->data[6] = 0.0;
  m->data[2] = m->data[7] = 1.0;  // set the top two rows
  // Evaluate the EOMS
  eoms();
  // Evaluate the 10x10 A matrix
  computeOutputs();

  // Get the 4x4 sub matrix of the 10x10 A matrix
  m->data[8] = A[71];
  m->data[9] = A[72];
  m->data[10] = A[77];
  m->data[11] = A[78];
  m->data[12] = A[81];
  m->data[13] = A[82];
  m->data[14] = A[87];
  m->data[15] = A[88];

  // Get the eigenvalues
  gsl_eigen_nonsymmv(m, evals, evecs, w);
  getFourValues();
} // calcEvals()

void Whipple::getFourValues(void)
{
  int i;
  double valr, vali;
  if (evalCase() == 0) {
    for (i = 0; i < 4; ++i)
      fourValues[i] = GSL_REAL(gsl_vector_complex_get(evals, i));
    return;
  } else
    for (i = 0; i < 4; ++i) {
      vali = GSL_IMAG(gsl_vector_complex_get(evals, i)); 
      valr = GSL_REAL(gsl_vector_complex_get(evals, i)); 
      if (vali == 0.0) {
        fourValues[i] = valr;
        continue;
      }
      fourValues[i] = valr;
      fourValues[++i] = vali;
    }
} // getFourValues()

int Whipple::evalCase(void)
{
  int i, real = 0;
  for (i = 0; i < 4; ++i)
   if (GSL_IMAG(gsl_vector_complex_get(evals, i)) == 0.0)
     ++real;

  if (real == 4)
    return 0;
  if (real == 2)
    return 1;
  // Final case, real == 0
  return 0;
} // evalCase()

void Whipple::evolve(double tj, double * state)
{
  gsl_odeiv_evolve_apply(e, c, s, &sys, &t, tj, &h, state);
} // evolve()
