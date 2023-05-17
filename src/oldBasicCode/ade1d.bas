REM MODULO ADE1D
'$INCLUDE: 'defin.h'


DECLARE SUB posicion (q(), w(), i)
DECLARE SUB junta (lr, lin)
DECLARE SUB despla (lr, lin)
DECLARE SUB enca (npag, lr, tg, lin, tc)
DECLARE SUB gmrm (al, el, gkl, eiyl, eizl, sm(), ciy, cjy, cy, c, ciz, cjz, cz)
DECLARE SUB mult1 (r(), d(), w())
DECLARE SUB mult3 (sm(), w(), d())


COMMON tc(), e(), g(), ct(), pes(), q(), s(), tghip(), tfile()
COMMON tpi, tpa, tpb, tpc, tpd, tpe, tpf, tpg, tdat, tpii, tpj, tph
COMMON n, jo, m, ncas, nmat, nhip, nhipr, itipo, ivr, npag
COMMON nmc, ato, icas, max1, lr, lin, tg, lo, nec, mban, njc, nma

DIM w(27), t(27), t1(13), t5(18), t8(30), r(9), sm(12, 12), d(12)
REM Apertura de campos

FIELD #1, 4 AS t1(1): k = 4
FOR i = 2 TO 13
    FIELD #1, k AS t1, 4 AS t1(i)
    k = k + 4
NEXT i

FIELD #4, 4 AS t(1): k = 4
FOR i = 2 TO 27
    FIELD #4, k AS t1, 4 AS t(i)
    k = k + 4
NEXT i

FIELD #5, 4 AS t5(1): k = 4
FOR i = 2 TO 18
    FIELD #5, k AS t1, 4 AS t5(i)
    k = k + 4
NEXT i

OPEN "r", #8, tfile(8), 122
FIELD #8, 2 AS tlar: k = 2
FOR i = 1 TO 30
    FIELD #8, k AS t1, 4 AS t8(i)
    k = k + 4
NEXT i

REM Comienzo del modulo
FOR i = 1 TO 12
  FOR j = i TO 12
    sm(i, j) = 0!
  NEXT j
NEXT i
icas = icas + 1
CALL enca(npag, lr, tg, lin, tc(icas))
FOR i = 1 TO nec
  q(i) = 0!
NEXT i
jj = max1 + 1
GET #1, jj
njc = CVS(t1(1))
nma = CVS(t1(2))
nmc = CVS(t1(3))
ato = CVS(t1(4))
PRINT #lr, "NJC = "; njc; TAB(12); "NMA = "; nma; TAB(24); "NMC = "; nmc; TAB(36); "ATO = "; ato
PRINT #lr,
lin = lin + 2
LSET tlar = MKI$(0)
FOR i = 1 TO 30
  LSET t8(i) = MKS$(0)
NEXT i
IF nhip = 0 THEN
  FOR i = 1 TO m
    PUT #8, i
  NEXT i
ELSE
  FOR i = 1 TO m
    j = ncas * (i - 1) + icas
    PUT #8, j
  NEXT i
END IF
FOR i = n + 1 TO 2 * n
  GET #5, i
  FOR j = 1 TO 18
    LSET t5(j) = MKS$(0!)
  NEXT j
  PUT #5, i
NEXT i

REM Determinacion de la contribucion de las cargas aplicadas
REM en los nodos al vector de cargas q()
IF njc > 0 THEN
  CALL junta(lr, lin)
  FOR i = 1 TO njc
    IF lin > 56 THEN
      CALL enca(npag, lr, tg, lin, tc(icas))
      CALL junta(lr, lin)
    END IF
    jj = jj + 1
    GET #1, jj
    j = CVS(t1(1))
    FOR k = 2 TO 7
rem      w(k) = CVS(t1(k))
      w(k-1) = CVS(t1(k))
    NEXT k
    CALL posicion(q(), w(), j * 6)
    PRINT #lr, USING "#####"; j;
rem    PRINT #lr, USING "#######.###"; w(2); w(3); w(4); w(5); w(6); w(7)
    PRINT #lr, USING "#######.###"; w(1); w(2); w(3); w(4); w(5); w(6)
    lin = lin + 1
    GET #5, j
    IF CVS(t5(17)) <> 0 THEN
      FOR k = 1 TO 6
        IF CVS(t5(k + 3)) + CVS(t5(k + 9)) <> 0 THEN
rem          aux = -w(k + 1)
          aux = -w(k)
        ELSE
          aux = 0
        END IF
        LSET t5(k) = MKS$(aux)
        LSET t5(k + 6) = MKS$(0!)
      NEXT k
      PUT #5, (j + n)
    END IF
  NEXT i
  PRINT #lr, : lin = lin + 1
END IF

REM Desplazamientos de apoyos

IF (lin + nma + 2) > 56 THEN CALL enca(npag, lr, tg, lin, tc(icas))
IF nma <> 0 THEN
  CALL despla(lr, lin)
  FOR i = 1 TO nma
    IF lin > 56 THEN
      CALL enca(npag, lr, tg, lin, tc(icas))
      CALL despla(lr, lin)
    END IF
    jj = jj + 1
    GET #1, jj
    j = CVS(t1(1))
    GET #5, j
    IF CVS(t5(17)) = 0 THEN
      PRINT "ERROR. Movimiento de apoyo en junta no restringida."
      PRINT "Revise la junta :"; j
      PRINT #lr, "ERROR. Movimiento de apoyo en junta no restringida."
      PRINT #lr, "Revise la junta :"; j
      END
    END IF
    FOR k = 7 TO 12
      w(k) = CVS(t1(k - 5))
      IF (CVS(t5(k - 3)) = 0 AND w(k) <> 0) THEN
        PRINT "ERROR. Movimiento de apoyo en coordenada no restringida."
        PRINT "Revise la junta :"; j
        PRINT #lr, "ERROR. Movimiento de apoyo en coordenada no restringida."
        PRINT #lr, "Revise la junta :"; j
        END
      END IF
    NEXT k
    GET #5, (j + n)
    FOR k = 7 TO 12
      LSET t5(k) = MKS$(w(k))
    NEXT k
    PUT #5, (j + n)
    PRINT #lr, USING "#####"; j;
    PRINT #lr, USING "#####.#####"; w(7); w(8); w(9); w(10); w(11); w(12)
    lin = lin + 1
  NEXT i
  PRINT #lr, : lin = lin + 1
  FOR i = 1 TO m
    GET #4, i
    ii = CVS(t(2))
    ij = CVS(t(3))
    jtest = 0
    i1 = ii
REM Determinacion de desplazamientos en el miembro (i)
    k = 0
    FOR j = 1 TO 2
      GET #5, (i1 + n)
      FOR l = 7 TO 12
        k = k + 1
        d(k) = CVS(t5(l))
        IF d(k) <> 0 THEN jtest = 1
      NEXT l
      i1 = ij
    NEXT j
    IF jtest > 0 THEN
REM Solo se ejecuta si el miembro tiene desplazamientos
REM en alguno de sus extremos
      FOR j = 6 TO 9
        w(j) = CVS(t(j))
      NEXT j
      w(1) = CVS(t(1))
      iar = CVS(t(4))
      FOR j = 21 TO 27
        w(j) = CVS(t(j))
      NEXT j
REM Calculo de la matriz de rigidez de miembro sm 
      CALL gmrm(w(1), w(6), w(7), w(8), w(9), sm(), w(21), w(22), w(23), w(24), w(25), w(26), w(27))

REM  Lectura de la matriz de transformaci¢n r
      k = 12
      FOR j = 1 TO 9
        r(j) = CVS(t(k))
        k = k + 1
      NEXT j
REM Calculo de los desplazamientos en coordenadas locales (p = r X p*)
      CALL mult1(r(), d(), w())
REM Calculo de las fuerzas primarias debido a
REM movimientos de apoyos
      CALL mult3(sm(), w(), d())
      IF nhip <> 0 THEN
        j = ncas * (i - 1) + icas
      ELSE
        j = i
      END IF
      LSET tlar = MKI$(1)
      FOR k = 1 TO 12
        LSET t8(k) = MKS$(d(k))
      NEXT k
      PUT #8, j
    END IF
  NEXT i
END IF

REM Determinacion de la contribucion de los cambios de temperaturas
REM a las cargas primarias
IF ato <> 0 THEN
  FOR i = 1 TO 12
    d(i) = 0!
  NEXT i
  FOR i = 1 TO m
     GET #4, i
     al = CVS(t(1))
     ii = CVS(t(2))
     ij = CVS(t(3))
     imat = CVS(t(5))
     ar = CVS(t(6))
     pi = ar * al * ct(imat) * ato
     d(1) = pi
     d(7) = -pi
     IF nhip <> 0 THEN
       j = ncas * (i - 1) + icas
     ELSE
       j = i
     END IF
     GET #8, j
     LSET tlar = MKI$(1)
     d(1) = d(1) + CVS(t8(1))
     LSET t8(1) = MKS$(d(1))
     d(7) = d(7) + CVS(t8(7))
     LSET t8(7) = MKS$(d(7))
     PUT #8, j
  NEXT i
END IF
max1 = jj
PRINT "Fin Programa "; tpi; TAB(30); "Parte "; tpd
CHAIN tpe
END

REM DEFDBL A-H, O-S, U-Z
SUB despla (lr, lin)
PRINT #lr, "DESPLAZAMIENTOS INICIALES EN LAS JUNTAS"
PRINT #lr, "Junta         Dx         Dy         Dz         Rx         Ry         Rz"
lin = lin + 2
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB enca (npag, lr, tg, lin, tc)
DEFINT I-N
DEFSTR T
  npag = npag + 1
  PRINT #lr, CHR$(12)
  ' $INCLUDE: 'enca.h'
  PRINT #lr, tc
  PRINT #lr,
  lin = 8
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB gmrm (al, el, gkl, eiyl, eizl, sm(), ciy, cjy, cy, c, ciz, cjz, cz)
FOR i = 1 TO 12
  FOR j = i TO 12
    sm(i, j) = 0!
  NEXT j
NEXT i
ct1 = ciz + cz
ct2 = cjz + cz
ct3 = ct1 + ct2
el2 = el / al
el3 = el2 / al
eiyl2 = eiyl / al
eiyl3 = eiyl2 / al
eizl2 = eizl / al
eizl3 = eizl2 / al
sm(1, 1) = el * c
sm(1, 7) = -el * c
sm(2, 2) = eizl3 * ct3
sm(2, 6) = eizl2 * ct1
sm(2, 8) = -eizl3 * ct3
sm(2, 12) = eizl2 * ct2
sm(4, 4) = gkl * c
sm(4, 10) = -gkl
sm(6, 6) = eizl * ciz
sm(6, 8) = -eizl2 * ct1
sm(6, 12) = eizl * cz
sm(7, 7) = el * c
sm(8, 8) = eizl3 * ct3
sm(8, 12) = -eizl2 * ct2
sm(10, 10) = gkl * c
sm(12, 12) = eizl * cjz
ct1 = ciy + cy
ct2 = cjy + cy
ct3 = ct1 + ct2
sm(3, 3) = eiyl3 * ct3
sm(3, 5) = -eiyl2 * ct1
sm(3, 9) = -eiyl3 * ct3
sm(3, 11) = -eiyl2 * ct2
sm(5, 5) = eiyl * ciy
sm(5, 9) = eiyl2 * ct1
sm(5, 11) = eiyl * cy
sm(9, 9) = eiyl3 * ct3
sm(9, 11) = eiyl2 * ct2
sm(11, 11) = eiyl * cjy
FOR i = 1 TO 11
  FOR j = i + 1 TO 12
     sm(j, i) = sm(i, j)
   NEXT j
NEXT i
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB junta (lr, lin)
PRINT #lr, "CARGAS APLICADAS EN LAS JUNTAS"
PRINT #lr, "Junta         Fx         Fy         Fz         Mx         My         Mz"
lin = lin + 2
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB mult1 (r(), d(), w())
FOR i = 1 TO 12
  w(i) = 0!
NEXT i
FOR l = 0 TO 11 STEP 3
  k = 1
  FOR i = 1 TO 3
    FOR j = 1 TO 3
      w(l + i) = w(l + i) + d(l + j) * r(k)
      k = k + 1
    NEXT j
  NEXT i
NEXT l
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB mult3 (sm(), w(), d())
FOR j = 1 TO 12
  d(j) = 0!
NEXT j
FOR k = 1 TO 12
  x = w(k)
  IF x <> 0 THEN
    FOR i = 1 TO 12
      d(i) = d(i) + x * sm(i, k)
    NEXT i
  END IF
NEXT k
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB posicion (q(), w(), i)
  kk = 6
  j = i - 5
  FOR k = i TO j STEP -1
     q(k) = q(k) + w(kk)
     kk = kk - 1
   NEXT k
END SUB
