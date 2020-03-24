%at least one node updates at each time
1{update(P,T,V):vertex(V)} :- exp(P), time(T), T<t.

vlabel(P,T+1,V,1) :- update(P,T,V), 1{noneNegative(P,T,V,Id):functionOr(V,Id)}, vertex(V), exp(P), not r_part(V), time(T), T<t.
vlabel(P,T+1,V,0) :- update(P,T,V), {noneNegative(P,T,V,Id):functionOr(V,Id)}0, vertex(V), exp(P), functionOr(V,_), not r_gen(V), time(T), T<t.

%keep all others
vlabel(P,T+1,V,S) :- not update(P,T,V), vlabel(P,T,V,S), time(T), T<t.

#show update/3.
