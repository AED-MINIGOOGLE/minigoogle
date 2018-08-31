# MiniGoogle/Engine

## Installation

### Consola 
#### Dependencies
First you must install `build-essential` to build the application.
##### On Debian/Ubuntu 
```
sudo apt-get install build-essential
```
#### Building
The application can be built using the Makefile.
```
cd minigoogle/engine
make
```
### Usage
To run the command-line version:
```
./search
```
###Web
#### Requirements
```sh
Install brew
copy and paste the next command in terminal 
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

#### Dependencies
```sh
Install libboost-all-dev
command for install libboost-all-dev
brew install boost
```
```sh
Install g++
command for install gcc 
brew install gcc
```
```sh
Install cmake
command for install cmake
brew install cmake 
```
```sh
Install make
command for install make
brew install make 
```

#### Compile and run server

```sh
cd core-server/
mkdir build
cd build
cmake ..
make
cd ..
Run the server: `./build/run_server`
```


