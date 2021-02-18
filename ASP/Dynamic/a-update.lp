%only one node updates at each time
1{update(P,T,V):vertex(V)}1 :- exp(P), time(T), T<t.
%:- input(V), update(_,_,V).

vlabel(P,T+1,V,1) :- update(P,T,V), 1{noneNegative(P,T,V,Id):functionOr(V,Id)}, vertex(V), exp(P), not r_part(V), not topologicalerror(V), time(T), T<t.
vlabel(P,T+1,V,0) :- update(P,T,V), {noneNegative(P,T,V,Id):functionOr(V,Id)}0, vertex(V), exp(P), functionOr(V,_), not r_gen(V), not topologicalerror(V), time(T), T<t.

%keep all others
vlabel(P,T+1,V,S) :- not update(P,T,V), vlabel(P,T,V,S), time(T), T<t.

%prevent updates that do not change the state of the network
:- update(P,T,V), vlabel(P,T,V,S), vlabel(P,T+1,V,S), time(T), T<t.


%there cannot exists 2 different times with the same update, which function's inputs are the same and the output is different
%any function applied in two time points with the same inputs cannot have different outputs
%ignore this to consider topological repairs
topologicalerror(V) :- time(T1), time(T2), T1 != T2, T1 < t, T2 < t, update(P1, T1, V), update(P2, T2, V), {vlabel(P1,T1,V1,S1) : vlabel(P2,T2,V1,S2), functionAnd(V,Id, V1), S1!=S2}0, vlabel(P1,T1+1,V,S3), vlabel(P2,T2+1,V,S4), S3 != S4, not input(V).
topologicalerror(V) :- time(T), T < t, update(P1, T, V), update(P2, T, V), P1 != P2, {vlabel(P1,T,V1,S1) : vlabel(P2,T,V1,S2), S1!=S2,  functionAnd(V,Id, V1)}0, vlabel(P1,T+1,V,S3), vlabel(P2,T+1,V,S4), S3 != S4, not input(V).

repair(V) :- topologicalerror(V).

#minimize {1@2,top,V : topologicalerror(V)}.

#show update/3.
#show topologicalerror/1.
