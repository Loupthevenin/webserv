/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_Post.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:58:00 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/27 13:02:00 by opdi-bia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

bool isValidFilename(const std::string &name) {
    if (name.empty() || name.size() > 100)
        return false;
  
    for (size_t i = 0; i < name.size(); ++i) {
        char c = name[i];
        if (!(isalnum(c) || c == '-' || c == '_' || c == '.'))
            return false;
    }
  
    if (name.find("..") != std::string::npos)
        return false;
  
    return true;
}

int  create_file(HttpRequest &request, HttpResponse &response){
    std::string buffer = request.getBody();
    size_t found_file = buffer.find("file=");
    size_t found_data = buffer.find("data=");
    std::string fileName = buffer.substr(found_file + 5, found_data - 6);
    std::string fileData = buffer.substr(found_data + 5);
    std::cout << "ici file name  : " << fileName << " ici data = " << fileData <<  std::endl;
    if(isValidFilename(fileName) == false)
    {
        response.setStatus(400);
        response.setHeader("Content-Type", "text/plain");
        response.setBody("400 - Nom de fichier invalide");
        return(1);
    }
    std::ofstream file(fileName.c_str());
    file << fileData;
    file.close();
    response.setStatus(200);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("200 - Fichier cree");
    return(0);
}