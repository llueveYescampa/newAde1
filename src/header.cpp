#include <fstream>
using std::ostream;

#include<ctime>

#include <iomanip>
using std::setw;

#include "common.h"
void header(ostream &mycout)
{
  struct tm *current;
  time_t now;
  time(&now);
  current=localtime(&now);

  if (npag != 0) {
     mycout << "\f\n";
     mycout << "                                                                         Pagina" << setw(3) << npag++ << '\n';
  } // end if //

  mycout <<  "*************************************************************************\n";
  mycout <<  "* BLACK MOROCOIMA INGENIERIA Y PLANIFICACION       Programa ADE1        *\n";
  mycout <<  "* ANALISIS ESTATICO DE ESTRUCTURAS ESPACIALES. Version 3.1 Junio 2009   *\n";
  mycout <<  "* Programaci¢n: E.F.B.S.                                                *\n";
  mycout <<  "*************************************************************************\n";

  if (npag != 0) {
    mycout.fill('0');
    mycout << title
           << "   Date: " << setw(2) << current->tm_mon+1 << '-' << setw(2) << current->tm_mday << '-' << setw(4)<< current->tm_year+1900
           << "  Time: " << setw(2) << current->tm_hour << ':' << setw(2) << current->tm_min << ':' << setw(2) << current->tm_sec << '\n';
    mycout.fill(' ');
    lin=9;
  } else {
     ++npag;
  } // end if //
} // end of function identification()

//mycout << asctime(localtime(&now)) << '\n';
//mycout << ctime(&now) << '\n';
