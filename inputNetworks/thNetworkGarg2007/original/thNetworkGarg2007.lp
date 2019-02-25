edge(tcr, nfat, 1).

functionOr(nfat, 1).
functionAnd(nfat, 1, tcr).

edge(ifn_b, ifn_br, 1).

functionOr(ifn_br, 1).
functionAnd(ifn_br, 1, ifn_b).

edge(il_18, il_18r, 1).
edge(stat6, il_18r, 0).

functionOr(il_18r, 1).
functionAnd(il_18r, 1, il_18).
functionAnd(il_18r, 1, stat6).

edge(il_18r, irak, 1).

functionOr(irak, 1).
functionAnd(irak, 1, il_18r).

edge(t_bet, socs1, 1).
edge(stat1, socs1, 1).

functionOr(socs1, 1..2).
functionAnd(socs1, 1, t_bet).
functionAnd(socs1, 2, stat1).

edge(il_12, il_12r, 1).
edge(stat6, il_12r, 0).

functionOr(il_12r, 1).
functionAnd(il_12r, 1, il_12).
functionAnd(il_12r, 1, stat6).

edge(il_12r, stat4, 1).
edge(gata3, stat4, 0).

functionOr(stat4, 1).
functionAnd(stat4, 1, il_12r).
functionAnd(stat4, 1, gata3).

edge(t_bet, t_bet, 1).
edge(stat1, t_bet, 1).
edge(gata3, t_bet, 0).

functionOr(t_bet, 1..2).
functionAnd(t_bet, 1, t_bet).
functionAnd(t_bet, 1, gata3).
functionAnd(t_bet, 2, stat1).
functionAnd(t_bet, 2, gata3).

edge(nfat, ifn_g, 1).
edge(irak, ifn_g, 1).
edge(stat4, ifn_g, 1).
edge(t_bet, ifn_g, 1).
edge(stat3, ifn_g, 0).

functionOr(ifn_g, 1..4).
functionAnd(ifn_g, 1, nfat).
functionAnd(ifn_g, 1, stat3).
functionAnd(ifn_g, 2, irak).
functionAnd(ifn_g, 2, stat3).
functionAnd(ifn_g, 3, stat4).
functionAnd(ifn_g, 3, stat3).
functionAnd(ifn_g, 4, t_bet).
functionAnd(ifn_g, 4, stat3).

edge(ifn_g, ifn_gr, 1).

functionOr(ifn_gr, 1).
functionAnd(ifn_gr, 1, ifn_g).

edge(ifn_gr, jak1, 1).
edge(socs1, jak1, 0).

functionOr(jak1, 1).
functionAnd(jak1, 1, ifn_gr).
functionAnd(jak1, 1, socs1).

edge(jak1, stat1, 1).
edge(ifn_br, stat1, 1).

functionOr(stat1, 1..2).
functionAnd(stat1, 1, jak1).
functionAnd(stat1, 2, ifn_br).

edge(stat1, il_4, 0).
edge(gata3, il_4, 1).

functionOr(il_4, 1).
functionAnd(il_4, 1, stat1).
functionAnd(il_4, 1, gata3).

edge(il_4, il_4r, 1).
edge(socs1, il_4r, 0).

functionOr(il_4r, 1).
functionAnd(il_4r, 1, il_4).
functionAnd(il_4r, 1, socs1).

edge(il_4r, stat6, 1).

functionOr(stat6, 1).
functionAnd(stat6, 1, il_4r).

edge(stat6, gata3, 1).
edge(gata3, gata3, 1).
edge(t_bet, gata3, 0).

functionOr(gata3, 1..2).
functionAnd(gata3, 1, stat6).
functionAnd(gata3, 1, t_bet).
functionAnd(gata3, 2, gata3).
functionAnd(gata3, 2, t_bet).

edge(gata3, il_10, 1).

functionOr(il_10, 1).
functionAnd(il_10, 1, gata3).

edge(il_10, il_10r, 1).

functionOr(il_10r, 1).
functionAnd(il_10r, 1, il_10).

edge(il_10r, stat3, 1).

functionOr(stat3, 1).
functionAnd(stat3, 1, il_10r).


%exp(1).

%vlabel(P, tcr, 0) :- exp(P).
%vlabel(P, il_18, 0) :- exp(P).
%vlabel(P, il_12, 0) :- exp(P).
%vlabel(P, ifn_b, 0) :- exp(P).
fixed(tcr).
fixed(il_18).
fixed(il_12).
fixed(ifn_b).
