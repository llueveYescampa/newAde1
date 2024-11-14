REM MODULO ADE1F
'$INCLUDE: 'defin.h'


DECLARE SUB enca (npag, lr, tg, lin, tc)
DECLARE SUB Junta (lr, lin)
DECLARE SUB sustinv (s(), q(), iw, nec, ned)
DECLARE SUB mult1 (r(), d(), w())
DECLARE SUB mult2 (sm(), w(), d())
DECLARE SUB gmrm (al, el, gkl, eiyl, eizl, sm(), ciy, cjy, cy, c, ciz, cjz, cz)
DECLARE SUB posicion (q(), d(), ii, jj)
DECLARE SUB seccion (lr, lin)
DECLARE SUB posi (w(), q(), ii, ij)
DECLARE SUB reac (lr, lin)
DECLARE SUB chgsg (d())

DIM w(27), t(27), t1(13), t5(18), t8(30), r(9), sm(12, 12)
DIM d(30), delta(6)


COMMON tc(), e(), g(), ct(), pes(), q(), s(), tghip(), tfile()
COMMON tpi, tpa, tpb, tpc, tpd, tpe, tpf, tpg, tdat, tpii, tpj, tph
COMMON n, jo, m, ncas, nmat, nhip, nhipr, itipo, ivr, npag
COMMON nmc, ato, icas, max1, lr, lin, tg, lo, nec, mban, njc, nma

rem edgar = s(1)
rem edgar = q(nec)

REM Apertura de campos

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
IF mban < 0 THEN
  ksig = -1
  mban = -mban
ELSE
  ksig = 1
END IF
ned = mban * nec
CALL sustinv(s(), q(), mban - 1, nec, ned)
IF (lin + n + 2) > 56 THEN CALL enca(npag, lr, tg, lin, tc(icas))
CALL Junta(lr, lin)
k = 1
FOR i = 1 TO n
  GET #5, (i + n)
  PRINT #lr, USING "#####"; i;
  FOR j = 7 TO 12
     IF lin > 56 THEN
       CALL enca(npag, lr, tg, lin, tc(icas))
       CALL Junta(lr, lin)
     END IF
     PRINT #lr, USING "   ##.######^^^^"; (q(k) + CVS(t5(j)));
    k = k + 1
  NEXT j
  PRINT #lr, : lin = lin + 1
NEXT i
PRINT #lr, : lin = lin + 1
FOR i = 1 TO 12
  FOR j = i TO 12
    sm(i, j) = 0!
  NEXT j
NEXT i
IF (ksig < 0 AND lin > 54) THEN CALL enca(npag, lr, tg, lin, tc(icas))
IF (ksig > 0 AND lin > 51) THEN CALL enca(npag, lr, tg, lin, tc(icas))
CALL seccion(lr, lin)
FOR i = 1 TO m
  IF (ksig < 0 AND lin > 56) THEN
    CALL enca(npag, lr, tg, lin, tc(icas))
    CALL seccion(lr, lin)
  END IF
  IF (ksig > 0 AND lin > 53) THEN
    CALL enca(npag, lr, tg, lin, tc(icas))
    CALL seccion(lr, lin)
  END IF
  GET #4, i
  al = CVS(t(1))
  ii = CVS(t(2))
  ij = CVS(t(3))
  CALL posicion(q(), d(), ii, ij)
  FOR j = 6 TO 9
    w(j) = CVS(t(j))
  NEXT j
  FOR j = 21 TO 27
    w(j) = CVS(t(j))
  NEXT j
  w(1) = CVS(t(1))
  iar = CVS(t(4))
  CALL gmrm(w(1), w(6), w(7), w(8), w(9), sm(), w(21), w(22), w(23), w(24), w(25), w(26), w(27))
  k = 12
  FOR j = 1 TO 9
    r(j) = CVS(t(k))
    k = k + 1
  NEXT j
  CALL mult1(r(), d(), w())
  CALL mult2(sm(), w(), d())
  IF nhip <> 0 THEN
    j = ncas * (i - 1) + icas
  ELSE
    j = i
  END IF
  FOR k = 13 TO 30
    d(k) = 0!
  NEXT k
  GET #8, j
  LSET tlar = MKI$(1)
  FOR k = 1 TO 30
    d(k) = d(k) + CVS(t8(k))
  NEXT k
  CALL chgsg(d())
  al = al / 4
  kkk = 12
  FOR k = 1 TO 3
      kkk = kkk + 1
    d(kkk) = d(kkk) + d(1)
      kkk = kkk + 1
    d(kkk) = d(kkk) + d(2)
      kkk = kkk + 1
    d(kkk) = d(kkk) + d(3)
      kkk = kkk + 1
    d(kkk) = d(kkk) + d(4)
      kkk = kkk + 1
    d(kkk) = d(kkk) + d(5) + d(3) * k * al
      kkk = kkk + 1
    d(kkk) = d(kkk) + d(6) + d(2) * k * al
  NEXT k
  PRINT #lr, USING "#######"; i;
  PRINT #lr, USING "######"; ABS(ii);
  PRINT #lr, USING "####.###"; 0;
  FOR k = 1 TO 6
    PRINT #lr, USING "#######.###"; d(k);
  NEXT k
  PRINT #lr,
  lin = lin + 1
  IF ksig > 0 THEN
    kk = 13
    FOR k = 1 TO 3
      PRINT #lr, TAB(14); USING "####.###"; al * k;
      FOR l = 1 TO 6
        PRINT #lr, USING "#######.###"; d(kk);
        kk = kk + 1
      NEXT l
      PRINT #lr,
    NEXT k
    lin = lin + 3
  END IF
  PRINT #lr, TAB(8); USING "######"; ABS(ij);
  PRINT #lr, USING "####.###"; al * 4;
  FOR k = 7 TO 12
    PRINT #lr, USING "#######.###"; d(k);
  NEXT k
  PRINT #lr, : lin = lin + 1
  FOR k = 1 TO 30
    LSET t8(k) = MKS$(d(k))
  NEXT k
  PUT #8, j
NEXT i
PRINT #lr, : lin = lin + 1
REM Calculo de las reacciones
FOR i = 1 TO nec
  q(i) = 0!
NEXT i
FOR i = 1 TO m
  GET #4, i
  ii = CVS(t(2))
  ij = CVS(t(3))
  k = 12
  FOR l = 1 TO 3
    kk = l
    FOR j = 1 TO 3
      r(kk) = CVS(t(k))
      kk = kk + 3
      k = k + 1
    NEXT j
  NEXT l
  IF nhip <> 0 THEN
    j = ncas * (i - 1) + icas
  ELSE
    j = i
  END IF
  GET #8, j
  FOR l = 1 TO 12
    d(l) = CVS(t8(l))
  NEXT l
  CALL chgsg(d())
  CALL mult1(r(), d(), w())
  CALL posi(w(), q(), ii, ij)
NEXT i
k = 1
n2 = n * 2
j = n2
FOR i = n + 1 TO n2
  GET #5, i
  FOR l = 1 TO 6
    q(k) = q(k) + CVS(t5(l))
    LSET t5(l) = MKS$(q(k))
    k = k + 1
  NEXT l
  PUT #5, (j + icas)
  IF nhip <> 0 THEN
    j = j + ncas
  END IF
NEXT i
IF lin > 53 THEN CALL enca(npag, lr, tg, lin, tc(icas))
CALL reac(lr, lin)
k = 1
FOR i = 1 TO n
  IF lin > 56 THEN
    CALL enca(npag, lr, tg, lin, tc(icas))
    CALL reac(lr, lin)
  END IF
  PRINT #lr, USING "#####"; i;
  FOR j = 1 TO 6
    PRINT #lr, USING "#######.####"; q(k);
    k = k + 1
  NEXT j
  PRINT #lr, : lin = lin + 1
NEXT i
IF ksig = -1 THEN mban = -mban
CLOSE #7
CLOSE #8
PRINT "Fin Programa "; tpi; TAB(30); "Parte "; tpf
IF icas < ncas THEN
  CHAIN tpd
ELSEIF nhip > 0 THEN
  CHAIN tpg
END IF
END

REM DEFDBL A-H, O-S, U-Z
SUB chgsg (d())
  d(1) = -d(1)
  d(4) = -d(4)
  d(6) = -d(6)
  d(8) = -d(8)
  d(9) = -d(9)
  d(11) = -d(11)
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB enca (npag, lr, tg, lin, tc)
DEFINT I-N
DEFSTR T
  npag = npag + 1
  PRINT #lr, CHR$(12)
  '$INCLUDE: 'enca.h'
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
SUB Junta (lr, lin)
PRINT #lr, "DESPLAZAMIENTO DE LAS JUNTAS."
PRINT #lr, "Junta              Dx              Dy              Dz              Rx              Ry              Rz"
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
SUB mult2 (sm(), w(), d())
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
SUB posi (w(), q(), ii, ij)
kk = 6 * (ii - 1)
kkk = 6 * (ij - 1)
FOR i = 1 TO 6
  kk = kk + 1
  kkk = kkk + 1
  q(kk) = q(kk) + w(i)
  q(kkk) = q(kkk) + w(i + 6)
NEXT i
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB posicion (q(), d(), ii, jj)
kk = 6 * (ii - 1)
kkk = 6 * (jj - 1)
FOR i = 1 TO 6
  kk = kk + 1
  kkk = kkk + 1
  d(i) = q(kk)
  d(i + 6) = q(kkk)
NEXT i
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB reac (lr, lin)
PRINT #lr, "CARGAS Y/O REACCIONES EN LAS JUNTAS"
PRINT #lr, "Junta          FX          FY          FZ          MX          MY          MZ"
lin = lin + 2
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB seccion (lr, lin)
  PRINT #lr, "FUERZAS EN LAS SECCIONES DE LOS MIEMBROS"
  PRINT #lr, "Miembro Junta       x          N         Vy         Vz          T         My         Mz"
  lin = lin + 2
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB sustinv (s(), q(), iw, nec, ned)
FOR k = 1 TO nec - 1
  l = iw * (k - 1)
  kk = l + k
  IF s(kk) = 0 THEN
    q(k) = 0!
  ELSE
    jf = iw + k
    IF jf > nec THEN jf = nec
    FOR j = k + 1 TO jf
      q(j) = q(j) - s(l + j) * q(k)
    NEXT j
    q(k) = q(k) / s(kk)
  END IF
NEXT k
kk = ned - iw
IF s(kk) = 0 THEN
  q(nec) = 0!
ELSE
  q(nec) = q(nec) / s(kk)
END IF
FOR i = 1 TO nec - 1
  k = nec - i
  l = iw * (k - 1)
  kk = l + k
  IF s(kk) <> 0 THEN
    jf = iw + k
    IF jf > nec THEN jf = nec
    FOR j = k + 1 TO jf
      q(k) = q(k) - s(l + j) * q(j)
    NEXT j
  END IF
NEXT i
END SUB

