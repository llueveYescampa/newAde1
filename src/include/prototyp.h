#ifndef PROTOTYP_H
  #define PROTOTYP_H

  #include <fstream>
  using std::ostream;

  #include "element.h"


  void ade1a();
  void ade1b();
  void ade1c();
  void ade1d();
  void ade1e();
  void ade1f();
  void ade1g();

  void header(ostream &);
  void printElementInfo(const posInt &val, posInt &lin);
  void printJointInfo  (const posInt &val, posInt &lin);
  void localElementStiffnessMatrix(real **sm, const Element &member);
  void globalElementStiffnessMatrix(real **sm, const Element &member);

  //void mult1(real **sm, const Element &member,  real **sa);
  //void mult2(const Element &member, real **sa, real **sm);
  void mult3(const Element &member, const real *__restrict const d,  real *__restrict const w);
  void mult4(const Element &member, const real *__restrict const d,  real *__restrict const w);
  void mult5(real **sm, const real *__restrict const d,  real *__restrict const w, const posInt &rows);

#endif // End of prototyp_h
