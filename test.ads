VAR tmp $cVar
VAR ret 1

MARK factorial
LESS tmp 2
CJUMP end
MULT tmp ret ret
SUB tmp 1 tmp
JUMP factorial
MARK end

SET $cVar ret