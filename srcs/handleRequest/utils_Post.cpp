/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_Post.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:58:00 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/31 11:48:54 by opdi-bia         ###   ########.fr       */
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

int create_file(HttpRequest &request, HttpResponse &response) {
  std::string buffer = request.getBody();
  size_t found_file = buffer.find("file=");
  size_t found_data = buffer.find("data=");

  if (found_file == std::string::npos || found_data == std::string::npos) {
    response.setStatus(400);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("400 - Requête invalide");
    return (0);
  }
  found_file += 5;
  found_data += 5;

  std::string fileName = buffer.substr(found_file, found_data - found_file - 6);
  std::string fileData = buffer.substr(found_data);

  fileName.erase(0, fileName.find_first_not_of(" \t\r\n"));
  fileName.erase(fileName.find_last_not_of(" \t\r\n") + 1);
  fileData.erase(0, fileData.find_first_not_of(" \t\r\n"));
  fileData.erase(fileData.find_last_not_of(" \t\r\n") + 1);

  if (isValidFilename(fileName) == false) {
    response.setStatus(400);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("400 - Nom de fichier invalide");
    return (0);
  }
  std::ofstream file(fileName.c_str());
  if (!file) {
    response.setStatus(500);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("500 - Erreur lors de la création du fichier");
    return (0);
  }
  file << fileData;
  file.close();
  response.setStatus(200);
  response.setHeader("Content-Type", "text/plain");
  response.setBody("200 - Fichier cree");
  return (0);
}
