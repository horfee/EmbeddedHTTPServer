/*
 * HTTPServlet.cpp
 *
 *  Created on: 17 sept. 2015
 *      Author: horfee
 */

#include "HTTPServlet.h"

namespace alarmpi {

HTTPServlet::HTTPServlet() {
	server = NULL;
}

HTTPServer* HTTPServlet::getServer() {
	return server;
}

HTTPServlet::~HTTPServlet() {
	server = NULL;
}

void HTTPServlet::doGet(HTTPRequest &request, HTTPResponse &response) {}
void HTTPServlet::doPost(HTTPRequest &request, HTTPResponse &response) {}
void HTTPServlet::doPut(HTTPRequest &request, HTTPResponse &response) {}
void HTTPServlet::doHead(HTTPRequest &request, HTTPResponse &response) {}
void HTTPServlet::doOptions(HTTPRequest &request, HTTPResponse &response) {}
void HTTPServlet::doDelete(HTTPRequest &request, HTTPResponse &response) {}
void HTTPServlet::doTrace(HTTPRequest &request, HTTPResponse &response) {}
void HTTPServlet::doConnect(HTTPRequest &request, HTTPResponse &response) {}
void HTTPServlet::doPatch(HTTPRequest &request, HTTPResponse &response) {}

} /* namespace alarmpi */
