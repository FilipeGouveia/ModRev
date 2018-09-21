edge(c2,c1,1).
edge(c3,c1,0).
edge(c4,c1,1).
edge(c5,c1,1).
edge(c2,c4,0).
edge(c3,c5,1).

functionOr(c1,1..2).
functionAnd(c1,1,c2).
functionAnd(c1,1,c3).
functionAnd(c1,2,c4).
functionAnd(c1,2,c5).

functionOr(c4,1).
functionAnd(c4,1,c2).

functionOr(c5,1).
functionAnd(c5,1,c3).

obs_vlabel(c1,1).
obs_vlabel(c2,1).
obs_vlabel(c3,1).
obs_vlabel(c4,0).
obs_vlabel(c5,1).
