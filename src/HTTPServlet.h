/*
 * HTTPServlet.h
 *
 *  Created on: 17 sept. 2015
 *      Author: horfee
 */

#ifndef HTTPSERVER_HTTPSERVLET_H_
#define HTTPSERVER_HTTPSERVLET_H_

#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HTTPServer.h"

namespace alarmpi {

class HTTPServer;


class HTTPServlet {
public:
	HTTPServlet();
	virtual ~HTTPServlet();

	virtual void doGet(HTTPRequest &request, HTTPResponse &response);
	virtual void doPost(HTTPRequest &request, HTTPResponse &response);
	virtual void doPut(HTTPRequest &request, HTTPResponse &response);
	virtual void doHead(HTTPRequest &request, HTTPResponse &response);
	virtual void doOptions(HTTPRequest &request, HTTPResponse &response);
	virtual void doDelete(HTTPRequest &request, HTTPResponse &response);
	virtual void doTrace(HTTPRequest &request, HTTPResponse &response);
	virtual void doConnect(HTTPRequest &request, HTTPResponse &response);
	virtual void doPatch(HTTPRequest &request, HTTPResponse &response);


protected:
	HTTPServer *getServer();

private:
	HTTPServer *server;

	friend class HTTPServer;
};

} /* namespace alarmpi */

#endif /* HTTPSERVER_HTTPSERVLET_H_ */
