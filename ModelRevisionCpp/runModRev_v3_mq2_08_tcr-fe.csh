#! /bin/csh -f

set exec = ./modrev

set folders = (TCRsig40)

set fprob = (5 25 50 100)
set eprob = (5 10 15 20 25 50 75)
set rprob = (1 5 10 15)
set aprob = (1 5 10 15)
set all = 0

set output_dir = ../resultsV3.1
set t_dir = times
set input_dir = ../inputTestsV3
set iterations = (0 1 2 3 4 5 6 7 8 9)

if(! -e $output_dir) then
        mkdir $output_dir
endif
if(! -e $output_dir/$t_dir) then
	mkdir $output_dir/$t_dir
endif

limit cputime 600
limit memoryuse 2048m


foreach it1 ($iterations)
    foreach it2 ($iterations)

        set it = $it1$it2

        echo "Staring Iteration"
        echo $it
        echo "\n"

        foreach folder ($folders)

            if(! -e $output_dir/$folder) then
                mkdir $output_dir/$folder
            endif

            if(! -e $output_dir/$t_dir/$folder) then
                mkdir $output_dir/$t_dir/$folder
            endif

            if(! -e $output_dir/$folder/$it) then
                mkdir $output_dir/$folder/$it
            endif

            if(! -e $output_dir/$t_dir/$folder/$it) then
                mkdir $output_dir/$t_dir/$folder/$it
            endif


            ./../../runsolver -w $output_dir/$t_dir/$folder/$it/watcher-$folder-25-5-0-0-net-att.txt -v $output_dir/$t_dir/$folder/$it/var-$folder-25-5-0-0-net-att.txt -C 600 -M 2048 $exec $input_dir/$folder/$it/$folder-25-5-0-0-net-att.lp > $output_dir/$folder/$it/$folder-25-5-0-0-net-att-repair.txt

            ./../../runsolver -w $output_dir/$t_dir/$folder/$it/watcher-$folder-50-25-0-0-net-att.txt -v $output_dir/$t_dir/$folder/$it/var-$folder-50-25-0-0-net-att.txt -C 600 -M 2048 $exec $input_dir/$folder/$it/$folder-50-25-0-0-net-att.lp > $output_dir/$folder/$it/$folder-50-25-0-0-net-att-repair.txt

            ./../../runsolver -w $output_dir/$t_dir/$folder/$it/watcher-$folder-100-50-0-0-net-att.txt -v $output_dir/$t_dir/$folder/$it/var-$folder-100-50-0-0-net-att.txt -C 600 -M 2048 $exec $input_dir/$folder/$it/$folder-100-50-0-0-net-att.lp > $output_dir/$folder/$it/$folder-100-50-0-0-net-att-repair.txt



        end

    end
end

echo "END\n"
