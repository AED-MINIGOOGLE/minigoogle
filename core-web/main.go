package main

import (
  "time";
  "net/http";
  "io/ioutil";
  "github.com/gorilla/mux";
  "github.com/jenazads/logs"
)

const(
// hostnames
  Hostname = "localhost"
// http
  Httpprotocol   = "http://"
  ListenHTTP     = ":8080"
// index paths
  Template_index = "index.html"
  Template_about = "templates/about.html"
// directories
  css = "assets/css"
  depen = "assets/dependencies"
  font = "assets/fonts"
  js = "assets/js"
  img = "assets/images"
  
  CSS = http.Dir(css)
  Depen = http.Dir(depen)
  Font = http.Dir(font)
  JS = http.Dir(js)
  IMG = http.Dir(img)
)

var(
  Logger = logs.NewLogger(10000)
  cssHandler = http.FileServer(CSS)
  depenHandler = http.FileServer(Depen)
  fontHandler = http.FileServer(Font)
  jsHandler = http.FileServer(JS)
  imgHandler = http.FileServer(IMG)
  
  cssStrip = http.StripPrefix("/assets/css/", cssHandler)
  depenStrip = http.StripPrefix("/assets/dependencies/", depenHandler)
  fontStrip = http.StripPrefix("/assets/fonts/", fontHandler)
  jsStrip = http.StripPrefix("/assets/js/", jsHandler)
  imgStrip = http.StripPrefix("/assets/images/", imgHandler)
)

type Page struct {
  Title   string
  Body  []byte
}

func VerifyError(err error){
  if err !=nil {
    Logger.Error("%s",err)
    panic(err)
  }
}

// load page
func LoadPage(filename string) (*Page, error) {
  body, err := ioutil.ReadFile(filename)
  if err != nil {
    return nil, err
  }
  return &Page{Title: filename, Body: body}, nil
}

func LogServer(method, path, name string){
  Logger.Info("Started %s %s", method, path)
  Logger.Info("Executing BeaGons "+name+" Handler")
}

func IndexHandler(w http.ResponseWriter, r *http.Request) {
  start := time.Now()
  LogServer(r.Method, r.URL.Path,"Index")
  p, _ := LoadPage(Template_index)
  w.Write(p.Body)
  Logger.Info("Completed %s in %v\n", r.URL.Path, time.Since(start))
}

func AboutHandler(w http.ResponseWriter, r *http.Request) {
  start := time.Now()
  LogServer(r.Method, r.URL.Path,"About")
  p, _ := LoadPage(Template_about)
  w.Write(p.Body)
  Logger.Info("Completed %s in %v\n", r.URL.Path, time.Since(start))
}

func MuxInitService(muxHttp *http.ServeMux){
  server := &http.Server{
    Addr   : ListenHTTP,
    Handler: muxHttp,
  }
  Logger.Info("webAppClient-Go Service Start")
  Logger.Info("listening on %s%s%s",Httpprotocol,Hostname,ListenHTTP)
  err := server.ListenAndServe()
  //err := http.ListenAndServe(ListenHTTP, nil)
  VerifyError(err)
}

func HttpListenerServiceInit(){
  // router
  router := mux.NewRouter()
  router.HandleFunc("/", IndexHandler)
  router.HandleFunc("/about", AboutHandler)
  //router.NotFoundHandler = http.HandlerFunc(handlers.NotFoundHandler)
  
  muxHttp := http.NewServeMux()
  muxHttp.Handle("/", router)
  
  // directories
  muxHttp.Handle("/assets/css/", cssStrip)
  muxHttp.Handle("/assets/dependencies/", depenStrip)
  muxHttp.Handle("/assets/fonts/", fontStrip)
  muxHttp.Handle("/assets/js/", jsStrip)
  muxHttp.Handle("/assets/images/", imgStrip)
  
  MuxInitService(muxHttp)
}

func main() {
  HttpListenerServiceInit()
}

