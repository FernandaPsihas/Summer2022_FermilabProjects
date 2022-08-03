FILES="*root"

for f in $FILES
do
    rm -rf $f
    ln -s /sbnd/data/users/jaz8600/DopedLArTPCAnalysisFiles/Elecs/$f $f
done
