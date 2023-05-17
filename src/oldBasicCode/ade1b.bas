REM MODULO ADE1B
'$INCLUDE: 'defin.h'


DECLARE SUB enca (npag, lr, tg, lin)
DECLARE SUB junta (lr, lin)
DECLARE SUB miem (lr, lin)


COMMON SHARED tc(), e(), g(), ct(), pes(), q(), s(), tghip(), tfile()
COMMON tpi, tpa, tpb, tpc, tpd, tpe, tpf, tpg, tdat, tpii, tpj, tph
COMMON n, jo, m, ncas, nmat, nhip, nhipr, itipo, ivr, npag
COMMON nmc, ato, icas, max1, lr, lin, tg, lo, nec, mban, njc, nma

REM DIM tfile(9), tghip(2)

DIM w(27), t(27), t1(13), t5(18)

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

REM impresion de los datos de la estructura
npag = 0: PRINT #lr, CHR$(15)
CALL enca(npag, lr, tg, lin)
IF nhipr = 0 THEN
  naux = nhip
ELSE
  naux = nhipr
END IF
PRINT #lr,
PRINT #lr, "N="; n; TAB(12); "JO="; jo; TAB(24); "M="; m; TAB(36); "NCAS="; ncas; TAB(48); "NMAT="; nmat; TAB(60); "NHIP= "; nhip; TAB(72); "NHIPR = "; naux
PRINT #lr,
PRINT #lr, "Propiedades de los tipos de materiales."
PRINT #lr, "Tipo        E             G             Ct            Peso"
FOR i = 1 TO nmat
    PRINT #lr, i; TAB(12); e(i); TAB(26); g(i); TAB(40); ct(i); TAB(54); pes(i)
NEXT i
PRINT #lr,
lin = lin + 5 + nmat
IF (lin + n) > 56 THEN
   CALL enca(npag, lr, tg, lin)
END IF
CALL junta(lr, lin)
FOR j = 1 TO n
  IF lin > 56 THEN
    CALL enca(npag, lr, tg, lin)
    CALL junta(lr, lin)
  END IF
  GET #5, j
  FOR k = 1 TO 15
    w(k) = CVS(t5(k))
  NEXT k
  PRINT #lr, USING "##### #### #### #### #### #### #### #####.### #####.### #####.### ########## ########## ########## ########## ########## ##########"; j; w(4); w(5); w(6); w(7); w(8); w(9); w(1); w(2); w(3); w(10); w(11); w(12); w(13); w(14); w( _
15)
  lin = lin + 1
NEXT j
PRINT #lr,
lin = lin + 1
vol = 0
peso = 0
IF (lin + m) > 56 THEN
  CALL enca(npag, lr, tg, lin)
END IF
CALL miem(lr, lin)
FOR i = 1 TO m
  IF lin > 56 THEN
    CALL enca(npag, lr, tg, lin)
    CALL miem(lr, lin)
  END IF
  GET #4, i
  FOR k = 1 TO 11
    w(k) = CVS(t(k))
  NEXT k
  FOR k = 21 TO 27
    w(k) = CVS(t(k))
  NEXT k
  imat = w(5)
  a = e(imat) / w(1)
  w(6) = w(6) / a
  w(8) = w(8) / a
  w(9) = w(9) / a
  w(7) = w(7) * w(1) / g(imat)
  vol = vol + w(1) * w(6)
  peso = peso + pes(imat) * w(1) * w(6)
  iar = w(4)
  IF iar <> 0 THEN
    IF iar = 3 OR iar = 1 THEN w(2) = -w(2)
    IF iar = 3 OR iar = 2 THEN w(3) = -w(3)
  END IF
  IF w(10) > 0 THEN
    PRINT #lr, USING "#### ### ### ###.## ##.###^^^^ ##.###^^^^ ##.###^^^^ ##.###^^^^ ###.## ###.## ###.##"; i; ABS(w(2)); ABS(w(3)); w(1); w(6); w(7); w(8); w(9); w(25); w(26); w(27);
    PRINT #lr, USING "###.## ###.## ###.## ###.## #### ####.## ###.##"; w(21); w(22); w(23); w(24); imat; w(10); w(11)
  ELSE
    PRINT #lr, USING "#### ### ### ###.## ##.###^^^^ ##.###^^^^ ##.###^^^^ ##.###^^^^ ###.## ###.## ###.##"; i; ABS(w(2)); ABS(w(3)); w(1); w(6); w(7); w(8); w(9); w(25); w(26); w(27);
    PRINT #lr, USING "###.## ###.## ###.## ###.## #### "; w(21); w(22); w(23); w(24); imat
  END IF
  lin = lin + 1
NEXT i
PRINT #lr,
PRINT #lr, "Volumen ="; vol; TAB(39); "Peso="; peso
PRINT #lr,
lin = lin + 3

PRINT "Fin Programa "; tpi; TAB(30); "Parte "; tpb
IF ncas <= 0 THEN END
IF ivr <> 0 THEN CHAIN tpd
CHAIN tpc
END

REM DEFDBL A-H, O-S, U-Z
SUB enca (npag, lr, tg, lin)
  npag = npag + 1
  IF npag > 1 THEN
    PRINT #lr, CHR$(12)
  END IF
  '$INCLUDE: 'enca.h'
  lin = 8
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB junta (lr, lin)
  PRINT #lr, "Datos de las Juntas."
  PRINT #lr, "Junta  Rdx  Rdy  Rdz  Rrx  Rry  Rrz         X         Y         Z         Kdx        Kdy        Kdz        Krx        Kry        Krz"
  lin = lin + 2
END SUB

REM DEFDBL A-H, O-S, U-Z
SUB miem (lr, lin)
  PRINT #lr, "Datos de los miembros."
  PRINT #lr, "Mbro  Ji  Jj   Long.      Area          K        Iyy        Izz   Cixy   Cjxy   Cxy   Cixz   Cjxz   Cxz   Cn=Ct T de M     b      h"
  lin = lin + 2
END SUB

