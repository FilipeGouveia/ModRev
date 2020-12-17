#! /bin/csh -f

set version = 1.2.3
set bundleDir = bundle


if(! -e $bundleDir) then
        mkdir $bundleDir
endif

if(! -e $bundleDir/$version) then
        mkdir $bundleDir/$version
endif

if( -e $bundleDir/$version/ModRev) then
        printf "This bundle already exists. Please remove previous files at %s\n" $bundleDir/$version
        exit 0
endif

mkdir $bundleDir/$version/ModRev

mkdir $bundleDir/$version/ModRev/ASP
cp ASP/README-deploy.md $bundleDir/$version/ModRev/ASP/README.md
mkdir $bundleDir/$version/ModRev/ASP/ConsistencyCheck
cp ASP/ConsistencyCheck/core-ss.lp $bundleDir/$version/ModRev/ASP/ConsistencyCheck/
mkdir $bundleDir/$version/ModRev/ASP/Dynamic
cp ASP/Dynamic/*.lp $bundleDir/$version/ModRev/ASP/Dynamic/
cp -R examples $bundleDir/$version/ModRev/

cp ModelRevisionCpp/*.h $bundleDir/$version/ModRev/
cp ModelRevisionCpp/*.cc $bundleDir/$version/ModRev/
cp ModelRevisionCpp/makefile $bundleDir/$version/ModRev/
cp ModelRevisionCpp/README.md $bundleDir/$version/ModRev/
cp CHANGELOG.txt $bundleDir/$version/ModRev/
cp LICENSE $bundleDir/$version/ModRev/
cp -R ModelRevisionCpp/libOSX $bundleDir/$version/ModRev/
cp -R ModelRevisionCpp/libUNIX $bundleDir/$version/ModRev/
cp -R ModelRevisionCpp/headers $bundleDir/$version/ModRev/

printf "check_ASP=true\n" >> $bundleDir/$version/ModRev/config.txt
printf "ASP_dir=./ASP/\n" >> $bundleDir/$version/ModRev/config.txt
printf "ASP_CC_SS=./ASP/ConsistencyCheck/core-ss.lp\n" >> $bundleDir/$version/ModRev/config.txt
printf "ASP_CC_D=./ASP/Dynamic/core.lp\n" >> $bundleDir/$version/ModRev/config.txt
printf "ASP_CC_D_A=./ASP/Dynamic/a-update.lp\n" >> $bundleDir/$version/ModRev/config.txt
printf "ASP_CC_D_S=./ASP/Dynamic/s-update.lp\n" >> $bundleDir/$version/ModRev/config.txt
printf "ASP_CC_D_MA=./ASP/Dynamic/ma-update.lp\n" >> $bundleDir/$version/ModRev/config.txt
printf "compareLevelFunction=true\n" >> $bundleDir/$version/ModRev/config.txt
printf "exactMiddleFunctionDetermination=true\n" >> $bundleDir/$version/ModRev/config.txt
printf "forceOptimum=false\n" >> $bundleDir/$version/ModRev/config.txt

cd $bundleDir/$version
zip -r ModRev-$version.zip ModRev -x .DS_Store
cd ../..
