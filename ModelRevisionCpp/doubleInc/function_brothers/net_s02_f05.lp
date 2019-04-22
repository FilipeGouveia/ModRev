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

functionOr(ste9, 1..2).
functionAnd(ste9, 1, cdc2_cdc13).
functionAnd(ste9, 1, cdc2_cdc13_a).
functionAnd(ste9, 1, sk).
functionAnd(ste9, 2, cdc2_cdc13).
functionAnd(ste9, 2, cdc2_cdc13_a).
functionAnd(ste9, 2, ste9).
functionAnd(ste9, 2, pp).


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


exp(p1).
obs_vlabel(p1,start,0).
obs_vlabel(p1,cdc2_cdc13,0).
obs_vlabel(p1,pp,0).
obs_vlabel(p1,cdc25,1).
obs_vlabel(p1,ste9,0).
obs_vlabel(p1,rum1,1).
obs_vlabel(p1,slp1,0).
obs_vlabel(p1,wee1_mik1,0).
obs_vlabel(p1,cdc2_cdc13_a,0).
obs_vlabel(p1,sk,0).

exp(p2).
obs_vlabel(p2,start,0).
obs_vlabel(p2,cdc2_cdc13,0).
obs_vlabel(p2,pp,0).
obs_vlabel(p2,ste9,0).
obs_vlabel(p2,rum1,1).
obs_vlabel(p2,slp1,0).
obs_vlabel(p2,wee1_mik1,0).
obs_vlabel(p2,cdc2_cdc13_a,0).
obs_vlabel(p2,sk,0).
obs_vlabel(p2,cdc25,0).

exp(p3).
obs_vlabel(p3,start,0).
obs_vlabel(p3,cdc2_cdc13,0).
obs_vlabel(p3,pp,0).
obs_vlabel(p3,cdc25,1).
obs_vlabel(p3,ste9,0).
obs_vlabel(p3,rum1,1).
obs_vlabel(p3,slp1,0).
obs_vlabel(p3,wee1_mik1,1).
obs_vlabel(p3,cdc2_cdc13_a,0).
obs_vlabel(p3,sk,0).

exp(p4).
obs_vlabel(p4,start,0).
obs_vlabel(p4,cdc2_cdc13,0).
obs_vlabel(p4,pp,0).
obs_vlabel(p4,ste9,0).
obs_vlabel(p4,rum1,1).
obs_vlabel(p4,slp1,0).
obs_vlabel(p4,wee1_mik1,1).
obs_vlabel(p4,cdc2_cdc13_a,0).
obs_vlabel(p4,sk,0).
obs_vlabel(p4,cdc25,0).

exp(p5).
obs_vlabel(p5,start,0).
obs_vlabel(p5,cdc2_cdc13,0).
obs_vlabel(p5,pp,0).
obs_vlabel(p5,ste9,1).
obs_vlabel(p5,rum1,1).
obs_vlabel(p5,slp1,0).
obs_vlabel(p5,wee1_mik1,0).
obs_vlabel(p5,cdc2_cdc13_a,0).
obs_vlabel(p5,sk,0).
obs_vlabel(p5,cdc25,0).

exp(p6).
obs_vlabel(p6,start,0).
obs_vlabel(p6,cdc2_cdc13,0).
obs_vlabel(p6,pp,0).
obs_vlabel(p6,ste9,1).
obs_vlabel(p6,rum1,1).
obs_vlabel(p6,slp1,0).
obs_vlabel(p6,wee1_mik1,1).
obs_vlabel(p6,cdc2_cdc13_a,0).
obs_vlabel(p6,sk,0).
obs_vlabel(p6,cdc25,0).

exp(p7).
obs_vlabel(p7,start,0).
obs_vlabel(p7,cdc2_cdc13,0).
obs_vlabel(p7,pp,0).
obs_vlabel(p7,cdc25,1).
obs_vlabel(p7,ste9,1).
obs_vlabel(p7,rum1,1).
obs_vlabel(p7,slp1,0).
obs_vlabel(p7,wee1_mik1,0).
obs_vlabel(p7,cdc2_cdc13_a,0).
obs_vlabel(p7,sk,0).

exp(p8).
obs_vlabel(p8,start,0).
obs_vlabel(p8,cdc2_cdc13,0).
obs_vlabel(p8,pp,0).
obs_vlabel(p8,cdc25,1).
obs_vlabel(p8,ste9,1).
obs_vlabel(p8,rum1,1).
obs_vlabel(p8,slp1,0).
obs_vlabel(p8,wee1_mik1,1).
obs_vlabel(p8,cdc2_cdc13_a,0).
obs_vlabel(p8,sk,0).

exp(p9).
obs_vlabel(p9,start,0).
obs_vlabel(p9,cdc2_cdc13,0).
obs_vlabel(p9,pp,0).
obs_vlabel(p9,ste9,1).
obs_vlabel(p9,rum1,0).
obs_vlabel(p9,slp1,0).
obs_vlabel(p9,wee1_mik1,0).
obs_vlabel(p9,cdc2_cdc13_a,0).
obs_vlabel(p9,sk,0).
obs_vlabel(p9,cdc25,0).

exp(p10).
obs_vlabel(p10,start,0).
obs_vlabel(p10,cdc2_cdc13,0).
obs_vlabel(p10,pp,0).
obs_vlabel(p10,cdc25,1).
obs_vlabel(p10,ste9,1).
obs_vlabel(p10,rum1,0).
obs_vlabel(p10,slp1,0).
obs_vlabel(p10,wee1_mik1,0).
obs_vlabel(p10,cdc2_cdc13_a,0).
obs_vlabel(p10,sk,0).

exp(p11).
obs_vlabel(p11,start,0).
obs_vlabel(p11,cdc2_cdc13,0).
obs_vlabel(p11,pp,0).
obs_vlabel(p11,ste9,1).
obs_vlabel(p11,rum1,0).
obs_vlabel(p11,slp1,0).
obs_vlabel(p11,wee1_mik1,1).
obs_vlabel(p11,cdc2_cdc13_a,0).
obs_vlabel(p11,sk,0).
obs_vlabel(p11,cdc25,0).

exp(p12).
obs_vlabel(p12,start,0).
obs_vlabel(p12,cdc2_cdc13,0).
obs_vlabel(p12,pp,0).
obs_vlabel(p12,cdc25,1).
obs_vlabel(p12,ste9,1).
obs_vlabel(p12,rum1,0).
obs_vlabel(p12,slp1,0).
obs_vlabel(p12,wee1_mik1,1).
obs_vlabel(p12,cdc2_cdc13_a,0).
obs_vlabel(p12,sk,0).

exp(p13).
obs_vlabel(p13,cdc2_cdc13,0).
obs_vlabel(p13,pp,0).
obs_vlabel(p13,ste9,1).
obs_vlabel(p13,cdc2_cdc13_a,0).
obs_vlabel(p13,sk,1).


