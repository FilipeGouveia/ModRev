1{potencialRemR1(N):selected(N)}1 :- rule(r1).
:- rule(r1), not potencialRemR1(_).

potencialRem(N) :- potencialRemR1(N).

selectedR1(N) :- selected(N), not potencialRemR1(N), rule(r1).

dominate(N1,N) :- selected(N), potencialRem(N1), N!=N1.
dominate(N,N1) :- id(N1), id(N2), selected(N2), containsPS(N1,N2), potencialRem(N), N2!=N, N1!=N.
dominate(N,N1) :- id(N1), id(N2), selected(N2), containsPS(N2,N1), potencialRem(N), N2!=N, N1!=N.

independent(N1,N) :- id(N), not dominate(N1,N), potencialRem(N1), N!=N1.
:- id(N), independent(N1,N), dominate(N1,N).

:- independent(N,_), not potencialRem(N).
:- dominate(N,_), not potencialRem(N).

:- potencialRem(N), {potencialRemR1(N);potencialRemR2(N);potencialRemR3(N)}0.

%#show independent/1.
%#show dominate/1.

:- potencialRemR1(N), id(N2), independent(N,N2), containsPS(N2,N), rule(r1).

:- rule(r1), element(E), potencialRemR1(N2), {powerSet(N,E):selectedR1(N)}0.

:- rule(r1), not selectedR1(_).
:- selectedR1(_), not rule(r1).

#show selectedR1/1.