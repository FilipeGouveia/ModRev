edge(c1,c2,0).
edge(c1,c3,1).
edge(c2,c1,1).
edge(c2,c3,1).
edge(c4,c2,1).
edge(c4,c3,0).

functionOr(c1,1).
functionAnd(c1,1,c2).

functionOr(c2,1).
functionAnd(c2,1,c1).
functionAnd(c2,1,c4).

functionOr(c3,1..2).
functionAnd(c3,1,c1).
functionAnd(c3,2,c2).
functionAnd(c3,2,c4).

obs_vlabel(c1,1).
obs_vlabel(c2,0).
obs_vlabel(c3,0).
obs_vlabel(c4,0).