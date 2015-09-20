/*
 * HTTPFileServlet.h
 *
 *  Created on: 17 sept. 2015
 *      Author: horfee
 */

#ifndef HTTPSERVER_HTTPFILESERVLET_H_
#define HTTPSERVER_HTTPFILESERVLET_H_

#include "HTTPServlet.hpp"

namespace httpserver {

/*
 * Implementation of a simple file serving servlet
 */
class HTTPFileServlet: public HTTPServlet {
public:
	HTTPFileServlet(std::string rootFolder);
	virtual ~HTTPFileServlet();

	// search into the root folder to get the requested file,
	// or the list of files if the requested resource if a folder.
	void doGet(HTTPRequest &request, HTTPResponse &response);

	// it does not proceed data parameters
	void doPost(HTTPRequest &request, HTTPResponse &response);

private:
	std::string rootFolder;
};

} /* namespace httpserver */

#endif /* HTTPSERVER_HTTPFILESERVLET_H_ */
