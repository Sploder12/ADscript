VAR tmp $cVar
VAR ret 1
MARK 2
LESS tmp 2
CJUMP 8
MULT tmp ret ret
SUB tmp 1 tmp
JUMP 2
MARK 8
SET $cVar ret