package main

import (
  "time";
  "bytes";
  "net/http";
  "io/ioutil";
  "encoding/json";
  "html/template";
  "github.com/gorilla/mux";
  "github.com/jenazads/logs";
  "github.com/jenazads/gojwt";
)

type DataFound struct{
  Id      int      `json:"id"`
  Title   string   `json:"title"`
  Content string   `json:"content"`
}

type SearchQuery struct{
  State    int     `json:"state"`
  Query    string  `json:"query"`
}

type FinishQuery struct{
  State      int  `json:"state"`
  IdRequest  int  `json:"idRequest"`
}

type Data struct {
  Result    []DataFound  `json:"result"`
  IdRequest int          `json:"idRequest"`
  NroPages int           `json:"nroPages"`
}

var DataPages Data
var AuxIdRequest int = -1
var WasQuery bool = false

const(
// hostnames
  Hostname = "localhost"
// http
  Httpprotocol   = "http://"
  ListenHTTP     = ":8080"
// index paths
  Template_index = "index.html"
  Template_about = "templates/about.html"
  Template_pagesfound = "templates/pagesfound.html"
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

func FromJSON(data []byte) (Data, error) {
  elements := make(map[string]Data)
  err := json.Unmarshal(data, &elements)
  return Data{Result: elements["data"].Result, IdRequest: elements["data"].IdRequest, NroPages: elements["data"].NroPages}, err
}

func SearchHandler(w http.ResponseWriter, r *http.Request) {
  r.ParseForm()
  start := time.Now()
  
  sq := SearchQuery{State: 0, Query: r.Form["search-text"][0]}
  jsonObj, _ := gojwt.ToJSON(sq)
  req, err := http.NewRequest("POST", "http://"+Hostname+":8090/search",  bytes.NewBuffer(jsonObj))
  //rsp, _ := http.Post(Hostname+":8090/search", "application/x-www-form-urlencoded", jsonObj)
  if err!=nil{
    Logger.Error("Error: %s", err)
    return
  }
  req.Header.Set("Content-Type", "application/json;application/x-www-form-urlencoded")
  client := &http.Client{}
  resp, err := client.Do(req)
  if err!=nil{
    Logger.Error("Error: %s", err)
    return
  }
  defer resp.Body.Close()
  body, _ :=ioutil.ReadAll(resp.Body)
  Logger.Info("Response: %s", body)
  
  //body:="{ \"data\": { \"result\":[ { \"id\": 132, \"title\": \"titulo de documento...\", \"content\": \"Todo el contenido del documento.\" }, { \"id\": 2, \"title\": \"titulo de otro documento...\", \"content\": \"Todo el contenido de otro  documento.\" } ], \"idRequest\": 2 , \"nroPages\":10}}"
  
  DataPages, _ = FromJSON([]byte(body))
  LogServer(r.Method, r.URL.Path,"Search")
  Logger.Info("method: %s", r.Method)
  Logger.Info("Completed %s in %v\n", r.URL.Path, time.Since(start))
  http.Redirect(w, r, "/pagesfound", http.StatusMovedPermanently)
}

func IndexHandler(w http.ResponseWriter, r *http.Request) {
  if WasQuery {
    sq := FinishQuery{State: 2, IdRequest: AuxIdRequest}
    jsonObj, _ := gojwt.ToJSON(sq)
    req, err := http.NewRequest("POST", "http://"+Hostname+":8090/search",  bytes.NewBuffer(jsonObj))
    if err!=nil{
      Logger.Error("Error: %s", err)
      return
    }
    req.Header.Set("Content-Type", "application/json;application/x-www-form-urlencoded")
    client := &http.Client{}
    resp, err := client.Do(req)
    if err!=nil{
      Logger.Error("Error: %s", err)
      return
    }
    defer resp.Body.Close()
    WasQuery = false
  }
  if AuxIdRequest != 0 {
    AuxIdRequest = 0
  }
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

func PagesFoundHandler(w http.ResponseWriter, r *http.Request) {
  AuxIdRequest = DataPages.IdRequest
  WasQuery = true
  start := time.Now()
  LogServer(r.Method, r.URL.Path,"Pages Found")
  t, _ := template.ParseFiles(Template_pagesfound)
  t.Execute(w, DataPages)
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
  router.HandleFunc("/pagesfound", PagesFoundHandler)
  router.HandleFunc("/search", SearchHandler).Methods("GET")
  
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

