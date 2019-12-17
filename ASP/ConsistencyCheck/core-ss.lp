sign(0;1).
complement(T,S) :- sign(S),sign(T),T!=S.

vertex(V) :- edge(V,_,_).
vertex(V) :- edge(_,V,_).

%generate
1{vlabel(P,V,S):sign(S)}1:-vertex(V), exp(P).
{r_gen(V)} :- vertex(V), not fixed(V).
{r_part(V)} :- vertex(V), not fixed(V).


:-vlabel(P,V,S), obs_vlabel(P,V,T),complement(S,T).

% functions
%one positive or negative contribution in a clause
onePositive(P,V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S), vlabel(P,V2,S), exp(P).
oneNegative(P,V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S), vlabel(P,V2,T), complement(S,T), exp(P).

% none positive or none negative on a clause
% nonePositive(V,Id) :- not onePositive(V,Id), oneNegative(V,Id).
noneNegative(P,V,Id) :- onePositive(P,V,Id), not oneNegative(P,V,Id).

vlabel(P,V,1) :- 1{noneNegative(P,V,Id):functionOr(V,Id)}, vertex(V), exp(P), not r_part(V).
vlabel(P,V,0) :- {noneNegative(P,V,Id):functionOr(V,Id)}0, vertex(V), exp(P), functionOr(V,_), not r_gen(V).

%:- r_gen(V), r_part(V).

repair(V) :- r_gen(V).
repair(V) :- r_part(V).

%not sure if this makes sense
%either prevent the repair on non observed nodes
%or minimize the repairs needed
%:- repair(V), vertex(V), not obs_vlabel(V,_).
#minimize {1,V : repair(V)}.
#minimize {1,g,V : r_gen(V)}.
#minimize {1,p,V : r_part(V)}.

#show r_gen/1.
#show r_part/1.
#show vlabel/3.