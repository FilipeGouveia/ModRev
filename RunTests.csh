#! /bin/csh -f

printf "Starting\n"
set NOW = `date +"%Y-%m-%d-%T"`
set log = log-tests-$NOW.txt
set toLog = 0

set os = `uname -s`

set bin = bin-$os
set resdir = results-Functions
set functionDir = Functions

if ($toLog > 0) then
    echo '' >> $log
endif

if(! -e $resdir) then
    if ($toLog > 0) then
        echo 'Creating results directory.' >> $log
    endif
    mkdir $resdir
endif

if(! -e $bin/powersetGen) then
    printf "\tCompiling powerset generator\n"
    if ($toLog > 0) then
        echo '\tCompiling powerset generator\n' >> $log
    endif
    make -C $functionDir clean
    make -C $functionDir
    cp $functionDir/powersetGen $bin/.
    make -C $functionDir clean
endif

limit cputime 10800
limit memoryuse 2048m

if ($toLog > 0) then
    echo 'Starting.' >> $log
endif

foreach dir ($functionDir/facts/*/)

    set basedir = `basename $dir`

    if(! -e $resdir/$basedir) then
        mkdir $resdir/$basedir
    endif

    if(! -e $functionDir/facts/$basedir/powerSet$basedir.lp) then
        ./$bin/powersetGen $basedir > $functionDir/facts/powerSet$basedir.lp
    endif

   foreach file ($dir/clauses*)
        set basefile = `basename $file .lp`
        #(time ./$bin/clingo 0 $functionDir/HasseD2.lp $functionDir/fatherR1.lp $functionDir/fatherR2.lp $functionDir/fatherR3.lp $functionDir/facts/$basedir/powerSet$basedir.lp $file > $resdir/$basedir/$basefile-sol.txt) > $resdir/$basedir/$basefile-time.txt
   end
end

		#./../../runsolver -w $resdir/$ressubdir/watcher-$goal.txt -v $resdir/$ressubdir/var-$goal.txt -C 600 ./$solver files/board.lp 16 files/robots.lp files/goals.lp $goal 1 > $resdir/$ressubdir/solution-$goal.txt

goto done

##
done:
        printf " Done.\n"
        if ($toLog > 0) then
            echo '' >> $log
            echo 'Done.' >> $log
        endif
        exit 0
##
error:
        printf " Failed.\n"
        if ($toLog > 0) then
            echo '' >> $log
            echo 'Error.' >> $log
        endif
        exit 1