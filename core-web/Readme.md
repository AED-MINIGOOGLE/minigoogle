# Installing on Linux

First we need to install in this order:

## Installing Go language

Go is an Open Source programming language developed by Google Inc.  

* To install go latest:

      wget https://storage.googleapis.com/golang/go1.9.1.linux-amd64.tar.gz

* Unzip in `/usr/local` directory:

      sudo tar -C /usr/local -xzf go1.9.1.linux-amd64.tar.gz
      tar -C ./ -xzf go1.9.1.linux-amd64.tar.gz

* Then, modify `/etc/profile` file, add this line:

      export PATH=$PATH:/usr/local/go/bin

* Also, add these lines in `/home/$USER/.profile` file:

      export GOPATH=$HOME/golangProjects
      export GOROOT=$HOME/go
      export PATH=$PATH:$GOROOT/bin

* Finally, reboot or execute this:

      source .profile
      sudo source /etc/profile

### Installing some libraries or dependencies for Golang

Then install some libraries like gorilla (framework web) and logs (system logger).

#### Gorilla

Gorilla is a Web Framework to develop using Golang RESTful services, you can find more information pressing here.  
You can install on local writting this on terminal:

    go get github.com/gorilla/mux

#### Logs

Logs is a library that offers a logger, with alert, warning, success and error messages.

    go get github.com/jenazads/logs

### Setting up the service

Once installed components, just run in `ED2018-MCS/website`:

    go build main.go
    sudo ./main &

For more information about installation, press [here](http://jenazads.com/frameworks/Create-a-REST-service-using-Go-Language-and-BeeGo-Framework).

# Installing on MAC

First we need to install in this order:

## Golang

To install go latest:  
go to [golang page](https://golang.org/dl/) and choose the option for MAC.  
Download .pkg option and install it, restart you terminal session.

The create your `$HOME/go` directory and then install components:

    go get github.com/gorilla/mux
    go get github.com/jenazads/logs

Then, go to `$HOME/go/src` and clone the project:

    git clone https://github.com/jeffersonquispe/ED2018-MCS.git

Once installed components, just run in `ED2018-MCS/website`:

    go build main.go
    sudo ./main &

