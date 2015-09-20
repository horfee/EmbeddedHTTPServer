/*
 * HTTPFileServlet.cpp
 *
 *  Created on: 17 sept. 2015
 *      Author: horfee
 */

#include "HTTPFileServlet.hpp"

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>


#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif

#define FILE_404	"./404.html"

namespace httpserver {

HTTPFileServlet::HTTPFileServlet(std::string rootFolder): rootFolder(rootFolder) {
}

HTTPFileServlet::~HTTPFileServlet() {
}



static const struct table_entry {
	const char *extension;
	const char *content_type;
} content_type_table[] = {
	{ "txt", "text/plain" },
	{ "c", "text/plain" },
	{ "h", "text/plain" },
	{ "html", "text/html" },
	{ "htm", "text/htm" },
	{ "css", "text/css" },
	{ "gif", "image/gif" },
	{ "jpg", "image/jpeg" },
	{ "jpeg", "image/jpeg" },
	{ "png", "image/png" },
	{ "pdf", "application/pdf" },
	{ "ps", "application/postsript" },
	{ NULL, NULL },
};

/* Try to guess a good content-type for 'path' */
static const char *
guess_content_type(string path)
{
	const char *last_period, *extension;
	const struct table_entry *ent;
	last_period = strrchr(path.c_str(), '.');
	if (!last_period || strchr(last_period, '/'))
		goto not_found; /* no exension */
	extension = last_period + 1;
	for (ent = &content_type_table[0]; ent->extension; ++ent) {
		if (!evutil_ascii_strcasecmp(ent->extension, extension))
			return ent->content_type;
	}

not_found:
	return "application/misc";
}


void HTTPFileServlet::doPost(HTTPRequest &request, HTTPResponse &response) {
	this->doGet(request, response);
}

void HTTPFileServlet::doGet(HTTPRequest &request, HTTPResponse &response) {

	try {

		auto createFolderIndex = [&](std::string path){
			struct dirent *ent;
			DIR* d;
			if (!(d = opendir(path.c_str()))) {
				throw "Invalid Path";
			}

			ostringstream os;
			os << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "	<head>\n"
			<<		"		<meta http-equiv='Content-Type' content='text/html; charset=UTF-8' />"
			<<		"		<meta charset='utf-8'>\n"
			<<		"		<title>" << request.getURL() << "</title>\n"
			<<		"		<base href='" << request.getURL() << "'/>\n"
			<<		"		<style>"
			<<		"			html {"
			<<		"				background-color: lightgrey;"
			<<		"			}"
			<<		"			body {"
			<<		"				 margin:50px;"
			<<		"				 background-color: white;"
			<<		"				 border: 1px thin solid grey;"
			<<		"				 border-radius: 10px;"
			<<		"				 padding: 3px;"
			<<		"			}"
			<<		"			h1 {"
			<<		"				color:grey;"
			<<		"			}"
			<<		"		</style>"
			<<		"	</head>\n"
			<<		"	<body>\n"
			<<		"		<h1>" << request.getURL() << "</h1>\n"
			<<		"		<br/>\n"
			<<		"		<ul>\n";

			while ((ent = readdir(d))) {
				std::string name(ent->d_name);
				os << "   		<li><a href=\"" << request.getURL() << (request.getURL()[request.getURL().size() - 1] != '/'? "/" : "") << name << "\">" << name << "</a>\n";
			}
			os	<< 	"		</ul>\n"
				<<	"	</body>\n"
				<<	"</html>";
			closedir(d);
			return os.str();
		};


		std::string fileName(rootFolder);
		if ( fileName[fileName.length() - 1] != '/' ) fileName += "/";
			// here fileName ends with "/"

		if ( request.getURL().length() == 0 ) {
			response << createFolderIndex(fileName);
			response.setContentType("text/html");
		} else if ( request.getURL()[0] == '/') {
			struct stat buffer;
			fileName += request.getURL().substr(1);
			if (stat (fileName.c_str(), &buffer) == 0) {
				if ( S_ISDIR(buffer.st_mode) ) {
					response << createFolderIndex(fileName);
					response.setContentType("text/html");
				} else {
					response.appendFile(fileName);
					response.setContentType(guess_content_type(fileName));
				}

			} else {
				std::string file404 = rootFolder;
				if ( file404[file404.size() - 1] != '/' ) file404 += "/";
				file404 += FILE_404;
				response.appendFile(file404);
				response.setContentType(guess_content_type(FILE_404));
			}

		}

	}catch ( const std::exception &e ){
		std::cerr << e.what() << std::endl;
	}
}

} /* namespace httpserver */
