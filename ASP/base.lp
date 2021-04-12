sign(0;1).
complement(T,S) :- sign(S),sign(T),T!=S.

%inferring vertex from edges
vertex(V) :- edge(V,_,_).
vertex(V) :- edge(_,V,_).

% each vertex may or may not be in need of repair
{r_gen(V)} :- vertex(V), not fixed(V).
{r_part(V)} :- vertex(V), not fixed(V).

repair(V) :- r_gen(V).
repair(V) :- r_part(V).

#show repair/1.
#show r_gen/1.
#show r_part/1.