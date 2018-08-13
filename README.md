# geant_material_screening
A tool to model the screening efficiency of various materials, and of various thicknesses, for an x-ray detector

# geant_xray_detector

## To build:
```
source <your geant4 build path>/bin/geant4.sh
cd build
cmake ../
make -j8
cd ../
```

## To run:
```
build/main energy_sweep.mac
```
## To analyze output, or to change sweep parameters:
look at energy_sweep.ipynb
