2{potencialRemR3(N):selected(N)}2 :- rule(r3).
:- rule(r3), not potencialRemR3(_).

potencialRem(N) :- potencialRemR3(N).

selectedR3(N) :- selected(N), not potencialRemR3(N), rule(r3).

%#show potencialRemR3/1.

%not r1
invalidR1(N) :- potencialRemR3(N), id(N2), independent(N,N2), containsPS(N2,N), rule(r3).
validR1(N) :- not invalidR1(N), potencialRemR3(N), rule(r3).
invalidR1(N1) :- rule(r3), potencialRemR3(N1), element(E), {powerSet(N,E):selected(N), N!=N1}0.
:- rule(r3), potencialRemR3(N), validR1(N).

%#show invalidR1/1.
%#show validR1/1.


%not r2
potencialR32(N) :- independent(N2,N), potencialRemR3(N2), containsPS(N,N2), N!=N2.
:- rule(r3), 1{potencialR32(N):id(N)}.

%union of clauses
potencialR3(N) :- id(N), potencialRemR3(N1), potencialRemR3(N2), N1!=N2, containsPS(N,N1), containsPS(N,N2), N!=N1, N!=N2.
duplicateElementR3(E,N) :- element(E), potencialR3(N), potencialRemR3(N1), N1!=N, powerSet(N,E), powerSet(N1,E).
selectedR3(N) :- potencialR3(N), {element(E):powerSet(N,E), not duplicateElementR3(E,N)}0.

%#show potencialR3/1.

:- rule(r3), element(E), potencialRemR3(N2), {powerSet(N,E):selectedR3(N)}0.

:- rule(r3), not selectedR3(_).
:- selectedR3(_), not rule(r3).

#show selectedR3/1.