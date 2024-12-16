#include <fstream>
using std::ostream;

#include<ctime>

#include <iomanip>
using std::setw;

#include <cstring>
using std::string;

#include "language.h"
#include "typedef.h"

void header(ostream &mycout,
        string  title,
        posInt  &lin,
        posInt  &npag
        )
{
    struct tm *current;
    time_t now;
    time(&now);
    current=localtime(&now);


#ifdef ENGLISH

    if (npag != 0) {
        mycout << "\f\n";
        mycout << "                                                                           Page" << setw(3) << npag++ << '\n';
    } // end if //

    mycout <<  "*************************************************************************\n";
    mycout <<  "* BLACK MOROCOIMA ENGINEERING AND PLANNING            Program newAde1   *\n";
    mycout <<  "* STATIC ANALYSIS OF SPATIAL STRUCTURES     Version 4.0 November 2024   *\n";
    mycout <<  "* Programming: E.F.B.S.                                                 *\n";
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

#endif  

#ifdef ESPANOL

    if (npag != 0) {
        mycout << "\f\n";
        mycout << "                                                                         Pagina" << setw(3) << npag++ << '\n';
    } // end if //

    mycout <<  "**************************************************************************\n";
    mycout <<  "* BLACK MOROCOIMA INGENIERIA Y PLANIFICACION             Programa newAde1*\n";
    mycout <<  "* ANALISIS ESTATICO DE ESTRUCTURAS ESPACIALES. Version 4.0 Noviembre 2024*\n";
    mycout <<  "* Programaci¢n: E.F.B.S.                                                 *\n";
    mycout <<  "**************************************************************************\n";

    if (npag != 0) {
        mycout.fill('0');
        mycout << title
            << "  Fecha: " << setw(2) << current->tm_mon+1 << '-' << setw(2) << current->tm_mday << '-' << setw(4)<< current->tm_year+1900
            << " Tiempo: " << setw(2) << current->tm_hour << ':' << setw(2) << current->tm_min << ':' << setw(2) << current->tm_sec << '\n';
        mycout.fill(' ');
        lin=9;
    } else {
        ++npag;
    } // end if //

#endif  


} // end of function identification()
