%time control per experiment
time(P,T) :- exp(P), obs_vlabel(P,T,_,_).
time(P,T) :- time(P,T+1), T+1 > 0.

%generate
1{vlabel(P,T,V,S):sign(S)}1:-vertex(V), exp(P), time(P,T).

:-vlabel(P,T,V,S1), obs_vlabel(P,T,V,S2), complement(S1,S2).

% functions
%one positive or negative contribution in a clause
onePositive(P,T,V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S), vlabel(P,T,V2,S), exp(P), time(P,T).
oneNegative(P,T,V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S1), vlabel(P,T,V2,S2), complement(S1,S2), exp(P), time(P,T).

% none negative on a clause
noneNegative(P,T,V,Id) :- onePositive(P,T,V,Id), not oneNegative(P,T,V,Id).

%input nodes
input(V) :- not functionOr(V,_), vertex(V).
%vlabel(P,T+1,V,S) :- input(V), vlabel(P,T,V,S), exp(P), time(P,T+1), not repair(V).
vlabel(P,T+1,V,0) :- input(V), vlabel(P,T,V,0), exp(P), time(P,T+1), not r_gen(V).
vlabel(P,T+1,V,1) :- input(V), vlabel(P,T,V,1), exp(P), time(P,T+1), not r_part(V).


%not sure if this makes sense
%either prevent the repair on non observed nodes
%or minimize the repairs needed
%:- repair(V), vertex(V), not obs_vlabel(V,_).
#minimize {1@1,V : repair(V)}.
%#minimize {1@1,g,V : r_gen(V), not topologicalerror(V)}.
#minimize {1@1,g,V : r_gen(V)}.
%#minimize {1@1,p,V : r_part(V), not topologicalerror(V)}.
#minimize {1@1,p,V : r_part(V)}.

#show vlabel/4.