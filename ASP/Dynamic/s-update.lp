%TODO - cut T+1 as the last
vlabel(P,T+1,V,1) :- 1{noneNegative(P,T,V,Id):functionOr(V,Id)}, vertex(V), exp(P), not r_part(V), not topologicalerror(V), time(T), T<t.
vlabel(P,T+1,V,0) :- {noneNegative(P,T,V,Id):functionOr(V,Id)}0, vertex(V), exp(P), functionOr(V,_), not r_gen(V), not topologicalerror(V), time(T), T<t.

%there cannot exists 2 different times with the same update, which function's inputs are the same and the output is different
%any function applied in two time points with the same inputs cannot have different outputs
%ignore this to consider topological repairs
topologicalerror(V) :- time(T1), time(T2), T1 != T2, T1 < t, T2 < t, vertex(V), {vlabel(P1,T1,V1,S1): vlabel(P2,T2,V1,S2), S1!=S2, functionAnd(V,Id, V1)}0, vlabel(P1,T1+1,V,S3), vlabel(P2,T2+1,V,S4), S3 != S4, not input(V).
topologicalerror(V) :- time(T), T < t, exp(P1), exp(P2), P1 != P2, vertex(V), {vlabel(P1,T,V1,S1): vlabel(P2,T,V1,S2), S1!=S2, functionAnd(V,Id, V1)}0, vlabel(P1,T+1,V,S3), vlabel(P2,T+1,V,S4), S3 != S4, not input(V).

repair(V) :- topologicalerror(V).

#minimize {1@2,top,V : topologicalerror(V)}.

#show topologicalerror/1.