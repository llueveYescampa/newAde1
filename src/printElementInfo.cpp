#include<iostream>
using std::cout;

#include "element.h"
#include "language.h"

void printElementInfo(const posInt &val, posInt &lin)
{
#ifdef ENGLISH

    switch (val) {
        case 1:
            cout << "Elements Data.\n"
                << "Elmt  Ji  Jj   Long.      Area          K        Iyy        Izz   Cixy   Cjxy   Cxy   Cixz   Cjxz   Cxz   Cn=Ct T of M";


            if(elementRecord.bz > 0 and elementRecord.by == 0) {
                cout << "  Diam";
            } else if(elementRecord.bz > 0 and elementRecord.by > 0) {
                cout << "     b      h";
            } // end if//

            cout << '\n';
            lin += 2;
            break;
        case 2:
            cout << "LOADS AND TEMPERATURE VARIATIONS IN MEMBERS (local oXYZ system of reference).\n";
            ++lin;
            break;
        case 3:
            cout << "PRIMARY FORCES AT ELEMENTS ENDS\n"
                << "Element   Node             N           Vy           Vz            T           My           Mz\n";
            lin+=2;
            break;
        case 4:
            cout << "FORCES AT ELEMENTS SECTIONS\n"
                << "Element  Node       x          N         Vy         Vz          T         My         Mz\n";
            lin+=2;
            break;
        case 5:
            cout << "\nFORCES IN ELEMENT SECTIONS FOR THE LOADS HYPOTESTS\n";
            lin+=2;
            break;
        case 6:
            cout << "Element       x        Hip          N           Vy          Vz           T          My          Mz\n";
            ++lin;
            break;
    } // end switch //
#endif  

#ifdef ESPANOL
    switch (val) {
        case 1:
            cout << "Datos de los miembros.\n"
                << "Mbro  Ji  Jj   Long.      Area          K        Iyy        Izz   Cixy   Cjxy   Cxy   Cixz   Cjxz   Cxz   Cn=Ct T de M";


            if(elementRecord.bz > 0 and elementRecord.by == 0) {
                cout << "  Diam";
            } else if(elementRecord.bz > 0 and elementRecord.by > 0) {
                cout << "     b      h";
            } // end if//


            cout << '\n';
            lin += 2;
            break;
        case 2:
            cout << "CARGAS Y VARIACIONES DE TEMPERATURA EN LOS MIEMBROS (referidas al systema local oxyz).\n";
            ++lin;
            break;
        case 3:
            cout << "FUERZAS PRIMARIAS EN LOS EXTREMOS DE LOS MIEMBROS\n"
                << "Miembro  Junta             N           Vy           Vz            T           My           Mz\n";
            lin+=2;
            break;
        case 4:
            cout << "FUERZAS EN LAS SECCIONES DE LOS MIEMBROS\n"
                << "Miembro Junta       x          N         Vy         Vz          T         My         Mz\n";
            lin+=2;
            break;
        case 5:
            cout << "\nFUERZAS EN LAS SECCIONES DE LOS MIEMBROS PARA LAS HIPOTESIS DE SOLICITACIONES\n";
            lin+=2;
            break;
        case 6:
            cout << "Miembro       x        Hip          N           Vy          Vz           T          My          Mz\n";
            ++lin;
            break;
    } // end switch //

#endif  
} // end of elementHeader //
