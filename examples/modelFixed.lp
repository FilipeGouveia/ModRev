vertex(v1).
vertex(v2).
vertex(v3).
edge(v1,v2,1).
edge(v1,v3,1).
edge(v2,v1,1).
edge(v3,v1,1).
edge(v3,v2,0).
functionOr(v1,1..1).
functionAnd(v1,1,v2).
functionAnd(v1,1,v3).
functionOr(v2,1..2).
functionAnd(v2,1,v1).
functionAnd(v2,2,v3).
functionOr(v3,1..1).
functionAnd(v3,1,v1).

fixed(v1,v3).