#pragma once
#include <string>
#include <vector>
class AgendaModel {
	std::string id, dia, inicio, fin, titulo, detalle;
	std::string tipo = "service";
	std::vector<std::string> responsables;
public:
	const std::string& getId() const; const std::string& getDia() const;
	const std::string& getInicio() const; const std::string& getFin() const;
	const std::string& getTitulo() const; const std::string& getDetalle() const;
	const std::string& getTipo() const; const std::vector<std::string>& getResponsables() const;
	void setId(const std::string& value); void setDia(const std::string& value);
	void setInicio(const std::string& value); void setFin(const std::string& value);
	void setTitulo(const std::string& value); void setDetalle(const std::string& value);
	void setTipo(const std::string& value); void setResponsables(const std::vector<std::string>& value);
};
