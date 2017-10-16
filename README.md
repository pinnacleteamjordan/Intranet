# Intranet

## Adding Views
```
mkdir views
nvim pagename.html
nvim main.cpp
use existing examples to make your own
```

## Compiling
#### Requirements
GNU/Linux
- The project can be modified to not need this requirement however until then it uses unistd.h

Boost libs

Python Libs
- Optional, used for google authentication and gathering analytics data

PostgreSQL Libs
- Optional, only use if using database

#### Build Commands
cmake .

make

## Known Issues
AnalyticsData()
- does not work, the python libs are messed up and I know this.

## Credit given where credit is due
```
crow_all.h
https://github.com/ipkn/crow
```
