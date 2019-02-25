edge(cdc2_cdc13, cdc25, 1).
edge(pp, cdc25, 0).
edge(cdc25, cdc25, 1).

functionOr(cdc25, 1..3).
functionAnd(cdc25, 1, cdc2_cdc13).
functionAnd(cdc25, 1, pp).
functionAnd(cdc25, 2, cdc25).
functionAnd(cdc25, 2, pp).
functionAnd(cdc25, 3, cdc2_cdc13).
functionAnd(cdc25, 3, cdc25).

edge(ste9, cdc2_cdc13, 0).
edge(rum1, cdc2_cdc13, 0).
edge(slp1, cdc2_cdc13, 0).

functionOr(cdc2_cdc13, 1).
functionAnd(cdc2_cdc13, 1, ste9).
functionAnd(cdc2_cdc13, 1, rum1).
functionAnd(cdc2_cdc13, 1, slp1).

edge(ste9, cdc2_cdc13_a, 0).
edge(rum1, cdc2_cdc13_a, 0).
edge(slp1, cdc2_cdc13_a, 0).
edge(wee1_mik1, cdc2_cdc13_a, 0).
edge(cdc25, cdc2_cdc13_a, 1).

functionOr(cdc2_cdc13_a, 1).
functionAnd(cdc2_cdc13_a, 1, ste9).
functionAnd(cdc2_cdc13_a, 1, rum1).
functionAnd(cdc2_cdc13_a, 1, slp1).
functionAnd(cdc2_cdc13_a, 1, wee1_mik1).
functionAnd(cdc2_cdc13_a, 1, cdc25).

edge(slp1, pp, 1).

functionOr(pp, 1).
functionAnd(pp, 1, slp1).

edge(cdc2_cdc13, rum1, 0).
edge(cdc2_cdc13_a, rum1, 0).
edge(rum1, rum1, 1).
edge(sk, rum1, 0).
edge(pp, rum1, 1).

functionOr(rum1, 1..5).
functionAnd(rum1, 1, cdc2_cdc13).
functionAnd(rum1, 1, cdc2_cdc13_a).
functionAnd(rum1, 1, rum1).
functionAnd(rum1, 1, sk).
functionAnd(rum1, 2, cdc2_cdc13).
functionAnd(rum1, 2, cdc2_cdc13_a).
functionAnd(rum1, 2, pp).
functionAnd(rum1, 2, sk).
functionAnd(rum1, 3, cdc2_cdc13).
functionAnd(rum1, 3, cdc2_cdc13_a).
functionAnd(rum1, 3, pp).
functionAnd(rum1, 3, rum1).
functionAnd(rum1, 4, cdc2_cdc13).
functionAnd(rum1, 4, pp).
functionAnd(rum1, 4, rum1).
functionAnd(rum1, 4, sk).
functionAnd(rum1, 5, cdc2_cdc13_a).
functionAnd(rum1, 5, pp).
functionAnd(rum1, 5, rum1).
functionAnd(rum1, 5, sk).

edge(start, sk, 1).

functionOr(sk, 1).
functionAnd(sk, 1, start).

edge(cdc2_cdc13_a, slp1, 1).

functionOr(slp1, 1).
functionAnd(slp1, 1, cdc2_cdc13_a).

edge(cdc2_cdc13, ste9, 0).
edge(cdc2_cdc13_a, ste9, 0).
edge(ste9, ste9, 1).
edge(sk, ste9, 0).
edge(pp, ste9, 1).

functionOr(ste9, 1..5).
functionAnd(ste9, 1, cdc2_cdc13).
functionAnd(ste9, 1, cdc2_cdc13_a).
functionAnd(ste9, 1, ste9).
functionAnd(ste9, 1, sk).
functionAnd(ste9, 2, cdc2_cdc13).
functionAnd(ste9, 2, cdc2_cdc13_a).
functionAnd(ste9, 2, pp).
functionAnd(ste9, 2, sk).
functionAnd(ste9, 3, cdc2_cdc13).
functionAnd(ste9, 3, cdc2_cdc13_a).
functionAnd(ste9, 3, pp).
functionAnd(ste9, 3, ste9).
functionAnd(ste9, 4, cdc2_cdc13).
functionAnd(ste9, 4, pp).
functionAnd(ste9, 4, ste9).
functionAnd(ste9, 4, sk).
functionAnd(ste9, 5, cdc2_cdc13_a).
functionAnd(ste9, 5, pp).
functionAnd(ste9, 5, ste9).
functionAnd(ste9, 5, sk).


edge(cdc2_cdc13, wee1_mik1, 0).
edge(wee1_mik1, wee1_mik1, 1).
edge(pp, wee1_mik1, 1).

functionOr(wee1_mik1, 1..3).
functionAnd(wee1_mik1, 1, cdc2_cdc13).
functionAnd(wee1_mik1, 1, wee1_mik1).
functionAnd(wee1_mik1, 2, cdc2_cdc13).
functionAnd(wee1_mik1, 2, pp).
functionAnd(wee1_mik1, 3, pp).
functionAnd(wee1_mik1, 3, wee1_mik1).


%inputnode
%vlabel(P, start, 0) :- exp(P).
fixed(start).
%exp(1).