# GIF++ DAQ v5.0 for IPBus
#### Author: Felipe Silva (felipe.silva@cern.ch)
#### Original Author: Alexis Fagot (alexis.fagot@ugent.be)

## Development Env

### Build

```
docker build -t gif_daq_dev_env docker/dev_env
```

### Run Development Env

```
docker run --name gif_daq -v `pwd`:/gif_daq -v `pwd`/buffer_zone/operation:/var/operation/RUN -v `pwd`/buffer_zone/webdcs:/var/webdcs/HVSCAN/ -it --rm gif_daq_dev_env
```

### Compile

```
cd build
cmake ..
make install
cd ..
```

### Run

```
bin/daq buffer_zone/outputs.txt
```

##


## Compilation (OLD BUT GOLD)

This application requires 3 non-optional dependencies:

* **[CAEN USB Driver](http://www.caen.it/csite/CaenProd.jsp?idmod=417&parent=11)** to mount the VME hardware
* **[CAEN VME Library](http://www.caen.it/csite/CaenProd.jsp?idmod=689&parent=38)** to communicate with the VME hardware
* **[ROOT](https://root.cern.ch/downloading-root) to organize the collected data into a TTree**

The CAEN VME library will not be packaged by distributions and will need to be installed manually.

To compile the GIF++ DAQ project, simply do :

    mkdir build
    cd build
    cmake ..
    make install

You may want to erase and remake the build directory in between compilations to avoid problems (unless minor C++ code change).

## Usage (OLD BUT GOLD)

This data acquisition software is designed to acquire data in the new Gamma Irradiation Facility (GIF++) at CERN.
In this facility, muon detector (Resistive Plate Chambers, Cathode Strip Chambers, Micromegas, Drift Tubes, Gas Electron Multipliers, etc...) performances are
being tested in an irradiated environment (gammas from a Cs137 source of 14TBq).
The goal is to certify upgraded detectors for the High Luminosity LHC phase of the Large Hadron Collider were they will suffer a higher radiation background.

In our case, the software is meant to test CMS RPCs.
The digital LVDS signals at the output of the detectors are sent to VME Time to Digital Converters (TDCs) from the CAEN company.
The trigger signal is built from 3 scintillators placed respectively before the bunker area of GIF, inside the bunker nearby the RPCs under test, and after
the bunker area along the muon beam line.
This trigger allows to record the data present in the buffer of the TDCs and to store it into ROOT TTrees.

Note that this new version of the DAQ is not meant to be used as a standalone program anymore.
A web interface has been designed to automate the data taking of an entire High Voltage scan.
This web interface launches the DAQ and sends START signals at the beginning of every run and shut downs the DAQ via a STOP signal when the scan is completed.
These signals are sent through the common file "run" placed in a common DCS/DAQ folder.

