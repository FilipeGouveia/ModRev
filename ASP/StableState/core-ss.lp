%generate
1{vlabel(P,V,S):sign(S)}1:-vertex(V), exp(P), not time(P,_).

:-vlabel(P,V,S1), obs_vlabel(P,V,S2),complement(S1,S2).

% functions
%one positive or negative contribution in a clause
onePositive(P,V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S), vlabel(P,V2,S), exp(P).
oneNegative(P,V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S), vlabel(P,V2,T), complement(S,T), exp(P).

% none positive or none negative on a clause
% nonePositive(V,Id) :- not onePositive(V,Id), oneNegative(V,Id).
noneNegative(P,V,Id) :- onePositive(P,V,Id), not oneNegative(P,V,Id).

vlabel(P,V,1) :- 1{noneNegative(P,V,Id):functionOr(V,Id)}, vertex(V), exp(P), not r_part(V).
vlabel(P,V,0) :- {noneNegative(P,V,Id):functionOr(V,Id)}0, vertex(V), exp(P), functionOr(V,_), not r_gen(V), not time(P,_).


%not sure if this makes sense
%either prevent the repair on non observed nodes
%or minimize the repairs needed
%:- repair(V), vertex(V), not obs_vlabel(V,_).
#minimize {1,V : repair(V)}.
#minimize {1,g,V : r_gen(V)}.
#minimize {1,p,V : r_part(V)}.

#show vlabel/3.
