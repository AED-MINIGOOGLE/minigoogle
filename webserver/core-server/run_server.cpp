#include "server_http.hpp"
#include "../engine/cli.hpp"
#include "../engine/utility.hpp"
#include <iostream>
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

int main() {
    HttpServer server;
    server.config.port = 8090;

    int count = 1;
    string query;
    //Get HTTP | get example 
    server.resource["^/example$"]["GET"] = [&count](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;
        stream << count;
        count++;
        response->write_get(stream,header);
    };

    //Post HTTP | post example
    server.resource["^/search$"]["POST"] = [&query](
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
            query = pt.get<string>("query");
            CliApp cli;
            /*vector<char> cquery;
            for(auto s : query){
                cquery.push_back(s);
            }*/
            int n = cli.prueba(3);
            cout << n << endl;
            /* auto result = cli.SearchWeb(query);            
            if (result.size() <= 0) {
                cout << "Not found." << endl;
            } else {
                for (auto &it : result) {
			        for (auto& m : it.first)
                        cout << (*(it.second)) << ": " << m.first << ", dbindex: " << m.second << " results." << endl;
                }
            }*/
            //auto newcount = pt.get<string>("count");
            //count = stol(newcount);
            /*for (boost::property_tree::ptree::value_type& rowPair:pt.get_child("polygon")) {
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
            tree->insert(new Polygon<dtype>(pv, identifier_polygon));*/
            json_string = "{\"status\": true}";
            stream << json_string;
            response->write_get(stream,header);
        } catch (const exception &e) {
            response->write(
                SimpleWeb::StatusCode::client_error_bad_request,
                e.what()
            );
        }
    };

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
