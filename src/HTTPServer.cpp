/*
 * HTTPServer.cpp
 *
 *  Created on: 10 août 2015
 *      Author: horfee
 */

#include "HTTPServer.h"
#include <functional>
#include <thread>
#include <regex>
#include <algorithm>

namespace httpserver {



HTTPServer::HTTPServer(string listeningAddr, int listeningPort) {
	serverAddr = listeningAddr;
	port = listeningPort;
	base = NULL;
	http = NULL;
	allowedMethods = EVHTTP_REQ_GET | EVHTTP_REQ_POST ;
	defaultServlet = NULL;
}

HTTPServer::~HTTPServer() {
	stop();
}

void HTTPServer::setDefaultServlet(HTTPServlet *servlet) {
	defaultServlet = servlet;
}

void HTTPServer::mainCallBack(struct evhttp_request *req, void *ctx) {
	HTTPServer* self = (HTTPServer*)ctx;

	HTTPRequest httpRequest(req);
	HTTPResponse httpResponse;

	std::string url = httpRequest.getURL();
	std::smatch matches;


	std::map<std::string, HTTPServlet*>::iterator it;
	bool found = false;
	for(it = self->handlers.begin(); it != self->handlers.end() && !found; ++it) {
		std::regex key(it->first, std::regex::grep|std::regex::icase);
		if (std::regex_match(url, matches, key) ) {
			HTTPServlet* servlet = it->second;
			found = true;
			enum evhttp_cmd_type method = evhttp_request_get_command(req);
			switch(method) {
			case EVHTTP_REQ_GET:
				servlet->doGet(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_POST:
				servlet->doPost(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_HEAD:
				servlet->doHead(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_PUT:
				servlet->doPut(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_DELETE:
				servlet->doDelete(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_OPTIONS:
				servlet->doOptions(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_TRACE:
				servlet->doTrace(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_CONNECT:
				servlet->doConnect(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_PATCH:
				servlet->doPatch(httpRequest, httpResponse);
				break;
			}

			evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", httpResponse.getContentType().c_str());
			for(auto header : httpResponse.getHeaders()) {
				evhttp_add_header(evhttp_request_get_output_headers(req), header.first.c_str(), header.second.c_str());
			}
			evhttp_send_reply(req, httpResponse.getCode(), httpResponse.getReason().c_str(),httpResponse.getBuffer());
		}


	}

	if ( !found ) {
		HTTPServlet *defaultServlet = self->defaultServlet;
		if ( defaultServlet != NULL ) {
			enum evhttp_cmd_type method = evhttp_request_get_command(req);
			switch(method) {
			case EVHTTP_REQ_GET:
				defaultServlet->doGet(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_POST:
				defaultServlet->doPost(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_HEAD:
				defaultServlet->doHead(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_PUT:
				defaultServlet->doPut(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_DELETE:
				defaultServlet->doDelete(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_OPTIONS:
				defaultServlet->doOptions(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_TRACE:
				defaultServlet->doTrace(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_CONNECT:
				defaultServlet->doConnect(httpRequest, httpResponse);
				break;
			case EVHTTP_REQ_PATCH:
				defaultServlet->doPatch(httpRequest, httpResponse);
				break;
			}

			evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", httpResponse.getContentType().c_str());
			for(auto header : httpResponse.getHeaders()) {
				evhttp_add_header(evhttp_request_get_output_headers(req), header.first.c_str(), header.second.c_str());
			}
			evhttp_send_reply(req, httpResponse.getCode(), httpResponse.getReason().c_str(),httpResponse.getBuffer());
		} else {
			evhttp_send_error(req, HTTP_NOTIMPLEMENTED, "No handler found for this url");
		}

	}
}

void HTTPServer::addServlet(std::string url, HTTPServlet *servlet) {
	handlers[url] = servlet;
	servlet->server = this;
}

void HTTPServer::setAllowedMethods(ev_uint16_t methods ) {

	allowedMethods = methods;
	if (http != NULL ) {
		evhttp_set_allowed_methods(http, allowedMethods);
	}

}


void HTTPServer::start() {

	/* Create new event base */
	base = event_base_new();
	if (!base) {
		throw "Unable to start server";
	}
	http = evhttp_new(base);

	if (!http) {
		throw "Unable to create evhttp";
	}
	evhttp_set_allowed_methods(http, allowedMethods);

	evhttp_set_gencb(http, &HTTPServer::mainCallBack , this);
	// static_cast(void(*)(struct evhttp_request *, void *))t, (char *)"/");

	/* Now we tell the evhttp what port to listen on */
	struct evhttp_bound_socket *handle = evhttp_bind_socket_with_handle(http, serverAddr.c_str(), port);
	if (!handle) {
		throw "Unable to bind address " + serverAddr + ":" + to_string(port);
	}

	/* Lets rock */
	std::thread t([&](){
		event_base_dispatch(base);
	});
	t.detach();

}

void HTTPServer::stop() {
	event_base_loopexit(base, NULL);
	evhttp_free(http);
	event_base_free(base);
}

} /* namespace httpserver */
