REM MODULO ADE1G
'$INCLUDE: 'defin.h'

DECLARE SUB enca (npag, lr, tg, lin, tc)
DECLARE SUB miem (lr, lin)
DECLARE SUB leahip (ncas(), fac(), t1(), nhip, jj, lr)
DECLARE SUB eschip (ncas(), fac(), nhip, jj, lr)
DECLARE SUB nodos (lr, lin)


COMMON tc(), e(), g(), ct(), pes(), q(), s(), tghip(), tfile()
COMMON tpi, tpa, tpb, tpc, tpd, tpe, tpf, tpg, tdat, tpii, tpj, tph
COMMON n, jo, m, ncas, nmat, nhip, nhipr, itipo, ivr, npag
COMMON nmc, ato, icas, max1, lr, lin, tg, lo, nec, mban, njc, nma

DIM t(22), t1(13), t5(18), t8(30), r(9)
DIM d(30), ncas(30), fac(30)

REM Apertura de campos

FIELD #1, 4 AS t1(1): k = 4
FOR i = 2 TO 13
    FIELD #1, k AS t1, 4 AS t1(i)
    k = k + 4
NEXT i

FIELD #4, 4 AS t(1): k = 4
FOR i = 2 TO 20
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
IF nhip > nhipr THEN
  nhir = nhip
ELSE
  nhir = nhipr
END IF
DIM conv(nhir, 30)

IF mban < 0 THEN
  icons = 12
ELSE
  icons = 30
END IF
tf = "FUERZAS EN LAS SECCIONES DE LOS MIEMBROS PARA LAS HIPOTESIS DE SOLICITACIONES"
tr = "REACCIONES EN LAS JUNTAS PARA LAS HIPOTESIS DE SOLICITACIONES"
mncas = m * ncas
jj = max1 + 1
CALL enca(npag, lr, tg, lin, tghip(1))
PRINT #lr, "Hipotesis  Caso de Carga  Factor"
CALL leahip(ncas(), fac(), t1(), nhip, jj, lr)
FOR i = 1 TO 30
  LSET t8(i) = MKS$(0)
NEXT i
FOR l = mncas + 1 TO mncas + m * nhip
  PUT #8, l
NEXT l

CALL enca(npag, lr, tg, lin, tf)
CALL miem(lr, lin)
iii = 0
FOR i = 0 TO mncas - 1 STEP ncas
  k = 1
  FOR ii = 1 TO nhip
    FOR l = 1 TO icons
      d(l) = 0!
    NEXT l
    kk = k
    k = k + 1
    FOR j = 1 TO fac(kk)
      GET #8, (i + ncas(k))
      FOR l = 1 TO icons
        d(l) = d(l) + CVS(t8(l)) * fac(k)
      NEXT l
      k = k + 1
    NEXT j
    FOR l = 1 TO icons
      LSET t8(l) = MKS$(d(l))
    NEXT l
    PUT #8, (mncas + iii * nhip + ncas(kk))
  NEXT ii
  iii = iii + 1
NEXT i
mnhip = m * nhip
ii = 1
FOR i = mncas TO mnhip + mncas - 1 STEP nhip
  IF mban > 0 AND lin > (56 - 5 * nhip) THEN
    CALL enca(npag, lr, tg, lin, tf)
    CALL miem(lr, lin)
  ELSEIF mban < 0 AND lin > (56 - 2 * nhip) THEN
    CALL enca(npag, lr, tg, lin, tf)
    CALL miem(lr, lin)
  END IF
  FOR j = 1 TO nhip
    GET #8, (i + j)
    FOR l = 1 TO icons
      conv(j, l) = CVS(t8(l))
    NEXT l
  NEXT j
  GET #4, ii
  al = CVS(t(1)) / 4
  PRINT #lr, USING "#######"; ii;
  PRINT #lr, USING "####.###"; 0;
  FOR j = 1 TO nhip
    PRINT #lr, TAB(22); USING "####"; j;
    FOR k = 1 TO 6
      PRINT #lr, USING "########.###"; conv(j, k);
    NEXT k
    PRINT #lr,
    lin = lin + 1
  NEXT j
  IF mban > 0 THEN
    FOR k = 1 TO 3
      PRINT #lr, TAB(8); USING "####.###"; al * k;
      FOR j = 1 TO nhip
        kk = 13 + (k - 1) * 6
        PRINT #lr, TAB(22); USING "####"; j;
        FOR l = 1 TO 6
          PRINT #lr, USING "########.###"; conv(j, kk);
          kk = kk + 1
        NEXT l
        PRINT #lr,
      NEXT j
    NEXT k
    lin = lin + 3 * nhip
  END IF
  PRINT #lr, TAB(8); USING "####.###"; al * 4;
  FOR j = 1 TO nhip
    PRINT #lr, TAB(22); USING "####"; j;
    FOR k = 7 TO 12
      PRINT #lr, USING "########.###"; conv(j, k);
    NEXT k
    PRINT #lr,
  NEXT j
  lin = lin + nhip
  ii = ii + 1
NEXT i
IF nhipr = 0 THEN
  tghip(2) = "HIPOTESIS DE SOLICITACIONES DE REACCION EN LAS JUNTAS"
  CALL enca(npag, lr, tg, lin, tghip(2))
  PRINT #lr, "Hipotesis  Caso de Carga  Factor"
  CALL eschip(ncas(), fac(), nhip, jj, lr)
  nhipr = nhip
ELSE
  CALL enca(npag, lr, tg, lin, tghip(2))
  PRINT #lr, "Hipotesis  Caso de Carga  Factor"
  CALL leahip(ncas(), fac(), t1(), nhipr, jj, lr)
END IF

nncas = n * (ncas + 2)
iii = 0
FOR i = (2 * n) TO (nncas - 1) STEP ncas
  k = 1
  FOR ii = 1 TO nhipr
    FOR l = 1 TO 6
      d(l) = 0!
    NEXT l
    kk = k
    k = k + 1
    FOR j = 1 TO fac(kk)
      GET #5, (i + ncas(k))
      FOR l = 1 TO 6
        d(l) = d(l) + CVS(t5(l)) * fac(k)
      NEXT l
      k = k + 1
    NEXT j
    FOR l = 1 TO 6
      LSET t5(l) = MKS$(d(l))
    NEXT l
    PUT #5, (nncas + iii * nhipr + ncas(kk))
  NEXT ii
  iii = iii + 1
NEXT i


CALL enca(npag, lr, tg, lin, tr)
CALL nodos(lr, lin)
k = nncas + 1
FOR i = 1 TO n
  IF lin > (56 - nhipr) THEN
    CALL enca(npag, lr, tg, lin, tr)
    CALL nodos(lr, lin)
 END IF
  PRINT #lr, USING "#####"; i;
  FOR j = 1 TO nhipr
    GET #5, k
    k = k + 1
    PRINT #lr, TAB(10); USING "##"; j;
     FOR l = 1 TO 6
       PRINT #lr, USING "########.###"; CVS(t5(l));
     NEXT l
     PRINT #lr,
     lin = lin + 1
  NEXT j
NEXT i
CLOSE
PRINT "Fin Programa "; tpi; TAB(30); "Parte "; tpg
END

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
SUB eschip (ncas(), fac(), nhip, jj, lr)
k = 1
FOR i = 1 TO nhip
  PRINT #lr, USING "#####"; ncas(k);
  kk = k
  k = k + 1
  FOR j = 1 TO fac(kk)
    PRINT #lr, TAB(17); USING "##"; ncas(k);
    PRINT #lr, TAB(25); USING "###.####"; fac(k)
    k = k + 1
  NEXT j
  PRINT #lr,
NEXT i
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB leahip (ncas(), fac(), t1(), nhip, jj, lr)
k = 1
FOR i = 1 TO nhip
  GET #1, jj
  jj = jj + 1
  ncas(k) = CVS(t1(1))
  PRINT #lr, USING "#####"; ncas(k);
  fac(k) = CVS(t1(2))
  kk = k
  k = k + 1
  FOR j = 1 TO fac(kk)
    GET #1, jj
    jj = jj + 1
    ncas(k) = CVS(t1(1))
    fac(k) = CVS(t1(2))
    PRINT #lr, TAB(17); USING "##"; ncas(k);
    PRINT #lr, TAB(25); USING "###.####"; fac(k)
    k = k + 1
  NEXT j
  PRINT #lr,
NEXT i
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB miem (lr, lin)
  PRINT #lr, "Miembro       x        Hip          N          Vy          Vz           T          My          Mz"
  lin = lin + 1
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB nodos (lr, lin)
  PRINT #lr, "Junta   Hip          Fx          Fy          Fz          Mx          My          Mz"
  lin = lin + 1
END SUB

