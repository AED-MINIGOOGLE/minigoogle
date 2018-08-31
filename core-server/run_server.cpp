#include "server_http.hpp"
#include "../engine/cli.hpp"
#include "../engine/utility.hpp"
#include "json_parser.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <string>
#include <utility>
//Models for rtree
// Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// Added for the default_resource example
#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>
#ifdef HAVE_OPENSSL
#include "crypto.hpp"
#endif

using namespace std;
using namespace boost::property_tree;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

struct ResponseStruct{
    search_result response;
    bool is_busy;
    ResponseStruct(search_result _sr, bool _ib):response(_sr), is_busy(_ib){};
};
int main() {
    CliApp cli;
    HttpServer server;
    server.config.port = 8090;

    int count = 0;
    string query;
    cli.RunWeb();
    
    cout << "finish" << endl;
    map<int, search_result> responses;
    //vector<search_result> responses;
    //Get HTTP | get example 
    server.resource["^/example$"]["GET"] = [&count](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;
        stream << count;
        count++;
        response->write_get(stream,header);
    };

    //Post HTTP | post example
    server.resource["^/search$"]["POST"] = [&responses,&cli,&query,&count](
            shared_ptr<HttpServer::Response> response,
            shared_ptr<HttpServer::Request> request
        ) {
        stringstream stream;
        string json_string = "";
        SimpleWeb::CaseInsensitiveMultimap header;
        try {
            /*
            * ENVIAR JSON: {
            *   'count': 3
            * }
            */
            ptree pt;
            read_json(request->content, pt);
            cout << pt.get<int>("state") << endl;
            if(pt.get<int>("state") == 0){
                query = pt.get<string>("query");
                search_result result = cli.SearchWeb(query);
                responses.insert(std::pair<int,search_result>(count, result));

                if (result.size() <= 0) 
                    json_string = "{\"status\": false}";
                else{
                    json_string = "{\"data\": { \"result\": [";
                    int c = 0;
                    for(auto &it : result){
                        string contain = cli.engine.mDocs[it.second]->contain;
                        string title = cli.engine.mDocs[it.second]->title;
                        cout << title << endl;
                        contain = escapeJsonString(contain);
                        title = escapeJsonString(title);
                        json_string += "{ \"id\":\"" + to_string(it.second) + "\", \"title\": \"" + title + "\", \"content\":\"" + contain + "\"},"; 
                        c++;
                        if(c>=20) {
                            break;
                        }
                    }
                    json_string.pop_back();
                    json_string += "], \"idRequest\": " + to_string(count) + " }}";
                    count++;
                    stream << json_string;
                    json_string = "";
                    response->write_get(stream,header);  
                }
            }
            else if(pt.get<int>("state") == 1){
                int id = pt.get<int>("idRequest");
                string nroPage = pt.get<string>("nroPage");
                search_result current_result = responses.find(id)->second;
                json_string = "{\"data\": { \"result\": [";
                for(int i = 20*(stoi(nroPage)-1);i<20*stoi(nroPage);i++){
                    if(i >= current_result.size()){
                        break;
                    }
                    string contain = cli.engine.mDocs[current_result[i].second]->contain;
                    string title = cli.engine.mDocs[current_result[i].second]->title;
                    cout << title << endl;
                    contain = escapeJsonString(contain);
                    title = escapeJsonString(title);
                    json_string += "{ \"id\":\"" + to_string(current_result[i].second) + "\", \"title\": \"" + title + "\", \"content\":\"" + contain + "\"},"; 
                }
                    json_string.pop_back();
                    json_string += "], \"idRequest\": " + to_string(id) + " }}";
                    //cout << json_string << endl;
                    stream << json_string;
                    json_string = "";
                    response->write_get(stream,header);
            }
            else{
                int id = pt.get<int>("idRequest");
                responses.erase(id);
                json_string = "{ \"state\": true}";
                stream << json_string;
                response->write_get(stream,header);
            }

 
            /*if (result.size() <= 0) 
                json_string = "{\"status\": false}";
            else{
                json_string = "{\"data\": ";
                for(auto &it : result){
                    count++;
                    json_string += "[ \"id\": " + to_string(it.second) + ", \"title\": " + cli.engine.mDocs[it.second]->title + ", \"content\": " +  cli.engine.mDocs[it.second]->contain + "],"; 
                    if (count % 20 == 0 && result.size()>count) {
					        break;
                    }
                }
                json_string.pop_back();
                json_string += " }";
                response->write_get(stream,header);                
            }
            json_string.pop_back();
            json_string += " }";
            cout << json_string << endl;

            //auto newcount = pt.get<string>("count");
            //count = stol(newcount);
            for (boost::property_tree::ptree::value_type& rowPair:pt.get_child("polygon")) {
                for (boost::property_tree::ptree::value_type& itemPair : rowPair.second) {
                    int value = itemPair.second.get_value<int>();
                    v.push_back(value);
                }
            }
            for (size_t i = 0; i < v.size(); i += 2) {
                P point(v[i], v[i+1]);
                pv.push_back(point);
            }
            int identifier_polygon = count++;
            tree->insert(new Polygon<dtype>(pv, identifier_polygon));
            stream << json_string;
            response->write_get(stream,header);*/
        } catch (const exception &e) {
            response->write(
                SimpleWeb::StatusCode::client_error_bad_request,
                e.what()
            );
        }
    };

    //Option rtree
    server.resource["^/search$"]["OPTIONS"] = [](
            shared_ptr<HttpServer::Response> response,
            shared_ptr<HttpServer::Request> request
        ) {
        stringstream stream;
        string json_string = "";
        SimpleWeb::CaseInsensitiveMultimap header;
        try {
            json_string = "['status': true]";
            stream << json_string;
            response->write_get(stream,header);
        } catch (const exception &e) {
            response->write(
                SimpleWeb::StatusCode::client_error_bad_request,
                e.what()
            );
        }
    };

    //
    //Option rtree
    server.resource["^/example$"]["OPTIONS"] = [](
            shared_ptr<HttpServer::Response> response,
            shared_ptr<HttpServer::Request> request
        ) {
        stringstream stream;
        string json_string = "";
        SimpleWeb::CaseInsensitiveMultimap header;
        try {
            json_string = "['status': true]";
            stream << json_string;
            response->write_get(stream,header);
        } catch (const exception &e) {
            response->write(
                SimpleWeb::StatusCode::client_error_bad_request,
                e.what()
            );
        }
    };

    server.on_error = [](shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
        // Handle errors here
        // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
    };

    thread server_thread([&server]() {
        // Start server
        server.start();
    });

    // Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));
    server_thread.join();
}
