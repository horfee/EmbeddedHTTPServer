/*
 * HTTPResponse.cpp
 *
 *  Created on: 11 août 2015
 *      Author: horfee
 */

#include "HTTPResponse.h"
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>

namespace alarmpi {

HTTPResponse::HTTPResponse() {
	buffer = evbuffer_new();
	code = HTTP_OK;
	reason = "";
	contentType = "";//"html/text";

}

HTTPResponse::~HTTPResponse() {
	evbuffer_free(buffer);
}

int HTTPResponse::getCode() const {
	return code;
}

void HTTPResponse::setCode(int code) {
	this->code = code;
}

std::string HTTPResponse::getReason() const {
	return reason;
}

void HTTPResponse::setReason(std::string reason) {
	this->reason = reason;
}

evbuffer* HTTPResponse::getBuffer() {
	return buffer;
}

void HTTPResponse::addHeader(std::string header, std::string value) {
	headers[header] = value;
}

void HTTPResponse::removeHeader(std::string header) {
	headers.erase(header);
}

std::map<std::string, std::string> HTTPResponse::getHeaders() const {
	return headers;
}

HTTPResponse& operator<<(HTTPResponse& out, const std::string& str) {
	evbuffer_add_printf(out.buffer, "%s", str.c_str());
	return out;
}


HTTPResponse& operator<<(HTTPResponse& out, int& value) {
	evbuffer_add_printf(out.buffer, "%d", value);
	return out;
}

HTTPResponse& operator<<(HTTPResponse& out, long& value) {
	evbuffer_add_printf(out.buffer, "%ld", value);
	return out;
}

HTTPResponse& operator<<(HTTPResponse& out, float& value) {
	evbuffer_add_printf(out.buffer, "%f", value);
	return out;
}

HTTPResponse& operator<<(HTTPResponse& out, double& value) {
	evbuffer_add_printf(out.buffer, "%lf", value);
	return out;
}

HTTPResponse& operator<<(HTTPResponse& out, bool& value) {
	evbuffer_add_printf(out.buffer, "%d", value);
	return out;
}

HTTPResponse& operator<<(HTTPResponse& out, const char * value) {
	evbuffer_add_printf(out.buffer, "%s", value);
	return out;
}

std::string HTTPResponse::getContentType() const {
	return contentType;
}

void HTTPResponse::setContentType(std::string contentType) {
	this->contentType = contentType;
}

HTTPResponse& operator<<(HTTPResponse& out, Json::Value& value) {
	Json::StyledWriter writer;
	return out << writer.write(value);
}

void HTTPResponse::appendFile(std::string fileName) {

	FILE* f = fopen(fileName.c_str(), "r");
	if ( f != NULL ) {
		this->appendFile(f);
//		fclose(f);
	} else {
		throw FileNotFoundException();
	}


}
void HTTPResponse::appendFile(FILE * f) {
	struct stat st;
	int fd = fileno(f);
	if ( fstat(fd, &st) < 0 ) throw "File not found";
	evbuffer_add_file(buffer, fd, 0, st.st_size);
}


} /* namespace alarmpi */
