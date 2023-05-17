REM MODULO ADE1
'$INCLUDE: 'defin.h'

DECLARE SUB dibujo (n, m, njo, itipo, x(), y(), z(), zx(), zy(), nji(), njj())

COMMON SHARED tc(), e(), g(), ct(), pes(), q(), s(), tghip(), tfile()
COMMON tpi, tpa, tpb, tpc, tpd, tpe, tpf, tpg, tdat, tpii, tpj, tph
COMMON n, jo, m, ncas, nmat, nhip, nhipr, itipo, ivr, npag
COMMON nmc, ato, icas, max1, lr, lin, tg, lo, nec, mban, njc, nma

DIM tfile(9), tghip(2)


DIM w(22), t(27), t1(13), t5(18), t8(30)
REM Comienzo del modulo
CLS
PRINT "*************************************************************************"
PRINT "* BLACK MOROCOIMA INGENIERIA Y PLANIFICACION       Programa ADE1        *"
PRINT "* ANALISIS ESTATICO DE ESTRUCTURAS ESPACIALES. Version 3.0 Junio 2009   *"
PRINT "* Programaci¢n: E.F.B.S.                                                *"
PRINT "*************************************************************************"
PRINT
PRINT "Nombre del disco para el area de trabajo: "; : td = INPUT$(1): PRINT td
PRINT "Nombre del archivo de datos de entrada: "; : INPUT tfile(3)
PRINT "Desea consierar las deformaciones por cortante (S/N): "; : tfile(2) = INPUT$(1): PRINT tfile(2)
lr = 6
PRINT "Salida por (P)antalla/(I)mpresora/(D)isco :"; : ta = INPUT$(1): PRINT ta
IF ta = "D" OR ta = "d" THEN
  PRINT "Nombre del archivo de datos para la salida: "; : INPUT tnomb
  tnomb = tnomb + ".SAL"
  OPEN "o", #lr, tnomb
ELSEIF ta = "I" OR ta = "i" THEN
  OPEN "o", #lr, "LPT1:"
  WIDTH #lr, 132
  PRINT #lr, CHR$(15)
ELSE
  OPEN "o", #lr, "SCRN:"
  CLS
END IF
tfile(1) = td + ":ade1.dat": tfile(4) = td + ":miembro.dat": tfile(5) = td + ":junta.dat"
tfile(6) = td + ":detnen.dat": tfile(7) = td + ":matriz.dat": tfile(8) = td + ":miembr1.dat"
tpi = "ade1": tpa = "ade1a": tpb = "ade1b": tpc = "ade1cop"
tpd = "ade1d": tpe = "ade1e": tpf = "ade1f": tpg = "ade1g"

REM Apertura de archivos
OPEN "i", #3, tfile(3)
OPEN "r", #1, tfile(1), 52
FIELD #1, 4 AS t1(1): k = 4
FOR i = 2 TO 13
    FIELD #1, k AS t1, 4 AS t1(i)
    k = k + 4
NEXT i
OPEN "r", #4, tfile(4), 108
FIELD #4, 4 AS t(1): k = 4
FOR i = 2 TO 27
    FIELD #4, k AS t1, 4 AS t(i)
    k = k + 4
NEXT i
OPEN "r", #5, tfile(5), 72
FIELD #5, 4 AS t5(1): k = 4
FOR i = 2 TO 18
    FIELD #5, k AS t1, 4 AS t5(i)
    k = k + 4
NEXT i
mban = 1
max2 = 6

REM Lectura de datos de la estructura
INPUT #3, tg
INPUT #3, n, jo, m, ncas, nmut, nhip, nhipr, itipo, ivr
nmat = nmut
IF nmat < 1 THEN nmat = 1
nec = 6 * n
ncus = ncas
IF ncus < 1 THEN ncus = 1
IF m < 0 THEN
    mban = -mban
    m = -m
END IF

REM Dimensionamiento dinamico de arreglos
DIM e(nmat), g(nmat), ct(nmat), pes(nmat), tc(ncus), q(nec)
DIM zx(n), zy(n), nji(m), njj(m), x(n), y(n), z(n)
FOR i = 1 TO nmat
  INPUT #3, j, e(j), g(j), ct(j), pes(j)
NEXT i
FOR i = 1 TO 13
  LSET t1(i) = MKS$(0)
NEXT i
jj = 0: w(9) = 0: w(10) = 0: w(11) = 0
FOR i = 1 TO n
  FOR k = 1 TO 4
      INPUT #3, w(k)
      j = w(1)
      x(j) = w(2)
      y(j) = w(3)
      z(j) = w(4)
      LSET t1(k) = MKS$(w(k))
  NEXT k
  jj = jj + 1
  PUT #1, jj
NEXT i
FOR i = 1 TO jo
  FOR k = 1 TO 13
      INPUT #3, w(k)
  NEXT k
  IF itipo = 1 THEN
    w(4) = 1
    w(5) = 1
    w(6) = 1
  ELSEIF itipo = 2 THEN
    w(2) = 1
    w(3) = 1
    w(7) = 1
  END IF
  FOR k = 1 TO 13
    LSET t1(k) = MKS$(w(k))
  NEXT k
  jj = jj + 1
  PUT #1, jj
NEXT i
FOR i = 1 TO m
  FOR k = 1 TO 10
      INPUT #3, w(k)
  NEXT k
  j = ABS(w(1))
  nji(j) = ABS(w(2))
  njj(j) = ABS(w(3))
  IF w(1) < 0 THEN INPUT #3, w(11), w(12), w(13)
  FOR k = 1 TO 13
     LSET t1(k) = MKS$(w(k))
  NEXT k
  jj = jj + 1
  PUT #1, jj
NEXT i
njo = n + jo
CALL dibujo(n, m, njo, itipo, x(), y(), z(), zx(), zy(), nji(), njj())
PRINT "¨Desea continuar S/N? :": trres = INPUT$(1)
IF (trres = "n" OR trres = "N") THEN END
REM Lectura de datos de las solicitaciones

max1 = jj: icas = 0
IF ncas > 0 THEN
  FOR jcas = 1 TO ncas
    INPUT #3, tc(jcas)
    FOR k = 1 TO 4
      INPUT #3, w(k)
      LSET t1(k) = MKS$(w(k))
    NEXT k
    jj = jj + 1
    PUT #1, jj
    njc = w(1): nma = w(2): nmc = w(3)
    IF njc > 0 THEN
      FOR i = 1 TO njc
        FOR k = 1 TO 7
          INPUT #3, w(k)
          LSET t1(k) = MKS$(w(k))
        NEXT k
        jj = jj + 1
        PUT #1, jj
      NEXT i
    END IF
    IF nma > 0 THEN
      FOR i = 1 TO nma
        FOR k = 1 TO 7
          INPUT #3, w(k)
          LSET t1(k) = MKS$(w(k))
        NEXT k
        jj = jj + 1
        PUT #1, jj
      NEXT i
    END IF
    IF nmc > 0 THEN
      FOR ii = 1 TO nmc
        a = 0
        FOR k = 1 TO 11
          INPUT #3, w(k)
          IF k > 1 AND k < 7 THEN a = a + w(k)
          LSET t1(k) = MKS$(w(k))
        NEXT k
        jj = jj + 1
        PUT #1, jj
        IF a > 0 THEN
          IF w(2) > 0 THEN
            FOR l = 1 TO w(2)
              FOR i = 1 TO 6
                INPUT #3, a
                LSET t1(i) = MKS$(a)
              NEXT i
              jj = jj + 1
              PUT #1, jj
            NEXT l
          END IF
          IF w(3) > 0 THEN
            FOR l = 1 TO w(3)
              FOR i = 1 TO 5
                INPUT #3, a
                LSET t1(i) = MKS$(a)
              NEXT i
              jj = jj + 1
              PUT #1, jj
            NEXT l
          END IF
          IF w(4) > 0 THEN
            FOR l = 1 TO w(4)
              FOR i = 1 TO 9
                INPUT #3, a
                LSET t1(i) = MKS$(a)
              NEXT i
              jj = jj + 1
              PUT #1, jj
            NEXT l
          END IF
          IF w(5) > 0 THEN
            FOR l = 1 TO w(5)
              FOR i = 1 TO 5
                INPUT #3, a
                LSET t1(i) = MKS$(a)
              NEXT i
              jj = jj + 1
              PUT #1, jj
            NEXT l
          END IF
          IF w(6) > 0 THEN
            FOR l = 1 TO w(6)
              FOR i = 1 TO 3
                INPUT #3, a
                LSET t1(i) = MKS$(a)
              NEXT i
              jj = jj + 1
              PUT #1, jj
            NEXT l
          END IF
        END IF
      NEXT ii
    END IF
  NEXT jcas
END IF
IF nhip > 0 THEN
  nhir = nhip
  IF nhipr > 0 THEN
    lli = 2
  ELSE
   lli = 1
  END IF
  FOR iii = 1 TO lli
    IF iii = 2 THEN nhir = nhipr
    INPUT #3, tghip(iii)
    FOR ii = 1 TO nhir
      INPUT #3, l, mcas
      LSET t1(1) = MKS$(l)
      LSET t1(2) = MKS$(mcas)
      jj = jj + 1
      PUT #1, jj
      FOR k = 1 TO mcas
        INPUT #3, i, fac
        LSET t1(1) = MKS$(i)
        LSET t1(2) = MKS$(fac)
        jj = jj + 1
        PUT #1, jj
      NEXT k
    NEXT ii
  NEXT iii
END IF
CLOSE #3
PRINT tg
PRINT "Fin del programa "; tpi
CHAIN tpa
END

REM DEFDBL A-H, O-S, U-Z
SUB dibujo (nn, ne, nntot, itipo, x(), y(), z(), zx(), zy(), nji(), njj())
SCREEN 10, 0, 0, 0
iii3 = 3
IF itipo > 0 THEN iii3 = 1
FOR iii = 1 TO iii3
  FOR i = 1 TO nn
    SELECT CASE iii <= iii3
      CASE iii = 1
        zx(i) = x(i)
        zy(i) = -y(i)
      CASE iii = 2
        zx(i) = x(i)
        zy(i) = z(i)
      CASE iii = 3
        zx(i) = z(i)
        zy(i) = -y(i)
      CASE ELSE
    END SELECT
  NEXT i
  xmin = 1E+10
  ymin = 1E+10
  FOR i = 1 TO nn
    a = zx(i)
    b = zy(i)
    IF a < xmin THEN xmin = a
    IF b < ymin THEN ymin = b
  NEXT i
  FOR i = 1 TO nn
    zx(i) = zx(i) - xmin
    zy(i) = zy(i) - ymin
  NEXT i
  xmax = -1E+10
  ymax = -1E+10
  FOR i = 1 TO nn
    a = zx(i)
    b = zy(i)
    IF a > xmax THEN xmax = a
    IF b > ymax THEN ymax = b
  NEXT i
  IF xmax = 0 THEN xmax = 1
  IF ymax = 0 THEN ymax = 1
  FOR i = 1 TO nn
    zx(i) = zx(i) * 639 / xmax
    zy(i) = zy(i) * 159 / ymax
  NEXT i
  IF iii <> 1 THEN
     LOCATE 23, 1: PRINT "Presione una tecla para continuar"
     WHILE INKEY$ = ""
     WEND
  END IF
  CLS
  SELECT CASE iii <= iii3
    CASE iii = 1
       PRINT "Plano OXY"
    CASE iii = 2
       PRINT "Plano OXZ"
    CASE iii = 3
       PRINT "Plano OZY"
    CASE ELSE
  END SELECT
  FOR i = 1 TO ne
    jjj = nntot + i
    j1 = nji(i)
    j2 = njj(i)
    LINE (zx(j1), zy(j1))-(zx(j2), zy(j2))
  NEXT i
NEXT iii
LOCATE 23, 1: PRINT "Presione una tecla para continuar"
WHILE INKEY$ = ""
WEND
SCREEN 0, 0, 0
CLS
END SUB

