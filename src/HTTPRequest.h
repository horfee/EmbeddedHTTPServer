/*
 * HTTPRequest.h
 *
 *  Created on: 11 août 2015
 *      Author: horfee
 */

#ifndef HTTPSERVER_HTTPREQUEST_H_
#define HTTPSERVER_HTTPREQUEST_H_
#include <evhttp.h>
#include <map>
#include <string>

namespace alarmpi {

class HTTPRequest {
public:
	HTTPRequest(struct evhttp_request* req);
	virtual ~HTTPRequest();


	std::string getURL();

	enum evhttp_cmd_type getMethod() const;

	const std::map<std::string, std::string> getParameters();

	bool hasParameter(std::string parameter);

	std::string getParameter(std::string parameter);

	const std::map<std::string, std::string> getHeaders();

	std::string getHeader(std::string header);

	bool hasHeader(std::string header);

	std::string getData();

private:
	struct evhttp_request* request;
	std::map<std::string, std::string>* parameters;
	std::map<std::string, std::string>* headers;
	std::string* url;
	std::string* data;
};

} /* namespace alarmpi */

#endif /* HTTPSERVER_HTTPREQUEST_H_ */
