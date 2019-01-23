#! /bin/csh -f

set exec = ./../RandomNetworkMess/modmess

#set folders = (boolean_cell_cycle fissionYeastDavidich2008 fissionYeastDavidich2008Modified SP_1cell TCRsig40 thNetworkGarg2007)
set folders = (TCRsig40)

#set fprob = (5 25 50 100)
set fprob = (5)
#set eprob = (0 25 50 75)
set eprob = (25 50 75)
set rprob = (0 5 10 15)
set aprob = (0 5 10 15)

foreach folder ($folders)

    foreach f ($fprob)
        foreach e ($eprob)
            foreach r ($rprob)
                foreach a ($aprob)

                    $exec $folder/original/$folder.lp $f $e $r $a $folder/$folder-$f-$e-$r-$a-net.lp $folder/$folder-$f-$e-$r-$a.log
                    cat $folder/$folder-$f-$e-$r-$a-net.lp $folder/original/attractors.lp > $folder/$folder-$f-$e-$r-$a-net-att.lp

                end
            end
        end
    end

end