
1{potencialRemR3(N):selected(N)}1 :- rule(r3).

2{selectedR3(N):potencialR25(N,N2),potencialRemR3(N2)} :- rule(r3).

selectedR3(N) :- selected(N), not potencialRemR3(N), rule(r3).

%:- selectedR3(N1), potencialR25(N1,N2), potencialRemR3(N2), {element(E):powerSet(N1,E), selectedR3(N3), id(N3), not powerSet(N3,E), N1 != N3}0.
duplicateElementR3(E,N1) :- powerSet(N1,E), selectedR3(N1), potencialR25(N1,N2), potencialRemR3(N2), 1{selectedR3(N3):id(N3), N3!=N1, powerSet(N3,E)}.
:- selectedR3(N1), potencialR25(N1,N2), potencialRemR3(N2), {element(E):powerSet(N1,E), not duplicateElementR3(E,N1)}0.

%selectedR3(N) :- id(N), selected(N), 1{selectedR3(N3):potencialR25(N3,_)}, {selectedR3(N2):father(N,N2),id(N2)}0.
:- selectedR3(N), id(N), selected(N), 1{selectedR3(N2):father(N,N2),id(N2)}.

:- element(E), selectedR3(N1), id(N1), {powerSet(N,E):selectedR3(N)}0.

:- rule(r3), not selectedR3(_).

:- selectedR3(_), not rule(r3).

#show selectedR3/1.
%#show potencialRemR3/1.