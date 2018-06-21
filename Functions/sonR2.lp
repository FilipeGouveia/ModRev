1{potencialRemR2(N):selected(N)}1 :- rule(r2).
:- rule(r2), not potencialRemR2(_).

potencialRem(N) :- potencialRemR2(N).

selectedR2(N) :- selected(N), not potencialRemR2(N), rule(r2).

potencialR2(N) :- independent(N2,N), potencialRemR2(N2), containsPS(N,N2), N!=N2.

selectedR2(N) :- potencialR2(N), rule(r2), {potencialR2(N2):containsPS(N,N2), N!=N2}0.

:- potencialRemR2(N2), element(E), rule(r2), {powerSet(N,E):selectedR2(N)}0.

:- rule(r2), {potencialR2(N):selectedR2(N)}0.
:- potencialR2(N), potencialRemR2(N2), not independent(N2,N), N2!=N.

:- rule(r2), not selectedR2(_).
:- selectedR2(_), not rule(r2).

#show selectedR2/1.
%#show potencialR2/1.
%#show potencialRem/1.