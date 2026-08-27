#include "PatrocinadorService.h"
#include <stdexcept>

PatrocinadorService::PatrocinadorService(PatrocinadorRepository& repository) : Service<PatrocinadorModel, PatrocinadorRepository, std::string>(repository) {}
std::vector<PatrocinadorModel> PatrocinadorService::findAll() const { return repo.findAll(); }
PatrocinadorModel PatrocinadorService::findById(const std::string& id) const { validateId(id); return repo.findById(id); }
PatrocinadorModel PatrocinadorService::insert(const PatrocinadorModel& item) { validate(item); return repo.insert(item); }
bool PatrocinadorService::update(const PatrocinadorModel& item) { updateAndFetch(item.getId(), item); return true; }
PatrocinadorModel PatrocinadorService::updateAndFetch(const std::string& id, const PatrocinadorModel& item) const { validateId(id); return repo.update(id, item); }
bool PatrocinadorService::remove(const std::string& id) { validateId(id); return repo.remove(id); }
void PatrocinadorService::validate(const PatrocinadorModel& item) {
	if (item.getNombre().empty()) throw std::invalid_argument("El patrocinador requiere nombre");
	if (item.getNivel() != "principal" && item.getNivel() != "aliado") throw std::invalid_argument("Nivel de patrocinador invalido");
}
void PatrocinadorService::validateId(const std::string& id) {
	if (id.empty()) throw std::invalid_argument("El id de patrocinador no puede estar vacio");
}
