#include "PatrocinadorService.h"
#include <stdexcept>

PatrocinadorService::PatrocinadorService(PatrocinadorRepository& repository) : repo(repository) {}
std::vector<PatrocinadorModel> PatrocinadorService::findAll() const { return repo.findAll(); }
PatrocinadorModel PatrocinadorService::insert(const PatrocinadorModel& item) const { validate(item); return repo.insert(item); }
PatrocinadorModel PatrocinadorService::update(const std::string& id, const PatrocinadorModel& item) const { validateId(id); return repo.update(id, item); }
bool PatrocinadorService::remove(const std::string& id) const { validateId(id); return repo.remove(id); }
void PatrocinadorService::validate(const PatrocinadorModel& item) {
	if (item.getNombre().empty()) throw std::invalid_argument("El patrocinador requiere nombre");
	if (item.getNivel() != "principal" && item.getNivel() != "aliado") throw std::invalid_argument("Nivel de patrocinador invalido");
}
void PatrocinadorService::validateId(const std::string& id) {
	if (id.empty()) throw std::invalid_argument("El id de patrocinador no puede estar vacio");
}
