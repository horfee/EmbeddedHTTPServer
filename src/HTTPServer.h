/*
 * HTTPServer.h
 *
 *  Created on: 10 août 2015
 *      Author: horfee
 */

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_
#include <string>
#include <evhttp.h>
#include <map>
#include <vector>
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HTTPServlet.h"

using namespace std;

namespace httpserver {

class HTTPServlet;

class HTTPServer {
public:
	HTTPServer(string listeningAddr, int listeningPort);
	virtual ~HTTPServer();

	void start();
	void stop();

	void addServlet(std::string url, HTTPServlet *servlet);
	void setDefaultServlet(HTTPServlet *servlet);

	void setAllowedMethods(ev_uint16_t methods );
private:

	int port;
	string serverAddr;

	struct event_base *base;
	struct evhttp *http;

	std::map<std::string, HTTPServlet*> handlers;
	HTTPServlet *defaultServlet;

	static void mainCallBack(struct evhttp_request *req, void *arg);

	ev_uint16_t allowedMethods;
};

} /* namespace httpserver */

#endif /* HTTPSERVER_H_ */
