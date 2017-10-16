# Intranet

## Adding Views
1) mkdir views
2) nvim pagename.html
3) nvim main.cpp
4) use existing examples to make your own

## Compiling
#### Requirements
GNU/Linux The project can be modified to not need this requirement however until then it uses unistd.h

Boost libs

Python Libs (optional, used for google authentication)

PostgreSQL Libs (optional, only use if using database)

#### Build Commands
cmake .

make

## Known Issues
AnalyticsData() does not work, the python libs are messed up and I know this.
