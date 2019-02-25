edge(cdc2_cdc13_b1, cdc25, 1).
edge(pp, cdc25, 0).
edge(cdc2_cdc13_b2, cdc25, 1).

functionOr(cdc25, 1..2).
functionAnd(cdc25, 1, cdc2_cdc13_b1).
functionAnd(cdc25, 1, pp).
functionAnd(cdc25, 2, cdc2_cdc13_b1).
functionAnd(cdc25, 2, cdc2_cdc13_b2).

edge(ste9, cdc2_cdc13_b1, 0).
edge(rum1, cdc2_cdc13_b1, 0).
edge(slp1, cdc2_cdc13_b1, 0).
edge(cdc2_cdc13_b1, cdc2_cdc13_b1, 1).
edge(cdc2_cdc13_b2, cdc2_cdc13_b1, 1).

functionOr(cdc2_cdc13_b1, 1..2).
functionAnd(cdc2_cdc13_b1, 1, ste9).
functionAnd(cdc2_cdc13_b1, 1, rum1).
functionAnd(cdc2_cdc13_b1, 1, slp1).
functionAnd(cdc2_cdc13_b1, 2, cdc2_cdc13_b1).
functionAnd(cdc2_cdc13_b1, 2, cdc2_cdc13_b2).

edge(cdc2_cdc13_b1, cdc2_cdc13_b2, 1).
edge(ste9, cdc2_cdc13_b2, 0).
edge(rum1, cdc2_cdc13_b2, 0).
edge(slp1, cdc2_cdc13_b2, 0).
edge(wee1_mik1, cdc2_cdc13_b2, 0).
edge(cdc25, cdc2_cdc13_b2, 1).

functionOr(cdc2_cdc13_b2, 1).
functionAnd(cdc2_cdc13_b2, 1, cdc2_cdc13_b1).
functionAnd(cdc2_cdc13_b2, 1, ste9).
functionAnd(cdc2_cdc13_b2, 1, rum1).
functionAnd(cdc2_cdc13_b2, 1, slp1).
functionAnd(cdc2_cdc13_b2, 1, wee1_mik1).
functionAnd(cdc2_cdc13_b2, 1, cdc25).

edge(slp1, pp, 1).

functionOr(pp, 1).
functionAnd(pp, 1, slp1).

edge(sk, rum1, 0).
edge(cdc2_cdc13_b1, rum1, 0).
edge(pp, rum1, 1).

functionOr(rum1, 1..2).
functionAnd(rum1, 1, sk).
functionAnd(rum1, 1, cdc2_cdc13_b1).
functionAnd(rum1, 2, pp).

edge(start, sk, 1).

functionOr(sk, 1).
functionAnd(sk, 1, start).

edge(cdc2_cdc13_b1, slp1, 1).
edge(cdc2_cdc13_b2, slp1, 1).

functionOr(slp1, 1).
functionAnd(slp1, 1, cdc2_cdc13_b1).
functionAnd(slp1, 1, cdc2_cdc13_b2).

edge(sk, ste9, 0).
edge(cdc2_cdc13_b1, ste9, 0).
edge(pp, ste9, 1).

functionOr(ste9, 1..2).
functionAnd(ste9, 1, sk).
functionAnd(ste9, 1, cdc2_cdc13_b1).
functionAnd(ste9, 2, pp).

edge(cdc2_cdc13_b1, wee1_mik1, 0).
edge(pp, wee1_mik1, 1).

functionOr(wee1_mik1, 1..2).
functionAnd(wee1_mik1, 1, cdc2_cdc13_b1).
functionAnd(wee1_mik1, 2, pp).


%inputnode
%vlabel(P, start, 0) :- exp(P).
fixed(start).
%exp(1).
