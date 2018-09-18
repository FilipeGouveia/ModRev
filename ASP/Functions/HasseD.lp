%number of elements
#const n = 4.
#const p = 15.

element(1..n).
id(1..p).

{powerSet(N,E) : id(N), element(E)}.
:- id(N), not powerSet(N,_).
1{powerSet(N2,E2):element(E2), E2!=E1, not powerSet(N1,E2)} :- id(N1), id(N2), N2 > N1, element(E1), powerSet(N1,E1).

powerSet(p,E) :- element(E).

% N1 contains N2
containsPS(N1,N2) :- id(N1), id(N2), {powerSet(N2,E):element(E), not powerSet(N1,E)}0.
countPS(N,S) :- id(N), S = #count {element(E):powerSet(N,E)}.
:- containsPS(N1,N2), id(N1), id(N2), N1 < N2.
:- countPS(N1,S1), countPS(N2,S2), id(N1), id(N2), N1 < N2, S1 > S2.

#show powerSet/2.