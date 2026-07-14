#ifndef APPLAYER_H
#define APPLAYER_H

#include <string>

using namespace std;

// two simple application layer services for the project

string appHTTP_Client(string host, string path);
string appHTTP_Server(string request);

string appDNS_Query(string domain);
string appDNS_Reply(string domain, string ip);

#endif
