sign(0;1).
time(0..t).
complement(T,S) :- sign(S),sign(T),T!=S.

vertex(V) :- edge(V,_,_).
vertex(V) :- edge(_,V,_).

%generate
1{vlabel(P,T,V,S):sign(S)}1:-vertex(V), exp(P), time(T).
{r_gen(V)} :- vertex(V), not fixed(V).
{r_part(V)} :- vertex(V), not fixed(V).


:-vlabel(P,T,V,S1), obs_vlabel(P,T,V,S2), complement(S1,S2).

% functions
%one positive or negative contribution in a clause
onePositive(P,T,V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S), vlabel(P,T,V2,S), exp(P), time(T).
oneNegative(P,T,V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S1), vlabel(P,T,V2,S2), complement(S1,S2), exp(P), time(T).

% none negative on a clause
noneNegative(P,T,V,Id) :- onePositive(P,T,V,Id), not oneNegative(P,T,V,Id).

%input nodes
input(V) :- not functionOr(V,_), vertex(V).
vlabel(P,T+1,V,S) :- input(V), vlabel(P,T,V,S), exp(P), time(T), T<t, not repair(V).


%:- r_gen(V), r_part(V).

repair(V) :- r_gen(V).
repair(V) :- r_part(V).

%not sure if this makes sense
%either prevent the repair on non observed nodes
%or minimize the repairs needed
%:- repair(V), vertex(V), not obs_vlabel(V,_).
#minimize {1@1,V : repair(V)}.
%#minimize {1@1,g,V : r_gen(V), not topologicalerror(V)}.
#minimize {1@1,g,V : r_gen(V)}.
%#minimize {1@1,p,V : r_part(V), not topologicalerror(V)}.
#minimize {1@1,p,V : r_part(V)}.

#show repair/1.
#show r_gen/1.
#show r_part/1.
#show vlabel/4.