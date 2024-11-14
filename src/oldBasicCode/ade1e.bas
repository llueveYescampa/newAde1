REM MODULO ADE1E (Provicional)
'$INCLUDE: 'defin.h'

DECLARE SUB posici (q(), d(), i)
DECLARE SUB mult1 (r(), d(), w())
DECLARE SUB gmrm (al, el, gkl, eiyl, eizl, sm(), ciy, cjy, cy, c, ciz, cjz, cz)
DECLARE SUB mult2 (sm(), d(), w())
DECLARE SUB wk1 (wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
DECLARE SUB wk2 (wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
DECLARE SUB pk1 (wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
DECLARE SUB fuerza (lr, lin)
DECLARE SUB enca (npag, lr, tg, lin, tc)
DECLARE SUB cargaMembers (lr, lin)


COMMON tc(), e(), g(), ct(), pes(), q(), s(), tghip(), tfile()
COMMON tpi, tpa, tpb, tpc, tpd, tpe, tpf, tpg, tdat, tpii, tpj, tph
COMMON n, jo, m, ncas, nmat, nhip, nhipr, itipo, ivr, npag
COMMON nmc, ato, icas, max1, lr, lin, tg, lo, nec, mban, njc, nma

DIM w(30), t(27), t1(13), t5(18), t8(30), r(9)
DIM d(12), sm(12, 12), ww(11)


REM edgar = q(nec)

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

FIELD #8, 2 AS tlar: k = 2
FOR i = 1 TO 30
  FIELD #8, k AS t1, 4 AS t8(i)
  k = k + 4
NEXT i
 
REM Comienzo del modulo
jjj = max1
logico = 1
IF nmc > 0 THEN
  CALL cargaMembers(lr, lin)
FOR i = 1 TO nmc   ' Solo se ejecuta si existen miembros cargados
    IF lin > 56 THEN
      CALL enca(npag, lr, tg, lin, tc(icas))
      CALL cargaMembers(lr, lin)
    END IF
  PRINT #lr, : lin = lin + 1
  FOR ii = 1 TO 30
    w(ii) = 0!
  NEXT ii
  FOR ii = 1 TO 12
    d(ii) = 0!
    FOR j = 1 TO 12
      sm(ii, j) = 0!
    NEXT j
  NEXT ii
  jjj = jjj + 1
  GET #1, jjj
  ii = CVS(t1(1))
  npu = CVS(t1(2))
  npc = CVS(t1(3))
  npv = CVS(t1(4))
  npar = CVS(t1(5))
  ntor = CVS(t1(6))
  ato1 = CVS(t1(7))
  dty = CVS(t1(8))
  dtz = CVS(t1(9))
  by = CVS(t1(10))
  bz = CVS(t1(11))

REM buscar una mejor manera de considerar miembros repetidos.....
  IF npu > 0 THEN
     REDIM wx1(npu), wy1(npu), wz1(npu), aw(npu), bw(npu)
  END IF
  IF npc > 0 THEN
     REDIM px1(npc), py1(npc), pz1(npc), ap(npc)
  END IF
  IF npv > 0 THEN
     REDIM wvxi(npv), wvyi(npv), wvzi(npv), wvxj(npv), wvyj(npv), wvzj(npv), awv(npv), bwv(npv)
  END IF
  IF npar > 0 THEN
     REDIM parX(npar), parY(npar), parZ(npar), aPar(npar)
  END IF
  IF ntor > 0 THEN
     REDIM axialTotor(ntor), aTor(ntor), bTor(ntor)
  END IF
  
  
  IF (npu + npc + npv + npar + ntor + ato1 + dty + dtz = 0) THEN
     logico = 0
     npu = npu1
     npc = npc1
     npv = npv1
     npar = npar1
     ntor = ntor1
     ato1 = ato11
     dty = dty1
     dtz = dtz1
  ELSE
     logico = 1
     npu1 = npu
     npc1 = npc
     npv1 = npv
     npar1 = npar
     ntor1 = ntor
     ato11 = ato1
     dty1 = dty
     dtz1 = dtz
  END IF
  PRINT #lr, "Miembro"; USING "######"; ii;
  PRINT #lr, "        ATO1 ="; USING "#####.###"; ato1;
  PRINT #lr, " DTY ="; USING "#####.###"; dty;
  PRINT #lr, " DTZ ="; USING "#####.###"; dtz
  lin = lin + 1
    
  GET #4, ii
  al = CVS(t(1))
  iar = CVS(t(4))
  imat = CVS(t(5))
  FOR j = 6 TO 9
    ww(j - 5) = CVS(t(j))
  NEXT j
  IF by = 0 THEN
     by = CVS(t(11))
  END IF
  IF bz = 0 THEN
     bz = CVS(t(10))
  END IF
  
  FOR j = 21 TO 27
    ww(j - 16) = CVS(t(j))
  NEXT j
  axiso = 0!
  vyisoi = 0!
  vyisoj = 0!
  pariso = 0!
  vzisoi = 0!
  vzisoj = 0!
  
REM Calculo de las fuerzas primarias debido a cargas uniformes
   FOR ll = 1 TO npu  ' Solo se ejecuta si existen cargas uniformes
     IF logico = 0 THEN
       k = 0
       wx = wx1(ll)
       wy = wy1(ll)
       wz = wz1(ll)
       a = aw(ll)
       b = bw(ll)
     ELSE
       jjj = jjj + 1
       GET #1, jjj
       k = CVS(t1(1))
       wx = CVS(t1(2))
       wy = CVS(t1(3))
       wz = CVS(t1(4))
       a = CVS(t1(5))
       b = CVS(t1(6))
       wx1(ll) = wx
       wy1(ll) = wy
       wz1(ll) = wz
       aw(ll) = a
       bw(ll) = b
     END IF
     IF a < 0 THEN a = -a * al
     IF b < 0 THEN
       b = -b * al
     ELSEIF b = 0 THEN
       b = al
     END IF
     
     IF k > 0 THEN
       ji = CVS(t(2))
       jj = CVS(t(3))
       GET #5, ji
       xi = CVS(t5(1))
       yi = CVS(t5(2))
       zi = CVS(t5(3))
       GET #5, jj
       xj = CVS(t5(1))
       yj = CVS(t5(2))
       zj = CVS(t5(3))
       IF k = 1 THEN
         CALL wk1(wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
       ELSEIF k = 2 THEN
         CALL wk2(wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
       END IF
     END IF
     
     PRINT #lr, "Npu = "; USING "#"; ll;
     PRINT #lr, "  Wx ="; USING "#####.###"; wx;
     PRINT #lr, " Wy ="; USING "#####.###"; wy;
     PRINT #lr, " Wz ="; USING "#####.###"; wz;
     PRINT #lr, " A  ="; USING "#####.###"; a;
     PRINT #lr, " B ="; USING "#####.###"; b
     lin = lin + 1
     
     IF wx <> 0 THEN
       d(7) = d(7) - wx * b * (a + b * .5) / (ww(1) * al)
       axiso = axiso - wx * b
     END IF
     IF wy <> 0 THEN
        d(6) = d(6) - wy * b * (al - a - .5 * b) * (4 * al * (2 * a + b) - (2 * a + b) * (2 * a + b) - b * b) / (24 * al * al * ww(4))
        d(12) = d(12) + wy * b * (2 * a + b) * (2 * al * al - a * a - (a + b) * (a + b)) / (24 * al * al * ww(4))
        vyisoi = vyisoi - wy * b * (al - a - .5 * b) / al
        vyisoj = vyisoj - wy * b * (a + .5 * b) / al
     END IF
     IF wz <> 0 THEN
       d(5) = d(5) + wz * b * (al - a - .5 * b) * (4 * al * (2 * a + b) - (2 * a + b) * (2 * a + b) - b * b) / (24 * al * al * ww(3))
       d(11) = d(11) - wz * b * (2 * a + b) * (2 * al * al - a * a - (a + b) * (a + b)) / (24 * al * al * ww(3))
       vzisoi = vzisoi - wz * b * (al - a - .5 * b) / al
       vzisoj = vzisoj - wz * b * (a + .5 * b) / al
     END IF

     kk = 12
     FOR l = 1 TO 3
       xi = al / 4 * l
       IF xi < a THEN
         ax = 0!
         Vy = 0!
         Vz = 0!
'         par = 0!
         amy = 0!
         amz = 0!
       ELSEIF (xi >= a AND xi <= (a + b)) THEN
         ax = -(xi - a) * wx
         Vy = (xi - a) * wy
         Vz = (xi - a) * wz
'         par = 0!
         amy = Vz * (xi - a) * .5
         amz = Vy * (xi - a) * .5
       ELSEIF (xi > (a + b)) THEN
         ax = -wx * b
         Vy = wy * b
         Vz = wz * b
'         par = 0!
         amy = Vz * (b * .5 + (xi - a - b))
         amz = Vy * (b * .5 + (xi - a - b))
       END IF
         kk = kk + 1
       w(kk) = w(kk) + ax
         kk = kk + 1
       w(kk) = w(kk) + Vy
         kk = kk + 1
       w(kk) = w(kk) + Vz
         kk = kk + 2
'       w(kk) = w(kk) + par
'         kk = kk + 1
       w(kk) = w(kk) + amy
         kk = kk + 1
       w(kk) = w(kk) + amz
     NEXT l
   NEXT ll



REM Calculo de las fuerzas primarias debido a cargas concentradas
   FOR ll = 1 TO npc  ' Solo se ejecuta si existen cargas concentradas
     IF logico = 0 THEN
       k = 0
       px = px1(ll)
       py = py1(ll)
       pz = pz1(ll)
       a = ap(ll)
     ELSE
       jjj = jjj + 1
       GET #1, jjj
       k = CVS(t1(1))
       px = CVS(t1(2))
       py = CVS(t1(3))
       pz = CVS(t1(4))
       a = CVS(t1(5))
       px1(ll) = px
       py1(ll) = py
       pz1(ll) = pz
       ap(ll) = a
     END IF
     IF a < 0 THEN a = -a * al
     
     IF k > 0 THEN
       ji = CVS(t(2))
       jj = CVS(t(3))
       GET #5, ji
       xi = CVS(t5(1))
       yi = CVS(t5(2))
       zi = CVS(t5(3))
       GET #5, jj
       xj = CVS(t5(1))
       yj = CVS(t5(2))
       zj = CVS(t5(3))
       CALL pk1(px, py, pz, xi, yi, zi, xj, yj, zj, al)
     END IF
     
     PRINT #lr, "Npc = "; USING "#"; ll;
     PRINT #lr, "  Px ="; USING "#####.###"; px;
     PRINT #lr, " Py ="; USING "#####.###"; py;
     PRINT #lr, " Pz ="; USING "#####.###"; pz;
     PRINT #lr, " A ="; USING "#####.###"; a
     lin = lin + 1
     
     
     IF px <> 0 THEN
       d(7) = d(7) - px * a / (ww(1) * al)
       axiso = axiso - px
     END IF
     IF py <> 0 THEN
        d(6) = d(6) - py * a * (al - a) * (2 * al - a) / (6 * al * al * ww(4))
        d(12) = d(12) + py * a * (al * al - a * a) / (6 * al * al * ww(4))
        vyisoi = vyisoi - py * (al - a) / al
        vyisoj = vyisoj - py * a / al
     END IF
     IF pz <> 0 THEN
       d(5) = d(5) + pz * a * (al - a) * (2 * al - a) / (6 * al * al * ww(3))
       d(11) = d(11) - pz * a * (al * al - a * a) / (6 * al * al * ww(3))
       vzisoi = vzisoi - pz * (al - a) / al
       vzisoj = vzisoj - pz * a / al
     END IF

     kk = 12
     FOR l = 1 TO 3
       xi = al / 4 * l
       IF xi < a THEN
         ax = 0!
         Vy = 0!
         Vz = 0!
'         par = 0!
         amy = 0!
         amz = 0!
       ELSEIF (xi >= a) THEN
         ax = -px
         Vy = py
         Vz = pz
'         par = 0!
         amy = pz * (xi - a)
         amz = py * (xi - a)
       END IF
         kk = kk + 1
       w(kk) = w(kk) + ax
         kk = kk + 1
       w(kk) = w(kk) + Vy
         kk = kk + 1
       w(kk) = w(kk) + Vz
         kk = kk + 2
'       w(kk) = w(kk) + par
'         kk = kk + 1
       w(kk) = w(kk) + amy
         kk = kk + 1
       w(kk) = w(kk) + amz
     NEXT l
   NEXT ll





REM Calculo de las fuerzas primarias debido a cargas linealmente variables
   FOR ll = 1 TO npv  ' Solo se ejecuta si existen linealmente variables
     IF logico = 0 THEN
       k = 0
       wx = wvxi(ll)
       px = wvxj(ll)
       wy = wvyi(ll)
       py = wvyj(ll)
       wz = wvzi(ll)
       pz = wvzj(ll)
       a = awv(ll)
       b = bwv(ll)
     ELSE
       jjj = jjj + 1
       GET #1, jjj
       k = CVS(t1(1))
       wx = CVS(t1(2))
       px = CVS(t1(3))
       wy = CVS(t1(4))
       py = CVS(t1(5))
       wz = CVS(t1(6))
       pz = CVS(t1(7))
       a = CVS(t1(8))
       b = CVS(t1(9))
       wvxi(ll) = wx
       wvxj(ll) = px
       wvyi(ll) = wy
       wvyj(ll) = py
       wvzi(ll) = wz
       wvzj(ll) = pz
       awv(ll) = a
       bwv(ll) = b
     END IF
     IF a < 0 THEN a = -a * al
     IF b < 0 THEN
       b = -b * al
     ELSEIF b = 0 THEN
       b = al
     END IF
      
     IF k > 0 THEN
       ji = CVS(t(2))
       jj = CVS(t(3))
       GET #5, ji
       xi = CVS(t5(1))
       yi = CVS(t5(2))
       zi = CVS(t5(3))
       GET #5, jj
       xj = CVS(t5(1))
       yj = CVS(t5(2))
       zj = CVS(t5(3))
       IF k = 1 THEN
         CALL wk1(wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
         CALL wk1(px, py, pz, xi, yi, zi, xj, yj, zj, al)
       ELSEIF k = 2 THEN
         CALL wk2(wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
         CALL wk2(px, py, pz, xi, yi, zi, xj, yj, zj, al)
       END IF
     END IF
     
     
     
     
     
     PRINT #lr, "Npv = "; USING "#"; ll;
     PRINT #lr, "  Wxi ="; USING "####.###"; wx;
     PRINT #lr, " Wxj ="; USING "####.###"; px;
     PRINT #lr, " Wyi ="; USING "####.###"; wy;
     PRINT #lr, " Wyj ="; USING "####.###"; py;
     PRINT #lr, " Wzi ="; USING "###.###"; wz;
     PRINT #lr, " Wzj ="; USING "###.###"; pz;
     PRINT #lr, " A  ="; USING "#####.###"; a;
     PRINT #lr, " B ="; USING "#####.###"; b
     lin = lin + 1
     
     c = al - a - b
     
     atemp1 = px
     atemp2 = py
     atemp3 = pz
     IF c > 0 THEN
       px = wx + (px - wx) / b * (al - a)
       py = wy + (py - wy) / b * (al - a)
       pz = wz + (pz - wz) / b * (al - a)
       myloop = 2
     ELSE
       myloop = 1
     END IF
       
       
       FOR iv = 1 TO myloop
          d(7) = d(7) - (al - a) / (ww(1) * al) * (.5 * a * (wx + px) + (al - a) * (wx + 2 * px) / 6)
          axiso = axiso - .5 * (wx + px) * (al - a)
       
       
          d(6) = d(6) - (al - a) * (al - a) * (wy / (24 * al * al * ww(4)) * (al * al + 2 * al * a - a * a) + (py - wy) / (360 * al * al * ww(4)) * (7 * al * al + 6 * a * al - 3 * a * a))
          d(12) = d(12) + (wy / (24 * al * al * ww(4)) * (al * al - a * a) * (al * al - a * a) + (py - wy) / (360 * al * al * ww(4)) * (al - a) * (al - a) * (8 * al * al + 9 * a * al + 3 * a * a))
          
          ra = (al - a) * (al - a) * (.5 * wy / al + (py - wy) / (6 * al))
          vyisoi = vyisoi - ra
          vyisoj = vyisoj + ra - .5 * (al - a) * (wy + py)
        
          d(5) = d(5) + (al - a) * (al - a) * (wz / (24 * al * al * ww(3)) * (al * al + 2 * al * a - a * a) + (pz - wz) / (360 * al * al * ww(3)) * (7 * al * al + 6 * a * al - 3 * a * a))
          d(11) = d(11) - (wz / (24 * al * al * ww(3)) * (al * al - a * a) * (al * al - a * a) + (pz - wz) / (360 * al * al * ww(3)) * (al - a) * (al - a) * (8 * al * al + 9 * a * al + 3 * a * a))
        
          ra = (al - a) * (al - a) * (.5 * wz / al + (pz - wz) / (6 * al))
          vzisoi = vzisoi - ra
          vzisoj = vzisoj + ra - .5 * (al - a) * (wz + pz)
          
          
     kk = 12
     FOR l = 1 TO 3
       xi = al / 4 * l
       IF xi < a THEN
         ax = 0!
         Vy = 0!
         Vz = 0!
         amy = 0!
         amz = 0!
       ELSEIF (xi >= a) THEN
'         wpx = wx + (px - wx) / (al-a) * (xi-a)
'         ax = -(xi - a) * 0.5*(wx+wpx)
'         wpy = wy + (py - wy) / (al - a) * (xi - a)
'         Vy = (xi - a) * .5 * (wy + wpy)
'         wpz = wz + (pz - wz) / (al - a) * (xi - a)
'         Vz = (xi - a) * .5 * (wz + wpz)
         ax = -(xi - a) * (wx + .5 * (px - wx) * (xi - a) / (al - a))
         Vy = (xi - a) * (wy + .5 * (py - wy) * (xi - a) / (al - a))
         Vz = (xi - a) * (wz + .5 * (pz - wz) * (xi - a) / (al - a))
         amy = (xi - a) * (xi - a) * (.5 * wz + (pz - wz) * (xi - a) / (6 * (al - a)))
         amz = (xi - a) * (xi - a) * (.5 * wy + (py - wy) * (xi - a) / (6 * (al - a)))
       END IF
         kk = kk + 1
       w(kk) = w(kk) + ax
         kk = kk + 1
       w(kk) = w(kk) + Vy
         kk = kk + 1
       w(kk) = w(kk) + Vz
         kk = kk + 2
         ' mas dos porque no hay torque
       w(kk) = w(kk) + amy
         kk = kk + 1
       w(kk) = w(kk) + amz
     NEXT l
     
          
          px = -px
          py = -py
          pz = -pz
          wx = atemp1
          wy = atemp2
          wz = atemp3
          a = a + b
       NEXT iv
     
   NEXT ll
     


REM Calculo de las fuerzas primarias debido a Momento concentrados
   FOR ll = 1 TO npar  ' Solo se ejecuta si existen Momentos concentrados
     IF logico = 0 THEN
       k = 0
       px = parX(ll)
       py = parY(ll)
       pz = parZ(ll)
       a = aPar(ll)
     ELSE
       jjj = jjj + 1
       GET #1, jjj
       k = CVS(t1(1))
       px = CVS(t1(2))
       py = CVS(t1(3))
       pz = CVS(t1(4))
       a = CVS(t1(5))
       parX(ll) = px
       parY(ll) = py
       parZ(ll) = pz
       aPar(ll) = a
     END IF
     IF a < 0 THEN a = -a * al
     
     IF k > 0 THEN
       ji = CVS(t(2))
       jj = CVS(t(3))
       GET #5, ji
       xi = CVS(t5(1))
       yi = CVS(t5(2))
       zi = CVS(t5(3))
       GET #5, jj
       xj = CVS(t5(1))
       yj = CVS(t5(2))
       zj = CVS(t5(3))
       CALL pk1(px, py, pz, xi, yi, zi, xj, yj, zj, al)
     END IF
     
     PRINT #lr, "Npar= "; USING "#"; ll;
     PRINT #lr, "  Wx ="; USING "#####.###"; px;
     PRINT #lr, " Wy ="; USING "#####.###"; py;
     PRINT #lr, " Wz ="; USING "######.###"; pz;
     PRINT #lr, " A ="; USING "#######.###"; a
     lin = lin + 1
     
     
     IF px <> 0 THEN
       d(10) = d(10) - px * a / (ww(2) * al)
       pariso = pariso - px
     END IF
     IF py <> 0 THEN
       d(5) = d(5) - py / (6 * ww(3) * al * al) * (2 * al * al - 6 * a * al + 3 * a * a)
       d(11) = d(11) + py / (6 * ww(3) * al * al) * (al * al - 3 * a * a)
       vzisoi = vzisoi - py / al
       vzisoj = vzisoj + py / al
     END IF
     IF pz <> 0 THEN
       d(6) = d(6) - pz / (6 * ww(4) * al * al) * (2 * al * al - 6 * a * al + 3 * a * a)
       d(12) = d(12) + pz / (6 * ww(4) * al * al) * (al * al - 3 * a * a)
       vyisoi = vyisoi + pz / al
       vyisoj = vyisoj - pz / al
     END IF
     kk = 12
     FOR l = 1 TO 3
       xi = al / 4 * l
       IF xi < a THEN
         par = 0!
         amy = 0!
         amz = 0!
       ELSEIF (xi >= a) THEN
         par = -px
         amy = py
         amz = -pz
       END IF
         kk = kk + 4
       w(kk) = w(kk) + par
         kk = kk + 1
       w(kk) = w(kk) + amy
         kk = kk + 1
       w(kk) = w(kk) + amz
     NEXT l
   NEXT ll

REM Calculo de las fuerzas primarias debido a Torsor uniforme a lo largo del eje x local
   FOR ll = 1 TO ntor  ' Solo se ejecuta si existen Torsor uniforme a lo largo del eje x local
     IF logico = 0 THEN
       px = axialTor(ll)
       a = aTor(ll)
       b = bTor(ll)
     ELSE
       jjj = jjj + 1
       GET #1, jjj
       px = CVS(t1(1))
       a = CVS(t1(2))
       b = CVS(t1(3))
       axialTor(ll) = px
       aTor(ll) = a
       bTor(ll) = b
     END IF
     IF a < 0 THEN a = -a * al
     IF b < 0 THEN
       b = -b * al
     ELSEIF b = 0 THEN
       b = al
     END IF
     
     PRINT #lr, "Ntor= "; USING "#"; ll;
     PRINT #lr, "  T ="; USING "#####.###"; px;
     PRINT #lr, " A ="; USING "#######.###"; a;
     PRINT #lr, " B ="; USING "#######.###"; b
     lin = lin + 1
     
     d(10) = d(10) - px / (ww(2) * al) * (a * b + .5 * b * b)
     pariso = pariso - px * b
          
     kk = 16
     FOR l = 1 TO 3
       xi = al / 4 * l
       IF xi < a THEN
         par = 0!
       ELSEIF (xi >= a AND xi <= (a + b)) THEN
         par = -(xi - a) * px
       ELSEIF (xi > (a + b)) THEN
         par = -px * b
       END IF
       w(kk) = w(kk) + par
       kk = kk + 6
     NEXT l
   NEXT ll



   REM Determinacion de la contribucion de los cambios de temperaturas
   IF ato1 <> 0 THEN
      ato1 = ato1 - ato
      d(7) = d(7) - ato1 * al * ct(imat)
   END IF
   
   IF dty <> 0 THEN
      d(6) = d(6) + al * ct(imat) * dty / (2 * by)
      d(12) = d(12) - al * ct(imat) * dty / (2 * by)
   END IF
   
   IF dtz <> 0 THEN
      d(5) = d(5) - al * ct(imat) * dtz / (2 * bz)
      d(11) = d(11) + al * ct(imat) * dtz / (2 * bz)
   END IF


REM otros casos de carga en el mismo miembro
 
  CALL gmrm(al, ww(1), ww(2), ww(3), ww(4), sm(), ww(5), ww(6), ww(7), ww(8), ww(9), ww(10), ww(11))
  CALL mult2(sm(), d(), w())
  w(1) = axiso - w(7)
  w(2) = vyisoi + (w(6) + w(12)) / al
  w(3) = vzisoi - (w(5) + w(11)) / al
  w(4) = pariso - w(10)
  w(8) = vyisoj - (w(6) + w(12)) / al
  w(9) = vzisoj + (w(5) + w(11)) / al
  IF nhip <> 0 THEN
    j = ncas * (ii - 1) + icas
  ELSE
    j = ii
  END IF
  GET #8, j
  LSET tlar = MKI$(1)
  FOR k = 1 TO 30
    w(k) = w(k) + CVS(t8(k))
    LSET t8(k) = MKS$(w(k))
  NEXT k
  PUT #8, j
NEXT i ' no hay mas miembros cargados
PRINT #lr, : lin = lin + 1
END IF




logico = 0
FOR i = 1 TO m
  IF nhip <> 0 THEN
    j = ncas * (i - 1) + icas
  ELSE
    j = i
  END IF
  GET #8, j
  IF CVI(tlar) <> 0 THEN
    FOR k = 1 TO 12
      d(k) = CVS(t8(k))
    NEXT k
    GET #4, i
    ji = CVS(t(2))
    jj = CVS(t(3))
    k = 12
    FOR l = 1 TO 3
      kk = l
      FOR j = 1 TO 3
        r(kk) = CVS(t(k))
        kk = kk + 3
        k = k + 1
      NEXT j
    NEXT l
    CALL mult1(r(), d(), w())
    FOR l = 1 TO 6
      d(l) = w(l)
    NEXT l
    CALL posici(q(), d(), ji * 6)
    FOR l = 1 TO 6
      d(l) = w(l + 6)
    NEXT l
    CALL posici(q(), d(), jj * 6)
    
    IF (nma + nmc + ato > 0) THEN
      IF lin > 53 THEN
        CALL enca(npag, lr, tg, lin, tc(icas))
        CALL fuerza(lr, lin)
        logico = 1
      END IF
      IF logico = 0 THEN
        CALL fuerza(lr, lin)
        logico = 1
      END IF
      PRINT #lr, USING "#######"; i;
      PRINT #lr, USING "#######"; ji;
      PRINT #lr, TAB(16); USING "#########.###"; -CVS(t8(1)); CVS(t8(2)); CVS(t8(3)); -CVS(t8(4)); CVS(t8(5)); -CVS(t8(6))
      PRINT #lr, TAB(12); USING "###"; jj;
      PRINT #lr, TAB(16); USING "#########.###"; CVS(t8(7)); -CVS(t8(8)); -CVS(t8(9)); CVS(t8(10)); -CVS(t8(11)); CVS(t8(12))
      lin = lin + 2
      IF lin > 55 THEN
        CALL enca(npag, lr, tg, lin, tc(icas))
        CALL fuerza(lr, lin)
      END IF
    END IF
    
    
  END IF
NEXT i
PRINT #lr,
lin = lin + 1
max1 = jjj
CLOSE #8
PRINT "Fin Programa "; tpi; TAB(30); "Parte "; tpe
IF ivr = 0 THEN CHAIN tpf
IF icas < ncas THEN CHAIN tpd
END

REM DEFDBL A-H, O-S, U-Z
SUB cargaMembers (lr, lin)
PRINT #lr, "CARGAS Y VARIACIONES DE TEMPERATURA EN LOS MIEMBROS (referidas la systema local oxyx)."
lin = lin + 1
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
SUB fuerza (lr, lin)
  PRINT #lr, "FUERZAS PRIMARIAS EN LOS EXTREMOS DE LOS MIEMBROS"
  PRINT #lr, "Miembro  Junta             N           Vy           Vz            T           My           Mz"
  lin = lin + 2
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
SUB mult2 (sm(), d(), w())
FOR j = 1 TO 12
  w(j) = 0!
NEXT j
FOR k = 1 TO 12
  IF d(k) <> 0 THEN
    FOR i = 1 TO 12
      w(i) = w(i) + d(k) * sm(i, k)
    NEXT i
  END IF
NEXT k
END SUB


REM DEFDBL A-H, O-S, U-Z
SUB posici (q(), d(), i)
  kk = 6
  j = i - 5
  FOR k = i TO j STEP -1
    q(k) = q(k) - d(kk)
    kk = kk - 1
  NEXT k
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB pk1 (wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
  px = wx
  py = wy
  pz = wz
  zji = zj - zi
  xji = xj - xi
  all = SQR((zji * zji + xji * xji))
  coseno = all / al
  seno = (yj - yi) / al
  wy = py * coseno
  wx = py * seno
  coseno = xji / all
  seno = zji / all
  wx = wx + px * coseno + pz * seno
  wz = pz * coseno - px * seno
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB wk1 (wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
  wx1 = wx
  wy1 = wy
  zji = zj - zi
  xji = xj - xi
  all = SQR((zji * zji + xji * xji))
  coseno = all / al
  seno = (yj - yi) / al
  wy = wy1 * coseno
  wx = wy1 * seno
  coseno = xji / all
  seno = zji / all
  wx = wx + wx1 * coseno + wz * seno
  wz = wz * coseno - wx1 * seno
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB wk2 (wx, wy, wz, xi, yi, zi, xj, yj, zj, al)
  wx1 = wx
  zji = zj - zi
  xji = xj - xi
  all = SQR((zji * zji + xji * xji))
  coseno = all / al
  seno = (yj - yi) / al
  wx = wy * seno / coseno
  coseno = xji / all
  seno = zji / all
  wx = wx + wx1 * coseno / seno + wz * seno / coseno
  wz = wz - wx
END SUB

