#include<iostream>
#include "language.h"

#include "typedef.h"

using std::cout;

void printJointInfo(const posInt &val, posInt &lin)
{
#ifdef ENGLISH
    switch (val) {
        case 1:
            cout << "Nodes Data.\n"
                << "Node   Rdx  Rdy  Rdz  Rrx  Rry  Rrz         X         Y         Z         Kdx        Kdy        Kdz        Krx        Kry        Krz\n";
            lin += 2;
            break;
        case 2:
            cout << "LOADS APPLIED TO THE NODES\n"
                << "Node          Fx         Fy         Fz         Mx         My         Mz\n";
            lin += 2;
            break;
        case 3:
            cout << "NODES INITIAL DISPLACEMENTS\n"
                << "Node          Dx         Dy         Dz         Rx         Ry         Rz\n";
            lin += 2;
            break;
        case 4:
            cout << "NODES DISPLACEMENTS.\n"
                << "Node               Dx              Dy              Dz              Rx              Ry              Rz\n";
            lin += 2;
            break;
        case 5:
            cout <<  "LOADS AND/OR REACTIONS AT NODES\n"
                << "Node           FX          FY          FZ          MX          MY          MZ\n";
            lin += 2;
            break;
        case 6:
            cout << "\nREACTIONS AT NODES FOR THE LOADS HYPOTESYS\n"
                <<"Node    Hip          Fx          Fy          Fz          Mx          My          Mz\n";
            lin+=3;
            break;
        default:
            break;
    } // end switch //
#endif  

#ifdef ESPANOL
    switch (val) {
        case 1:
            cout << "Datos de las Juntas.\n"
                << "Junta  Rdx  Rdy  Rdz  Rrx  Rry  Rrz         X         Y         Z         Kdx        Kdy        Kdz        Krx        Kry        Krz\n";
            lin += 2;
            break;
        case 2:
            cout << "CARGAS APLICADAS EN LAS JUNTAS\n"
                << "Junta         Fx         Fy         Fz         Mx         My         Mz\n";
            lin += 2;
            break;
        case 3:
            cout << "DESPLAZAMIENTOS INICIALES EN LAS JUNTAS\n"
                << "Junta         Dx         Dy         Dz         Rx         Ry         Rz\n";
            lin += 2;
            break;
        case 4:
            cout << "DESPLAZAMIENTO DE LAS JUNTAS.\n"
                << "Junta              Dx              Dy              Dz              Rx              Ry              Rz\n";
            lin += 2;
            break;
        case 5:
            cout <<  "CARGAS Y/O REACCIONES EN LAS JUNTAS\n"
                << "Junta          FX          FY          FZ          MX          MY          MZ\n";
            lin += 2;
            break;
        case 6:
            cout << "\nREACCIONES EN LAS JUNTAS PARA LAS HIPOTESIS DE SOLICITACIONES\n"
                <<"Junta   Hip          Fx          Fy          Fz          Mx          My          Mz\n";
            lin+=3;
            break;
        default:
            break;
    } // end switch //
#endif  
} // end of jointHeader //
