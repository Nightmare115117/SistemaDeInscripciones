#pragma once
#include <string>
class StaffModel {
	std::string id, nombre, rol, area, correo, telefono;
public:
	const std::string& getId() const; const std::string& getNombre() const;
	const std::string& getRol() const; const std::string& getArea() const;
	const std::string& getCorreo() const; const std::string& getTelefono() const;
	void setId(const std::string& value); void setNombre(const std::string& value);
	void setRol(const std::string& value); void setArea(const std::string& value);
	void setCorreo(const std::string& value); void setTelefono(const std::string& value);
};
