#include "AppLayer.h"
#include <iostream>

using namespace std;

string appHTTP_Client(string host, string path)
{
    string req = "GET " + path + " HTTP/1.1 Host: " + host;
    cout << "[Application HTTP] client request: " << req << endl;
    return req;
}

string appHTTP_Server(string request)
{
    cout << "[Application HTTP] server got: " << request << endl;
    string resp = "HTTP/1.1 200 OK body=Welcome";
    cout << "[Application HTTP] server reply: " << resp << endl;
    return resp;
}

string appDNS_Query(string domain)
{
    string q = "DNS QUERY " + domain;
    cout << "[Application DNS] " << q << endl;
    return q;
}

string appDNS_Reply(string domain, string ip)
{
    string r = "DNS REPLY " + domain + " -> " + ip;
    cout << "[Application DNS] " << r << endl;
    return r;
}
