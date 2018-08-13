#! /bin/csh -f

set os = `uname -s`

set bin = bin-$os

./$bin/clingo ConsistencyCheck/core-ss.lp --opt-mode=optN $argv
