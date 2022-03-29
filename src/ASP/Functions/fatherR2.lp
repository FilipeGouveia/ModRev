potencialR25(N1) :- id(N1), id(N2), selected(N2), father(N2,N1), {containsPS(N3,N1): independent(N3)}0.

potencialR25(N1,N2) :- id(N1), id(N2), potencialR25(N1), father(N2,N1), {potencialR25(N3):father(N3,N1)}0.

{selectedR2(N):potencialR25(N,_)}1.

selectedR2(N) :- id(N), selected(N), 1{selectedR2(N3):potencialR25(N3,_)}, {selectedR2(N2):father(N,N2),id(N2)}0.

:- element(E), rule(r2), {powerSet(N,E):selectedR2(N)}0.

:- rule(r2), not selectedR2(_).

:- selectedR2(_), not rule(r2).

#show selectedR2/1.