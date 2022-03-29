%at least one node updates at each time
1{update(P,T,V):vertex(V)} :- exp(P), time(P,T), time(P,T+1).

vlabel(P,T+1,V,1) :- update(P,T,V), 1{noneNegative(P,T,V,Id):functionOr(V,Id)}, vertex(V), exp(P), not r_part(V), time(P,T)+1.
vlabel(P,T+1,V,0) :- update(P,T,V), {noneNegative(P,T,V,Id):functionOr(V,Id)}0, vertex(V), exp(P), functionOr(V,_), not r_gen(V), time(P,T+1).

%keep all others
vlabel(P,T+1,V,S) :- not update(P,T,V), vlabel(P,T,V,S), time(P,T+1).

#show update/3.
