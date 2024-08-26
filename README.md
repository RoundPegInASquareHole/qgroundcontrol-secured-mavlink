# QGroundControl Ground Control Station

[![Releases](https://img.shields.io/github/release/mavlink/QGroundControl.svg)](https://github.com/mavlink/QGroundControl/releases)
[![Travis Build Status](https://travis-ci.org/mavlink/qgroundcontrol.svg?branch=master)](https://travis-ci.org/mavlink/qgroundcontrol)
[![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/crxcm4qayejuvh6c/branch/master?svg=true)](https://ci.appveyor.com/project/mavlink/qgroundcontrol)

[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/mavlink/qgroundcontrol?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)


*QGroundControl* (QGC) is an intuitive and powerful ground control station (GCS) for UAVs.

The primary goal of QGC is ease of use for both first time and professional users.
It provides full flight control and mission planning for any MAVLink enabled drone, and vehicle setup for both PX4 and ArduPilot powered UAVs. Instructions for *using QGroundControl* are provided in the [User Manual](https://docs.qgroundcontrol.com/en/) (you may not need them because the UI is very intuitive!)

All the code is open-source, so you can contribute and evolve it as you want.
The [Developer Guide](https://dev.qgroundcontrol.com/en/) explains how to [build](https://dev.qgroundcontrol.com/en/getting_started/) and extend QGC.


Key Links:
* [Website](http://qgroundcontrol.com) (qgroundcontrol.com)
* [User Manual](https://docs.qgroundcontrol.com/en/)
* [Developer Guide](https://dev.qgroundcontrol.com/en/)
* [Discussion/Support](https://docs.qgroundcontrol.com/en/Support/Support.html)
* [Contributing](https://dev.qgroundcontrol.com/en/contribute/)
* [License](https://github.com/mavlink/qgroundcontrol/blob/master/COPYING.md)

## How to install version v4.2.9 using docker

* Clone repository using --recurse-submodules flag

* git checkout v4.2.9 (or any other v4.2 derivated branch like this one or devel)

* ./deploy/docker/install-qt-linux.sh

* mkdir build

* docker build --file ./deploy/docker/Dockerfile-build-linux -t qgc-linux-docker .

* docker run --rm -v ${PWD}:/project/source -v ${PWD}/build:/project/build qgc-linux-docker

* Run QGC executing the file QGroundControl or qgroundcontrol-start.sh whose can be found in build/staging/

### Troubleshooting

* git checkout might fail due to some files it complains about. Delete all of them and do the checkout. Restore them all afterwards.

* If an error related to nlohmann/detail/abi_macros.hpp raises, do: git submodule sync --recursive and git submodule update --recursive and try again.

## MAVLink submodule

Since MAVLink had to be modified for this purpose, a different git submodule than the original is being used.

* [Modified MAVLink](https://github.com/RoundPegInASquareHole/c_library_v2-secured-mavlink)
* [Modified MAVLink generator](https://github.com/RoundPegInASquareHole/mavlink-secured-mavlink)
