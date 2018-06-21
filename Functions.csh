#! /bin/csh -f

if($#argv != 3) then
    printf "Error: incorrect number of arguments\n"
    printf "\t ./Functions.csh <s|f> <powerset file> <function file>\n"
    exit 0
endif


set os = `uname -s`
set bin = bin-$os
set files = ""


if($1 == "f") then
    set files = "Functions/fatherR1.lp Functions/fatherR2.lp Functions/fatherR3.lp"
else
    if($1 == "s") then
        set files = "Functions/sonR1.lp Functions/sonR2.lp Functions/sonR3.lp"
    else
        printf "Error: first argument must be 's' or 'f'\n"
        exit 0
    endif
endif


./$bin/clingo 0 Functions/HasseD2.lp $files $2 $3
