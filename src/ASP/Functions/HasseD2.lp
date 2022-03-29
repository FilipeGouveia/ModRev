%number of elements

element(1..n).
id(1..p).

% N1 contains N2
containsPS(N1,N2) :- id(N1), id(N2), {element(E): powerSet(N2,E), not powerSet(N1,E)}0.

% N1 is father of N2 (contains N2)
father(N1,N2) :- containsPS(N1,N2), id(N1), id(N2), C = #count {element(E): powerSet(N1,E), not powerSet(N2,E)}, C=1.

selected(N1) :- id(N1), id(N2), clause(N2,_), {element(E) : powerSet(N1,E), not clause(N2,E)}0, {element(E) : clause(N2,E), not powerSet(N1,E)}0.
countPS(N,S) :- id(N), S = #count {element(E):powerSet(N,E)}.

:- element(E), {powerSet(N,E):selected(N)}0.

1{rule(r1;r2;r3)}1.

%#show clause/2.
%#show powerSet/2.
%#show selected/1.
% #show countPS/2.
%#show containsPS/2.
% #show father/2.
#show rule/1.


% TODO retirar simetrias de powerset senao explodem o numero de solucoes
%:- containsPS(N1,N2), id(N1), id(N2), N1 < N2.
%:- countPS(N1,S1), countPS(N2,S2), id(N1), id(N2), N1 < N2, S1 > S2.
%erro(N1,N2) :- id(N1), id(N2), countPS(N1,S), countPS(N2,S), element(E1), powerSet(N1,E1), not powerSet(N2,E1), element(E2), powerSet(N2,E2), not powerSet(N1,E2), N1 < N2, E1 > E2.
%:- id(N1), id(N2), erro(N1,N2), {element(E1): powerSet(N1,E1), not powerSet(N2,E1), element(E2), powerSet(N2,E2), E1 < E2}0.
%:- id(N1), id(N2), countPS(N1,S), countPS(N2,S), N1 < N2, {element(E1): powerSet(N1,E1), not powerSet(N2,E1), element(E2), powerSet(N2,E2), E1 < E2}0.
%:- id(N1), id(N2), countPS(N1,S), countPS(N2,S), 1{element(E1): powerSet(N1,E1), not powerSet(N2,E1), element(E2), powerSet(N2,E2), E1 < E2}0, N1 > N2.
%#show erro/2.
