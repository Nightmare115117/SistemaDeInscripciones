#pragma once
#include <string>
#include <vector>
#include "Generics/Entity.h"

struct ProblematicaDTO {
	std::string id;
	std::string code;
	std::string title;
	std::string description;
};

class EventoModel : public Entity<int> {
	int cupo = 60;
	int minIntegrantes = 3;
	int maxIntegrantes = 5;
	bool registroAbierto = true;
	std::vector<ProblematicaDTO> problematicas;
public:
	int getCupo() const;
	int getMinIntegrantes() const;
	int getMaxIntegrantes() const;
	bool getRegistroAbierto() const;
	const std::vector<ProblematicaDTO>& getProblematicas() const;
	void setCupo(int value);
	void setMinIntegrantes(int value);
	void setMaxIntegrantes(int value);
	void setRegistroAbierto(bool value);
	void setProblematicas(const std::vector<ProblematicaDTO>& value);
};

class CorreoEstadoModel {
	bool configurado = false;
	bool habilitado = false;
	std::string remitente;
public:
	bool getConfigurado() const;
	bool getHabilitado() const;
	const std::string& getRemitente() const;
	void setConfigurado(bool value);
	void setHabilitado(bool value);
	void setRemitente(const std::string& value);
};
