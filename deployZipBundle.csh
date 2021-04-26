#! /bin/csh -f

set version = 1.3.0
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
cp ASP/base.lp $bundleDir/$version/ModRev/ASP/
mkdir $bundleDir/$version/ModRev/ASP/StableState
cp ASP/StableState/core-ss.lp $bundleDir/$version/ModRev/ASP/StableState/
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

#printf "check_ASP=true\n" >> $bundleDir/$version/ModRev/config.txt
#printf "compareLevelFunction=true\n" >> $bundleDir/$version/ModRev/config.txt
#printf "exactMiddleFunctionDetermination=true\n" >> $bundleDir/$version/ModRev/config.txt
#printf "forceOptimum=false\n" >> $bundleDir/$version/ModRev/config.txt

# Redo files location in make file

cd $bundleDir/$version
zip -r ModRev-$version.zip ModRev -x .DS_Store
cd ../..
