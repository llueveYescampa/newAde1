REM MODULO ADE1A
'$INCLUDE: 'defin.h'

COMMON SHARED tc(), e(), g(), ct(), pes(), q(), s(), tghip(), tfile()
COMMON tpi, tpa, tpb, tpc, tpd, tpe, tpf, tpg, tdat, tpii, tpj, tph
COMMON n, jo, m, ncas, nmat, nhip, nhipr, itipo, ivr, npag
COMMON nmc, ato, icas, max1, lr, lin, tg, lo, nec, mban, njc, nma

DIM w(27), t(27), t1(13), t5(18), r(9)

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
REM Comienzo del modulo

IF mban < 0 THEN
  ksig = 1
ELSE
  ksig = 0
END IF
FOR i = 1 TO 18
  w(i) = 0
NEXT i
IF itipo = 1 THEN
  w(6) = 1
  w(7) = 1
  w(8) = 1
ELSEIF itipo = 2 THEN
  w(4) = 1
  w(5) = 1
  w(9) = 1
END IF
lo = 0
FOR i = 1 TO 18
  LSET t5(i) = MKS$(w(i))
NEXT i
jj = 0
FOR i = 1 TO n
  jj = jj + 1
  GET #1, jj
  j = CVS(t1(1))
  FOR k = 1 TO 3
    LSET t5(k) = t1(k + 1)
  NEXT k
  PUT #5, j
NEXT i
FOR i = 1 TO jo
  lo1 = 0
  jo1 = 0
  jj = jj + 1
  GET #1, jj
  j = CVS(t1(1))
  GET #5, j
  FOR k = 4 TO 15
    LSET t5(k) = t1(k - 2)
  NEXT k
  FOR k = 10 TO 15
    al = CVS(t5(k))
    IF al > 0 THEN
      lo1 = 1
      jo1 = 1
      lo = lo + 1
    ELSE
      l = CVS(t5(k - 6))
      IF l > 0 THEN
        jo1 = 1
      END IF
    END IF
  NEXT k
  LSET t5(16) = MKS$(lo1)
  LSET t5(17) = MKS$(jo1)
  PUT #5, j
NEXT i
REM Complemantacion en el archivo 4 de los datos de los miembros.
REM Determinaci¢n del semiancho de banda
mban = 0
jjj = jj
FOR j = 1 TO m
  jjj = jjj + 1
  GET #1, jjj
  FOR k = 1 TO 13
    w(k) = CVS(t1(k))
  NEXT k
  i = w(1)
  ji = w(2)
  jj = w(3)
  imat = w(4)
  ar = w(5)
  ax = w(6)
  ay = w(7)
  az = w(8)
  z1 = w(9)
  z2 = w(10)
  IF imat = 0 THEN imat = 1
  icp = 0
  IF i < 0 THEN
     i = -i
     icp = 1
     xp = w(11)
     yp = w(12)
     zp = w(13)
  END IF
  IF (ar + ax + ay + az) = 0 THEN
    ar = ara
    ax = axa
    ay = aya
    az = aza
    bz = bza
    by = bya
    fy = fya
    fz = fza
  ELSE
    IF (ay + az) <> 0 THEN
       bz = 0
       by = 0
       fy = ay * 1.5 / ar
       fz = az * 1.5 / ar
    ELSE
       bz = ar
       by = ax
       IF by = 0 THEN   ' Seccion circular
          d = .5 * bz
          ar = 3.141592654# * d * d
          ax = .5 * ar * d * d
          ay = .5 * ax
          az = ay
          fy = ay * 10 / (ar * 9)
          fz = az * 10 / (ar * 9)
       ELSE        ' Seccion rectangular
          ar = bz * by
          ax = ar / 12
          ay = ax * bz * bz
          az = ax * by * by
          IF bz < by THEN
             c = bz
             d = by
          ELSE
             c = by
             d = bz
          END IF
          f = c / d
          ax = c * c * c * d * (1 - .63 * f + .052 * f ^ 5) / 3
          fy = ay * 1.2 / ar
          fz = az * 1.2 / ar
       END IF
    END IF
    ara = ar
    axa = ax
    aya = ay
    aza = az
    bza = bz
    bya = by
    fya = fy
    fza = fz
  END IF
  iar = 0
  IF ji < 0 AND jj > 0 THEN
    iar = 1
    ji = -ji
  ELSEIF ji > 0 AND jj < 0 THEN
    iar = 2
    jj = -jj
  ELSEIF ji < 0 AND jj < 0 THEN
    iar = 3
    ji = -ji
    jj = -jj
  END IF
  IF ji >= jj THEN PRINT #lr, "ERROR JI >= JJ EN EL MIEMBRO :"; i
  IF ji >= jj THEN ivr = 1
  j1 = jj - ji
  IF j1 > mban THEN mban = j1
  GET #5, ji
  xlp = CVS(t5(1))
  ylp = CVS(t5(2))
  zlp = CVS(t5(3))
  GET #5, jj
  cx = CVS(t5(1))
  cy = CVS(t5(2))
  cz = CVS(t5(3))
  cx = cx - xlp
  cy = cy - ylp
  cz = cz - zlp
  al = SQR(cx * cx + cy * cy + cz * cz)
  cx = cx / al
  cy = cy / al
  cz = cz / al
  IF icp <> 0 THEN
    xlp = xp - xlp
    ylp = yp - ylp
    zlp = zp - zlp
  END IF
  q = cx * cx + cz * cz
REM Miembros verticales
  IF q < .001 THEN
    FOR k = 1 TO 9
      r(k) = 0
    NEXT k
    r(2) = cy
    REM Miembro vertical sin rotacion longitudinal 
    IF icp = 0 THEN
      r(4) = -cy
      r(9) = 1
    ELSE
    REM Miembro vertical con rotacion longitudinal
      a = SQR(xlp * xlp + zlp * zlp)
      c = -xlp * cy / a
      ss = zlp / a
      r(4) = -c * cy
      r(6) = ss
      r(7) = ss * cy
      r(9) = c
    END IF
REM Miembro no verticales
  ELSE
    q = SQR(q)
    r(1) = cx
    r(2) = cy
    r(3) = cz
    REM Miembros no verticales sin rotacion longitudinal
    r(4) = -cx * cy / q
    r(5) = q
    r(6) = -cy * cz / q
    r(7) = -cz / q
    r(9) = cx / q
    IF icp = 0 THEN
      r(8) = 0
    ELSE
    REM miembro no vertical con rotacion longitudinal
      a = r(4) * xlp + r(5) * ylp + r(6) * zlp
      c = r(7) * xlp + r(9) * zlp
      d = SQR(a * a + c * c)
      ss = c / d
      c = a / d
      r(4) = (-cx * cy * c - cz * ss) / q
      r(5) = q * c
      r(6) = (-cz * cy * c + cx * ss) / q
      r(7) = (cx * cy * ss - cz * c) / q
      r(8) = -q * ss
      r(9) = (cy * cz * ss + cx * c) / q
    END IF
  END IF
  z1 = z1 / al
  z2 = z2 / al
  z3 = 1 - z1 - z2
  a = e(imat) / al
  ar = ar * a
  ay = ay * a
  az = az * a
  a = g(imat) / al
  ax = ax * a
  w(1) = al
  w(2) = ji
  w(3) = jj
  w(4) = iar
  w(5) = imat
  w(6) = ar
  w(7) = ax
  w(8) = ay
  w(9) = az
  w(10) = bz
  w(11) = by
  IF tfile(2) = "S" OR tfile(2) = "s" THEN
    fy = fy * e(imat) / (g(imat) * al * al)
    fz = fz * e(imat) / (g(imat) * al * al)
  ELSE
    fy = 0
    fz = 0
  END IF
  c1 = z3 * (3 * z1 * z3 + 3 * z1 * z1 + z3 * z3) / 3
  c2 = z3 * (3 * z2 * z3 + 3 * z2 * z2 + z3 * z3) / 3
  c3 = z3 * (6 * z2 * z1 + 3 * z3 - 2 * z3 * z3) / 6
  w(21) = c1 + fy * z3
  w(22) = c2 + fy * z3
  w(23) = c3 - fy * z3
  w(24) = w(21) * w(22) - w(23) * w(23)
  w(21) = w(21) / w(24)
  w(22) = w(22) / w(24)
  w(23) = w(23) / w(24)
  w(25) = c1 + fz * z3
  w(26) = c2 + fz * z3
  w(27) = c3 - fz * z3
  w(24) = w(25) * w(26) - w(27) * w(27)
  w(25) = w(25) / w(24)
  w(26) = w(26) / w(24)
  w(27) = w(27) / w(24)
  w(24) = 1 / z3
  IF iar = 1 THEN
    w(26) = w(26) - w(27) * w(27) / w(25)
    w(25) = 0
    w(27) = 0
    w(22) = w(22) - w(23) * w(23) / w(21)
    w(21) = 0
    w(23) = 0
  ELSEIF iar = 2 THEN
    w(25) = w(25) - w(27) * w(27) / w(26)
    w(26) = 0
    w(27) = 0
    w(21) = w(21) - w(23) * w(23) / w(22)
    w(22) = 0
    w(23) = 0
  ELSEIF iar = 3 THEN
    w(21) = 0
    w(22) = 0
    w(23) = 0
    w(25) = 0
    w(26) = 0
    w(27) = 0
  END IF
  FOR k = 12 TO 20
    w(k) = r(k - 11)
  NEXT k
  FOR k = 1 TO 27
    LSET t(k) = MKS$(w(k))
  NEXT k
  PUT #4, i
  PRINT "Mbro= "; i; TAB(15); "Ji="; ji; TAB(30); "Jj="; jj; TAB(45); "L="; USING "##.###"; al
NEXT j
mban = 6 * (mban + 1)
IF ksig = 1 THEN mban = -mban
PRINT "Fin Programa "; tpi; TAB(30); "Parte "; tpa
CHAIN tpb
END

