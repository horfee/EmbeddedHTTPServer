/*
 * HTTPResponse.h
 *
 *  Created on: 11 août 2015
 *      Author: horfee
 */

#ifndef HTTPSERVER_HTTPRESPONSE_H_
#define HTTPSERVER_HTTPRESPONSE_H_
#include <evhttp.h>
#include <string>
#include <iostream>
#include "../json/json.h"
#include <exception>

namespace alarmpi {

class FileNotFoundException: public std::exception {};


typedef std::ostream& (*ostream_manipulator)(std::ostream&);

class HTTPResponse {


public:
	friend HTTPResponse& operator<<(HTTPResponse& out, const std::string& str);
	friend HTTPResponse& operator<<(HTTPResponse& out, int& value);
	friend HTTPResponse& operator<<(HTTPResponse& out, long& value);
	friend HTTPResponse& operator<<(HTTPResponse& out, float& value);
	friend HTTPResponse& operator<<(HTTPResponse& out, double& value);
	friend HTTPResponse& operator<<(HTTPResponse& out, bool& value);
	friend HTTPResponse& operator<<(HTTPResponse& out, const char * value);
	friend HTTPResponse& operator<<(HTTPResponse& out, Json::Value& value);

	void appendFile(std::string fileName);
	void appendFile(FILE * f);

//	friend HTTPResponse& operator<<(HTTPResponse& out, int fd);


	typedef HTTPResponse& (*HTTPResponseManipulator)(HTTPResponse&);

	    // take in a function with the custom signature
	    HTTPResponse& operator<<(HTTPResponseManipulator manip)
	    {
	        // call the function, and return it's value
	        return manip(*this);
	    }

	    // define the custom endl for this stream.
	    // note how it matches the `HTTPResponseManipulator`
	    // function signature
	    static HTTPResponse& endl(HTTPResponse& stream)
	    {
	        stream << std::endl;
	        return stream;
	    }

	    // this is the type of std::cout
	    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

	    // this is the function signature of std::endl
	    typedef CoutType& (*StandardEndLine)(CoutType&);

	    // define an operator<< to take in std::endl
	    HTTPResponse& operator<<(StandardEndLine manip)
	    {
	        // call the function, but we cannot return it's value
	        manip(std::cout);

	        return *this;
	    }


	evbuffer* getBuffer();

	HTTPResponse();
	virtual ~HTTPResponse();

	int getCode() const;
	void setCode(int code);

	std::string getReason() const;
	void setReason(std::string reason);


	std::string getContentType() const;
	void setContentType(std::string contentType);

	void addHeader(std::string header, std::string value);
	void removeHeader(std::string header);

	std::map<std::string, std::string> getHeaders()const;

protected:

private:

	std::string reason;
	int code;
	evbuffer* buffer;
	std::string contentType;
	std::map<std::string, std::string> headers;
};

} /* namespace alarmpi */

#endif /* HTTPSERVER_HTTPRESPONSE_H_ */
