/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_handleRequest.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:57:19 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/25 20:55:19 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

void sendError(int fd, int statusCode, const std::string &body) {
  HttpResponse response;
  response.setStatus(statusCode);
  // filepath here;
  response.setBody(body);

  std::string message = response.toString();
  send(fd, message.c_str(), message.size(), 0);
}

void closeConnexion(int fd, Epoll &epoll,
                    std::map<int, HttpRequest> &requests) {
  requests.erase(fd);
  epoll.removeFd(fd);
  close(fd);
}

std::string readFile(const std::string &filePath) {
  std::ifstream file(filePath.c_str());
  if (!file.is_open())
    return "404";

  std::string content;
  std::string line;
  while (std::getline(file, line))
    content += line + "\n";
  file.close();
  return content;
}

std::string listFilesInDirectory(const std::string &directory) {
  std::ostringstream oss;
  oss << "{ \"files\": [";

  DIR *dir = opendir(directory.c_str());
  if (!dir)
    return "{ \"Error\": \"open directory\"";

  struct dirent *entry;
  bool first = true;
  while ((entry = readdir(dir)) != NULL) {
    std::string filename(entry->d_name);
    if (filename != "." && filename != "..") {
      if (!first)
        oss << ", ";
      first = false;
      oss << "\"" << filename << "\"";
    }
  }

  closedir(dir);
  oss << "] }";
  return oss.str();
}

void setFdNonBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl(F_GETFL) failed");
    return;
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    perror("fcntl(O_NONBLOCK) failed");
    return;
  }
}


bool    body_size(HttpRequest &request, Location location){
    if(location.get_body_client() != 0)
    {
        size_t maxBodySize = location.get_body_client();
        if (request.getBody().size() > maxBodySize) {
            return(false);
        }
    }
    return(true);
}


int     check_file(std::string& filePath){
    struct stat s;
	if (stat(filePath.c_str(), &s) != 0)
		return (404);
    if (S_ISDIR(s.st_mode))
        return (403);
    if (access(filePath.c_str(), R_OK) != 0)
        return (403);
    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open())
        return (500);
    file.close();
    return(200);
}

std::string buildErrorResponse(int code) {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << code << " ";

    switch (code) {
        case 403: oss << "Forbidden"; break;
        case 404: oss << "Not Found"; break;
        case 405: oss << "Method not allowed"; break;
        case 413: oss << "Payload Too Large"; break;
        default:  oss << "Error"; break;
    }

    std::string body = "<html><body><h1>" + oss.str() + "</h1></body></html>";
    oss << body;
    return (oss.str());
}

std::string check_error_server(int code, Server &serverConf){
    std::ostringstream oss;
    oss << code; 
    std::string c_error = oss.str();
    std::string err_page = serverConf.get_error_page(c_error);
    if(!err_page.empty())
        return(err_page);
    return("");
}

std::string set_autoindex(const std::string& filePath){
    std::ostringstream html;
    struct dirent* entry;
    DIR* dir = opendir(filePath.c_str());
    
    if (!dir) {
        std::cerr << "[ERROR] Impossible d'ouvrir le dossier : " << filePath << std::endl;
        return ("");
    }
    html << "<html><head><title>Index of " << filePath << "</title></head><body>";
    html << "<h1>Index of " << filePath << "</h1><ul>";
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
		if (name == "." || name == "..")
            continue;
        std::string link = filePath;
        if (!link.empty() && link[link.size() - 1] != '/')
            link += '/';
        link += name;
        std::string fullPath = filePath + "/" + name;
        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
            link += '/';
        html << "<li><a href=\"" << link << "\">" << name << "</a></li>";
    }
    closedir(dir);
    html << "</ul></body></html>";
    return (html.str());
}


std::string  check_header(std::string uri){
    std::map<std::string, std::string> c_type;
    c_type[".html"] = "text/html";
    c_type[".js"]   = "application/javascript";
    c_type[".css"]  = "text/css";
    c_type[".json"] = "application/json";
    c_type[".sh"] = "cgi";
    c_type[".py"] = "cgi";
    std::size_t found = uri.find_last_of(".");
    
    if (found!=std::string::npos)
    {
		std::string extension = uri.substr(found);
		std::map<std::string, std::string>::iterator it = c_type.find(extension);
		if (it != c_type.end())
			return it->second;
        return("application/octet-stream");
    }
    return("application/octet-stream");
}
