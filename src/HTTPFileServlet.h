/*
 * HTTPFileServlet.h
 *
 *  Created on: 17 sept. 2015
 *      Author: horfee
 */

#ifndef HTTPSERVER_HTTPFILESERVLET_H_
#define HTTPSERVER_HTTPFILESERVLET_H_

#include "HTTPServlet.h"

namespace alarmpi {

class HTTPFileServlet: public HTTPServlet {
public:
	HTTPFileServlet(std::string rootFolder);
	virtual ~HTTPFileServlet();

	void doGet(HTTPRequest &request, HTTPResponse &response);

private:
	std::string rootFolder;
};

} /* namespace alarmpi */

#endif /* HTTPSERVER_HTTPFILESERVLET_H_ */
