#/bin/bash

source /usr/local/geant4/geant4.10.04.p01-osx/bin/geant4.sh
cd build
make -j8
cd ..
build/main energy_sweep.mac