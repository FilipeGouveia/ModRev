sign(0;1).
complement(T,S) :- sign(S),sign(T),T!=S.

vertex(V) :- edge(V,_,_).
vertex(V) :- edge(_,V,_).

%generate
1{vlabel(V,S):sign(S)}1:-vertex(V).
{r_gen(V)} :- vertex(V).
{r_part(V)} :- vertex(V).


:-vlabel(V,S), obs_vlabel(V,T),complement(S,T).

% functions
%one positive or negative contribution in a clause
onePositive(V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S), vlabel(V2,S).
oneNegative(V,Id) :- functionAnd(V,Id, V2), edge(V2,V,S), vlabel(V2,T), complement(S,T).

% none positive or none negative on a clause
% nonePositive(V,Id) :- not onePositive(V,Id), oneNegative(V,Id).
noneNegative(V,Id) :- onePositive(V,Id), not oneNegative(V,Id).

vlabel(V,1) :- 1{noneNegative(V,Id):functionOr(V,Id)}, vertex(V), not r_part(V).
vlabel(V,0) :- {noneNegative(V,Id):functionOr(V,Id)}0, vertex(V), not r_gen(V).

repair(f,V) :- r_gen(V).
repair(f,V) :- r_part(V).

%not sure if this makes sense
%either prevent the repair on non observed nodes
%or minimize the repairs needed
%:- repair(_,V), vertex(V), not obs_vlabel(V,_).
#minimize {1,V : repair(_,V)}.

#show r_gen/1.
#show r_part/1.