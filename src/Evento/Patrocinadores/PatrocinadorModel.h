#pragma once
#include <string>
#include "Generics/Entity.h"
class PatrocinadorModel : public Entity<std::string> {
	std::string nombre, logo, url;
	std::string nivel = "aliado";
public:
	const std::string& getId() const; const std::string& getNombre() const;
	const std::string& getLogo() const; const std::string& getUrl() const;
	const std::string& getNivel() const;
	void setId(const std::string& value); void setNombre(const std::string& value);
	void setLogo(const std::string& value); void setUrl(const std::string& value);
	void setNivel(const std::string& value);
};
