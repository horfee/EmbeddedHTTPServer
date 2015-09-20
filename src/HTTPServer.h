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
#include <exception>

using namespace std;

namespace httpserver {

class HTTPServlet;

class EventException : public exception {};
class HttpEventException : public EventException {};
class BindHttpEventException : public HttpEventException {
public:
	BindHttpEventException(std::string listeningAddress, int listeningPort);
	virtual ~BindHttpEventException();

	std::string getAddress() const;
	int getPort() const;

private:
	std::string address;
	int port;
};

/*
 * This is the http server implementation
 */
class HTTPServer {
public:

	/*
	 * Create and bind a new HTTP server
	 * The listening address and port are not bound after constructor
	 */
	HTTPServer(string listeningAddr, int listeningPort);
	virtual ~HTTPServer();

	/*
	 * Will start to listen to HTTP Event
	 * it will try to bind to the listening address and port
	 *
	 */
	void start();

	void stop();

	/*
	 * Register a new handler for a given url.
	 * The url could be a regex (see std::regex::grep )
	 * The first servlet that matches the url regex will be used to flush a response
	 * Then no other servlet will be used to handle the request, so if 2 servlets could match a request,
	 * only one will be used and no order could be assume to determine which one will be used.
	 * A good tip is to insure the url are exclusive
	 */
	void addServlet(std::string url, HTTPServlet *servlet);

	/*
	 * Set a default servlet to handle all requests that are not handled by any other servlet.
	 */
	void setDefaultServlet(HTTPServlet *servlet);

	/*
	 * the server could filter the request by http methods.
	 * Use this method to combine multiple HTTP Method allowed.
	 * The default accepted methods are EVHTTP_REQ_GET | EVHTTP_REQ_POST
	 * Possible values are :
	 * 		EVHTTP_REQ_GET
	 * 		EVHTTP_REQ_POST
	 * 		EVHTTP_REQ_HEAD
	 * 		EVHTTP_REQ_PUT
	 * 		EVHTTP_REQ_DELETE
	 * 		EVHTTP_REQ_OPTIONS
	 * 		EVHTTP_REQ_TRACE
	 * 		EVHTTP_REQ_CONNECT
	 * 		EVHTTP_REQ_PATCH
	 */
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
