dominate(N) :- selectedR1(N).
dominate(N1) :- id(N1), id(N2), selectedR1(N2), containsPS(N1,N2).
dominate(N1) :- id(N1), id(N2), selectedR1(N2), containsPS(N2,N1).

independent(N) :- id(N), not dominate(N).
:- id(N), independent(N), dominate(N).

maxIndependent(N) :- id(N), independent(N), countPS(N,S), {countPS(N2,S2): independent(N2), S < S2}0.

%#show maxIndependent/1.
%#show independent/1.
%#show dominate/1.

:- rule(r1), not maxIndependent(_).
selected(N) :- selectedR1(N), id(N), rule(r1), 1{maxIndependent(N2):id(N2)}.
1{selected(N):maxIndependent(N)}1 :- rule(r1).

:- element(E), rule(r1), {powerSet(N,E):selected(N)}0.

:- rule(r1), not selectedR1(_).
:- selectedR1(_), not rule(r1).

#show selectedR1/1.