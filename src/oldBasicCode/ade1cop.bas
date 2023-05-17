REM MODULO ADE1C
'$INCLUDE: 'defin.h'

DECLARE SUB gmrm (al, el, gkl, eiyl, eizl, sm(), ciy, cjy, cy, c, ciz, cjz, cz)
DECLARE SUB mult1 (r(), sm(), sa())
DECLARE SUB mult2 (r(), sm(), sa())
DECLARE SUB ind (ji, jj, index())
DECLARE SUB trian (A(), mb, n)
DECLARE SUB formematriz (s(), sm(), index(), mban)


COMMON SHARED tc(), e(), g(), ct(), pes(), q(), s(), tghip(), tfile()
COMMON tpi, tpa, tpb, tpc, tpd, tpe, tpf, tpg, tdat, tpii, tpj, tph
COMMON n, jo, m, ncas, nmat, nhip, nhipr, itipo, ivr, npag
COMMON nmc, ato, icas, max1, lr, lin, tg, lo, nec, mban, njc, nma


DIM w(27), t(27), t1(13), t5(18), r(9), sa(12, 12), sm(12, 12), index(12)

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

REM Comienzo del modulo
IF mban < 0 THEN
  ksig = 1
  mban = -mban
END IF
DIM s(mban * nec)
FOR i = 1 TO ned
s(i) = 0!
NEXT i

FOR i = 1 TO m
   GET #4, i
   FOR j = 6 TO 9
     w(j) = CVS(t(j))
   NEXT j
   w(1) = CVS(t(1))
   ji = CVS(t(2))
   jj = CVS(t(3))
   iar = CVS(t(4))
   imat = CVS(t(5))
   FOR j = 21 TO 27
     w(j) = CVS(t(j))
   NEXT j
   k = 12
   FOR l = 1 TO 3
     kk = l
     FOR j = 1 TO 3
       r(kk) = CVS(t(k))
       kk = kk + 3
       k = k + 1
     NEXT j
   NEXT l
   CALL gmrm(w(1), w(6), w(7), w(8), w(9), sm(), w(21), w(22), w(23), w(24), w(25), w(26), w(27))
   CALL mult1(r(), sm(), sa())
   CALL mult2(r(), sm(), sa())
   CALL ind(ji, jj, index())
   CALL formematriz(s(), sm(), index(), mban)
NEXT i
REM Incorporacion de ceros el la diagonal para juntas restringidas e
REM incorporacion de las rigideces de los resortes a la matriz de
REM rigides de la estructura
kk = 1
i = 1
kount1 = 0
kount2 = 0
FOR k = 6 TO 12
  w(k) = 0
NEXT k
WHILE (lo > kount1 OR jo > kount2)
  GET #5, i
  jo1 = CVS(t5(17))
  IF jo1 <> 0 THEN
    kount2 = kount2 + 1
    lo1 = CVS(t5(16))
    IF lo1 = 1 THEN
      mm = 12
    ELSE
      mm = 6
    END IF
    FOR k = 1 TO mm
      w(k) = CVS(t5(k + 3))
    NEXT k
    FOR k = 1 TO 6
      IF w(k) = 1 THEN
        s(kk) = 0!
      ELSEIF w(k + 6) > 0 THEN
        s(kk) = s(kk) + w(k + 6)
        kount1 = kount1 + 1
      END IF
      kk = kk + mban
    NEXT k
  ELSE
    kk = kk + 6 * mban
  END IF
  i = i + 1
WEND
rem edgar = q(nec)

CALL trian(s(), mban, nec)

IF ksig = 1 THEN mban = -mban
CLOSE #7
PRINT "Fin Programa "; tpi; TAB(30); "Parte "; tpc
CHAIN tpd
END

REM DEFDBL A-H, O-S, U-Z
SUB formematriz (s(), sm(), index(), mban)
FOR l = 1 TO 12
  FOR j = l TO 12
    ii = index(l)
    jj = index(j)
    ii = (ii - 1) * mban + jj - ii + 1
    s(ii) = s(ii) + sm(l, j)
  NEXT j
NEXT l
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
SUB ind (ii, jj, index())
kk = 6 * (ii - 1)
kkk = 6 * (jj - 1)
FOR i = 1 TO 6
  kk = kk + 1
  kkk = kkk + 1
  index(i) = kk
  index(i + 6) = kkk
NEXT i
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB mult1 (r(), sm(), sa())
FOR l = 0 TO 11 STEP 3
  kk = 1
  FOR j = 1 TO 3
    lj = l + j
    FOR i = 1 TO 12
      sa(i, lj) = 0!
    NEXT i
    FOR k = 1 TO 3
      x = r(kk)
      kk = kk + 1
      IF x <> 0 THEN
        lk = l + k
        FOR i = 1 TO 12
          sa(i, lj) = sa(i, lj) + x * sm(i, lk)
        NEXT i
      END IF
    NEXT k
  NEXT j
NEXT l
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB mult2 (r(), sm(), sa())
FOR l = 0 TO 11 STEP 3
  kk = 1
  FOR i = 1 TO 3
    li = l + i
    FOR j = 1 TO 12
      sm(li, j) = 0!
    NEXT j
    FOR k = 1 TO 3
      x = r(kk)
      kk = kk + 1
      IF x <> 0 THEN
        lk = l + k
        FOR j = 1 TO 12
          sm(li, j) = sm(li, j) + x * sa(lk, j)
        NEXT j
      END IF
    NEXT k
  NEXT i
NEXT l
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB trian (A(), mb, n)
mb1 = mb - 1
jj = mb1
kk = 1
FOR i = 1 TO n - 1
  IF (A(kk) <> 0) THEN
    IF (i > n - mb1) THEN
      jj = n - i
    END IF
    FOR j = 1 TO jj
      ii = kk + j
      l = kk + j * mb
      IF (A(ii) <> 0 AND A(l) <> 0) THEN
        am = A(ii) / A(kk)
        FOR k = j TO jj
          A(l) = A(l) - am * A(ii)
          ii = ii + 1
          l = l + 1
          NEXT k
        A(kk + j) = am
      END IF
    NEXT j
  END IF
  kk = kk + mb
NEXT i
END SUB

